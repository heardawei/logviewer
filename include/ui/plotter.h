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

  int capacity() const;
  void set_capacity(int capacity);

  QCPGraph *create_graph(const QString &name);
  void set_data(QCPGraph *graph,
                const QVector<double> &keys,
                const QVector<double> &values);

  void add_data(QCPGraph *graph, double keys, double values);

  void add_data(QCPGraph *graph,
                const QVector<double> &keys,
                const QVector<double> &values);

  void set_sync_xy_range(bool enable);

  void set_scatter(QCPGraph *graph, bool enable);

  void set_scatter(bool enable);

  void rescale_axis();

  void set_x_datetime_fmt();

  bool cache_full(QCPGraph *graph) const;

  void clear_cache(QCPGraph *graph);

  void clear_cache_before(QCPGraph *graph, double x);

 protected slots:
  void replot_impl();

 protected:
  int m_capacity{};
  bool m_sync_xy_range{};
  bool m_first_rescale{true};
};

}  // namespace logviewer
