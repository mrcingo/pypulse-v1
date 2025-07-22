import importlib.resources
import cefpulse  # your pybind11 module

# Get absolute path to embedded cefpulsesub.exe inside the package
with importlib.resources.path("cefpulse", "cefpulsesub.exe") as exe_path:
    cefpulse.Initialize(str(exe_path))
