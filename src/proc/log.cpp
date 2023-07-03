#include "proc/log.h"

#include <format>

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

QSharedPointer<BaseRecord> Record::parse(const QStringList &toks)
{
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

  if (!ptr->parse(toks.sliced(1)))
  {
    //
    ptr.clear();
  }

  return ptr;
}

Log::Log(QObject *parent)
    : QObject(parent)
{
}

Log::~Log() {}

QVector<double> Log::t() const
{
  auto proj = [](const QSharedPointer<BaseRecord> &rec)
  { return rec->real_quantity().data(RealQuantity::Index::TIME); };
  return std::views::all(m_records) | std::views::transform(proj) |
         std::ranges::to<QVector<double>>();
}

QVector<double> Log::bg_x() const
{
  auto proj = [](const QSharedPointer<BaseRecord> &rec)
  { return rec->real_quantity().data(RealQuantity::Index::BG_X); };
  return std::views::all(m_records) | std::views::transform(proj) |
         std::ranges::to<QVector<double>>();
}

QVector<double> Log::bg_y() const
{
  auto proj = [](const QSharedPointer<BaseRecord> &rec)
  { return rec->real_quantity().data(RealQuantity::Index::BG_Y); };
  return std::views::all(m_records) | std::views::transform(proj) |
         std::ranges::to<QVector<double>>();
}

QVector<double> Log::bg_z() const
{
  auto proj = [](const QSharedPointer<BaseRecord> &rec)
  { return rec->real_quantity().data(RealQuantity::Index::BG_Z); };
  return std::views::all(m_records) | std::views::transform(proj) |
         std::ranges::to<QVector<double>>();
}

QVector<double> Log::ba_x() const
{
  auto proj = [](const QSharedPointer<BaseRecord> &rec)
  { return rec->real_quantity().data(RealQuantity::Index::BA_X); };
  return std::views::all(m_records) | std::views::transform(proj) |
         std::ranges::to<QVector<double>>();
}

QVector<double> Log::ba_y() const
{
  auto proj = [](const QSharedPointer<BaseRecord> &rec)
  { return rec->real_quantity().data(RealQuantity::Index::BA_Y); };
  return std::views::all(m_records) | std::views::transform(proj) |
         std::ranges::to<QVector<double>>();
}

QVector<double> Log::ba_z() const
{
  auto proj = [](const QSharedPointer<BaseRecord> &rec)
  { return rec->real_quantity().data(RealQuantity::Index::BA_Z); };
  return std::views::all(m_records) | std::views::transform(proj) |
         std::ranges::to<QVector<double>>();
}

QVector<double> Log::px() const
{
  auto proj = [](const QSharedPointer<BaseRecord> &rec)
  { return rec->real_quantity().data(RealQuantity::Index::P_X); };
  return std::views::all(m_records) | std::views::transform(proj) |
         std::ranges::to<QVector<double>>();
}

QVector<double> Log::py() const
{
  auto proj = [](const QSharedPointer<BaseRecord> &rec)
  { return rec->real_quantity().data(RealQuantity::Index::P_Y); };
  return std::views::all(m_records) | std::views::transform(proj) |
         std::ranges::to<QVector<double>>();
}

void Log::parse(const QString &filename)
{
  auto parse_impl = [=](const QString &filename)
  {
    QFile fin(filename);
    if (!fin.open(QIODeviceBase::OpenModeFlag::ReadOnly))
    {
      qDebug() << "打开文件失败: " << filename
               << ", 原因: " << fin.errorString();
      return false;
    }

    QTextStream sin(&fin);

    while (true)
    {
      const auto &line = sin.readLine();
      if (line.isEmpty())
      {
        break;
      }
      parse_line(line);
    }

    qDebug() << "parsed " << m_records.size() << " rows";

    return true;
  };

  auto future = QtConcurrent::run(parse_impl, filename);
  auto watcher = new QFutureWatcher<bool>;
  connect(watcher,
          &QFutureWatcher<bool>::finished,
          this,
          [=]() { emit parse_finished(watcher->result()); });
  watcher->setFuture(future);
}

void Log::clear() { m_records.clear(); }

bool Log::parse_line(const QString &line)
{
  auto ptr =
      Record::parse(line.split(' ', Qt::SplitBehaviorFlags::SkipEmptyParts));
  if (!ptr)
  {
    return false;
  }

  m_records.emplace_back(std::move(ptr));
  return true;
}

}  // namespace logviewer