# SerialLogger

SerialLogger is a C++20 GLFW/ImGui application for discovering serial ports and logging their
data. Windows uses SetupAPI for discovery. Ubuntu 24.04 uses `libudev` to discover USB serial
devices such as `/dev/ttyUSB0` and `/dev/ttyACM0`.

## Supported platforms

| Platform | Build and GUI | Serial discovery |
| --- | --- | --- |
| Windows | Supported | SetupAPI |
| Ubuntu 24.04 | Supported | `libudev` |
| Ubuntu 24.04 under WSL | Build, tests, and GUI smoke testing supported | Depends on separately configured USB passthrough |
| macOS | Not supported | Not implemented |

WSL is useful for development but does not automatically expose Windows COM or USB devices as
Linux `/dev/tty*` devices. WSLg can display the GUI when it is available. Physical serial-device
validation must be completed on a native Ubuntu installation or a WSL setup with USB passthrough.

## Prerequisites

- CMake 3.28 or newer
- A C++20 compiler
- [vcpkg](https://github.com/microsoft/vcpkg), bootstrapped and available through `VCPKG_ROOT`

On Ubuntu 24.04, install the system libraries required by `libudev`, GLFW, and OpenGL:

```bash
sudo apt-get update
sudo apt-get install --yes \
  build-essential cmake pkg-config libudev-dev \
  libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
  libgl1-mesa-dev xvfb
```

vcpkg is cross-platform. It supplies the portable C++ dependencies declared in `vcpkg.json`
(`fmt`, GLFW, ImGui, Boost, and GoogleTest); `libudev-dev` is an Ubuntu system dependency.

## Install vcpkg

Choose a location outside the repository and set `VCPKG_ROOT` to it:

```bash
git clone --branch 2026.07.29 --depth 1 https://github.com/microsoft/vcpkg.git "$HOME/vcpkg"
"$HOME/vcpkg/bootstrap-vcpkg.sh" -disableMetrics
export VCPKG_ROOT="$HOME/vcpkg"
```

On Windows PowerShell:

```powershell
git clone --branch 2026.07.29 --depth 1 https://github.com/microsoft/vcpkg.git $HOME\vcpkg
& $HOME\vcpkg\bootstrap-vcpkg.bat -disableMetrics
$env:VCPKG_ROOT = "$HOME\vcpkg"
```

## Build and test

### Ubuntu 24.04 and WSL

```bash
export VCPKG_ROOT="$HOME/vcpkg"
cmake --preset linux-release -DENABLE_TESTS=ON
cmake --build --preset linux-release --target SerialLogger netcore_tests netlibapp_linux_tests
ctest --preset linux-release --output-on-failure
```

The executable is `build/linux-release/bin/SerialLogger`. In a headless environment, verify
startup with:

```bash
timeout 5s xvfb-run -a build/linux-release/bin/SerialLogger
```

An exit status of `124` means the application remained running until the expected timeout.

### Windows

```powershell
$env:VCPKG_ROOT = "$HOME\vcpkg"
cmake --preset windows-release -DENABLE_TESTS=ON
cmake --build --preset windows-release --target SerialLogger netcore_tests
ctest --preset windows-release --output-on-failure
```

## Native Ubuntu serial-device acceptance

On a native Ubuntu 24.04 machine:

1. Connect a USB serial device and confirm a `/dev/ttyUSB*` or `/dev/ttyACM*` node exists.
2. Ensure the current user may access serial devices:

   ```bash
   groups
   sudo usermod -aG dialout "$USER"
   ```

   Log out and back in after modifying group membership.
3. Run `build/linux-release/bin/SerialLogger`.
4. Confirm the connected device is discovered, can be opened, and incoming data is logged.

Use `ls -l /dev/ttyUSB* /dev/ttyACM*` to inspect permissions. Absence of such devices in WSL is
normal and is not a SerialLogger failure.
