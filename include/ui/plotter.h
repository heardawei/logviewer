#pragma once

#include <QtCharts/QChartView>
#include <QtCharts/QXYSeries>

#include "ui/axis.h"

namespace logviewer
{

class Plotter : public QChartView
{
 public:
  explicit Plotter(QWidget *parent = nullptr);
  virtual ~Plotter();

  qsizetype create_series(const QString &name);
  qsizetype create_series(const QString &name, const QList<QPointF> &pts);
  bool append(qsizetype series_id, qreal x, qreal y);
  bool append(qsizetype series_id, const QPointF &pt);
  bool append(qsizetype series_id, const QList<QPointF> &pts);

  void clear();

 protected:
  virtual QXYSeries *new_series() = 0;

 protected:
  QChart *m_chart{};
  Axis *m_x_axis{};
  Axis *m_y_axis{};
};

class LinePlotter : public Plotter
{
  Q_OBJECT
 protected:
  virtual QXYSeries *new_series() override;
};

class ScatterPlotter : public Plotter
{
  Q_OBJECT
 protected:
  virtual QXYSeries *new_series() override;
};

}  // namespace logviewer
