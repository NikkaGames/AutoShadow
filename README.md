# AutoShadow

AutoShadow is a native Android IL2CPP/Unity runtime instrumentation tool. It hooks into the Unity engine using JNI, OpenGL, and IL2CPP exported symbols to overlay a real-time graphical interface for debugging, reverse engineering, and dumping game structures.

> **Author:** Nikka  
> **Target Platforms:** Android (Unity IL2CPP-based games)  
> **Language:** C++  
> **Core Frameworks:** ImGui, OpenGL ES 2.0, JNI, IL2CPP API, Unity

---

## Features

- **Runtime IL2CPP Symbol Resolution:**  
  Dynamically resolves and binds exported IL2CPP functions via `xdl_sym()` from a shared object.

- **Unity Object Access & Manipulation:**  
  Direct integration with Unity structures like `Vector2`, `Vector3`, `Quaternion`, and Unity objects for rendering or data extraction.

- **ImGui Overlay (OpenGL ES):**  
  Renders a native ImGui interface on top of Unity content using `imgui_impl_opengl3` and `imgui_impl_android`.

- **Game Dumping Toggle:**  
  Controlled by the `dump_game` flag—when enabled, allows runtime analysis or dumping of internal data.

- **Cross-structure Vector Support:**  
  Includes custom and Unity-specific vector math headers (e.g., `Vector3Silent`, `Vector3idk.hpp`, `Vec3.h`) for compatibility across various Unity versions.

---

## Project Structure

```
AutoShadow/
├── il2cpp_dump.cpp        # Core implementation file (entry point)
├── il2cpp_dump.h          # Declarations for IL2CPP dumping
├── log.h                  # Logging utilities
├── utilities.h            # Custom helper functions
├── Unity/                 # Unity structure definitions
│   ├── Vector3.h
│   ├── Quaternion.h
│   └── ...
├── ImGui/                 # ImGui core and backend implementation
│   ├── imgui.h
│   ├── imgui_impl_android.h
│   ├── imgui_impl_opengl3.h
│   └── ...
```

---

## Dependencies

- [ImGui](https://github.com/ocornut/imgui) (with Android + OpenGL ES backend)
- Android NDK
- Custom IL2CPP headers (`il2cpp-tabledefs.h`, `il2cpp-api` macros)
- Unity native structure mappings

---

## How It Works

### IL2CPP Symbol Hooking
```cpp
#define DO_API(r, n, p) r (*n) p
#include IL2CPPAPI
#undef DO_API
```
Symbols are resolved using:
```cpp
n = (r (*) p)xdl_sym(il2cpp_handle, #n, NULL);
```

### OpenGL Overlay Rendering
- Utilizes EGL and GLES2 to hook into the Unity rendering context.
- Renders an ImGui overlay on top of the active Unity scene.

---

## Requirements

- Android device (rooted or with appropriate injection method)
- Target game built with Unity + IL2CPP
- Compiled as `.so` and injected into the Unity game process

---

## Build Instructions

1. Clone this repo.
2. Setup Android NDK environment.
3. Compile as a shared object (`.so`) using CMake or ndk-build.
4. Copy and replace original (`libmain.so`) with the compiled (`libmain.so`) in app lib directory.

---

## Disclaimer

This project is intended for **educational and debugging** purposes only. Usage in production games or for unauthorized memory access may violate terms of service or legal agreements. Use responsibly.

---

## Credits
  
- ImGui contributors  
- IL2CPP symbol research contributors  
- Maintained by **[@NikkaGames](https://github.com/NikkaGames)**
