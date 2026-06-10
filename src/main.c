#include <stdio.h>
#include <stddef.h>

typedef struct {
  const char *title;
  const char *summary;
  const char *evidence_target;
  const char *tags[8];
  size_t tag_count;
} project_profile_t;

static const project_profile_t profile = {
  "Embedded Linux / Yocto Image",
  "Custom Linux image for Raspberry Pi, BeagleBone, or i.MX-class hardware with Yocto recipes, kernel config, service units, and rootfs notes.",
  "Board-support fluency, package ownership, appliance-style Linux builds, and reproducible edge deployment.",
  {
  "Yocto",
  "BitBake",
  "Kernel config",
  "Systemd",
  "Device tree",
  "Rootfs"
  },
  6u
};

int main(void) {
  printf("%s\n", profile.title);
  printf("Summary: %s\n", profile.summary);
  printf("Evidence target: %s\n", profile.evidence_target);
  printf("Stack:");

  for (size_t index = 0; index < profile.tag_count; ++index) {
    printf(" %s%s", profile.tags[index], index + 1u == profile.tag_count ? "" : ",");
  }

  printf("\n");
  return 0;
}
