#include "yocto_image/YoctoImage.hpp"

#include <cassert>
#include <sstream>
#include <string>

namespace {

bool contains(const std::string& value, const std::string& needle) {
  return value.find(needle) != std::string::npos;
}

yocto_image::ImageReport validate(
    yocto_image::MachineProfile machine = yocto_image::demoMachine(),
    std::vector<yocto_image::PackageRecipe> recipes = yocto_image::demoRecipes(),
    std::vector<yocto_image::SystemdUnit> units = yocto_image::demoUnits(),
    yocto_image::BemsGatewayConfig bems = yocto_image::demoBemsConfig(),
    yocto_image::OtaConfig ota = yocto_image::demoOtaConfig()) {
  yocto_image::YoctoImageValidator validator;
  return validator.validate(machine, recipes, units, bems, ota);
}

void acceptsBemsOtaImage() {
  const auto report = validate();

  assert(report.accepted);
  assert(report.reason == "Yocto BEMS appliance image accepted");
  assert(report.checks.size() == 5U);
}

void rejectsMissingBemsRecipe() {
  auto recipes = yocto_image::demoRecipes();
  for (auto& recipe : recipes) {
    if (recipe.name == "bems-edge-ai-gateway") {
      recipe.installed = false;
    }
  }

  const auto report = validate(yocto_image::demoMachine(),
                               recipes,
                               yocto_image::demoUnits());

  assert(!report.accepted);
  assert(contains(report.reason, "bems-edge-ai-gateway"));
}

void rejectsDisabledBemsService() {
  auto units = yocto_image::demoUnits();
  units.front().enabled = false;

  const auto report =
      validate(yocto_image::demoMachine(), yocto_image::demoRecipes(), units);

  assert(!report.accepted);
  assert(contains(report.reason, "service"));
}

void rejectsImageWithoutSecureBoot() {
  auto machine = yocto_image::demoMachine();
  machine.secureBoot = false;

  const auto report = validate(machine);

  assert(!report.accepted);
  assert(contains(report.reason, "secure boot"));
}

void rejectsOtaWithoutRollback() {
  auto ota = yocto_image::demoOtaConfig();
  ota.rollbackEnabled = false;

  const auto report = validate(yocto_image::demoMachine(),
                               yocto_image::demoRecipes(),
                               yocto_image::demoUnits(),
                               yocto_image::demoBemsConfig(),
                               ota);

  assert(!report.accepted);
  assert(contains(report.reason, "rollback"));
}

void reporterIncludesEvidence() {
  const auto report = validate();
  std::ostringstream output;
  yocto_image::TextImageReporter reporter(output);
  reporter.publish(report);

  assert(contains(output.str(), "yocto_image=PASS"));
  assert(contains(output.str(), "bems-gateway"));
  assert(contains(output.str(), "ota-policy"));
}

}  // namespace

int main() {
  acceptsBemsOtaImage();
  rejectsMissingBemsRecipe();
  rejectsDisabledBemsService();
  rejectsImageWithoutSecureBoot();
  rejectsOtaWithoutRollback();
  reporterIncludesEvidence();
  return 0;
}
