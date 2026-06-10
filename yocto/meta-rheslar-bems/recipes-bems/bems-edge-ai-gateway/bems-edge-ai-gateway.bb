SUMMARY = "BEMS edge AI gateway service"
LICENSE = "MIT"
SRC_URI = "git://github.com/rheslar1/bems-edge-ai-gateway.git;protocol=https;branch=main \
           file://bems-edge-ai-gateway.service"
SRCREV = "AUTOREV"

S = "${WORKDIR}/git"

inherit cmake systemd

SYSTEMD_SERVICE:${PN} = "bems-edge-ai-gateway.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

do_install:append() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/bems-edge-ai-gateway.service ${D}${systemd_system_unitdir}/
}
