#pragma once

#include <QtCharts/QChartView>

#include "ui/axis.h"

namespace logviewer
{

class Plotter : public QChartView
{
  Q_OBJECT
 public:
  explicit Plotter(QWidget *parent = nullptr);
  virtual ~Plotter();

 signals:

 public slots:
  qsizetype create_series(const QString &name);
  qsizetype create_series(const QString &name, const QList<QPointF> &pts);
  bool append(qsizetype series_id, qreal x, qreal y);
  bool append(qsizetype series_id, const QPointF &pt);
  bool append(qsizetype series_id, const QList<QPointF> &pts);

  void clear();

 protected:
  QChart *m_chart{};
  Axis *m_x_axis{};
  Axis *m_y_axis{};
};
}  // namespace logviewer
