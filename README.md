# SerialLogger

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![C++](https://img.shields.io/badge/C%2B%2B-20-blue)]()
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20MacOS-lightgrey)]()

## Overview
**SerialLogger** is a high-performance, cross-platform serial communication logging tool written in modern C++ (C++20). It is designed to efficiently log and analyze serial data streams, making it an ideal solution for embedded systems debugging, hardware communication analysis, and industrial automation.

## Features
- **Cross-Platform Support**: Runs on Windows, Linux, and macOS.
- **C++20**: Leveraging the latest C++ features for better performance and maintainability.
- **Modular Architecture**: Easily extendable and customizable.
- **ImGui-based GUI**: User-friendly graphical interface for data visualization.
- **Boost Asio for Networking**: Provides networking capabilities for remote logging.
- **Lightweight & Fast**: Optimized for high-performance logging.

## Folder Structure
```
SerialLogger/
│-- CMakeLists.txt          # Project build configuration
│-- prepare.bat             # Preparation script
│-- vcpkg.json              # Dependencies configuration
│
├── app/                    # Application source code
├── cmake/                  # CMake utilities and scripts
├── external/               # External dependencies
├── modules/                # Core modules
├── tools/                  # Utility scripts and tools
```

## Build Instructions
### Prerequisites
Ensure you have the following installed:
- **CMake (>= 3.30)**
- **C++20 Compiler** (GCC, Clang, or MSVC)
- **Vcpkg** (for dependency management)
- **Boost, fmt, GLFW, ImGui, ImPlot, OpenGL** (handled via vcpkg)

### Build Steps
#### Linux/macOS
```sh
git clone https://github.com/madcray34/SerialLogger.git
cd SerialLogger
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
make -j$(nproc)
```

#### Windows (Powershell)
```powershell
git clone https://github.com/madcray34/SerialLogger.git
cd SerialLogger
mkdir build; cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

## Usage
After building, you can run SerialLogger:
```sh
./SerialLogger
```

## Configuration Options
SerialLogger provides multiple configuration options in `CMakeLists.txt`:
- `ENABLE_WARNINGS` (ON/OFF) - Enables compiler warnings.
- `ENABLE_WARNINGS_AS_ERRORS` (ON/OFF) - Treats warnings as errors.
- `ENABLE_CLANG_FORMAT` (ON/OFF) - Enables Clang format enforcement.
- `ENABLE_CLANG_TIDY` (ON/OFF) - Enables Clang static analysis.
- `ENABLE_LTO` (ON/OFF) - Enables Link Time Optimization for better performance.

## Dependencies
SerialLogger uses several external libraries:
- **Boost** (circular_buffer, asio, system)
- **fmt** (formatting library)
- **GLFW** (window management)
- **ImGui** (GUI framework)
- **ImPlot** (plotting library for ImGui)
- **OpenGL** (rendering backend)

All dependencies are managed via [vcpkg](https://vcpkg.io/).

## Contribution
Contributions are welcome! Please follow these steps:
1. Fork the repository.
2. Create a new branch (`feature-branch`).
3. Commit your changes.
4. Push to your fork.
5. Open a pull request.

## License
This project is licensed under the [MIT License](LICENSE).

## Contact
For issues, feature requests, or general inquiries, please open an [issue](https://github.com/madcray34/SerialLogger/issues) or reach out via email.

---
*Made with ❤️ by madcray34*

