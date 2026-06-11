# Embedded Linux / Yocto Image Design Package

## Purpose

Custom Linux image for Raspberry Pi, BeagleBone, or i.MX-class hardware with Yocto recipes, kernel config, service units, and rootfs notes.

This package defines the project as an implementation-ready embedded system. It covers system architecture, requirements, interface boundaries, runtime design, validation evidence, and phased delivery.

## Project Profile

| Field | Value |
| --- | --- |
| Repository | `rheslar1/embedded-linux-yocto-image` |
| Primary stack | C++17, C++ Design Patterns, SOLID, Yocto, BitBake, Kernel config, Systemd, Device tree, Rootfs |
| Review proof point | Board-support fluency, package ownership, appliance-style Linux builds, and reproducible edge deployment. |

## Artifacts

- [System Design](system-design.md)
- [Requirements](requirements.md)
- [Interface Control](interface-control.md)
- [Runtime Design](runtime-design.md)
- [Validation Plan](validation-plan.md)
- [Implementation Roadmap](implementation-roadmap.md)
- [Draw.io UML](diagrams/system-design.drawio)
- [PNG UML](diagrams/system-design.png)
