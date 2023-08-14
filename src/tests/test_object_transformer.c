#include "test.h"

START_TEST(test_move_object) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/cube.obj", &error);
  ck_assert_int_eq(error, SUCCESS);
  double precision = 0.000001;

  while (object->verticesRead != object->verticesNumber)
    readNextVertex(object, &error);
  while (object->polygonsRead != object->polygonsNumber)
    readNextPolygon(object, &error);
  ck_assert_int_eq(error, SUCCESS);

  moveObject(object, 0.1, 0.2, -0.3, &error);
  ck_assert_int_eq(error, SUCCESS);

  double resultVertices[8][3] = {{0, 0, 0}, {0, 0, 2}, {0, 2, 0}, {0, 2, 2},
                                 {2, 0, 0}, {2, 0, 2}, {2, 2, 0}, {2, 2, 2}};

  for (int i = 0; i < 8; i++) {
    Vertex vertex = object->vertices[i];
    ck_assert_int_eq(error, SUCCESS);
    ck_assert_double_eq_tol(vertex.x, resultVertices[i][0] + 0.1, precision);
    ck_assert_double_eq_tol(vertex.y, resultVertices[i][1] + 0.2, precision);
    ck_assert_double_eq_tol(vertex.z, resultVertices[i][2] - 0.3, precision);
  }

  freeObject(object);
}

START_TEST(test_move_error) {
  int error = SUCCESS;
  moveObject(NULL, 1, 2, 3, &error);
  ck_assert_int_eq(error, NULL_POINTER);
}
END_TEST

START_TEST(test_rotate_x) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/cube.obj", &error);
  double precision = 0.000005;

  while (object->verticesRead != object->verticesNumber)
    readNextVertex(object, &error);
  while (object->polygonsRead != object->polygonsNumber)
    readNextPolygon(object, &error);
  ck_assert_int_eq(error, SUCCESS);

  rotateObjectX(object, 3.141592 * 2, &error);
  ck_assert_int_eq(error, SUCCESS);

  double resultVertices[8][3] = {{0, 0, 0}, {0, 0, 2}, {0, 2, 0}, {0, 2, 2},
                                 {2, 0, 0}, {2, 0, 2}, {2, 2, 0}, {2, 2, 2}};

  for (int i = 0; i < 8; i++) {
    Vertex vertex = object->vertices[i];
    ck_assert_double_eq_tol(vertex.x, resultVertices[i][0], precision);
    ck_assert_double_eq_tol(vertex.y, resultVertices[i][1], precision);
    ck_assert_double_eq_tol(vertex.z, resultVertices[i][2], precision);
  }

  freeObject(object);
}
END_TEST

START_TEST(test_rotate_y) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/cube.obj", &error);
  double precision = 0.000005;

  while (object->verticesRead != object->verticesNumber)
    readNextVertex(object, &error);
  while (object->polygonsRead != object->polygonsNumber)
    readNextPolygon(object, &error);
  ck_assert_int_eq(error, SUCCESS);

  rotateObjectY(object, 3.141592 * 2, &error);
  ck_assert_int_eq(error, SUCCESS);

  double resultVertices[8][3] = {{0, 0, 0}, {0, 0, 2}, {0, 2, 0}, {0, 2, 2},
                                 {2, 0, 0}, {2, 0, 2}, {2, 2, 0}, {2, 2, 2}};

  for (int i = 0; i < 8; i++) {
    Vertex vertex = object->vertices[i];
    ck_assert_double_eq_tol(vertex.x, resultVertices[i][0], precision);
    ck_assert_double_eq_tol(vertex.y, resultVertices[i][1], precision);
    ck_assert_double_eq_tol(vertex.z, resultVertices[i][2], precision);
  }

  freeObject(object);
}
END_TEST

START_TEST(test_rotate_z) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/cube.obj", &error);
  double precision = 0.000005;

  while (object->verticesRead != object->verticesNumber)
    readNextVertex(object, &error);
  while (object->polygonsRead != object->polygonsNumber)
    readNextPolygon(object, &error);
  ck_assert_int_eq(error, SUCCESS);

  rotateObjectZ(object, 3.141592 * 2, &error);
  ck_assert_int_eq(error, SUCCESS);

  double resultVertices[8][3] = {{0, 0, 0}, {0, 0, 2}, {0, 2, 0}, {0, 2, 2},
                                 {2, 0, 0}, {2, 0, 2}, {2, 2, 0}, {2, 2, 2}};

  for (int i = 0; i < 8; i++) {
    Vertex vertex = object->vertices[i];
    ck_assert_double_eq_tol(vertex.x, resultVertices[i][0], precision);
    ck_assert_double_eq_tol(vertex.y, resultVertices[i][1], precision);
    ck_assert_double_eq_tol(vertex.z, resultVertices[i][2], precision);
  }

  freeObject(object);
}
END_TEST

START_TEST(test_rotate_x_error) {
  int error = SUCCESS;
  rotateObjectX(NULL, 100.2, &error);
  ck_assert_int_eq(error, NULL_POINTER);
}
END_TEST

START_TEST(test_rotate_y_error) {
  int error = SUCCESS;
  rotateObjectY(NULL, 100.2, &error);
  ck_assert_int_eq(error, NULL_POINTER);
}
END_TEST

START_TEST(test_rotate_z_error) {
  int error = SUCCESS;
  rotateObjectZ(NULL, 100.2, &error);
  ck_assert_int_eq(error, NULL_POINTER);
}
END_TEST

START_TEST(test_scale) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/cube.obj", &error);
  double precision = 0.000005;

  while (object->verticesRead != object->verticesNumber)
    readNextVertex(object, &error);
  while (object->polygonsRead != object->polygonsNumber)
    readNextPolygon(object, &error);
  ck_assert_int_eq(error, SUCCESS);

  scaleObject(object, 10, 0.1, 34.23, &error);
  ck_assert_int_eq(error, SUCCESS);

  double resultVertices[8][3] = {{0, 0, 0}, {0, 0, 2}, {0, 2, 0}, {0, 2, 2},
                                 {2, 0, 0}, {2, 0, 2}, {2, 2, 0}, {2, 2, 2}};

  for (int i = 0; i < 8; i++) {
    Vertex vertex = object->vertices[i];
    ck_assert_double_eq_tol(vertex.x, resultVertices[i][0] * 10, precision);
    ck_assert_double_eq_tol(vertex.y, resultVertices[i][1] * 0.1, precision);
    ck_assert_double_eq_tol(vertex.z, resultVertices[i][2] * 34.23, precision);
  }

  freeObject(object);
}
END_TEST

START_TEST(test_scale_error) {
  int error = SUCCESS;
  scaleObject(NULL, 1, 1, 1, &error);
  ck_assert_int_eq(error, NULL_POINTER);

  error = SUCCESS;
  Object *object = initObject("tests/test_files/cube.obj", &error);
  double precision = 0.000005;

  while (object->verticesRead != object->verticesNumber)
    readNextVertex(object, &error);
  while (object->polygonsRead != object->polygonsNumber)
    readNextPolygon(object, &error);
  ck_assert_int_eq(error, SUCCESS);

  scaleObject(object, 10, 0, 34.23, &error);
  ck_assert_int_eq(error, SCALE_BY_ZERO);

  double resultVertices[8][3] = {{0, 0, 0}, {0, 0, 2}, {0, 2, 0}, {0, 2, 2},
                                 {2, 0, 0}, {2, 0, 2}, {2, 2, 0}, {2, 2, 2}};

  for (int i = 0; i < 8; i++) {
    Vertex vertex = object->vertices[i];
    ck_assert_double_eq_tol(vertex.x, resultVertices[i][0], precision);
    ck_assert_double_eq_tol(vertex.y, resultVertices[i][1], precision);
    ck_assert_double_eq_tol(vertex.z, resultVertices[i][2], precision);
  }
  freeObject(object);

  error = SUCCESS;
  object = initObject("tests/test_files/cube.obj", &error);

  while (object->verticesRead != object->verticesNumber)
    readNextVertex(object, &error);
  while (object->polygonsRead != object->polygonsNumber)
    readNextPolygon(object, &error);
  ck_assert_int_eq(error, SUCCESS);

  scaleObject(object, 10, -1, 34.23, &error);
  ck_assert_int_eq(error, SCALE_BY_NEGATIVE);

  for (int i = 0; i < 8; i++) {
    Vertex vertex = object->vertices[i];
    ck_assert_double_eq_tol(vertex.x, resultVertices[i][0], precision);
    ck_assert_double_eq_tol(vertex.y, resultVertices[i][1], precision);
    ck_assert_double_eq_tol(vertex.z, resultVertices[i][2], precision);
  }
  freeObject(object);
}
END_TEST

Suite *suite_object_transformer(void) {
  Suite *s = suite_create("SuiteObjectTransformer");

  TCase *tc_object_transformer = tcase_create("TCaseObjectTransformer");
  tcase_set_timeout(tc_object_transformer, 10);
  tcase_add_test(tc_object_transformer, test_move_object);
  tcase_add_test(tc_object_transformer, test_move_error);
  tcase_add_test(tc_object_transformer, test_rotate_x);
  tcase_add_test(tc_object_transformer, test_rotate_y);
  tcase_add_test(tc_object_transformer, test_rotate_z);
  tcase_add_test(tc_object_transformer, test_rotate_x_error);
  tcase_add_test(tc_object_transformer, test_rotate_y_error);
  tcase_add_test(tc_object_transformer, test_rotate_z_error);
  tcase_add_test(tc_object_transformer, test_scale);
  tcase_add_test(tc_object_transformer, test_scale_error);

  suite_add_tcase(s, tc_object_transformer);

  return s;
}
