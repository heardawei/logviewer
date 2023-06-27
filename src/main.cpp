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
#include "ui/mainwindow.h"

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setOrganizationName("align_algo_alibaba_inc");
  a.setApplicationName("logviewer");

  using namespace logviewer;

  auto w = new MainWindow;
  auto log = new Log;

  auto parse_finished = [=](bool success)
  {
    if (!success)
    {
      return;
    }
    w->set_t_bg_x_points(log->t_bg_x_points());
    w->set_t_bg_y_points(log->t_bg_y_points());
    w->set_t_bg_z_points(log->t_bg_z_points());
    w->set_t_ba_x_points(log->t_ba_x_points());
    w->set_t_ba_y_points(log->t_ba_y_points());
    w->set_t_ba_z_points(log->t_ba_z_points());
  };

  a.connect(log, &Log::parse_finished, w, parse_finished);
  a.connect(w, &MainWindow::open_log, log, &Log::parse);
  a.connect(&a, &QApplication::aboutToQuit, log, &QObject::deleteLater);
  a.connect(&a, &QApplication::aboutToQuit, w, &QObject::deleteLater);

  w->resize(1024, 768);
  w->show();

  return a.exec();
}
