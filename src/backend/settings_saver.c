#include "settings_saver.h"

void writeSettings(Settings settings, char* filePath) {
  FILE* file = fopen(filePath, "w");
  if (file == NULL) return;

  fwrite(&settings, sizeof(settings), 1, file);
  fclose(file);
}

Settings* readSettings(char* filePath, Settings* defaultSettings) {
  if (defaultSettings == NULL) return defaultSettings;
  FILE* file = fopen(filePath, "r");
  if (file == NULL) return defaultSettings;

  fread(defaultSettings, sizeof(*defaultSettings), 1, file);
  fclose(file);

  return defaultSettings;
}
