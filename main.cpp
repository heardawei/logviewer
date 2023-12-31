// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <random>

#include <QtCore>
#include <QtCharts>
#include <QtWidgets>

#include "ui/plotter.h"

using namespace logviewer;

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto plotter = new Plotter;

    auto series = plotter->create_series("timestamp-x");

    QMainWindow window;
    window.setCentralWidget(plotter);
    window.resize(400, 300);
    window.show();

    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_real_distribution<qreal> u(0, 20);

    QTimer *timer = new QTimer;
    timer->connect(timer, &QTimer::timeout, timer, [=, &u, &e]() {
        auto sec = QDateTime::currentSecsSinceEpoch();
        auto y = u(e);
        plotter->append(series, static_cast<qreal>(sec), y);
        qDebug() << sec << " " << y;
    });
    timer->start(1000);

    return a.exec();
}
