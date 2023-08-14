#ifndef SRC_BACKEND_COMMON_H_
#define SRC_BACKEND_COMMON_H_

#include "object_reader.h"

/// @brief Updates maxVertex and minVertex of Object
/// @param object Object
/// @param vertex new Vertex
void updateMaxMin(Object* object, Vertex vertex);

/// @brief Return maximum of two values
/// @param a First value
/// @param b Second value
/// @return Maximum value
double max(double a, double b);

/// @brief Return minimum of two values
/// @param a First value
/// @param b Second value
/// @return minimum value
double min(double a, double b);

#endif  // SRC_BACKEND_COMMON_H_
