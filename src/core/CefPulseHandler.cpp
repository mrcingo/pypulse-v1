// MIT License
//
// Copyright (c) 2025 PyPulse
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "CefPulseHandler.h"

#include <sstream>
#include <string>

#include "CefPulseUtils.h"

#include "include/base/cef_callback.h"
#include "include/cef_app.h"
#include "include/cef_parser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace {
    CefPulseHandler* g_instance = nullptr;
}

CefPulseHandler::CefPulseHandler(void) 
{
    DCHECK(!g_instance);
    g_instance = this;
}

CefPulseHandler::~CefPulseHandler() 
{
    g_instance = nullptr;
}

CefPulseHandler* CefPulseHandler::GetInstance() 
{
    return g_instance;
}

void CefPulseHandler::OnTitleChange
(
    CefRefPtr<CefBrowser> browser,
    const CefString&      title
) 
{
    CEF_REQUIRE_UI_THREAD();

    auto browser_view = CefBrowserView::GetForBrowser(browser);

    CefRefPtr<CefWindow> window = browser_view->GetWindow();

    if (window) 
    {
        window->SetTitle(title);
    }
}


void CefPulseHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) 
{
    CEF_REQUIRE_UI_THREAD();

    browser_list_.push_back(browser);
}

bool CefPulseHandler::DoClose(CefRefPtr<CefBrowser> browser) 
{
    CEF_REQUIRE_UI_THREAD();
    
    if (browser_list_.size() == 1) {
      is_closing_ = true;
    }   
    
    return false;
}

void CefPulseHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) 
{
    CEF_REQUIRE_UI_THREAD();

    BrowserList::iterator c_browser = browser_list_.begin();

    for (; c_browser != browser_list_.end(); ++c_browser) 
    {
        if ((*c_browser) -> IsSame(browser)) 
        {
            browser_list_.erase(c_browser);
            break;
        }
    }

    if (browser_list_.empty()) 
    {
        CefQuitMessageLoop();
    }
}

void CefPulseHandler::OnLoadError
(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    ErrorCode errorCode,
    const CefString& errorText,
    const CefString& failedUrl
) 
{
    CEF_REQUIRE_UI_THREAD();

    if (errorCode == ERR_ABORTED) 
    {
        return;
    }

    std::stringstream errorResponse;
    errorResponse << 
        "<html>" << 
            "<body bgcolor=\"white\">"
                "<h2>" << 
                    "Failed to load URL " << std::string(failedUrl) << " with error " << std::string(errorText)<< " (" << errorCode << ")." << 
                "</h2>" <<
            "</body>" << 
        "</html>";

    frame -> LoadURL(GetDataURI(errorResponse.str(), "text/html"));
}

void CefPulseHandler::ShowMainWindow() 
{
    if (!CefCurrentlyOn(TID_UI)) 
    {
        CefPostTask(
            TID_UI, 
            base::BindOnce(&CefPulseHandler::ShowMainWindow, this)
        );
        return;
    }

    if (browser_list_.empty()) {
        return;
    }

    auto main_browser = browser_list_.front();

    auto browser_view = CefBrowserView::GetForBrowser(main_browser);

    if (auto window = browser_view -> GetWindow()) 
    {
        window->Show();
    }
}

void CefPulseHandler::CloseAllBrowsers(bool force_close) 
{
    if (!CefCurrentlyOn(TID_UI)) 
    {
        CefPostTask(
            TID_UI, 
            base::BindOnce(&CefPulseHandler::CloseAllBrowsers, this, force_close)
        );
        return;
    }

    if (browser_list_.empty()) 
    {
        return;
    }

    BrowserList::const_iterator c_browser = browser_list_.begin();

    for (; c_browser != browser_list_.end(); ++c_browser) 
    {
        (*c_browser) -> GetHost() -> CloseBrowser(force_close);
    }
}

#if defined(OS_WIN)
#include <windows.h>

void CefPulseHandler::PlatformTitleChange
(
    CefRefPtr<CefBrowser> browser,
    const CefString& title
) 
{
  CefWindowHandle hWnd = browser -> GetHost() -> GetWindowHandle();
  if (hWnd) 
  {
    SetWindowText(hWnd, std::wstring(title).c_str());
  }
}

#endif