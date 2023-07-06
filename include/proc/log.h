#pragma once

#include <algorithm>
#include <iterator>
#include <ranges>

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QVector>

#include "data/record.h"

namespace logviewer
{
class LogReader : public QObject
{
  Q_OBJECT

 public slots:
  void do_parse(const QString &filename);

 signals:
  void started();
  void line_parsed(BaseRecordPtr);
  void finished();
  void error(const QString &);

 protected:
  BaseRecordPtr parse_line(const QString &line);
};

class Log : public QObject
{
  Q_OBJECT
 public:
  Log(QObject *parent = nullptr);
  ~Log();

  double t(qsizetype i) const;
  double bg_x(qsizetype i) const;
  double bg_y(qsizetype i) const;
  double bg_z(qsizetype i) const;
  double ba_x(qsizetype i) const;
  double ba_y(qsizetype i) const;
  double ba_z(qsizetype i) const;
  double px(qsizetype i) const;
  double py(qsizetype i) const;

  void reset() { m_index = 0; }

  bool get_until(double elapse, QVector<BaseRecordPtr> &records);

 signals:
  void started();
  void line_parsed(qsizetype);
  void finished();
  void error(const QString &);

 public slots:
  void parse(const QString &filename);
  void clear();

 protected slots:
  void on_line_parsed(BaseRecordPtr ptr);

 protected:
  bool m_running{};
  QVector<BaseRecordPtr> m_records;
  qsizetype m_index{};
  QThread m_reader_thread;
  LogReader *m_reader;
};

}  // namespace logviewer
