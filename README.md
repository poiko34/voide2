# Voide Editor 2.0 🖊️

**Voide** is a lightweight, high-performance cross-platform terminal text editor written in C++17. It interfaces directly with terminal drivers to provide a "Raw Mode" editing experience on both UNIX-like systems (Linux, macOS) and Windows.

---

## ✨ Features

* **True Cross-Platform**: Native support for Linux (termios) and Windows (WinAPI)
* **Zero Dependencies**: Built using standard C++ libraries and system headers
* **Raw Input Handling**: Immediate response to keypresses without waiting for `Enter`
* **ANSI Color Support**: High-contrast interface with a dedicated status bar
* **Static Cross-Compilation**: Build standalone `.exe` from Linux using MinGW-w64
* **Resource Management**: Uses RAII to safely restore terminal state on exit

---

## ⌨️ Controls

| Key        | Action                             |
| ---------- | ---------------------------------- |
| Arrow Keys | Navigate through text              |
| Ctrl + S   | Save current file                  |
| Ctrl + C   | Exit (with unsaved changes prompt) |

---

## 🚀 Getting Started

### Prerequisites

* CMake 3.10 or higher
* C++17-compatible compiler (GCC, Clang, or MSVC)

---

### 🔧 Local Build (Linux / Windows)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

---

### 🪟 Cross-Compiling for Windows (from Linux)

Requires `mingw-w64`.

```bash
mkdir build_win
cd build_win
cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake
make
```

---

## 📂 Project Structure

```
src/
 ├── main.cpp               # Main event loop and document orchestration
 ├── terminal_manager.cpp  # Raw mode handling
 ├── keycode.cpp           # Input parsing
 └── init.cpp              # Platform-specific initialization
```

---

## 🛠️ Technical Implementation

* **Linux**: Configures `termios` (disables `ECHO`, `ICANON`)
* **Windows**: Uses `SetConsoleMode` with `ENABLE_VIRTUAL_TERMINAL_PROCESSING`
* **Rendering**: Optimized ANSI escape sequences for minimal redraw

---

## 📌 Notes

* Designed for performance and minimalism
* No external libraries required
* Works directly with terminal I/O

---

## 📜 License

This project is licensed under the MIT License — free to use, modify and distribute.
