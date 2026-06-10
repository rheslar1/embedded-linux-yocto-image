# Validation Plan

## Current Host Checks

- CMake configure completes.
- C++17 Yocto image validator builds.
- Executable accepts the i.MX93 BEMS appliance image with RAUC OTA.
- CTest verifies BEMS recipe presence, BEMS systemd enablement, secure boot policy, and OTA rollback policy.
- GitHub Actions runs configure, build, executable smoke run, and CTest.

## Hardware Evidence To Add

- BitBake build log for the BEMS image.
- `systemctl status bems-edge-ai-gateway` from first boot.
- RAUC/SWUpdate/Mender slot status before and after update.
- Device tree and kernel config evidence for the target board.
- Read-only rootfs mount evidence.
- CI screenshot after the public repository is pushed.

## Project-Specific Evidence Target

Board-support fluency, package ownership, appliance-style Linux builds, and reproducible edge deployment.
