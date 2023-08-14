#include <check.h>

#include "../backend/object_reader.h"
#include "../backend/object_transformer.h"
#include "../backend/settings_saver.h"

/// @brief Suite, holding tests for object_reader
/// @return Suite
Suite *suite_object_reader();

/// @brief Suite, holding tests for object_transformer
/// @return Suite
Suite *suite_object_transformer();

/// @brief Suite, holding tests for settings_reader
/// @return Suite
Suite *suite_settings_saver();
