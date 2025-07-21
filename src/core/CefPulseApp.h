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

#ifndef CEF_PULSE_APP_H_
#define CEF_PULSE_APP_H_

#include "include/cef_app.h"

class CefPulseApp : public CefApp, public CefBrowserProcessHandler 
{
  public:
    explicit CefPulseApp(const std::string url);

    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override 
    { 
        return this; 
    }

    void OnContextInitialized() override;

    CefRefPtr<CefClient> GetDefaultClient() override;

  private:
    const std::string _browser_url;

    IMPLEMENT_REFCOUNTING(CefPulseApp);
};

#endif
