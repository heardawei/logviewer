#include "ui/plotter.h"

#include <ranges>

namespace logviewer
{
Plotter::Plotter(QWidget *parent)
    : QCustomPlot(parent)
{
  legend->setVisible(true);
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

void Plotter::set_data(QCPGraph *graph,
                       const QVector<double> &keys,
                       const QVector<double> &values)
{
  graph->setData(keys, values);

  if (m_sync_xy_range)
  {
    // TODO(ldw): bug fixed: expand range, not replace range
    const auto minmaxx = std::minmax_element(keys.cbegin(), keys.cend());
    const auto minmaxy = std::minmax_element(values.cbegin(), values.cend());
    const auto min = std::min(*minmaxx.first, *minmaxy.first);
    const auto max = std::max(*minmaxx.second, *minmaxy.second);
    graph->keyAxis()->setRange(min, max);
    graph->valueAxis()->setRange(min, max);
  }
  else
  {
    rescale_axis();
  }
  replot();
}

}  // namespace logviewer
