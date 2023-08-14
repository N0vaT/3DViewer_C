#include "object_reader.h"

int getLine(char** line, FILE* file);
int enlargeString(char** line, int size, int delta);
Vertex readVertex(char* string, int* error);
Polygon readPolygon(char* string, int* error);
int enlargeUIntArr(unsigned int** arr, int size, int delta);
double mean(double a, double b);

Object* initObject(char* filepath, int* error) {
  Object* result = calloc(1, sizeof(Object));
  if (*error != SUCCESS) return result;

  result->filePath = malloc((strlen(filepath) + 1) * sizeof(char));
  snprintf(result->filePath, BUF_SIZE, "%s", filepath);
  result->file = fopen(result->filePath, "r");

  if (result->file == NULL) {
    *error = FILE_NOT_FOUND;
    return result;
  }

  int next = fgetc(result->file);
  if (next == EOF || next == 0) {
    *error = FILE_NOT_READABLE;
    return result;
  }
  int next1 = fgetc(result->file);
  int next2 = fgetc(result->file);
  while (next != EOF && next1 != EOF && next2 != EOF) {
    if (next == '\n' && next1 == 'v' && next2 == ' ') break;
    if (next == '\n' && next1 == 'f' && next2 == ' ') break;
    next = next1;
    next1 = next2;
    next2 = fgetc(result->file);
  }
  fseek(result->file, -2, SEEK_CUR);
  result->verticesNumber = 0;
  result->verticesRead = 0;
  result->polygonsNumber = 0;
  result->polygonsRead = 0;
  while (next != EOF) {
    if (next == '#')
      while (next != '\n' && next != EOF) next = fgetc(result->file);
    if (next == 'v') {
      next = fgetc(result->file);
      if (next == ' ') result->verticesNumber++;
      continue;
    } else if (next == 'f') {
      next = fgetc(result->file);
      if (next == ' ') result->polygonsNumber++;
      continue;
    }
    next = fgetc(result->file);
  }

  rewind(result->file);

  result->vertices = malloc(sizeof(Vertex) * result->verticesNumber);
  result->polygons = malloc(sizeof(Polygon) * result->polygonsNumber);
  return result;
}

void freeObject(Object* object) {
  if (object == NULL) return;

  if (object->filePath != NULL) free(object->filePath);

  if (object->file != NULL) fclose(object->file);

  for (unsigned int i = 0; i < object->polygonsRead; i++)
    free(object->polygons[i].vertices);

  object->verticesNumber = 0;
  object->verticesRead = 0;
  if (object->vertices != NULL) free(object->vertices);

  object->polygonsNumber = 0;
  object->polygonsRead = 0;
  if (object->polygons != NULL) free(object->polygons);

  free(object);
}

Vertex readNextVertex(Object* object, int* error) {
  Vertex vertex = {0};
  if (object == NULL) *error = NULL_POINTER;
  if (object != NULL && object->verticesRead >= object->verticesNumber)
    *error = FILE_PARSE_ERROR;
  if (*error != SUCCESS) return vertex;

  char* line = NULL;
  int size = getLine(&line, object->file);
  while (!(size < 0) && !(line[0] == 'v' && line[1] == ' '))
    size = getLine(&line, object->file);
  vertex = readVertex(line, error);
  updateMaxMin(object, vertex);
  if (line != NULL) free(line);

  if (*error == SUCCESS) {
    object->vertices[object->verticesRead] = vertex;
    object->verticesRead++;
  }

  if (object->verticesRead == 0) {
    object->minVertex = vertex;
    object->maxVertex = vertex;
  }

  return vertex;
}

Polygon readNextPolygon(Object* object, int* error) {
  Polygon polygon = {0};
  if (object == NULL) *error = NULL_POINTER;
  if (object != NULL && object->polygonsRead >= object->polygonsNumber)
    *error = FILE_PARSE_ERROR;
  if (*error != SUCCESS) return polygon;

  char* line = NULL;
  int size = getLine(&line, object->file);
  while (!(size < 0) && !(line[0] == 'f' && line[1] == ' '))
    size = getLine(&line, object->file);
  polygon = readPolygon(line, error);
  for (unsigned int i = 0; i < polygon.verticesNumber; i++) {
    if (polygon.vertices[i] < 0)
      polygon.vertices[i] += object->verticesRead + 1;
  }
  if (line != NULL) free(line);

  if (*error == SUCCESS) {
    object->polygons[object->polygonsRead] = polygon;
    object->polygonsRead++;
  } else
    free(polygon.vertices);

  return polygon;
}

int whatIsNext(Object* object, int* error) {
  if (object == NULL) *error = NULL_POINTER;
  if (*error != SUCCESS) return 0;

  int next1 = fgetc(object->file);
  int next2 = fgetc(object->file);
  while (next1 != EOF && next2 != EOF) {
    if (next1 == 'v' && next2 == ' ') break;
    if (next1 == 'f' && next2 == ' ') break;

    next1 = next2;
    next2 = fgetc(object->file);
  }

  if (next1 == EOF || next2 == EOF) return -1;
  fseek(object->file, -2, SEEK_CUR);
  if (next1 == 'v') return 1;
  if (next1 == 'f') return 2;

  return 0;
}

void readAll(Object* object, int* error) {
  if (object == NULL) *error = NULL_POINTER;
  if (*error != SUCCESS) return;

  while (object->verticesRead != object->verticesNumber ||
         object->polygonsRead != object->polygonsNumber) {
    switch (whatIsNext(object, error)) {
      case -1:
        *error = FILE_PARSE_ERROR;
        break;
      case 0:
        return;
      case 1:
        readNextVertex(object, error);
        break;
      case 2:
        readNextPolygon(object, error);
        break;
    }
  }
}

Vertex middlePoint(Object* object, int* error) {
  Vertex vertex = {0};
  if (object == NULL) *error = NULL_POINTER;
  if (object != NULL && object->verticesRead == 0) *error = INSUFFICIENT_DATA;
  if (*error != SUCCESS) return vertex;

  vertex.x = mean(object->minVertex.x, object->maxVertex.x);
  vertex.y = mean(object->minVertex.y, object->maxVertex.y);
  vertex.z = mean(object->minVertex.z, object->maxVertex.z);

  return vertex;
}

// UTILITY FUNCTIONS

/// @brief Reads next line from function (until end of line of file is met)
/// @param line pointer to string
///     (if it is not NULL memory will be freed and reallocated,
///     must be freed after use)
/// @param file pointer to FILE
/// @return number of symbols in line,
///     -1 if end of file was met before any symbols were read
int getLine(char** line, FILE* file) {
  if (*line != NULL) free(*line);
  int size = BUF_SIZE;
  *line = malloc((size + 1) * sizeof(char));
  int i = 0;
  bool endOfFile = false;
  while (true) {
    if (i == size - 2) size = enlargeString(line, size, BUF_SIZE);
    int next = fgetc(file);
    if (next == EOF) endOfFile = true;
    if (next == '\n' || next == EOF) break;
    (*line)[i++] = next;
  }
  (*line)[i] = '\0';
  return (endOfFile ? -1 : i);
}

/// @brief Resizes string to store more symbols
/// @param line Pointer to string
/// @param size Initial size
/// @param delta Number of symbol to enlarge string by
/// @return Size of enlarged string
int enlargeString(char** line, int size, int delta) {
  *line = realloc(*line, (size + delta) * sizeof(char));
  return (size + delta);
}

/// @brief Parses string to Vertex
/// @param string String in format 'v  0.0  0.0  0.0'
/// @param error Pointer to int, representing an error
///     (must be SUCCESS, otherwise function won't work)
/// @return Parsed Vertex
Vertex readVertex(char* string, int* error) {
  Vertex vertex = {0};
  if (*error != SUCCESS) return vertex;

  if (strlen(string) < 7 || string[0] != 'v' || string[1] != ' ') {
    *error = FILE_PARSE_ERROR;
    return vertex;
  }

  string++;
  while (string[0] == ' ') string++;
  if (sscanf(string, "%lf", &vertex.x) != 1) {
    *error = FILE_PARSE_ERROR;
    return vertex;
  }
  while (string[0] != ' ') string++;
  while (string[0] == ' ') string++;
  if (sscanf(string, "%lf", &vertex.y) != 1) {
    *error = FILE_PARSE_ERROR;
    return vertex;
  }
  while (string[0] != ' ') string++;
  while (string[0] == ' ') string++;
  if (sscanf(string, "%lf", &vertex.z) != 1) {
    *error = FILE_PARSE_ERROR;
    return vertex;
  }

  return vertex;
}

/// @brief Parse string to Polygon
/// @param string String in format 'f  1  2  3'
/// @param error Pointer to int, representing an error
///     (must be SUCCESS, otherwise function won't work)
/// @return Parsed Polygon
Polygon readPolygon(char* string, int* error) {
  Polygon polygon = {0};
  if (*error != SUCCESS) return polygon;

  if (strlen(string) < 3 || string[0] != 'f' || string[1] != ' ') {
    *error = FILE_PARSE_ERROR;
    return polygon;
  }

  string++;
  while (string[0] == ' ') string++;
  int size = 3;
  unsigned int* arr = malloc((size) * sizeof(unsigned int));
  int i = 0;
  while (string[0] != '\0' && string[0] != 13 && *error == SUCCESS) {
    unsigned int next;
    if (sscanf(string, "%u", &next) != 1) {
      *error = FILE_PARSE_ERROR;
      break;
    }
    if (i == size) size = enlargeUIntArr(&arr, size, size);
    arr[i++] = next;

    while (string[0] != ' ' && string[0] != '\0') string++;
    while (string[0] == ' ') string++;
  }

  polygon.vertices = malloc(i * sizeof(unsigned int));
  polygon.verticesNumber = i;
  for (int j = 0; j < i; j++) polygon.vertices[j] = arr[j];
  free(arr);

  return polygon;
}

/// @brief Resizes unsigned int array to store more elements
/// @param arr Pointer to array
/// @param size Initial size
/// @param delta Number of elements to enlarge array by
/// @return Size of enlarged array
int enlargeUIntArr(unsigned int** arr, int size, int delta) {
  *arr = realloc(*arr, (size + delta) * sizeof(unsigned int));
  return (size + delta);
}

/// @brief Calculates mathematical mean value of two values
/// @param a First value
/// @param b Second value
/// @return Mean value
double mean(double a, double b) { return (a + b) / 2; }
