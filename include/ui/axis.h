#pragma once

#include <QValueAxis>

namespace logviewer
{
class Axis : public QValueAxis
{
 public:
  Axis(QObject* parent = nullptr);
  ~Axis();

 public slots:
  void update_range(qreal v);
  void update_range(const QList<qreal>& v);

 private:
  qreal m_min{DBL_MAX};
  qreal m_max{DBL_MIN};
};

}  // namespace logviewer
