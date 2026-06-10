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
