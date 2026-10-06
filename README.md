# Linux Device Information Monitor

**Individual capstone project — Linux, device drivers, system programming, and C++**

## 1. Project introduction

Linux Device Information Monitor is a small command-line application that reads CPU, memory, and uptime information through a Linux character device. A kernel module creates `/dev/hw_health`; the C++ program opens that device and reads the information using the POSIX `open()` and `read()` system calls.

The project demonstrates the path from information maintained by the Linux kernel to a user-space program. It is intentionally small enough to understand and explain in 5–10 minutes.

## 2. Features

- Linux miscellaneous character-device driver.
- Reports the total CPU count, online CPU count, total memory, free memory, and system uptime.
- C++17 command-line client using `open()`, `read()`, and `close()`.
- A `--demo` mode for trying the client without loading the kernel module.
- Parser and smoke tests for the user-space application.

## 3. Requirements

- Linux for the complete project and kernel-module demonstration.
- `g++` and `make` for the C++ application.
- Linux kernel headers for building the module.
- Root or `sudo` access for loading and unloading the module.

No Python, Java, web framework, or external library is used.

### macOS development note

macOS can be used to edit the project, run the C++ tests and demo, and upload the repository to GitHub. Run `make test` and `make demo` on macOS. Do not run `make module` on macOS; the driver must be compiled on Linux with matching Linux kernel headers. For the complete demonstration, copy or clone this repository into a Linux virtual machine or use a Linux computer.

## 4. Build and run

### User-space demo

```sh
make
./build/hw_monitor --demo
```

The demo mode uses sample values and does not require root access or a loaded kernel module.

### Real Linux device demonstration

Run these commands on Linux:

```sh
make module
sudo insmod build/hw_health.ko
./build/hw_monitor
sudo rmmod hw_health
```

If `/dev/hw_health` is not available, the client reports an error and suggests using demo mode or loading the module. The module can be inspected with:

```sh
cat /proc/devices
dmesg | tail -20
ls -l /dev/hw_health
```

## 5. Example output

```text
Linux Device Information Monitor
-----------------------------
Source        : kernel module (/dev/hw_health)
CPU cores     : 8
Online CPUs   : 8
Memory total  : 15872 MB
Memory free   : 6210 MB
Kernel uptime : 4312 seconds
```

## 6. Architecture

```mermaid
flowchart LR
  H[CPU and RAM hardware] --> K[Linux kernel]
  K --> D[hw_health.ko<br/>misc character driver]
  D -->|read()| A[hw_monitor C++ client]
  A --> T[Terminal output]
```

The driver is the boundary between privileged kernel space and unprivileged user space. The client does not access kernel memory or hardware registers directly; it requests a formatted snapshot from the kernel through the device file.

## 7. Repository structure

```text
.
├── app/main.cpp              # C++17 user-space client
├── driver/hw_health.c        # Linux misc character driver
├── tests/test_parser.cpp     # small C++ parser test
├── tests/smoke_test.sh       # build/run smoke test
├── docs/PRD.md               # requirements and six-stage plan
├── docs/DESIGN.md            # design decisions and data flow
├── docs/UML.md               # class, sequence and state diagrams
├── Makefile
└── README.md
```

## 8. Testing

```sh
make test
```

The parser test checks the key-value response format, and the smoke test runs the demo mode. Integration testing is completed on Linux by loading the module and running `./build/hw_monitor` against `/dev/hw_health`.

## 9. Limitations and future improvements

This is a read-only educational driver. It does not control a physical sensor, change hardware state, or provide alarms. A future version could add a real temperature sensor, polling with `poll()`, permissions via udev, and a writable configuration interface.

## 10. Five-minute presentation plan

1. State the problem and show the architecture diagram.
2. Explain that `misc_register()` creates the character device.
3. Point out the driver's `read` callback and the C++ `open()` and `read()` calls.
4. Explain the repository structure and testing approach.
5. If a Linux system is available, load the module and run the real demonstration.
