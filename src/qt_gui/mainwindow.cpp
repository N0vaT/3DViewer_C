#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  timer = new QTimer(this);
  glwidget = NULL;

  connect(ui->recordGifButton, &QPushButton::clicked, this,
          &MainWindow::recordGIF);

  connect(ui->rotateXLeftButton, &QPushButton::clicked, this,
          &MainWindow::rotate);
  connect(ui->rotateXRightButton, &QPushButton::clicked, this,
          &MainWindow::rotate);
  connect(ui->rotateYLeftButton, &QPushButton::clicked, this,
          &MainWindow::rotate);

  connect(ui->rotateYRightButton, &QPushButton::clicked, this,
          &MainWindow::rotate);
  connect(ui->rotateZLeftButton, &QPushButton::clicked, this,
          &MainWindow::rotate);
  connect(ui->rotateZRightButton, &QPushButton::clicked, this,
          &MainWindow::rotate);

  connect(ui->moveXLeftButton, &QPushButton::clicked, this, &MainWindow::move);
  connect(ui->moveYLeftButton, &QPushButton::clicked, this, &MainWindow::move);
  connect(ui->moveZLeftButton, &QPushButton::clicked, this, &MainWindow::move);
  connect(ui->moveXRightButton, &QPushButton::clicked, this, &MainWindow::move);
  connect(ui->moveYRightButton, &QPushButton::clicked, this, &MainWindow::move);
  connect(ui->moveZRightButton, &QPushButton::clicked, this, &MainWindow::move);

  connect(ui->shrinkButton, &QPushButton::clicked, this, &MainWindow::scale);
  connect(ui->enlargeButton, &QPushButton::clicked, this, &MainWindow::scale);

  connect(ui->dotColorButton, &QPushButton::clicked, this,
          &MainWindow::chooseColor);
  connect(ui->lineColorButton, &QPushButton::clicked, this,
          &MainWindow::chooseColor);
  connect(ui->backColorButton, &QPushButton::clicked, this,
          &MainWindow::chooseColor);

  connect(ui->screenshotButton, &QPushButton::clicked, this,
          &MainWindow::takeScreenshot);

  ui->rotateSpinBox->setValue(3.1415 / 10);
  ui->moveSpinBox->setValue(0.25);
  ui->scaleSpinBox->setValue(1.1);

  dotColor = QColor::fromRgb(255, 0, 0);
  dotColor = QColor::fromRgb(0, 255, 0);

  ui->projecttionComboBox->addItem("Parallel");
  ui->projecttionComboBox->addItem("Central");

  ui->dotTypeComboBox->addItem("Circle");
  ui->dotTypeComboBox->addItem("Square");
  ui->dotTypeComboBox->addItem("None");

  ui->lineTypeComboBox->addItem("Solid");
  ui->lineTypeComboBox->addItem("Dashed");
  ui->lineTypeComboBox->addItem("None");

  settings = (Settings *)malloc(sizeof(Settings));
  settings->backColor.red = 0;
  settings->backColor.green = 0;
  settings->backColor.blue = 0;
  settings->dotColor.red = 1;
  settings->dotColor.green = 0;
  settings->dotColor.blue = 0;
  settings->lineColor.red = 0;
  settings->lineColor.green = 1;
  settings->lineColor.blue = 0;
  settings->lineSize = 1;
  settings->dotSize = 5;
  settings->dotType = 1;
  settings->projectionType = 1;
  settings = readSettings((char *)"/tmp/settings", settings);

  connect(ui->applySettingButton, &QPushButton::clicked, this,
          &MainWindow::applySettings);

  connect(ui->openFileButton, &QPushButton::pressed, this,
          &MainWindow::openFile);

  ui->openGLWidget->setLayout(new QHBoxLayout());
}

MainWindow::~MainWindow() {
  if (timer != NULL) delete timer;
  freeObject(object);
  if (settings != NULL) free(settings);
  if (glwidget != NULL) delete glwidget;
  screens.clear();
  delete ui;
}

void MainWindow::openFile() {
  QString string = QFileDialog::getOpenFileName(this, tr("Open file"), "/Users",
                                                tr("Object file (*.obj)"));

  std::string stdString = string.toStdString();
  const char *filePath = stdString.c_str();
  freeObject(object);

  int error = SUCCESS;
  object = initObject((char *)filePath, &error);
  displayError(error);
  if (error == SUCCESS) {
    readAll(object, &error);
    displayError(error);
    ui->label_fileName->setText(string);
    ui->label_numberVertices->setText("" +
                                      QString::number(object->verticesNumber));
    ui->label_numberPolygons->setText("" +
                                      QString::number(object->polygonsNumber));

    minVertex = object->minVertex;
    maxVertex = object->maxVertex;
    updateGLWidget();
  } else {
    ui->label_fileName->setText("ERROR!!!!!");
  }
}

void MainWindow::rotate() {
  bool isLeft = (sender() == ui->rotateXLeftButton) ||
                (sender() == ui->rotateYLeftButton) ||
                (sender() == ui->rotateZLeftButton);
  double angle = ui->rotateSpinBox->value() * (isLeft ? 1 : -1);

  int error = SUCCESS;
  Vertex midPoint = middlePoint(object, &error);
  moveObject(object, -midPoint.x, -midPoint.y, -midPoint.z, &error);
  if (sender() == ui->rotateXLeftButton || sender() == ui->rotateXRightButton)
    rotateObjectX(object, angle, &error);
  else if (sender() == ui->rotateYLeftButton ||
           sender() == ui->rotateYRightButton)
    rotateObjectY(object, angle, &error);
  else
    rotateObjectZ(object, angle, &error);
  moveObject(object, midPoint.x, midPoint.y, midPoint.z, &error);

  displayError(error);
  if (error != SUCCESS) {
    return;
  }

  updateGLWidget();
}

void MainWindow::move() {
  bool isLeft = (sender() == ui->moveXLeftButton) ||
                (sender() == ui->moveYLeftButton) ||
                (sender() == ui->moveZLeftButton);
  double shift = ui->moveSpinBox->value() * (isLeft ? 1 : -1);

  int error = SUCCESS;
  if (sender() == ui->moveXLeftButton || sender() == ui->moveXRightButton)
    moveObject(object, shift, 0, 0, &error);
  else if (sender() == ui->moveYLeftButton || sender() == ui->moveYRightButton)
    moveObject(object, 0, shift, 0, &error);
  else
    moveObject(object, 0, 0, shift, &error);

  displayError(error);
  if (error != SUCCESS) {
    return;
  }

  updateGLWidget();
}

void MainWindow::scale() {
  double scale = ui->scaleSpinBox->value();

  if (sender() == ui->shrinkButton) scale = 1 / scale;

  int error = SUCCESS;
  scaleObject(object, scale, scale, scale, &error);
  displayError(error);

  if (error != SUCCESS) {
    return;
  }

  updateGLWidget();
}

void MainWindow::displayError(int error) {
  if (error == SUCCESS)
    ui->errorDescription->setText("SUCCESS");
  else if (error == FILE_NOT_FOUND)
    ui->errorDescription->setText("FILE NOT FOUND");
  else if (error == FILE_NOT_READABLE)
    ui->errorDescription->setText("FILE NOT READABLE");
  else if (error == FILE_PARSE_ERROR)
    ui->errorDescription->setText("FILE PARSE ERROR");
  else if (error == NULL_POINTER)
    ui->errorDescription->setText("NULL POINTER");
  else if (error == INSUFFICIENT_DATA)
    ui->errorDescription->setText("INSUFFICIENT DATA");
  else if (error == 10)
    ui->errorDescription->setText("WRONG FILE");
}

void MainWindow::updateGLWidget() {
  if (glwidget) {
    ui->openGLWidget->layout()->removeWidget(glwidget);
    delete glwidget;
  }
  glwidget = new GLWidget(object, settings, minVertex, maxVertex);
  ui->openGLWidget->layout()->addWidget(glwidget);
}

Settings *MainWindow::applySettings() {
  if (object == NULL) {
    MainWindow::openFile();
  }
  if (dotColorFlag) {
    settings->dotColor.red = dotColor.red() / 255.0;
    settings->dotColor.green = dotColor.green() / 255.0;
    settings->dotColor.blue = dotColor.blue() / 255.0;
  } else {
    settings->dotColor.red = 1;
    settings->dotColor.green = 0;
    settings->dotColor.blue = 0;
  }

  if (lineColorFlag) {
    settings->lineColor.red = lineColor.red() / 255.0;
    settings->lineColor.green = lineColor.green() / 255.0;
    settings->lineColor.blue = lineColor.blue() / 255.0;
  } else {
    settings->lineColor.red = 0;
    settings->lineColor.green = 1;
    settings->lineColor.blue = 0;
  }

  if (backColorFlag) {
    settings->backColor.red = backColor.red() / 255.0;
    settings->backColor.green = backColor.green() / 255.0;
    settings->backColor.blue = backColor.blue() / 255.0;
  } else {
    settings->backColor.red = 0;
    settings->backColor.green = 0;
    settings->backColor.blue = 0;
  }

  settings->lineSize = ui->lineSizeSpinBox->value();
  settings->dotSize = ui->dotSizeSpinBox->value();
  ;

  if (ui->projecttionComboBox->currentIndex() == 1)
    settings->projectionType = 2;
  else
    settings->projectionType = 1;

  if (ui->dotTypeComboBox->currentIndex() == 0)
    settings->dotType = 0;
  else if (ui->dotTypeComboBox->currentIndex() == 1) {
    settings->dotType = 1;
  } else {
    settings->dotType = 2;
  }

  if (ui->lineTypeComboBox->currentIndex() == 0)
    settings->lineType = 0;
  else if (ui->lineTypeComboBox->currentIndex() == 1) {
    settings->lineType = 1;
  } else if (ui->lineTypeComboBox->currentIndex() == 2) {
    settings->lineType = 2;
  }

  MainWindow::updateGLWidget();
  writeSettings(*settings, (char *)"/tmp/settings");
  return settings;
}

void MainWindow::chooseColor() {
  if (ui->dotColorButton == sender()) {
    dotColor = QColorDialog::getColor();
    dotColorFlag = true;
  } else if (ui->lineColorButton == sender()) {
    lineColor = QColorDialog::getColor();
    lineColorFlag = true;
  } else if (ui->backColorButton == sender()) {
    backColor = QColorDialog::getColor();
    backColorFlag = true;
  }
}

void MainWindow::takeScreenshot() {
  if (object == NULL) {
    return;
  }
  QScreen *screen = QGuiApplication::primaryScreen();
  QPixmap screenshot = screen->grabWindow(this->glwidget->winId());
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save Screenshot"), "",
      tr("BMP Files (*.bmp);; JPEG Files (*.jpeg)"));

  if (!fileName.isEmpty()) {
    QFileInfo fileInfo(fileName);
    QString format = fileInfo.suffix().toLower();

    if (format == "bmp") {
      screenshot.save(fileName, "BMP");
    } else if (format == "jpg" || format == "jpeg") {
      screenshot.save(fileName, "JPEG");
    } else {
      displayError(10);
    }
  }
}

void MainWindow::recordGIF() {
  if (object == NULL) return;

  if (timer == NULL) timer = new QTimer(this);
  timer->setInterval(1000 / 10);
  connect(timer, &QTimer::timeout, this, &MainWindow::tick);
  timer->start();
  timerCounter = 0;
}

void MainWindow::tick() {
  QPixmap pixmap = glwidget->grab();
  QImage image = pixmap.toImage();
  screens.append(
      image.scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));

  if (++timerCounter >= 5 * 10) {
    timer->stop();
    QGifImage *gif = new QGifImage();
    gif->setDefaultDelay(1000 / 10);
    for (const QImage &img : screens) {
      gif->addFrame(img);
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save GIF"), "",
                                                    tr("GIF Files (*.gif)"));

    if (!fileName.isEmpty()) {
      gif->save(fileName);
    }
    screens.clear();
    delete gif;
  }
}
