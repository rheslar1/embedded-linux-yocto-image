#include "yocto_image/YoctoImage.hpp"

#include <algorithm>
#include <ostream>
#include <string_view>
#include <utility>

namespace yocto_image {
namespace {

bool startsWith(std::string_view value, std::string_view prefix) {
  return value.substr(0U, prefix.size()) == prefix;
}

void addCheck(ImageReport& report,
              std::string step,
              const bool passed,
              std::string detail) {
  report.checks.push_back(
      ImageCheck{std::move(step),
                 passed ? CheckStatus::Pass : CheckStatus::Fail,
                 std::move(detail)});
}

bool hasRecipe(const std::vector<PackageRecipe>& recipes, std::string_view name) {
  return std::any_of(recipes.begin(), recipes.end(), [&](const auto& recipe) {
    return recipe.name == name && recipe.installed;
  });
}

bool hasEnabledUnit(const std::vector<SystemdUnit>& units,
                    std::string_view name,
                    const SystemdUnit** found = nullptr) {
  const auto unit = std::find_if(units.begin(), units.end(), [&](const auto& item) {
    return item.name == name && item.enabled;
  });
  if (unit == units.end()) {
    return false;
  }
  if (found != nullptr) {
    *found = &(*unit);
  }
  return true;
}

bool validateMachine(const MachineProfile& machine, std::string& reason) {
  if (machine.machine.empty() || machine.distro.empty()) {
    reason = "machine and distro must be set";
    return false;
  }

  if (!machine.systemd) {
    reason = "BEMS appliance image requires systemd";
    return false;
  }

  if (!machine.secureBoot) {
    reason = "secure boot must be enabled for OTA-capable appliance image";
    return false;
  }

  if (!machine.readOnlyRootfs) {
    reason = "rootfs must be read-only with writable state partitions";
    return false;
  }

  if (machine.kernelProvider.empty() || machine.deviceTree.empty()) {
    reason = "kernel provider and device tree must be declared";
    return false;
  }

  reason = machine.machine + " using " + machine.kernelProvider + " and " +
           machine.deviceTree;
  return true;
}

bool validateBems(const std::vector<PackageRecipe>& recipes,
                  const std::vector<SystemdUnit>& units,
                  const BemsGatewayConfig& bems,
                  std::string& reason) {
  if (!startsWith(bems.repositoryUrl,
                  "https://github.com/rheslar1/bems-edge-ai-gateway")) {
    reason = "BEMS repository URL must point to rheslar1/bems-edge-ai-gateway";
    return false;
  }

  if (!hasRecipe(recipes, "bems-edge-ai-gateway")) {
    reason = "bems-edge-ai-gateway recipe is not installed";
    return false;
  }

  const SystemdUnit* unit = nullptr;
  if (!hasEnabledUnit(units, bems.serviceName, &unit)) {
    reason = "BEMS systemd service is not enabled";
    return false;
  }

  if (!unit->afterNetworkOnline || !unit->restartOnFailure) {
    reason = "BEMS service must wait for network-online and restart on failure";
    return false;
  }

  if (!bems.bacnetIp || !bems.rabbitMq || !bems.tls || !bems.watchdog) {
    reason = "BEMS requires BACnet/IP, RabbitMQ, TLS, and watchdog integration";
    return false;
  }

  reason = "BEMS gateway recipe and service are installed";
  return true;
}

bool validateOta(const std::vector<PackageRecipe>& recipes,
                 const OtaConfig& ota,
                 std::string& reason) {
  const std::string backendPackage =
      ota.backend == OtaBackend::Rauc
          ? "rauc"
          : (ota.backend == OtaBackend::Swupdate ? "swupdate" : "mender-client");

  if (!hasRecipe(recipes, backendPackage)) {
    reason = backendPackage + " package is not installed";
    return false;
  }

  if (ota.activeSlot.empty() || ota.inactiveSlot.empty() ||
      ota.activeSlot == ota.inactiveSlot) {
    reason = "OTA active and inactive slots must be distinct";
    return false;
  }

  if (!ota.signedBundles || !ota.rollbackEnabled || !ota.bootloaderMarksGood) {
    reason = "OTA requires signed bundles, rollback, and bootloader mark-good";
    return false;
  }

  reason = toString(ota.backend) + " configured for " + ota.activeSlot +
           "/" + ota.inactiveSlot;
  return true;
}

}  // namespace

std::string toString(const CheckStatus status) {
  return status == CheckStatus::Pass ? "PASS" : "FAIL";
}

std::string toString(const OtaBackend backend) {
  switch (backend) {
    case OtaBackend::Rauc:
      return "RAUC";
    case OtaBackend::Swupdate:
      return "SWUpdate";
    case OtaBackend::Mender:
      return "Mender";
  }
  return "Unknown";
}

ImageReport YoctoImageValidator::validate(
    const MachineProfile& machine,
    const std::vector<PackageRecipe>& recipes,
    const std::vector<SystemdUnit>& units,
    const BemsGatewayConfig& bems,
    const OtaConfig& ota) const {
  ImageReport report;

  auto fail = [&](std::string reason) {
    report.accepted = false;
    report.reason = std::move(reason);
    return report;
  };

  std::string reason;
  const bool machineOk = validateMachine(machine, reason);
  addCheck(report, "machine-profile", machineOk, reason);
  if (!machineOk) {
    return fail(reason);
  }

  const bool corePackages =
      hasRecipe(recipes, "packagegroup-core-boot") &&
      hasRecipe(recipes, "kernel-modules") && hasRecipe(recipes, "openssh");
  reason = corePackages ? "core boot, kernel modules, and SSH installed"
                        : "required base packages missing";
  addCheck(report, "base-package-set", corePackages, reason);
  if (!corePackages) {
    return fail(reason);
  }

  const bool bemsOk = validateBems(recipes, units, bems, reason);
  addCheck(report, "bems-gateway", bemsOk, reason);
  if (!bemsOk) {
    return fail(reason);
  }

  const bool otaOk = validateOta(recipes, ota, reason);
  addCheck(report, "ota-policy", otaOk, reason);
  if (!otaOk) {
    return fail(reason);
  }

  const bool updateUnit = hasEnabledUnit(units, "rauc.service") ||
                          hasEnabledUnit(units, "swupdate.service") ||
                          hasEnabledUnit(units, "mender-client.service");
  reason = updateUnit ? "OTA service unit enabled" : "OTA service unit missing";
  addCheck(report, "ota-service", updateUnit, reason);
  if (!updateUnit) {
    return fail(reason);
  }

  report.accepted = true;
  report.reason = "Yocto BEMS appliance image accepted";
  return report;
}

TextImageReporter::TextImageReporter(std::ostream& stream) : stream_(stream) {}

void TextImageReporter::publish(const ImageReport& report) const {
  stream_ << "yocto_image=" << (report.accepted ? "PASS" : "FAIL")
          << " reason=\"" << report.reason << "\"\n";
  for (const auto& check : report.checks) {
    stream_ << "  [" << toString(check.status) << "] " << check.step << ": "
            << check.detail << '\n';
  }
}

MachineProfile demoMachine() {
  return MachineProfile{"imx93evk",
                        "rheslar-bems",
                        "linux-imx",
                        "freescale/imx93-11x11-evk.dtb",
                        true,
                        true,
                        true};
}

std::vector<PackageRecipe> demoRecipes() {
  return {
      PackageRecipe{"packagegroup-core-boot", "openembedded-core", "1.0", true},
      PackageRecipe{"kernel-modules", "openembedded-core", "6.6", true},
      PackageRecipe{"openssh", "meta-oe", "9.6", true},
      PackageRecipe{"bems-edge-ai-gateway", "meta-rheslar-bems", "2026.06", true},
      PackageRecipe{"rauc", "meta-rauc", "1.12", true},
      PackageRecipe{"ca-certificates", "openembedded-core", "2024", true},
  };
}

std::vector<SystemdUnit> demoUnits() {
  return {
      SystemdUnit{"bems-edge-ai-gateway.service",
                  "/usr/bin/bems-edge-ai-gateway --config /etc/bems/gateway.toml",
                  true,
                  true,
                  true},
      SystemdUnit{"rauc.service", "/usr/bin/rauc service", true, false, true},
  };
}

BemsGatewayConfig demoBemsConfig() {
  return BemsGatewayConfig{
      "https://github.com/rheslar1/bems-edge-ai-gateway",
      "bems-edge-ai-gateway.service",
      true,
      true,
      true,
      true};
}

OtaConfig demoOtaConfig() {
  return OtaConfig{OtaBackend::Rauc,
                   "rootfs.0",
                   "rootfs.1",
                   true,
                   true,
                   true};
}

}  // namespace yocto_image
