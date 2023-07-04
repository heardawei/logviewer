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
  void set_data(QCPGraph *graph,
                const QVector<double> &keys,
                const QVector<double> &values);

  void set_sync_xy_range(bool enable) { m_sync_xy_range = enable; }

  void set_scatter(QCPGraph *graph, bool enable)
  {
    if (enable)
    {
      graph->setLineStyle(QCPGraph::LineStyle::lsNone);
      graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
    }
    else
    {
      graph->setLineStyle(QCPGraph::LineStyle::lsLine);
    }
  }

  void rescale_axis()
  {
    if (m_first_rescale)
    {
      rescaleAxes(true);
      m_first_rescale = false;
    }
    else
    {
      rescaleAxes();
    }
  }

  void set_x_datetime_fmt()
  {
    // configure bottom axis to show date instead of number:
    auto dateTicker = QSharedPointer<QCPAxisTickerDateTime>::create();
    dateTicker->setDateTimeFormat("HH:mm:ss.zzz\nyyyy-MM-dd");
    xAxis->setTicker(dateTicker);
  }

 protected:
  bool m_sync_xy_range{};
  bool m_first_rescale{true};
};

}  // namespace logviewer
