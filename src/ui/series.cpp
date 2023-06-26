#include "ui/series.h"

//#include <algorithm>
//#include <concepts>
//#include <ranges>

//#include "util/algo_util.h"

namespace logviewer
{
namespace
{
//template <typename T>
//concept point_type = std::is_same_v<T, QPoint> || std::is_same_v<T, QPointF>;

// template <typename T>
// bool compare_x(const T &a, const T &b)
// {
//   return a.x() < b.x();
// }

// template <typename T>
// bool compare_y(const T &a, const T &b)
// {
//   return a.y() < b.y();
// }

bool compare_x(const QPointF &a, const QPointF &b) { return a.x() < b.x(); }

bool compare_y(const QPointF &a, const QPointF &b) { return a.y() < b.y(); }

}  // namespace

Series::Series(QObject *parent)
    : QScatterSeries(parent)
{
//  clear();
}

Series::~Series() {}

//void Series::append(qreal x, qreal y) { return append(QPointF{x, y}); }

//void Series::append(const QPointF &pt) { return append(QList<QPointF>{pt}); }

//void Series::append(const QList<QPointF> &pts)
//{
//  if (pts.empty())
//  {
//    return;
//  }

//  const auto [min_x, max_x] = std::ranges::minmax_element(pts, compare_x);
//  const auto [min_y, max_y] = std::ranges::minmax_element(pts, compare_y);

//  if (AlgoUtil::update_min_max(min_x->x(), m_min_x, m_max_x) ||
//      AlgoUtil::update_min_max(max_x->x(), m_min_x, m_max_x))
//  {
//    // TODO(ldw): AxisX.setRange
//  }

//  if (AlgoUtil::update_min_max(min_y->y(), m_min_y, m_max_y) ||
//      AlgoUtil::update_min_max(max_y->y(), m_min_y, m_max_y))
//  {
//    // TODO(ldw): AxisY.setRange
//  }
//  return QXYSeries::append(pts);
//}

//void Series::clear()
//{
//  m_min_x = DBL_MAX;
//  m_max_x = DBL_MIN;
//  m_min_y = DBL_MAX;
//  m_max_y = DBL_MIN;
//  return QXYSeries::clear();
//}

}  // namespace logviewer
