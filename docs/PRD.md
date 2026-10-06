# Project Requirements Document

## Problem

Beginners often see Linux commands and hardware concepts separately. This project shows how kernel-managed device information can be exposed through a device interface and consumed by a C++ program.

## Functional requirements

1. Register a Linux character device named `hw_health`.
2. Return CPU, memory and uptime values on `read()`.
3. Read the device from a C++17 client using POSIX system calls.
4. Print a human-readable report.
5. Provide a no-root demo mode and repeatable tests.

## Non-functional requirements

- Linux only; C/C++ only.
- Read-only and safe by default.
- Small enough for individual implementation and explanation.
- Buildable from a clean Git checkout with `make`.

## Six-stage plan

| Stage | Evidence in this repository |
|---|---|
| 1. Introduction | README sections 1–2 |
| 2. Requirements and plan | This PRD |
| 3. Design and architecture | DESIGN.md and UML.md |
| 4. Prototype | driver/hw_health.c and app/main.cpp |
| 5. Testing and integration | tests/ and `make test` |
| 6. Final presentation | README presentation plan and demo commands |
