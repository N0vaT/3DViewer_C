#include "common.h"

void updateMaxMin(Object* object, Vertex vertex) {
  object->maxVertex.x = max(vertex.x, object->maxVertex.x);
  object->maxVertex.y = max(vertex.y, object->maxVertex.y);
  object->maxVertex.z = max(vertex.z, object->maxVertex.z);
  object->minVertex.x = min(vertex.x, object->minVertex.x);
  object->minVertex.y = min(vertex.y, object->minVertex.y);
  object->minVertex.z = min(vertex.z, object->minVertex.z);
}

double max(double a, double b) {
  if (a >= b) return a;
  return b;
}

double min(double a, double b) {
  if (a <= b) return a;
  return b;
}
