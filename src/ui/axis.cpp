#include "ui/axis.h"

#include <ranges>

#include "util/algo_util.h"

namespace logviewer
{
Axis::Axis(QObject* parent)
    : QValueAxis(parent)
{
}

Axis::~Axis() {}

void Axis::update_range(qreal v)
{
  if (AlgoUtil::update_min_max(v, m_min, m_max))
  {
    setRange(m_min, m_max);
  }
}

void Axis::update_range(const QList<qreal>& vals)
{
  if (vals.empty())
  {
    return;
  }

  const auto [min, max] = std::ranges::minmax_element(vals);

  if (AlgoUtil::update_min_max(*min, m_min, m_max) ||
      AlgoUtil::update_min_max(*max, m_min, m_max))
  {
    setRange(m_min, m_max);
  }
}

}  // namespace logviewer