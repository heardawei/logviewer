#pragma once

#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QSplineSeries>

namespace logviewer
{
class LineSeries : public QLineSeries
{
  Q_OBJECT
 public:
  LineSeries(QObject *parent = nullptr);
  ~LineSeries();
};

class ScatterSeries : public QScatterSeries
{
  Q_OBJECT
 public:
  ScatterSeries(QObject *parent = nullptr);
  ~ScatterSeries();
};

}  // namespace logviewer
