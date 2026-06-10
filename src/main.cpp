#include "yocto_image/YoctoImage.hpp"

#include <iostream>

int main() {
  yocto_image::YoctoImageValidator validator;
  yocto_image::TextImageReporter reporter(std::cout);

  std::cout << "Embedded Linux / Yocto Image\n";
  std::cout << "Image: i.MX93 BEMS appliance with RAUC OTA\n\n";

  const auto report = validator.validate(yocto_image::demoMachine(),
                                         yocto_image::demoRecipes(),
                                         yocto_image::demoUnits(),
                                         yocto_image::demoBemsConfig(),
                                         yocto_image::demoOtaConfig());
  reporter.publish(report);
  return report.accepted ? 0 : 1;
}
