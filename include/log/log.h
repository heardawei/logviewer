#pragma once

#include <algorithm>
#include <ranges>

#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

namespace logviewer
{
qreal to_double(const QString &str) { return str.toDouble(); }

class RealListData
{
 public:
  bool parse(const QStringList &toks)
  {
    if (toks.size() != cols())
    {
      return false;
    }

    m_data = std::views::transform(toks, to_double) |
             std::ranges::to<QList<qreal>>();

    return true;
  }

  QList<qreal> &data() { return m_data; }
  const QList<qreal> &data() const { return m_data; }

  qreal &operator[](qsizetype i) { return m_data[i]; }
  const qreal &operator[](qsizetype i) const { return m_data[i]; }

  virtual constexpr qsizetype cols() const = 0;

 protected:
  QList<qreal> m_data;
};

// Grav
class Grav : public RealListData
{
 public:
  virtual constexpr qsizetype cols() const override { return 19 * 1; }
};

class Quantity
{
 public:
  enum class Index : qsizetype
  {
    TIME,
    P,
    V,
    R,
    BG,
    BA,
    MAX,
  };

  bool parse(const QStringList &toks)
  {
    if (toks.size() != cols())
    {
      return false;
    }

    m_data = std::views::transform(toks, to_double) |
             std::views::take(std::to_underlying(Index::MAX)) |
             std::ranges::to<QList<qreal>>();
    m_grav.parse(toks.sliced(std::to_underlying(Index::MAX)));

    return true;
  }

  qreal &data(Index idx) { return m_data[std::to_underlying(idx)]; }
  const qreal &data(Index idx) const { return m_data[std::to_underlying(idx)]; }

  Grav &grav() { return m_grav; }
  const Grav &grav() const { return m_grav; }

  constexpr qsizetype cols()
  {
    return std::to_underlying(Index::MAX) + m_grav.cols();
  }

 protected:
  QList<qreal> m_data;
  Grav m_grav;
};

// 真实量（真实位置）
class RealQuantity : public Quantity
{
};

// 状态量（误差）
class StateQuantity : public Quantity
{
};

// 状态量协方差
class StateQuantityCovariance
{
 public:
  bool parse(const QStringList &toks)
  {
    if (toks.size() != cols())
    {
      return false;
    }

    m_data = std::views::transform(toks, to_double) |
             std::ranges::to<QList<qreal>>();

    return true;
  }

  QList<qreal> &data() { return m_data; }
  const QList<qreal> &data() const { return m_data; }

  static constexpr qsizetype cols() { return 18 * 18; }

 protected:
  QList<qreal> m_data;
};

// 抽象传感器类型、输入数据的一行记录的基类
// 其内部包括真实量、状态量、状态量协方差
class BaseRecord
{
 public:
  // 传感器类型
  enum class SensorType
  {
    IMU = 0,
    ODOM = 1,
    CAMERA = 2,
    GNSS = 3,
  };

  virtual SensorType sensor_type() = 0;

  virtual qsizetype input_data_cols() const = 0;

  virtual bool parse_input_data(const QStringList &toks) = 0;

  bool parse(const QStringList &toks)
  {
    if (toks.size() != input_data_cols() + m_real_quantity.cols() +
                           m_state_quantity.cols() +
                           m_state_quantity_covariance.cols())
    {
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
    if (!m_state_quantity_covariance.parse(toks.sliced(offset, count)))
    {
      return false;
    }

    return true;
  }

  RealQuantity &real_quantity() { return m_real_quantity; }
  StateQuantity &state_quantity() { return m_state_quantity; }
  StateQuantityCovariance &state_quantity_covariance()
  {
    return m_state_quantity_covariance;
  }

  const RealQuantity &real_quantity() const { return m_real_quantity; }
  const StateQuantity &state_quantity() const { return m_state_quantity; }
  const StateQuantityCovariance &state_quantity_covariance() const
  {
    return m_state_quantity_covariance;
  }

 protected:
  RealQuantity m_real_quantity;
  StateQuantity m_state_quantity;
  StateQuantityCovariance m_state_quantity_covariance;
};

// IMU传感器输入数据
class IMUInputData : public RealListData
{
 public:
  enum class Index : qsizetype
  {
    TIME,
    GYRO_X,
    GYRO_Y,
    GYRO_Z,
    ACC_X,
    ACC_Y,
    ACC_Z,
    MAX,
  };

  virtual constexpr qsizetype cols() const override
  {
    return std::to_underlying(Index::MAX);
  }

  qreal &operator[](Index idx)
  {
    return (*static_cast<RealListData *>(this))[std::to_underlying(idx)];
  }
  const qreal &operator[](Index idx) const
  {
    return (*static_cast<const RealListData *>(this))[std::to_underlying(idx)];
  }

 protected:
  QList<qreal> m_data;
};

// Odom传感器输入数据
class OdomInputData : public RealListData
{
 public:
  enum class Index : qsizetype
  {
    TIME,
    VELO_X,
    VELO_Y,
    VELO_Z,
    MAX,
  };

  virtual constexpr qsizetype cols() const override
  {
    return std::to_underlying(Index::MAX);
  }

  qreal &operator[](Index idx)
  {
    return (*static_cast<RealListData *>(this))[std::to_underlying(idx)];
  }

  const qreal &operator[](Index idx) const
  {
    return (*static_cast<const RealListData *>(this))[std::to_underlying(idx)];
  }

 protected:
  QList<qreal> m_data;
};

// Camera传感器输入数据
class CameraInputData : public RealListData
{
 public:
  enum class Index : qsizetype
  {
    TIME,
    LOC_X,
    LOC_Y,
    LOC_Z,
    POSE_X,
    POSE_Y,
    POSE_Z,
    MAX,
  };

  virtual constexpr qsizetype cols() const override
  {
    return std::to_underlying(Index::MAX);
  }

  qreal &operator[](Index idx)
  {
    return (*static_cast<RealListData *>(this))[std::to_underlying(idx)];
  }
  const qreal &operator[](Index idx) const
  {
    return (*static_cast<const RealListData *>(this))[std::to_underlying(idx)];
  }

 protected:
  QList<qreal> m_data;
};

// GNSS传感器输入数据
class GNSSInputData : public CameraInputData
{
};

// 一行IMU传感器数据
class IMURecord : public BaseRecord
{
 public:
  virtual SensorType sensor_type() override { return SensorType::IMU; }

  virtual qsizetype input_data_cols() const override
  {
    return m_input_data.cols();
  }

  virtual bool parse_input_data(const QStringList &toks) override
  {
    return m_input_data.parse(toks);
  }

 protected:
  IMUInputData m_input_data;
};

// 一行Odom传感器数据
class OdomRecord : public BaseRecord
{
 public:
  virtual SensorType sensor_type() override { return SensorType::ODOM; }

  virtual qsizetype input_data_cols() const override
  {
    return m_input_data.cols();
  }

  virtual bool parse_input_data(const QStringList &toks) override
  {
    return m_input_data.parse(toks);
  }

 protected:
  OdomInputData m_input_data;
};

// 一行Camera传感器数据
class CameraRecord : public BaseRecord
{
 public:
  virtual SensorType sensor_type() override { return SensorType::CAMERA; }

  virtual qsizetype input_data_cols() const override
  {
    return m_input_data.cols();
  }

  virtual bool parse_input_data(const QStringList &toks) override
  {
    return m_input_data.parse(toks);
  }

 protected:
  CameraInputData m_input_data;
};

// 一行GNSS传感器数据
class GNSSRecord : public BaseRecord
{
 public:
  virtual SensorType sensor_type() override { return SensorType::GNSS; }

  virtual qsizetype input_data_cols() const override
  {
    return m_input_data.cols();
  }

  virtual bool parse_input_data(const QStringList &toks) override
  {
    return m_input_data.parse(toks);
  }

 protected:
  GNSSInputData m_input_data;
};

// 一行原始数据在这里开始解析
class Record
{
 public:
  static QSharedPointer<BaseRecord> parse(const QStringList &toks)
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
};

class Log
{
 public:
  Log();
  ~Log();

  bool parse(const QString &filename);

 protected:
  bool parse_line(const QString &line);

 protected:
  QList<QSharedPointer<BaseRecord>> m_records;
};

}  // namespace logviewer
