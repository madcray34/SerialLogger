# SerialLogger

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C%2B%2B-20-blue)]()
[![Zig](https://img.shields.io/badge/build-Zig-orange)]()

## Overview
**SerialLogger** is a serial communication logging tool written in modern C++ (C++20). It auto-detects serial ports and logs incoming data to the console. The current focus is a minimal Linux console application; a graphical interface and full Windows port are future tasks.

## Features
- **C++20**: Modern C++ core.
- **Console-based UI**: No GUI dependencies; data is printed to `stdout`.
- **Boost.Asio**: Async serial I/O.
- **Modular Architecture**: Core/application split with clean interfaces.

## Folder Structure
```
SerialLogger/
│-- build.zig              # Zig build script
│-- build.zig.zon          # Zig package manifest
│-- external/              # Cached dependency tarballs
│
├── application/           # Application source code
│   ├── include/           # App-level headers
│   ├── src/               # Console main and supervisor
│   └── modules/NetlibApp/ # Boost.Asio-based serial implementation
├── common/NetlibCore/     # Core interfaces and shared code
└── tests/                 # Unit tests
```

## Build Instructions

### Prerequisites
- **Zig** (tested with 0.16.0)
- **C++20 Compiler** (Zig bundles `clang`/`libc++`, but GCC/Clang also work)

Dependencies (Boost and GoogleTest) are fetched automatically by the Zig package manager.

### Linux
```sh
git clone https://github.com/madcray34/SerialLogger.git
cd SerialLogger
zig build
zig build test
./zig-out/bin/SerialLogger
```

If `zig build` fails to download dependencies over HTTPS (some environments have TLS issues with the Zig toolchain), pre-download them with curl:
```sh
./external/fetch-deps.sh
zig build
```

## Usage
```sh
./zig-out/bin/SerialLogger
```
The program scans for `/dev/ttyUSB*` and `/dev/ttyACM*` devices every 5 seconds and prints any data received from open ports to the console. Press `Ctrl-C` to stop.

## Dependencies
- **Boost.Asio** (serial I/O)
- **GoogleTest** (unit tests)

The following dependencies were removed to keep the project minimal:
- fmt, GLFW, ImGui, ImPlot, OpenGL

## Notes
- The real Linux serial-port enumerator is a stub for now; the architecture is ready for a proper implementation in the next step.
- The old Windows `SetupAPI` COM-port scanner is preserved for a future Windows port.

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
For issues, feature requests, or general inquiries, please open an [issue](https://github.com/madcray34/serialLogger/issues).

---
*Made with ❤️ by madcray34*
