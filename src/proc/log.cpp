#include "proc/log.h"

#include <format>
#include <span>

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QtDebug>

namespace logviewer
{
void LogReader::do_parse(const QString &filename)
{
  QFile fin(filename);
  if (!fin.open(QIODeviceBase::OpenModeFlag::ReadOnly))
  {
    auto e = QStringLiteral("打开文件失败: %1, 原因: %2")
                 .arg(filename)
                 .arg(fin.errorString());
    qDebug() << e;
    emit error(e);
    return;
  }

  QTextStream sin(&fin);

  while (true)
  {
    const auto &line = sin.readLine();
    if (line.isEmpty())
    {
      break;
    }

    auto ptr = parse_line(line);
    if (ptr)
    {
      emit line_parsed(ptr);
    }
    else
    {
      qWarning() << "parse line error";
    }
  }
}

BaseRecordPtr LogReader::parse_line(const QString &line)
{
  const auto toks = line.split(' ', Qt::SplitBehaviorFlags::SkipEmptyParts);

  BaseRecordPtr ptr;

  if (toks.empty())
  {
    return ptr;
  }

  bool to_int_ok = false;
  auto type =
      static_cast<BaseRecord::SensorType>(toks.front().toInt(&to_int_ok));
  if (!to_int_ok)
  {
    return ptr;
  }

  switch (type)
  {
  case BaseRecord::SensorType::IMU:
    ptr = std::make_shared<IMURecord>();
    break;
  case BaseRecord::SensorType::ODOM:
    ptr = std::make_shared<OdomRecord>();
    break;
  case BaseRecord::SensorType::CAMERA:
    ptr = std::make_shared<CameraRecord>();
    break;
  case BaseRecord::SensorType::GNSS:
    ptr = std::make_shared<GNSSRecord>();
    break;
  default:
    return ptr;
  }

  auto views =
      std::views::all(toks) | std::views::drop(1) |
      std::views::transform([](const QString &s) { return s.toStdString(); }) |
      std::ranges::to<std::vector<std::string>>();

  if (!ptr->parse(views))
  {
    ptr.reset();
  }

  return ptr;
}

Log::Log(QObject *parent)
    : QObject(parent)
    , m_reader(new LogReader)
{
  m_records.reserve(100000);
  m_reader->moveToThread(&m_reader_thread);
  m_reader->setParent(&m_reader_thread);
  connect(m_reader, &LogReader::started, this, &Log::started);
  connect(m_reader, &LogReader::line_parsed, this, &Log::on_line_parsed);
  connect(m_reader, &LogReader::finished, this, &Log::finished);
  connect(m_reader, &LogReader::finished, this, [=]() { m_running = false; });
  connect(m_reader, &LogReader::error, this, &Log::error);
  m_reader_thread.start();
}

Log::~Log()
{
  m_reader_thread.terminate();  // TODO(ldw): replace with quit()
  m_reader_thread.wait();
}

double Log::t(qsizetype i) const
{
  return m_records.at(i)->real_quantity().data(RealQuantity::Index::TIME);
}

double Log::bg_x(qsizetype i) const
{
  return m_records.at(i)->real_quantity().data(RealQuantity::Index::BG_X);
}

double Log::bg_y(qsizetype i) const
{
  return m_records.at(i)->real_quantity().data(RealQuantity::Index::BG_Y);
}

double Log::bg_z(qsizetype i) const
{
  return m_records.at(i)->real_quantity().data(RealQuantity::Index::BG_Z);
}

double Log::ba_x(qsizetype i) const
{
  return m_records.at(i)->real_quantity().data(RealQuantity::Index::BA_X);
}

double Log::ba_y(qsizetype i) const
{
  return m_records.at(i)->real_quantity().data(RealQuantity::Index::BA_Y);
}

double Log::ba_z(qsizetype i) const
{
  return m_records.at(i)->real_quantity().data(RealQuantity::Index::BA_Z);
}

double Log::px(qsizetype i) const
{
  return m_records.at(i)->real_quantity().data(RealQuantity::Index::P_X);
}

double Log::py(qsizetype i) const
{
  return m_records.at(i)->real_quantity().data(RealQuantity::Index::P_Y);
}

bool Log::get_until(double elapse, QVector<BaseRecordPtr> &records)
{
  if (m_index >= m_records.size() || m_records.empty())
  {
    return m_running;
  }

  const auto base_time =
      m_records.front()->real_quantity().data(RealQuantity::Index::TIME);
  const auto curr_time = base_time + elapse;

  auto beg = m_records.begin();
  std::advance(beg, m_index);

  const auto end = std::find_if(
      beg,
      m_records.end(),
      [=](BaseRecordPtr p) {
        return curr_time < p->real_quantity().data(RealQuantity::Index::TIME);
      });

  const auto size = std::distance(beg, end);
  records.reserve(size);
  records = std::decay_t<decltype(records)>(beg, end);
  m_index += size;

  return true;
}

void Log::parse(const QString &filename)
{
  m_running = true;
  QTimer::singleShot(0, m_reader, [=]() { m_reader->do_parse(filename); });
}

void Log::clear() { m_records.clear(); }

void Log::on_line_parsed(BaseRecordPtr ptr)
{
  auto size = m_records.size();
  m_records.emplace_back(std::move(ptr));
  emit line_parsed(size);
}

}  // namespace logviewer
