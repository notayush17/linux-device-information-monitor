# Linux Device Information Monitor

**Individual capstone project — Linux, device drivers, system programming and C++**

## 1. Project introduction

Linux Device Information Monitor is a small command-line application that reads a snapshot of Linux device and system information through a Linux character device. A kernel module creates `/dev/hw_health`; the C++ program opens that device with the POSIX `open()`/`read()` system calls and displays the result.

The project demonstrates the path from hardware information maintained by the Linux kernel to a user-space program. It is intentionally small enough to explain in 5–10 minutes.

## 2. Features

- Linux misc character-device driver.
- Driver reports CPU count, online CPU count, memory totals, and kernel uptime.
- C++17 command-line client using `open`, `read`, and `close`.
- A `--demo` mode for demonstrating the client without loading a module.
- Unit-style parser test and an end-to-end smoke-test script.

## 3. Requirements

- The final project runs on Linux because Linux kernel modules are not supported by macOS.
- macOS can be used to edit the project, run the C++ demo, run tests, and upload to GitHub.
- `g++`, `make`, and Linux kernel headers for the module.
- Root/sudo access only for loading and unloading the module.

No Python, Java, web framework, or external library is used.

### macOS development note

On macOS, run `make test` and `./build/hw_monitor --demo` to verify the C++ portion. Do not run `make module` on macOS; the driver must be compiled on Linux with matching Linux kernel headers. For the complete demonstration, copy or clone this repository into a Linux virtual machine or use a Linux computer.

## 4. Build and run

```sh
make
./build/hw_monitor --demo
```

To run the real kernel-module demonstration:

```sh
make module
sudo insmod build/hw_health.ko
./build/hw_monitor
sudo rmmod hw_health
```

If the device is missing, the client prints a helpful error. The module can be inspected with:

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
  K --> D[hw_health.ko\n/misc character driver]
  D -->|read()| A[hw_monitor C++ client]
  A --> T[Terminal output]
```

The driver is the boundary between privileged kernel space and unprivileged user space. The client does not access hardware registers directly; it requests a formatted snapshot from the kernel.

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

The test validates parsing of the driver's key/value response and runs the demo mode. Integration testing is completed by loading the module and running `./build/hw_monitor` against `/dev/hw_health`.

## 9. Limitations and future improvements

This is a read-only educational driver. It does not control a physical sensor, change hardware state, or provide alarms. A future version could add a real temperature sensor, polling with `poll()`, permissions via udev, and a writable configuration interface.

## 10. Five-minute presentation plan

1. State the problem and show the architecture diagram.
2. Point out `misc_register()` and the driver's `read` callback.
3. Point out the C++ `open()` and `read()` calls.
4. Run `make test`, then `./build/hw_monitor --demo`.
5. If kernel headers and sudo are available, load the module and repeat without `--demo`.
