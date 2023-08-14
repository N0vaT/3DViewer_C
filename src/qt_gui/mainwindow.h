#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QTimer>
#include <QWidget>
#include <QtOpenGLWidgets>

#include "QtWidgets/qpushbutton.h"

extern "C" {
#include "../backend/object_reader.h"
#include "../backend/object_transformer.h"
#include "../backend/settings_saver.h"
}

#include "gifmaker/gif_hash.h"
#include "gifmaker/gif_lib.h"
#include "gifmaker/gif_lib_private.h"
#include "gifmaker/qgifglobal.h"
#include "gifmaker/qgifimage.h"
#include "gifmaker/qgifimage_p.h"
#include "glwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  /// @brief MainWindow default constructor, is called by QT before opening
  /// MainWindow
  /// @param parent
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  /// @brief Opens .obj file then call updateGLWidget()
  ///     Is called by "Open file" button
  void openFile();

  /// @brief Rotates Object, then calls updateGLWidget().
  ///     Is called by any of "Rotate" buttons
  void rotate();

  /// @brief Moves Object, then calls updateGLWidget().
  ///     Is called by any of "Move" buttons
  void move();

  /// @brief Scales Object, then calls updateGLWidget().
  ///     Is called by any of "Scale" buttons
  void scale();

  /// @brief Updates dot/line Color by opening QColorDialog
  ///     Is called by "Dot color" and "Line color" buttons
  void chooseColor();

  // TODO (margetem) Updated function description
  /// @brief Takes a screenshot of MainWindow and saves it
  void takeScreenshot();

  void recordGIF();
  QTimer *timer;

  QList<QImage> screens;
  Vertex minVertex = {0.0, 0.0, 0.0};
  Vertex maxVertex = {0.0, 0.0, 0.0};
  ;

 private:
  Ui::MainWindow *ui;
  // QPushButton *renderButton;  // TODO delete???
  Object *object;
  // QWidget *canvas;  // TODO delete???
  GLWidget *glwidget;

  /// @brief Is used to display Object related operations error status in the
  /// GUI.
  ///     Should be called by after any Object related function was called
  /// @param error error status code
  void displayError(int error);

  /// @brief Updated GLWidget by removing the old one and creating a new one
  void updateGLWidget();

  /// @brief Updates Settings, then calls updateGLWidget().
  ///     Is called by "Apply settings" button and all of rotate/move/scale
  ///     buttons
  /// @return Settings pointer
  Settings *applySettings();
  QColor dotColor;
  QColor lineColor;
  QColor backColor;
  Settings *settings;
  bool dotColorFlag;
  bool lineColorFlag;
  bool backColorFlag;
  void tick();
  int timerCounter;
};
#endif  // MAINWINDOW_H
