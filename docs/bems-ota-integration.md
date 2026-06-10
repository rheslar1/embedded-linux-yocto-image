# BEMS and OTA Yocto Integration

## Image Intent

This image profile models an i.MX93 BEMS appliance:

- BEMS edge gateway from `https://github.com/rheslar1/bems-edge-ai-gateway`
- BACnet/IP polling and RabbitMQ telemetry
- TLS certificates in the rootfs
- systemd service ownership
- read-only root filesystem
- RAUC A/B rootfs OTA with bootloader mark-good and rollback

## Example Image Append

```bitbake
IMAGE_INSTALL:append = " bems-edge-ai-gateway rauc ca-certificates openssh"
IMAGE_FEATURES:append = " read-only-rootfs"
DISTRO_FEATURES:append = " systemd"
VIRTUAL-RUNTIME_init_manager = "systemd"
```

## BEMS Service

```ini
[Unit]
Description=BEMS Edge AI Gateway
After=network-online.target
Wants=network-online.target

[Service]
ExecStart=/usr/bin/bems-edge-ai-gateway --config /etc/bems/gateway.toml
Restart=on-failure
WatchdogSec=30

[Install]
WantedBy=multi-user.target
```

## OTA Layout

- `rootfs.0`: active root filesystem
- `rootfs.1`: inactive root filesystem
- signed RAUC bundle installs to inactive slot
- bootloader marks the new slot good after health validation
- failed health validation rolls back to the previously confirmed slot
