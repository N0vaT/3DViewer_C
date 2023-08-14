#include "glwidget.h"

GLWidget::GLWidget(Object* obj, Settings* set, Vertex minVertex,
                   Vertex maxVertex) {
  object = obj;
  settings = set;
  minV = minVertex;
  maxV = maxVertex;
}

GLWidget::~GLWidget() {
  // freeObject(object);
  // free(settings);
}

void GLWidget::initializeGL() {}

void GLWidget::paintGL() {
  double minCoord = minV.x;
  if (minCoord > minV.y) minCoord = minV.y;
  if (minCoord > minV.z) minCoord = minV.z;

  double maxCoord = maxV.x;
  if (maxCoord < maxV.y) maxCoord = maxV.y;
  if (maxCoord < maxV.z) maxCoord = maxV.z;

  minCoord *= 2;
  maxCoord *= 2;

  glClearColor(settings->backColor.red, settings->backColor.green,
               settings->backColor.blue, 1);

  glClear(GL_COLOR_BUFFER_BIT);  // чистим буфер
  glMatrixMode(GL_PROJECTION);   // устанавливаем матрицу
  glLoadIdentity();              // загружаем матрицу
  if (settings->projectionType == 1)
    glOrtho(minCoord, maxCoord, minCoord, maxCoord, minCoord,
            maxCoord);  // подготавливаем плоскости для матрицы
  else
    glFrustum(-1, 1, -1, 1, 0.5, 40);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPointSize(settings->dotSize);  // Размер точки
  if (settings->dotType == 0) {
    glEnable(GL_POINT_SMOOTH);
  }
  if (settings->dotType == 0 || settings->dotType == 1) {
    glBegin(GL_POINTS);
    glColor3f(settings->dotColor.red, settings->dotColor.green,
              settings->dotColor.blue);  // Цвет которым рисовать
    for (int i = 0; i < object->verticesRead; i++) {
      glVertex3d(object->vertices[i].x, object->vertices[i].y,
                 object->vertices[i].z);
    }
    glEnd();
  }

  if (settings->lineType == 0) {
    glEnable(GL_LINE);
  } else if (settings->lineType == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  }

  if (settings->lineType == 0 || settings->lineType == 1) {
    glLineWidth(settings->lineSize);
    glColor3f(settings->lineColor.red, settings->lineColor.green,
              settings->lineColor.blue);
    int k = 0;
    for (int i = 0; i < object->polygonsRead; i++) {
      glBegin(GL_LINE_LOOP);
      for (int j = 0; j < object->polygons[i].verticesNumber; j++) {
        k = object->polygons[i].vertices[j] - 1;
        glVertex3d(object->vertices[k].x, object->vertices[k].y,
                   object->vertices[k].z);
      }
      glEnd();
    }
  }
}
