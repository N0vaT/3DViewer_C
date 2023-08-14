#include <math.h>

#include "common.h"
#include "object_reader.h"

#ifndef SRC_BACKEND_OBJECT_TRANSFORMER_H_
#define SRC_BACKEND_OBJECT_TRANSFORMER_H_

#define SCALE_BY_ZERO 5
#define SCALE_BY_NEGATIVE 6

#define EPSILON 0.000001

/// @brief Shifts all vertices along axis X, Y and Z
/// @param object Object
/// @param moveX Angle of shift around X axis
/// @param moveY Angle of shift around Y axis
/// @param moveZ Angle of shift around Z axis
/// @param error Pointer to int, representing an error
///     (must be SUCCESS, otherwise function won't work)
void moveObject(Object* object, double moveX, double moveY, double moveZ,
                int* error);

/// @brief Rotates all vertices relative to origin (0,0,0) around X axis
/// @param object Object
/// @param rotateX Rotation along X axis (positive value means rotation counter
/// clockwise)
/// @param error Pointer to int, representing an error
///     (must be SUCCESS, otherwise function won't work)
void rotateObjectX(Object* object, double rotateX, int* error);

/// @brief Rotates all vertices relative to origin (0,0,0) around Y axis
/// @param object Object
/// @param rotateY Rotation along Y axis (positive value means rotation counter
/// clockwise)
/// @param error Pointer to int, representing an error
///     (must be SUCCESS, otherwise function won't work)
void rotateObjectY(Object* object, double rotateY, int* error);

/// @brief Rotates all vertices relative to origin (0,0,0) around Z axis
/// @param object Object
/// @param rotateZ Rotation along Z axis (positive value means rotation counter
/// clockwise)
/// @param error Pointer to int, representing an error
///     (must be SUCCESS, otherwise function won't work)
void rotateObjectZ(Object* object, double rotateZ, int* error);

/// @brief Scales all vertices relative to origin (0,0,0)
/// @param object Object
/// @param scaleX Scale along X axis
/// @param scaleY Scale along Y axis
/// @param scaleZ Scale along Z axis
/// @param error Pointer to int, representing an error
///     (must be SUCCESS, otherwise function won't work)
void scaleObject(Object* object, double scaleX, double scaleY, double scaleZ,
                 int* error);

#endif  // SRC_BACKEND_OBJECT_TRANSFORMER_H_
