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

#include "CefPulseApp.h"

#include <string>

#include "CefPulseHandler.h"

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"

namespace 
{
    class CefPulseWindowDelegate : public CefWindowDelegate 
    {
      public:
        CefPulseWindowDelegate
        (
            CefRefPtr<CefBrowserView> browser_view,
            cef_runtime_style_t runtime_style,
            cef_show_state_t initial_show_state
        ) : 
        browser_view_(browser_view),
        runtime_style_(runtime_style),
        initial_show_state_(initial_show_state) {}

        void OnWindowCreated(CefRefPtr<CefWindow> window) override 
        {
            window -> AddChildView(browser_view_);

            if (initial_show_state_ != CEF_SHOW_STATE_HIDDEN) 
            {
                window -> Show();
            }
        }

        void OnWindowDestroyed(CefRefPtr<CefWindow> window) override 
        {
            browser_view_ = nullptr;
        }

        bool CanClose(CefRefPtr<CefWindow> window) override 
        {
            CefRefPtr<CefBrowser> browser = browser_view_ -> GetBrowser();

            if (browser) {
                return browser -> GetHost() -> TryCloseBrowser();
            }

            return true;
        }

        CefSize GetPreferredSize(CefRefPtr<CefView> view) override 
        {
            return CefSize(800, 600);
        }

        cef_show_state_t GetInitialShowState(CefRefPtr<CefWindow> window) override 
        {
            return initial_show_state_;
        }

        cef_runtime_style_t GetWindowRuntimeStyle() override 
        {
            return runtime_style_;
        }

      private:
        CefRefPtr<CefBrowserView> browser_view_;
        const cef_runtime_style_t runtime_style_;
        const cef_show_state_t initial_show_state_;
        
        IMPLEMENT_REFCOUNTING(CefPulseWindowDelegate);
        DISALLOW_COPY_AND_ASSIGN(CefPulseWindowDelegate);
    };

    class CefPulseBrowserViewDelegate : public CefBrowserViewDelegate {
      public:
        explicit CefPulseBrowserViewDelegate(cef_runtime_style_t runtime_style): 
            runtime_style_(runtime_style) {}
        
        bool OnPopupBrowserViewCreated
        (
            CefRefPtr<CefBrowserView> browser_view,
            CefRefPtr<CefBrowserView> popup_browser_view,
            bool is_devtools
        ) override 
        {
            CefWindow::CreateTopLevelWindow(
                new CefPulseWindowDelegate(
                    popup_browser_view, 
                    runtime_style_, 
                    CEF_SHOW_STATE_NORMAL
                )
            );

            return true;
        }

      private:
        const cef_runtime_style_t runtime_style_;

        IMPLEMENT_REFCOUNTING(CefPulseBrowserViewDelegate);
        DISALLOW_COPY_AND_ASSIGN(CefPulseBrowserViewDelegate);
    };
}

CefPulseApp::CefPulseApp(const std::string url) :
    _browser_url(url) {} 

void CefPulseApp::OnContextInitialized() 
{
    CEF_REQUIRE_UI_THREAD();

    CefRefPtr<CefCommandLine>  command_line;
    CefRefPtr<CefBrowserView>  browser_view;
    CefBrowserSettings         browser_settings;
    
    cef_runtime_style_t runtime_style;
    runtime_style = CEF_RUNTIME_STYLE_DEFAULT;

    CefRefPtr<CefPulseHandler> handler(new CefPulseHandler());

    browser_view = CefBrowserView::CreateBrowserView
    (
        handler, _browser_url, browser_settings, nullptr, nullptr,
        new CefPulseBrowserViewDelegate(runtime_style)
    );

    CefWindow::CreateTopLevelWindow(new CefPulseWindowDelegate(
        browser_view, runtime_style, CEF_SHOW_STATE_NORMAL
    ));
}

CefRefPtr<CefClient> CefPulseApp::GetDefaultClient() 
{
    return CefPulseHandler::GetInstance();
}
