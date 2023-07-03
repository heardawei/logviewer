#pragma once

#include <QtCharts/QChartView>
#include <QtCharts/QXYSeries>

#include "ui/qcustomplot.h"

namespace logviewer
{

class Plotter : public QCustomPlot
{
 public:
  explicit Plotter(QWidget *parent = nullptr);
  virtual ~Plotter();

  QCPGraph *create_graph(const QString &name);
  QCPGraph *create_graph(const QString &name,
                         const QVector<double> &keys,
                         const QVector<double> &values);
};

}  // namespace logviewer
