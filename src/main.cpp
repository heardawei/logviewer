// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <random>

#include <QtCore>
#include <QtCharts>
#include <QtWidgets>

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QValueAxis *x_axis = new QValueAxis;
    QValueAxis *y_axis = new QValueAxis;

     QScatterSeries *series = new QScatterSeries;
//   QSplineSeries *series = new QSplineSeries();
   series->setName("spline");

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple spline chart example");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->addAxis(x_axis, Qt::AlignBottom);
    chart->addAxis(y_axis, Qt::AlignLeft);
    series->attachAxis(x_axis);
    series->attachAxis(y_axis);

    y_axis->setRange(0, 20);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();

    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_real_distribution<qreal> u(0, 20);

    QTimer *timer = new QTimer;
    timer->connect(timer, &QTimer::timeout, timer, [=, &u, &e]() {
        static auto minsec = QDateTime::currentSecsSinceEpoch();
        auto sec = QDateTime::currentSecsSinceEpoch();
        auto y = u(e);
        series->append(static_cast<double>(sec), y);
        x_axis->setRange(minsec, sec);
        qDebug() << sec << " " << y;
    });
    timer->start(1000);

    return a.exec();
}
