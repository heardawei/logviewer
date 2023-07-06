// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <random>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QDateTime>
#include <QtCore/QElapsedTimer>
#include <QtCore/QTimer>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include "proc/image.h"
#include "proc/log.h"
#include "ui/mainwindow.h"

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  using namespace logviewer;

  Log log;
  proc::ImageProc img1;
  proc::ImageProc img2;
  MainWindow w;

  auto load_img1_finished = [&](bool success)
  {
    if (!success)
    {
      return;
    }
    w.set_img1_files(img1.images());
  };

  auto load_img2_finished = [&](bool success)
  {
    if (!success)
    {
      return;
    }
    w.set_img2_files(img2.images());
  };

  w.register_handler(std::bind(
      &Log::get_until, &log, std::placeholders::_1, std::placeholders::_2));

  a.connect(&w, &MainWindow::open_log, &log, &Log::clear);
  a.connect(&w, &MainWindow::open_log, &log, &Log::parse);

  a.connect(&img1, &proc::ImageProc::load_finished, &w, load_img1_finished);
  a.connect(&w, &MainWindow::open_img1_dir, &img1, &proc::ImageProc::clear);
  a.connect(&w, &MainWindow::open_img1_dir, &img1, &proc::ImageProc::load);

  a.connect(&img2, &proc::ImageProc::load_finished, &w, load_img2_finished);
  a.connect(&w, &MainWindow::open_img2_dir, &img2, &proc::ImageProc::clear);
  a.connect(&w, &MainWindow::open_img2_dir, &img2, &proc::ImageProc::load);

  w.resize(1024, 768);
  w.show();

  return a.exec();
}
