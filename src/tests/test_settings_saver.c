#include "test.h"

START_TEST(test_settings_read_write) {
  Settings settings = {0};
  settings.dotSize = 123;
  settings.dotType = 4312;

  writeSettings(settings, "settings");
  Settings def = {0};
  Settings *res = readSettings("settings", &def);
  ck_assert_double_eq(res->dotSize, settings.dotSize);
  ck_assert_int_eq(res->dotType, settings.dotType);
}

START_TEST(test_settings_read_write_default) {
  Settings settings = {0};
  settings.dotSize = 123;
  settings.dotType = 4312;

  writeSettings(settings, "settings");
  Settings def = {0};
  def.dotSize = 5768;
  def.dotType = 54435;
  Settings *res = readSettings("non_existing_file", &def);
  ck_assert_double_eq(res->dotSize, def.dotSize);
  ck_assert_int_eq(res->dotType, def.dotType);
}

Suite *suite_settings_saver() {
  Suite *s = suite_create("SuiteSettingsSaver");

  TCase *tc_settings_saver = tcase_create("TCaseSettingsSaver");
  tcase_set_timeout(tc_settings_saver, 10);
  tcase_add_test(tc_settings_saver, test_settings_read_write);
  tcase_add_test(tc_settings_saver, test_settings_read_write_default);

  suite_add_tcase(s, tc_settings_saver);

  return s;
}
