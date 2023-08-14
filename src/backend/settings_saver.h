#include <stdio.h>

#ifndef SRC_BACKEND_SETTINGS_SAVER_H_
#define SRC_BACKEND_SETTINGS_SAVER_H_

/// @brief Represents color in RGB model.
///      For each color 1 represents maximum intensity, 0 - minimum
typedef struct Color {
  float red;
  float green;
  float blue;
} Color;

/// @brief Represents projection settings
typedef struct Settings {
  /// @brief 0 - means circle, 1 - means square, 2 - none
  int dotType;

  /// @brief Size of point
  float dotSize;

  /// @brief Color of point
  Color dotColor;

  /// @brief 0 - means solid, 1 - means dashed, 2 - none
  int lineType;

  /// @brief Size of line (width)
  float lineSize;

  /// @brief Color of line
  Color lineColor;

  /// @brief Color of background
  Color backColor;

  /// @brief 1 - means parallel projection, 2 - means central projection,
  int projectionType;
} Settings;

/// @brief Writes givent Settings to file,
///     if file doesn't exit it is created
/// @param settings Settings
/// @param filePath Path to file
void writeSettings(Settings settings, char* filePath);

/// @brief Reads settings from file.
///     If file exists function writes it's content to defaultSettings
///     and returns pointer to this structure.
///     If file doesn't exist functions return defaultSettings without any
///     changes
/// @param filePath Path to file
/// @param defaultSettings Pointer to default Settings
/// @return Pointer to Settings (is equal to *defaultSettings)
Settings* readSettings(char* filePath, Settings* defaultSettings);

#endif  // SRC_BACKEND_SETTINGS_SAVER_H_
