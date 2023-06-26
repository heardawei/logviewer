#include "ui/plotter.h"

#include <algorithm>
#include <ranges>

namespace logviewer
{
namespace
{

bool compare_x(const QPointF &a, const QPointF &b) { return a.x() < b.x(); }

bool compare_y(const QPointF &a, const QPointF &b) { return a.y() < b.y(); }

qreal proj_x(const QPointF &pt) { return pt.x(); }

qreal proj_y(const QPointF &pt) { return pt.y(); }

}  // namespace

Plotter::Plotter(QWidget *parent)
    : QChartView(parent)
    , m_chart(new QChart)
    , m_x_axis(new Axis)
    , m_y_axis(new Axis)
{
  // 图例
  m_chart->legend()->setAcceptHoverEvents(true);
  m_chart->legend()->setShowToolTips(true);

  // 标题
  // m_chart->setTitle("Example");

  // 添加动画效果
  m_chart->setAnimationOptions(QChart::SeriesAnimations);

  // 为图表添加x轴、y轴
  m_chart->addAxis(m_x_axis, Qt::AlignBottom);
  m_chart->addAxis(m_y_axis, Qt::AlignLeft);

  // 显示图表
  setChart(m_chart);
  // 反锯齿
  setRenderHint(QPainter::Antialiasing);
}

Plotter::~Plotter() {}

qsizetype Plotter::create_series(const QString &name)
{
  auto id = m_chart->series().size();

  auto series = new Series;
  series->setName(name);
  series->attachAxis(m_x_axis);
  series->attachAxis(m_y_axis);

  m_chart->addSeries(series);

  return id;
}

qsizetype Plotter::create_series(const QString &name, const QList<QPointF> &pts)
{
  auto id = create_series(name);
  append(id, pts);
  return id;
}

bool Plotter::append(qsizetype series_id, qreal x, qreal y)
{
  return append(series_id, QPointF(x, y));
}

bool Plotter::append(qsizetype series_id, const QPointF &pt)
{
  return append(series_id, QList<QPointF>{pt});
}

bool Plotter::append(qsizetype series_id, const QList<QPointF> &pts)
{
  auto series = m_chart->series();
  if (series_id >= series.size())
  {
    return false;
  }

  auto serie = qobject_cast<Series *>(series[series_id]);
  if (!serie)
  {
    return false;
  }

  if (pts.empty())
  {
    return false;
  }

  auto xs =
      std::views::transform(pts, proj_x) | std::ranges::to<QList<qreal>>();
  m_x_axis->update_range(xs);

  auto ys =
      std::views::transform(pts, proj_y) | std::ranges::to<QList<qreal>>();
  m_y_axis->update_range(ys);

  serie->append(pts);
  return true;
}

}  // namespace logviewer
