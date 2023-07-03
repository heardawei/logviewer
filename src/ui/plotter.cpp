#include "ui/plotter.h"

#include <ranges>

namespace logviewer
{
Plotter::Plotter(QWidget *parent)
    : QCustomPlot(parent)
{
  replot();
}

Plotter::~Plotter() {}

QCPGraph *Plotter::create_graph(const QString &name)
{
  auto graph = addGraph();
  assert(graph);

  graph->setName(name);
  graph->setBrush(Qt::NoBrush);

  return graph;
}

QCPGraph *Plotter::create_graph(const QString &name,
                                const QVector<double> &keys,
                                const QVector<double> &values)
{
  auto graph = create_graph(name);
  graph->setData(keys, values);
  auto minmaxx = std::minmax_element(keys.cbegin(), keys.cend());
  auto minmaxy = std::minmax_element(values.cbegin(), values.cend());
  graph->keyAxis()->setRange(*minmaxx.first, *minmaxx.second);
  graph->valueAxis()->setRange(*minmaxy.first, *minmaxy.second);
  replot();
  return graph;
}

}  // namespace logviewer
