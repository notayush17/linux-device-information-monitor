# Linux Device Information Monitor

**Individual capstone project — Linux, device drivers, system programming, and C++**

## 1. Project introduction

Linux Device Information Monitor is a small command-line application that reads CPU, memory, and uptime information through a Linux character device. A kernel module creates `/dev/hw_health`; the C++ program opens that device and reads the information using the POSIX `open()` and `read()` system calls.

The project demonstrates the path from information maintained by the Linux kernel to a user-space program. It is intentionally small enough to understand and explain in 5–10 minutes.

## 2. Features

- A Linux miscellaneous character-device driver that registers `/dev/hw_health` and provides a read-only interface for obtaining system information from the Linux kernel.
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

```text
CPU and RAM information
          |
          v
    Linux kernel
          |
          v
  hw_health.ko
  character driver
          |
          v
 /dev/hw_health
          |
          v
 C++ user-space client
          |
          v
   Terminal output
```

The driver is the boundary between privileged kernel space and unprivileged user space. The client does not access kernel memory or hardware registers directly; it requests a formatted snapshot from the kernel through the device file.

### Layered architecture

```text
+-------------------------------------------------------------+
|                    User Space                              |
|  hw_monitor (C++17 client)                                 |
|  - opens the device file                                   |
|  - reads the snapshot                                      |
|  - parses key=value data                                   |
+----------------------------+--------------------------------+
                             |  open() / read() / close()
+----------------------------v--------------------------------+
|                    Device Interface                        |
|  /dev/hw_health                                             |
|  Read-only character device                                |
+----------------------------+--------------------------------+
                             |  file_operations.read
+----------------------------v--------------------------------+
|                    Kernel Space                            |
|  hw_health.ko                                               |
|  - registers the misc device                               |
|  - collects CPU, memory, and uptime values                 |
|  - copies the formatted result to user space               |
+----------------------------+--------------------------------+
                             |
+----------------------------v--------------------------------+
|                    Linux Kernel Data                       |
|  CPU information | memory pages | system jiffies           |
+-------------------------------------------------------------+
```

### Read-request flow

```text
  User runs the program
          |
          v
  C++ client calls open("/dev/hw_health")
          |
          v
  Linux selects the driver's read callback
          |
          v
  Driver collects CPU, memory, and uptime values
          |
          v
  Driver creates a key=value text snapshot
          |
          v
  read() copies the snapshot to the C++ client
          |
          v
  Client parses the values and prints the report
```

### Device lifecycle

```text
        +----------+
        | Unloaded |
        +----+-----+
             | sudo insmod
             v
        +----------+
        | Ready    | <------------------+
        +----+-----+                    |
             | read()                   | read completed
             v                          |
        +----------+                    |
        | Reading  | -------------------+
        +----+-----+
             |
             | sudo rmmod
             v
        +----------+
        | Unloaded |
        +----------+
```

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

### Limitations

- The driver only reads information; it does not control hardware.
- The project does not include a graphical user interface.
- The driver does not generate alerts for high or low values.
- A Linux system with matching kernel headers is required for the kernel-module demonstration.

### Future improvements

- Add support for reading temperature sensors.
- Add alert messages when system values cross defined limits.
- Add polling with `poll()` so the application can receive updates.
- Add udev rules for clearer device permissions.
- Add a writable configuration interface.
