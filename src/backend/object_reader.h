#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

#ifndef SRC_BACKEND_OBJECT_READER_H_
#define SRC_BACKEND_OBJECT_READER_H_

/// @brief Holds three coordinates (X,Y and Z) of vertex
typedef struct Vertex {
  double x;
  double y;
  double z;
} Vertex;

/// @brief Holds numbers of vertices, forming a polygon
typedef struct Polygon {
  /// @brief Number of vertices (number of polygon faces)
  unsigned int verticesNumber;

  /// @brief An array of numbers of vertices
  ///     (is dynamicly allocated and must be freed after use)
  int* vertices;
} Polygon;

/// @brief Holds all the necessary data to parse an .obj file
///     To prevent long freezes while reading big .obj files
///     Object doesn't read an entire file at one time.
///     On the contrary it quickly runs through file one time,
///     to count number of Vertices and Polygons (initObject function).
///     Then, when readNextVertex/readNextPolygon function is called
///     it reads next Vertex/Polygon from file,
///     parses it to the according structure and puts to Object.
typedef struct Object {
  /// @brief String, holding path the file
  char* filePath;

  /// @brief Pointer to a FILE
  FILE* file;

  /// @brief Number of vertices in a file
  unsigned int verticesNumber;

  /// @brief Number of vertices that were already
  ///   read from file and stored in vertices array
  unsigned int verticesRead;

  /// @brief An array of vertices
  ///   (is dynamicly allocated and must be freed after use)
  Vertex* vertices;

  /// @brief Holds minimum X,Y,Z coordinates of Object vertices.
  ///   Holds (0,0,0) coordinates right after Object initialisation,
  ///   after first Vertex was read from file it will hold
  ///   minium coordinates for all read Vertices
  Vertex minVertex;

  /// @brief Same as minVertex, but holds maximum coordinates
  Vertex maxVertex;

  /// @brief Number of polygons in a file
  unsigned int polygonsNumber;

  /// @brief Number of polygons that were already read
  ///   from file and stored in polygons array
  unsigned int polygonsRead;

  /// @brief An array of polygons
  ///   (is dynamicly allocated and must be freed after use)
  Polygon* polygons;
} Object;

#define SUCCESS 0
#define FILE_NOT_FOUND 1
#define FILE_NOT_READABLE 2
#define FILE_PARSE_ERROR 3
#define NULL_POINTER 4
#define INSUFFICIENT_DATA 5

#define BUF_SIZE 255

/// @brief Initialises an Object by opening a file
///     and counting number of Vertices and Polygons in it.
///     Allocates all the necessary memory
/// @param filename String, holding a relative or absolute path to .obj file
/// @param error Pointer to int, representing an error
///     (must be SUCCESS, otherwise function won't work)
/// @return Object
Object* initObject(char* filepath, int* error);

/// @brief Frees all memory, allocated in an Object and its members
/// @param object
void freeObject(Object* object);

/// @brief Reads next Vertex from .obj file, represented by Object
/// @param object Object
/// @param error Pointer to int, representing an error code
///     (must be SUCCESS, otherwise function won't work)
/// @return next Vertex
Vertex readNextVertex(Object* object, int* error);

/// @brief Reads next Polygon from .obj file, represented by Object
/// @param object Object
/// @param error Pointer to int, representing an error code
///     (must be SUCCESS, otherwise function won't work)
/// @return next Polygon
Polygon readNextPolygon(Object* object, int* error);

/// @brief Reads file until line, representing
///     a Vertex ot a Polygons is met and returns a code, representing
///     if this line holds a Vertex or a Polygon
/// @param object Object
/// @param error Pointer to int, representing an error code
///     (must be SUCCESS, otherwise function won't work)
/// @return '-1' - if end of file was met,
///     '0' - if error was not SUCCESS,
///     '1' - if line represents a Vertex,
///     '2' - if line represents a Polygon
int whatIsNext(Object* object, int* error);

/// @brief Reads all Vertices and Polygons from Object represented file
/// @param object Object
/// @param error Pointer to int, representing an error code
///     (must be SUCCESS, otherwise function won't work)
void readAll(Object* object, int* error);

/// @brief Returns middle Vertex of all read Vertices in Object
///     Returns (0,0,0) and an error if is called before
///     any vertices were read from file, represented by Object
/// @param object Object
/// @param error Pointer to int, representing an error code
///     (must be SUCCESS, otherwise function won't work)
/// @return Middle Vertex
Vertex middlePoint(Object* object, int* error);

#endif  // SRC_BACKEND_OBJECT_READER_H_
