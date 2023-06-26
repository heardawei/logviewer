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

#include "ui/mainwindow.h"

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setOrganizationName("align_algo_alibaba_inc");
  a.setApplicationName("logviewer");

  logviewer::MainWindow window;
  window.resize(1024, 768);
  window.show();

  return a.exec();
}
