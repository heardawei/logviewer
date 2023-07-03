// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <random>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include "log/log.h"
#include "proc/image.h"
#include "ui/mainwindow.h"

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  using namespace logviewer;

  auto w = new MainWindow;
  auto log = new Log(&a);
  auto img_proc1 = new proc::ImageProc(&a);
  auto img_proc2 = new proc::ImageProc(&a);

  auto parse_log_finished = [=](bool success)
  {
    if (!success)
    {
      return;
    }
    w->set_t_bg_x_points(log->t(), log->bg_x());
    w->set_t_bg_y_points(log->t(), log->bg_y());
    w->set_t_bg_z_points(log->t(), log->bg_z());
    w->set_t_ba_x_points(log->t(), log->ba_x());
    w->set_t_ba_y_points(log->t(), log->ba_y());
    w->set_t_ba_z_points(log->t(), log->ba_z());
    w->set_t_px_py_points(log->px(), log->py());
  };

  auto load_img1_finished = [=](bool success)
  {
    if (!success)
    {
      return;
    }
    w->set_img1_files(img_proc1->images());
  };

  auto load_img2_finished = [=](bool success)
  {
    if (!success)
    {
      return;
    }
    w->set_img2_files(img_proc2->images());
  };

  a.connect(log, &Log::parse_finished, w, parse_log_finished);
  a.connect(w, &MainWindow::open_log, log, &Log::clear);
  a.connect(w, &MainWindow::open_log, log, &Log::parse);

  a.connect(img_proc1, &proc::ImageProc::load_finished, w, load_img1_finished);
  a.connect(w, &MainWindow::open_img1_dir, img_proc1, &proc::ImageProc::clear);
  a.connect(w, &MainWindow::open_img1_dir, img_proc1, &proc::ImageProc::load);

  a.connect(img_proc2, &proc::ImageProc::load_finished, w, load_img2_finished);
  a.connect(w, &MainWindow::open_img2_dir, img_proc2, &proc::ImageProc::clear);
  a.connect(w, &MainWindow::open_img2_dir, img_proc2, &proc::ImageProc::load);

  w->resize(1024, 768);
  w->show();

  return a.exec();
}
