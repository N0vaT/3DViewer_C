#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QOpenGLWidget>

extern "C" {
#include "../backend/object_reader.h"
#include "../backend/settings_saver.h"
}

class GLWidget : public QOpenGLWidget {
 public:
  /// @brief Default constructor
  /// @param obj Pointer to Object to draw
  /// @param set Pointer to Settings to use
  GLWidget(Object* obj, Settings* set, Vertex minVertex, Vertex maxVertex);

  ~GLWidget();

  void initializeGL();

  /// @brief Sets up GLWidget to draw data from Object with given Settings
  ///     Is called by QTPaintEvent
  void paintGL();

  Vertex minV;
  Vertex maxV;

 private:
  Object* object;
  Settings* settings;
};

#endif  // GLWIDGET_H
