#pragma once

#include <algorithm>
#include <ranges>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

namespace logviewer
{
class RealListData
{
 public:
  bool parse(const QStringList &toks);

  QList<qreal> &data();
  const QList<qreal> &data() const;

  qreal &operator[](qsizetype i);
  const qreal &operator[](qsizetype i) const;

  virtual constexpr qsizetype cols() const = 0;

 protected:
  QList<qreal> m_data;
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

  virtual constexpr qsizetype cols() const override;

  qreal &operator[](Index idx);
  const qreal &operator[](Index idx) const;

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

  virtual constexpr qsizetype cols() const override;

  qreal &operator[](Index idx);

  const qreal &operator[](Index idx) const;

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

  virtual constexpr qsizetype cols() const override;

  qreal &operator[](Index idx);
  const qreal &operator[](Index idx) const;

 protected:
  QList<qreal> m_data;
};

// GNSS传感器输入数据
class GNSSInputData : public CameraInputData
{
};

class Quantity : public RealListData
{
 public:
  enum class Index : qsizetype
  {
    TIME,
    P_X,
    P_Y,
    P_Z,
    V_X,
    V_Y,
    V_Z,
    R_X,
    R_Y,
    R_Z,
    BG_X,
    BG_Y,
    BG_Z,
    BA_X,
    BA_Y,
    BA_Z,
    GRAV_X,
    GRAV_Y,
    GRAV_Z,
    MAX,
  };

  qreal &data(Index idx);
  const qreal &data(Index idx) const;

  virtual constexpr qsizetype cols() const override;
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
class StateQuantityCovariance : public RealListData
{
 public:
  virtual constexpr qsizetype cols() const override;
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

  bool parse(const QStringList &toks);

  RealQuantity &real_quantity();
  StateQuantity &state_quantity();
  StateQuantityCovariance &state_quantity_covariance();

  const RealQuantity &real_quantity() const;
  const StateQuantity &state_quantity() const;
  const StateQuantityCovariance &state_quantity_covariance() const;

 protected:
  RealQuantity m_real_quantity;
  StateQuantity m_state_quantity;
  StateQuantityCovariance m_state_quantity_covariance;
};

// 一行IMU传感器数据
class IMURecord : public BaseRecord
{
 public:
  virtual SensorType sensor_type() override;

  virtual qsizetype input_data_cols() const override;

  virtual bool parse_input_data(const QStringList &toks) override;

 protected:
  IMUInputData m_input_data;
};

// 一行Odom传感器数据
class OdomRecord : public BaseRecord
{
 public:
  virtual SensorType sensor_type() override;

  virtual qsizetype input_data_cols() const override;

  virtual bool parse_input_data(const QStringList &toks) override;

 protected:
  OdomInputData m_input_data;
};

// 一行Camera传感器数据
class CameraRecord : public BaseRecord
{
 public:
  virtual SensorType sensor_type() override;

  virtual qsizetype input_data_cols() const override;

  virtual bool parse_input_data(const QStringList &toks) override;

 protected:
  CameraInputData m_input_data;
};

// 一行GNSS传感器数据
class GNSSRecord : public BaseRecord
{
 public:
  virtual SensorType sensor_type() override;

  virtual qsizetype input_data_cols() const override;

  virtual bool parse_input_data(const QStringList &toks) override;

 protected:
  GNSSInputData m_input_data;
};

// 一行原始数据在这里开始解析
class Record
{
 public:
  static QSharedPointer<BaseRecord> parse(const QStringList &toks);
};

class Log : public QObject
{
  Q_OBJECT
 public:
  Log();
  ~Log();

  QList<QPointF> t_bg_x_points() const;
  QList<QPointF> t_bg_y_points() const;
  QList<QPointF> t_bg_z_points() const;
  QList<QPointF> t_ba_x_points() const;
  QList<QPointF> t_ba_y_points() const;
  QList<QPointF> t_ba_z_points() const;
  QList<QPointF> px_py_points() const;

  qsizetype stride() const;

 signals:
  void parse_finished(bool);

 public slots:
  void parse(const QString &filename);
  void stride(qsizetype i);
  void clear();

 protected:
  bool parse_line(const QString &line);

 protected:
  qsizetype m_stride{5};
  QList<QSharedPointer<BaseRecord>> m_records;
};

}  // namespace logviewer
