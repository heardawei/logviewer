#pragma once

#include <QtCharts>

namespace logviewer
{
class Series : public QScatterSeries
{
  Q_OBJECT
 public:
  Series(QObject *parent = nullptr);
  ~Series();

  void append(qreal x, qreal y);
  void append(const QPointF &point);
  void append(const QList<QPointF> &points);

  void clear();

 private:
  qreal m_min_x{};
  qreal m_max_x{};
  qreal m_min_y{};
  qreal m_max_y{};
};

}  // namespace logviewer
