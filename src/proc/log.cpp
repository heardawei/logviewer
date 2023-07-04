#include "proc/log.h"

#include <format>
#include <span>

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QtDebug>

namespace logviewer
{
namespace
{
double to_double(const QString &str) { return str.toDouble(); }
}  // namespace

bool RealListData::parse(const QStringList &toks)
{
  if (toks.size() != cols())
  {
    return false;
  }

  m_data = std::views::all(toks) | std::views::transform(to_double) |
           std::ranges::to<QVector<double>>();

  return true;
}

QVector<double> &RealListData::data() { return m_data; }

const QVector<double> &RealListData::data() const { return m_data; }

double &RealListData::operator[](qsizetype i) { return m_data[i]; }

const double &RealListData::operator[](qsizetype i) const { return m_data[i]; }

constexpr qsizetype IMUInputData::cols() const
{
  return std::to_underlying(Index::MAX);
}

double &IMUInputData::operator[](Index idx)
{
  return m_data[std::to_underlying(idx)];
}

const double &IMUInputData::operator[](Index idx) const
{
  return m_data[std::to_underlying(idx)];
}

constexpr qsizetype OdomInputData::cols() const
{
  return std::to_underlying(Index::MAX);
}

double &OdomInputData::operator[](Index idx)
{
  return m_data[std::to_underlying(idx)];
}

const double &OdomInputData::operator[](Index idx) const
{
  return m_data[std::to_underlying(idx)];
}

constexpr qsizetype CameraInputData::cols() const
{
  return std::to_underlying(Index::MAX);
}

double &CameraInputData::operator[](Index idx)
{
  return m_data[std::to_underlying(idx)];
}

const double &CameraInputData::operator[](Index idx) const
{
  return m_data[std::to_underlying(idx)];
}

double &Quantity::data(Index idx) { return m_data[std::to_underlying(idx)]; }

const double &Quantity::data(Index idx) const
{
  return m_data[std::to_underlying(idx)];
}

constexpr qsizetype Quantity::cols() const
{
  return std::to_underlying(Index::MAX);
}

constexpr qsizetype StateQuantityCovariance::cols() const { return 18 * 18; }

bool BaseRecord::parse(const QStringList &toks)
{
  auto size = input_data_cols() + m_real_quantity.cols() +
              m_state_quantity.cols() + m_state_quantity_covariance.cols();
  if (toks.size() != size)
  {
    qDebug() << std::format("{} != {} ({} + {} + {} + {})",
                            toks.size(),
                            size,
                            input_data_cols(),
                            m_real_quantity.cols(),
                            m_state_quantity.cols(),
                            m_state_quantity_covariance.cols())
                    .data();
    return false;
  }

  // 解析传感器输入数据
  qsizetype offset = 0;
  qsizetype count = input_data_cols();
  if (!parse_input_data(toks.sliced(offset, count)))
  {
    return false;
  }

  // 解析真实量
  offset += count;
  count = m_real_quantity.cols();
  if (!m_real_quantity.parse(toks.sliced(offset, count)))
  {
    return false;
  }

  // 解析状态量
  offset += count;
  count = m_state_quantity.cols();
  if (!m_state_quantity.parse(toks.sliced(offset, count)))
  {
    return false;
  }

  // 解析状态量协方差
  offset += count;
  count = m_state_quantity_covariance.cols();
  // TODO(ldw): optimiz
  // if (!m_state_quantity_covariance.parse(toks.sliced(offset, count)))
  // {
  //   return false;
  // }

  return true;
}

RealQuantity &BaseRecord::real_quantity() { return m_real_quantity; }

StateQuantity &BaseRecord::state_quantity() { return m_state_quantity; }

StateQuantityCovariance &BaseRecord::state_quantity_covariance()
{
  return m_state_quantity_covariance;
}

const RealQuantity &BaseRecord::real_quantity() const
{
  return m_real_quantity;
}

const StateQuantity &BaseRecord::state_quantity() const
{
  return m_state_quantity;
}

const StateQuantityCovariance &BaseRecord::state_quantity_covariance() const
{
  return m_state_quantity_covariance;
}

BaseRecord::SensorType IMURecord::sensor_type() { return SensorType::IMU; }

qsizetype IMURecord::input_data_cols() const { return m_input_data.cols(); }

bool IMURecord::parse_input_data(const QStringList &toks)
{
  return m_input_data.parse(toks);
}

BaseRecord::SensorType OdomRecord::sensor_type() { return SensorType::ODOM; }

qsizetype OdomRecord::input_data_cols() const { return m_input_data.cols(); }

bool OdomRecord::parse_input_data(const QStringList &toks)
{
  return m_input_data.parse(toks);
}

BaseRecord::SensorType CameraRecord::sensor_type()
{
  return SensorType::CAMERA;
}

qsizetype CameraRecord::input_data_cols() const { return m_input_data.cols(); }

bool CameraRecord::parse_input_data(const QStringList &toks)
{
  return m_input_data.parse(toks);
}

BaseRecord::SensorType GNSSRecord::sensor_type() { return SensorType::GNSS; }

qsizetype GNSSRecord::input_data_cols() const { return m_input_data.cols(); }

bool GNSSRecord::parse_input_data(const QStringList &toks)
{
  return m_input_data.parse(toks);
}

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

QSharedPointer<BaseRecord> LogReader::parse_line(const QString &line)
{
  const auto toks = line.split(' ', Qt::SplitBehaviorFlags::SkipEmptyParts);

  QSharedPointer<BaseRecord> ptr;

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
    ptr = QSharedPointer<IMURecord>::create();
    break;
  case BaseRecord::SensorType::ODOM:
    ptr = QSharedPointer<OdomRecord>::create();
    break;
  case BaseRecord::SensorType::CAMERA:
    ptr = QSharedPointer<CameraRecord>::create();
    break;
  case BaseRecord::SensorType::GNSS:
    ptr = QSharedPointer<GNSSRecord>::create();
    break;
  default:
    return ptr;
  }

  auto views =
      std::views::all(toks) |
      std::views::transform([](const QString &s) { return QStringView(s); }) |
      std::ranges::to<QList<QStringView>>();

  if (!ptr->parse(toks.sliced(1)))
  {
    ptr.clear();
  }

  return ptr;
}

Log::Log(QObject *parent)
    : QObject(parent)
    , m_reader(new LogReader)
{
  m_records.reserve(100000);
  m_reader->moveToThread(&m_reader_thread);

  connect(m_reader, &LogReader::started, this, &Log::started);
  connect(m_reader, &LogReader::line_parsed, this, &Log::on_line_parsed);
  connect(m_reader, &LogReader::finished, this, &Log::finished);
  connect(m_reader, &LogReader::error, this, &Log::error);
  m_reader_thread.start();
}

Log::~Log()
{
  m_reader->deleteLater();
  m_reader_thread.quit();
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

void Log::parse(const QString &filename)
{
  QTimer::singleShot(0, m_reader, [=]() { m_reader->do_parse(filename); });
}

void Log::clear() { m_records.clear(); }

void Log::on_line_parsed(QSharedPointer<BaseRecord> ptr)
{
  auto size = m_records.size();
  m_records.emplace_back(std::move(ptr));
  emit line_parsed(size);
}

}  // namespace logviewer
