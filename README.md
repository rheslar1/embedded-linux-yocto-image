# Embedded Linux / Yocto Image

Custom Linux image for Raspberry Pi, BeagleBone, or i.MX-class hardware with Yocto recipes, kernel config, service units, and rootfs notes.

## Portfolio Purpose

This repository implements a host-testable Yocto image composition model for an i.MX93-class BEMS edge appliance. It validates board profile, kernel/device tree ownership, read-only rootfs, systemd, the BEMS edge AI gateway recipe and service, and signed RAUC-style A/B OTA readiness.

## Stack

- C++17
- C++ Design Patterns
- SOLID
- Yocto
- BitBake
- Kernel config
- Systemd
- Device tree
- Rootfs
- BEMS
- OTA
- RAUC

## Quick Start

```bash
cmake -S . -B build
cmake --build build
./build/embedded_linux_yocto_image
ctest --test-dir build --output-on-failure
```

## Implementation Slices

- i.MX93 machine profile with kernel provider and device tree checks.
- Read-only rootfs and systemd policy validation.
- BEMS edge gateway recipe/service integration from `rheslar1/bems-edge-ai-gateway`.
- BACnet/IP, RabbitMQ, TLS, and watchdog requirements for the BEMS service.
- RAUC-style signed A/B rootfs OTA policy with rollback and bootloader mark-good checks.
- Example Yocto layer files under `yocto/meta-rheslar-bems`.
- CTest coverage for accepted image composition, missing BEMS recipe, disabled BEMS service, missing secure boot, and OTA rollback policy.

## Evidence Target

Board-support fluency, package ownership, appliance-style Linux builds, and reproducible edge deployment.

## Remote

Intended public repository: https://github.com/rheslar1/embedded-linux-yocto-image

<!-- cpp17-solid-implementation:start -->
## C++17, Design Patterns, and SOLID Implementation

This repository includes a host-buildable C++17 implementation, not only documentation. The implementation applies:

- Strategy pattern for validation rules.
- Adapter interfaces for input samples and telemetry/reporting.
- Composite validation for combining safety and readiness checks.
- Facade orchestration through the project runtime class.
- SOLID boundaries between profile data, input acquisition, validation, telemetry encoding, and tests.
<!-- cpp17-solid-implementation:end -->

<!-- deep-architecture-links:start -->
## Deep Architecture and UML

- [Deep architecture](docs/deep-architecture.md)
- [Full UML Draw.io source](docs/diagrams/full-system-uml.drawio)
- [Full UML PNG export](docs/diagrams/full-system-uml.png)
<!-- deep-architecture-links:end -->

<!-- DESIGN_PACKAGE_START -->
## Detailed Design Package

This repository includes a structured design package for **Embedded Linux / Yocto Image**. The package captures the system boundary, runtime flow, hardware/software interfaces, validation strategy, and implementation roadmap.

| Artifact | Link |
| --- | --- |
| Design Index | [docs/design/README.md](docs/design/README.md) |
| System Design | [docs/design/system-design.md](docs/design/system-design.md) |
| Requirements | [docs/design/requirements.md](docs/design/requirements.md) |
| Interface Control | [docs/design/interface-control.md](docs/design/interface-control.md) |
| Runtime Design | [docs/design/runtime-design.md](docs/design/runtime-design.md) |
| Validation Plan | [docs/design/validation-plan.md](docs/design/validation-plan.md) |
| Implementation Roadmap | [docs/design/implementation-roadmap.md](docs/design/implementation-roadmap.md) |
| Draw.io UML | [docs/design/diagrams/system-design.drawio](docs/design/diagrams/system-design.drawio) |
| PNG UML | [docs/design/diagrams/system-design.png](docs/design/diagrams/system-design.png) |
<!-- DESIGN_PACKAGE_END -->
