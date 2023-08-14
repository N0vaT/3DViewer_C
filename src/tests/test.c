#include "test.h"

int main() {
  SRunner *sr = srunner_create(suite_object_reader());
  srunner_add_suite(sr, suite_object_transformer());
  srunner_add_suite(sr, suite_settings_saver());

  srunner_run_all(sr, CK_NORMAL);

  int numberFailed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (numberFailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
