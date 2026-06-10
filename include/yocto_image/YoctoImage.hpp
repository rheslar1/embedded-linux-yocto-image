#ifndef YOCTO_IMAGE_YOCTO_IMAGE_HPP_
#define YOCTO_IMAGE_YOCTO_IMAGE_HPP_

#include <iosfwd>
#include <string>
#include <vector>

namespace yocto_image {

enum class CheckStatus {
  Pass,
  Fail
};

enum class OtaBackend {
  Rauc,
  Swupdate,
  Mender
};

std::string toString(CheckStatus status);
std::string toString(OtaBackend backend);

struct MachineProfile {
  std::string machine;
  std::string distro;
  std::string kernelProvider;
  std::string deviceTree;
  bool secureBoot{};
  bool readOnlyRootfs{};
  bool systemd{};
};

struct PackageRecipe {
  std::string name;
  std::string layer;
  std::string version;
  bool installed{};
};

struct SystemdUnit {
  std::string name;
  std::string execStart;
  bool enabled{};
  bool afterNetworkOnline{};
  bool restartOnFailure{};
};

struct BemsGatewayConfig {
  std::string repositoryUrl;
  std::string serviceName;
  bool bacnetIp{};
  bool rabbitMq{};
  bool tls{};
  bool watchdog{};
};

struct OtaConfig {
  OtaBackend backend{OtaBackend::Rauc};
  std::string activeSlot;
  std::string inactiveSlot;
  bool signedBundles{};
  bool rollbackEnabled{};
  bool bootloaderMarksGood{};
};

struct ImageCheck {
  std::string step;
  CheckStatus status{CheckStatus::Fail};
  std::string detail;
};

struct ImageReport {
  bool accepted{};
  std::string reason;
  std::vector<ImageCheck> checks;
};

class YoctoImageValidator {
 public:
  ImageReport validate(const MachineProfile& machine,
                       const std::vector<PackageRecipe>& recipes,
                       const std::vector<SystemdUnit>& units,
                       const BemsGatewayConfig& bems,
                       const OtaConfig& ota) const;
};

class TextImageReporter {
 public:
  explicit TextImageReporter(std::ostream& stream);

  void publish(const ImageReport& report) const;

 private:
  std::ostream& stream_;
};

MachineProfile demoMachine();
std::vector<PackageRecipe> demoRecipes();
std::vector<SystemdUnit> demoUnits();
BemsGatewayConfig demoBemsConfig();
OtaConfig demoOtaConfig();

}  // namespace yocto_image

#endif  // YOCTO_IMAGE_YOCTO_IMAGE_HPP_
