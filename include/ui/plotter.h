#pragma once

#include <unordered_map>
#include <vector>

#include <QtCharts>
#include <QtWidgets>

#include "ui/axis.h"
#include "ui/series.h"

namespace logviewer
{

class Plotter : public QChartView
{
  Q_OBJECT
 public:
  explicit Plotter(QWidget *parent = nullptr);
  virtual ~Plotter();

 signals:

 public:
  qsizetype create_series(const QString &name);
  qsizetype create_series(const QString &name, const QList<QPointF> &pts);
  bool append(qsizetype series_id, qreal x, qreal y);
  bool append(qsizetype series_id, const QPointF &pt);
  bool append(qsizetype series_id, const QList<QPointF> &pts);

 protected:
  QChart *m_chart{};
  Axis *m_x_axis{};
  Axis *m_y_axis{};
  std::vector<Series *> m_series;
};
}  // namespace logviewer
