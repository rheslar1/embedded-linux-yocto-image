# Embedded Linux / Yocto Image Architecture

## Goal

Board-support fluency, package ownership, appliance-style Linux builds, and reproducible edge deployment.

## Runtime Shape

1. The image declares an i.MX93 machine, distro, kernel provider, and device tree.
2. Rootfs policy requires systemd, secure boot posture, and read-only rootfs behavior.
3. The BEMS gateway recipe pulls `rheslar1/bems-edge-ai-gateway` into the image.
4. The BEMS systemd unit waits for network-online and restarts on failure.
5. OTA policy installs signed RAUC/SWUpdate/Mender artifacts to the inactive rootfs slot.
6. Validation evidence records image readiness before a full BitBake build.

## C++17 Design Shape

- `YoctoImageValidator` owns image composition checks.
- `MachineProfile`, `PackageRecipe`, `SystemdUnit`, `BemsGatewayConfig`, and `OtaConfig` make the image contract explicit.
- `TextImageReporter` emits reviewable CI evidence.
- Yocto snippets under `yocto/` document the eventual layer/recipe shape.

## SOLID Notes

- Single Responsibility: machine, packages, units, BEMS, OTA, and reporting are separated.
- Open/Closed: additional machines, OTA backends, or services can be added through data without changing tests broadly.
- Liskov Substitution: RAUC, SWUpdate, and Mender share the same OTA contract.
- Interface Segregation: the validator consumes focused records, not a monolithic config blob.
- Dependency Inversion: future BitBake parsers can feed the same validation model.

## Boundaries

- `include/yocto_image/`: image composition model.
- `src/`: validator, reporter, and CLI demo.
- `yocto/`: sample Yocto layer, BEMS recipe, and systemd unit.
- `docs/`: validation plans, timing notes, hardware captures, and acceptance evidence.
- `tests/`: host-side tests for image composition gates.
- `.github/workflows/`: CI entry point for build and validation evidence.

## Validation Plan

- Build the host image validator with CMake.
- Run the executable and confirm the BEMS/OTA appliance image is accepted.
- Run CTest to validate BEMS recipe, systemd, secure boot, and OTA policies.
- Add BitBake build logs, boot logs, and RAUC/SWUpdate evidence after Yocto integration.
- Capture CI, terminal, and hardware evidence for the portfolio detail page.

## Expansion Notes

- Connect the host model to parsed BitBake metadata.
- Expand `meta-rheslar-bems` with kernel config fragments, device tree overlays, and OTA slot configuration.
- Reuse BEMS gateway CI artifacts as Yocto package inputs once release artifacts exist.

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
