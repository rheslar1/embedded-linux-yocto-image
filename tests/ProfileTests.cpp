#include <array>
#include <cassert>
#include <string_view>

class IReadinessRule {
 public:
  virtual ~IReadinessRule() = default;
  virtual bool passes(std::string_view evidenceTarget) const = 0;
};

class RequiredEvidenceRule final : public IReadinessRule {
 public:
  bool passes(std::string_view evidenceTarget) const override {
    return !evidenceTarget.empty();
  }
};

struct ProjectProfile {
  std::string_view title;
  std::string_view summary;
  std::string_view evidenceTarget;
  std::array<std::string_view, 9> tags;
};

constexpr ProjectProfile profile{
  "Embedded Linux / Yocto Image",
  "Custom Linux image for Raspberry Pi, BeagleBone, or i.MX-class hardware with Yocto recipes, kernel config, service units, and rootfs notes.",
  "Board-support fluency, package ownership, appliance-style Linux builds, and reproducible edge deployment.",
  {
    "C++17",
    "C++ Design Patterns",
    "SOLID",
    "Yocto",
    "BitBake",
    "Kernel config",
    "Systemd",
    "Device tree",
    "Rootfs"
  }
};

int main() {
  const RequiredEvidenceRule rule;
  assert(!profile.title.empty());
  assert(!profile.summary.empty());
  assert(rule.passes(profile.evidenceTarget));
  assert(profile.tags[0] == "C++17");
  return 0;
}
