# Design Notes

The kernel module uses Linux's `miscdevice` helper, which creates a character device with a dynamically assigned minor number and a predictable `/dev/hw_health` name. Its `read` callback formats a single snapshot and returns end-of-file on later reads.

The application uses a narrow text protocol: one `key=value` pair per line. This keeps the driver easy to inspect and makes the user/kernel boundary visible. The application validates required fields before displaying values.

This design demonstrates privileged versus unprivileged execution, device registration, file operations, kernel data structures, system calls, parsing, error handling, and build automation.

