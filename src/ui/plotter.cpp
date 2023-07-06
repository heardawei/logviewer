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

int Plotter::capacity() const { return m_capacity; }

void Plotter::set_capacity(int capacity) { m_capacity = capacity; }

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

  rescale_axis();

  if (m_sync_xy_range)
  {
    const auto x_range = graph->keyAxis()->range();
    const auto y_range = graph->valueAxis()->range();
    const auto lower = std::min(x_range.lower, y_range.lower);
    const auto upper = std::max(x_range.upper, y_range.upper);
    graph->keyAxis()->setRange(lower, upper);
    graph->valueAxis()->setRange(lower, upper);
  }

  replot(QCustomPlot::RefreshPriority::rpQueuedReplot);
}

void Plotter::add_data(QCPGraph *graph, double x, double y)
{
  if (cache_full(graph))
  {
    clear_cache(graph);
  }

  graph->addData(x, y);

  rescale_axis();

  if (m_sync_xy_range)
  {
    const auto x_range = graph->keyAxis()->range();
    const auto y_range = graph->valueAxis()->range();
    const auto lower = std::min(x_range.lower, y_range.lower);
    const auto upper = std::max(x_range.upper, y_range.upper);
    graph->keyAxis()->setRange(lower, upper);
    graph->valueAxis()->setRange(lower, upper);
  }

  replot(QCustomPlot::RefreshPriority::rpQueuedReplot);
}

void Plotter::add_data(QCPGraph *graph,
                       const QVector<double> &keys,
                       const QVector<double> &values)
{
  if (cache_full(graph))
  {
    clear_cache(graph);
  }

  graph->addData(keys, values);

  rescale_axis();

  if (m_sync_xy_range)
  {
    const auto x_range = graph->keyAxis()->range();
    const auto y_range = graph->valueAxis()->range();
    const auto lower = std::min(x_range.lower, y_range.lower);
    const auto upper = std::max(x_range.upper, y_range.upper);
    graph->keyAxis()->setRange(lower, upper);
    graph->valueAxis()->setRange(lower, upper);
  }

  replot(QCustomPlot::RefreshPriority::rpQueuedReplot);
}

void Plotter::set_sync_xy_range(bool enable) { m_sync_xy_range = enable; }

void Plotter::set_scatter(QCPGraph *graph, bool enable)
{
  if (enable)
  {
    graph->setLineStyle(QCPGraph::LineStyle::lsNone);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot, 2));
  }
  else
  {
    graph->setLineStyle(QCPGraph::LineStyle::lsLine);
  }
}

void Plotter::set_scatter(bool enable)
{
  for (int i = 0; i < graphCount(); i++)
  {
    set_scatter(graph(i), enable);
  }
}

void Plotter::rescale_axis()
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

void Plotter::set_x_datetime_fmt()
{
  // configure bottom axis to show date instead of number:
  auto dateTicker = QSharedPointer<QCPAxisTickerDateTime>::create();
  dateTicker->setDateTimeFormat("HH:mm:ss.zzz\nyyyy-MM-dd");
  xAxis->setTicker(dateTicker);
}

bool Plotter::cache_full(QCPGraph *graph) const
{
  return (capacity() > 0) && (graph->dataCount() > capacity());
}

void Plotter::clear_cache(QCPGraph *graph)
{
  //  graph->data()->clear();
  for (int i = 0; i < graphCount(); i++)
  {
    this->graph(i)->data()->clear();
  }
  rescaleAxes();
  m_first_rescale = true;
}

void Plotter::clear_cache_before(QCPGraph *graph, double x)
{
  graph->data()->removeBefore(x);
}

}  // namespace logviewer
