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

#include <pybind11/pybind11.h>

#include <windows.h>

#include "../CefPulseApp.h"

#include "include/cef_command_line.h"
#include "include/cef_sandbox_win.h"
#include "include/cef_version_info.h"

namespace {
    int Initialize(const std::wstring &subprocess_path_str)
    {
        HINSTANCE hInstance = GetModuleHandle(nullptr);
        CefMainArgs main_args(hInstance);

        CefRefPtr<CefPulseApp> app(new CefPulseApp("https://google.com"));
        
        CefSettings settings;
        settings.no_sandbox = true;

        CefString(&settings.browser_subprocess_path) = subprocess_path_str;

        if (!CefInitialize(main_args, settings, app.get(), nullptr)) {
            return CefGetExitCode();
        }

        CefRunMessageLoop();

        CefShutdown();

        return 0;
    }
}

PYBIND11_MODULE(cefpulse, m) {
    m.def("Initialize", &Initialize, pybind11::arg("subprocess_path"));
}
