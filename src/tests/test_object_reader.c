#include "test.h"

START_TEST(test_init_free_object) {
  int error = SUCCESS;
  char *filePath = "tests/test_files/cube.obj";
  Object *object = initObject(filePath, &error);
  ck_assert_int_eq(error, SUCCESS);
  ck_assert_str_eq(object->filePath, filePath);
  ck_assert_ptr_ne(object->file, NULL);
  ck_assert_int_eq(object->verticesRead, 0);
  ck_assert_int_eq(object->polygonsRead, 0);
  ck_assert_int_eq(object->verticesNumber, 8);
  ck_assert_int_eq(object->polygonsNumber, 10);
  ck_assert_double_eq(object->minVertex.x, 0);
  ck_assert_double_eq(object->minVertex.y, 0);
  ck_assert_double_eq(object->minVertex.z, 0);
  ck_assert_double_eq(object->maxVertex.x, 0);
  ck_assert_double_eq(object->maxVertex.y, 0);
  ck_assert_double_eq(object->maxVertex.z, 0);

  freeObject(object);
}
END_TEST

START_TEST(test_read_object) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/cube.obj", &error);
  ck_assert_int_eq(error, SUCCESS);
  double precision = 0.000001;

  double resultVertices[8][3] = {{0, 0, 0}, {0, 0, 2}, {0, 2, 0}, {0, 2, 2},
                                 {2, 0, 0}, {2, 0, 2}, {2, 2, 0}, {2, 2, 2}};

  int resultPolygons[10][3] = {{1, 7, 5}, {1, 3, 7}, {1, 4, 3}, {1, 2, 4},
                               {3, 8, 7}, {3, 4, 8}, {5, 7, 8}, {5, 8, 6},
                               {1, 5, 6}, {1, 6, 2}};

  for (int i = 0; i < 8; i++) {
    Vertex vertex = readNextVertex(object, &error);
    ck_assert_int_eq(error, SUCCESS);
    ck_assert_double_eq_tol(vertex.x, resultVertices[i][0], precision);
    ck_assert_double_eq_tol(vertex.y, resultVertices[i][1], precision);
    ck_assert_double_eq_tol(vertex.z, resultVertices[i][2], precision);
    ck_assert_int_eq(object->verticesRead, i + 1);
  }

  for (int i = 0; i < 10; i++) {
    Polygon polygon = readNextPolygon(object, &error);
    ck_assert_int_eq(error, SUCCESS);
    ck_assert_int_eq(polygon.verticesNumber, 3);
    ck_assert_int_eq(polygon.vertices[0], resultPolygons[i][0]);
    ck_assert_int_eq(polygon.vertices[1], resultPolygons[i][1]);
    ck_assert_int_eq(polygon.vertices[2], resultPolygons[i][2]);
    ck_assert_int_eq(object->polygonsRead, i + 1);
  }

  for (int i = 0; i < 8; i++) {
    Vertex vertex = object->vertices[i];
    ck_assert_int_eq(error, SUCCESS);
    ck_assert_double_eq_tol(vertex.x, resultVertices[i][0], precision);
    ck_assert_double_eq_tol(vertex.y, resultVertices[i][1], precision);
    ck_assert_double_eq_tol(vertex.z, resultVertices[i][2], precision);
  }

  for (int i = 0; i < 10; i++) {
    Polygon polygon = object->polygons[i];
    ck_assert_int_eq(error, SUCCESS);
    ck_assert_int_eq(polygon.verticesNumber, 3);
    ck_assert_int_eq(polygon.vertices[0], resultPolygons[i][0]);
    ck_assert_int_eq(polygon.vertices[1], resultPolygons[i][1]);
    ck_assert_int_eq(polygon.vertices[2], resultPolygons[i][2]);
  }

  ck_assert_int_eq(object->polygonsRead, object->polygonsNumber);
  ck_assert_int_eq(object->verticesRead, object->verticesNumber);

  ck_assert_double_eq(object->minVertex.x, 0);
  ck_assert_double_eq(object->minVertex.y, 0);
  ck_assert_double_eq(object->minVertex.z, 0);
  ck_assert_double_eq(object->maxVertex.x, 2);
  ck_assert_double_eq(object->maxVertex.y, 2);
  ck_assert_double_eq(object->maxVertex.z, 2);

  freeObject(object);
}

START_TEST(test_init_time) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/skull.obj", &error);
  ck_assert_int_eq(error, SUCCESS);
  ck_assert_int_ne(object->verticesNumber, 0);
  ck_assert_int_ne(object->polygonsNumber, 0);

  while (object->verticesRead != object->verticesNumber ||
         object->polygonsRead != object->polygonsNumber) {
    int whatNext = whatIsNext(object, &error);
    ck_assert_int_eq(error, SUCCESS);
    if (whatNext == -1) break;
    if (whatNext == 1) readNextVertex(object, &error);
    if (whatNext == 2) readNextPolygon(object, &error);
    ck_assert_int_eq(error, SUCCESS);
  }
  ck_assert_int_eq(object->verticesRead, object->verticesNumber);
  ck_assert_int_eq(object->polygonsRead, object->polygonsNumber);

  freeObject(object);

  object = initObject("tests/test_files/lamp.obj", &error);
  ck_assert_int_eq(error, SUCCESS);
  ck_assert_int_ne(object->verticesNumber, 0);
  ck_assert_int_ne(object->polygonsNumber, 0);

  while (object->verticesRead != object->verticesNumber ||
         object->polygonsRead != object->polygonsNumber) {
    int whatNext = whatIsNext(object, &error);
    ck_assert_int_eq(error, SUCCESS);
    if (whatNext == -1) break;
    if (whatNext == 1) readNextVertex(object, &error);
    if (whatNext == 2) readNextPolygon(object, &error);
    ck_assert_int_eq(error, SUCCESS);
  }
  ck_assert_int_eq(object->verticesRead, object->verticesNumber);
  ck_assert_int_eq(object->polygonsRead, object->polygonsNumber);

  freeObject(object);
}
END_TEST

START_TEST(test_init_error) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/doesNotExist.obj", &error);
  ck_assert_int_eq(error, FILE_NOT_FOUND);
  freeObject(object);

  error = SUCCESS;
  object = initObject("tests/test_files/empty.obj", &error);
  ck_assert_int_eq(error, FILE_NOT_READABLE);
  freeObject(object);

  error = FILE_PARSE_ERROR;
  object = initObject("tests/test_files/doesNotExist.obj", &error);
  ck_assert_ptr_eq(object->filePath, NULL);
  freeObject(object);
}

START_TEST(test_read_vertex_error) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/cube.obj", &error);
  ck_assert_int_eq(error, SUCCESS);

  readNextVertex(NULL, &error);
  ck_assert_int_eq(error, NULL_POINTER);

  error = SUCCESS;
  while (object->verticesRead != object->verticesNumber) {
    readNextVertex(object, &error);
    ck_assert_int_eq(error, SUCCESS);
  }
  readNextVertex(object, &error);
  ck_assert_int_eq(error, FILE_PARSE_ERROR);
  freeObject(object);

  error = SUCCESS;
  object = initObject("tests/test_files/test.obj", &error);
  ck_assert_int_eq(object->verticesNumber, 5);
  readNextVertex(object, &error);
  ck_assert_int_eq(error, FILE_PARSE_ERROR);
  error = SUCCESS;
  readNextVertex(object, &error);
  ck_assert_int_eq(error, SUCCESS);
  error = SUCCESS;
  readNextVertex(object, &error);
  ck_assert_int_eq(error, FILE_PARSE_ERROR);
  error = SUCCESS;
  readNextVertex(object, &error);
  ck_assert_int_eq(error, FILE_PARSE_ERROR);
  error = SUCCESS;
  readNextVertex(object, &error);
  ck_assert_int_eq(error, FILE_PARSE_ERROR);
  freeObject(object);
}
END_TEST

START_TEST(test_read_polygon_error) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/cube.obj", &error);
  ck_assert_int_eq(error, SUCCESS);

  readNextPolygon(NULL, &error);
  ck_assert_int_eq(error, NULL_POINTER);

  error = SUCCESS;
  while (object->polygonsRead != object->polygonsNumber) {
    readNextPolygon(object, &error);
    ck_assert_int_eq(error, SUCCESS);
  }
  readNextPolygon(object, &error);
  ck_assert_int_eq(error, FILE_PARSE_ERROR);
  freeObject(object);

  error = SUCCESS;
  object = initObject("tests/test_files/test1.obj", &error);
  ck_assert_int_eq(error, SUCCESS);
  ck_assert_int_eq(object->polygonsNumber, 5);
  readNextPolygon(object, &error);
  ck_assert_int_eq(error, FILE_PARSE_ERROR);
  error = SUCCESS;
  readNextPolygon(object, &error);
  ck_assert_int_eq(error, SUCCESS);
  error = SUCCESS;
  readNextPolygon(object, &error);
  ck_assert_int_eq(error, FILE_PARSE_ERROR);
  error = SUCCESS;
  readNextPolygon(object, &error);
  ck_assert_int_eq(error, FILE_PARSE_ERROR);
  error = SUCCESS;
  readNextPolygon(object, &error);
  ck_assert_int_eq(error, SUCCESS);
  freeObject(object);
}
END_TEST

START_TEST(test_whatisnext_error) {
  int error = SUCCESS;
  whatIsNext(NULL, &error);
  ck_assert_int_eq(error, NULL_POINTER);
}

START_TEST(test_big_string) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/test2.obj", &error);

  while (object->verticesRead != object->verticesNumber)
    readNextVertex(object, &error);
  ck_assert_int_eq(error, SUCCESS);

  freeObject(object);
}

START_TEST(test_read_all) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/cube.obj", &error);
  ck_assert_int_eq(error, SUCCESS);
  double precision = 0.000001;

  double resultVertices[8][3] = {{0, 0, 0}, {0, 0, 2}, {0, 2, 0}, {0, 2, 2},
                                 {2, 0, 0}, {2, 0, 2}, {2, 2, 0}, {2, 2, 2}};

  int resultPolygons[10][3] = {{1, 7, 5}, {1, 3, 7}, {1, 4, 3}, {1, 2, 4},
                               {3, 8, 7}, {3, 4, 8}, {5, 7, 8}, {5, 8, 6},
                               {1, 5, 6}, {1, 6, 2}};

  readAll(object, &error);
  ck_assert_int_eq(error, SUCCESS);

  for (int i = 0; i < 8; i++) {
    Vertex vertex = object->vertices[i];
    ck_assert_double_eq_tol(vertex.x, resultVertices[i][0], precision);
    ck_assert_double_eq_tol(vertex.y, resultVertices[i][1], precision);
    ck_assert_double_eq_tol(vertex.z, resultVertices[i][2], precision);
  }

  for (int i = 0; i < 10; i++) {
    Polygon polygon = object->polygons[i];
    ck_assert_int_eq(polygon.vertices[0], resultPolygons[i][0]);
    ck_assert_int_eq(polygon.vertices[1], resultPolygons[i][1]);
    ck_assert_int_eq(polygon.vertices[2], resultPolygons[i][2]);
  }

  ck_assert_int_eq(object->polygonsRead, object->polygonsNumber);
  ck_assert_int_eq(object->verticesRead, object->verticesNumber);

  ck_assert_double_eq(object->minVertex.x, 0);
  ck_assert_double_eq(object->minVertex.y, 0);
  ck_assert_double_eq(object->minVertex.z, 0);
  ck_assert_double_eq(object->maxVertex.x, 2);
  ck_assert_double_eq(object->maxVertex.y, 2);
  ck_assert_double_eq(object->maxVertex.z, 2);

  freeObject(object);
}

START_TEST(test_middle_point) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/cube.obj", &error);
  ck_assert_int_eq(error, SUCCESS);

  readAll(object, &error);
  ck_assert_int_eq(error, SUCCESS);

  ck_assert_double_eq(object->minVertex.x, 0);
  ck_assert_double_eq(object->minVertex.y, 0);
  ck_assert_double_eq(object->minVertex.z, 0);
  ck_assert_double_eq(object->maxVertex.x, 2);
  ck_assert_double_eq(object->maxVertex.y, 2);
  ck_assert_double_eq(object->maxVertex.z, 2);

  Vertex middle = middlePoint(object, &error);
  ck_assert_int_eq(error, SUCCESS);
  ck_assert_double_eq(middle.x, 1);
  ck_assert_double_eq(middle.y, 1);
  ck_assert_double_eq(middle.z, 1);

  freeObject(object);
}

START_TEST(test_middle_point_error) {
  int error = SUCCESS;
  Object *object = initObject("tests/test_files/cube.obj", &error);
  ck_assert_int_eq(error, SUCCESS);

  Vertex middle = middlePoint(NULL, &error);
  ck_assert_int_eq(error, NULL_POINTER);
  ck_assert_double_eq(middle.x, 0);
  ck_assert_double_eq(middle.y, 0);
  ck_assert_double_eq(middle.z, 0);

  error = SUCCESS;
  middle = middlePoint(object, &error);
  ck_assert_int_eq(error, INSUFFICIENT_DATA);
  ck_assert_double_eq(middle.x, 0);
  ck_assert_double_eq(middle.y, 0);
  ck_assert_double_eq(middle.z, 0);

  freeObject(object);
}

Suite *suite_object_reader(void) {
  Suite *s = suite_create("SuiteObjectReader");

  TCase *tc_object_reader = tcase_create("TCaseObjectReader");
  tcase_set_timeout(tc_object_reader, 10);
  tcase_add_test(tc_object_reader, test_init_free_object);
  tcase_add_test(tc_object_reader, test_read_object);
  tcase_add_test(tc_object_reader, test_init_time);
  tcase_add_test(tc_object_reader, test_init_error);
  tcase_add_test(tc_object_reader, test_read_vertex_error);
  tcase_add_test(tc_object_reader, test_read_polygon_error);
  tcase_add_test(tc_object_reader, test_whatisnext_error);
  tcase_add_test(tc_object_reader, test_big_string);
  tcase_add_test(tc_object_reader, test_read_all);
  tcase_add_test(tc_object_reader, test_middle_point);
  tcase_add_test(tc_object_reader, test_middle_point_error);

  suite_add_tcase(s, tc_object_reader);

  return s;
}
