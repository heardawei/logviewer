#pragma once

#include <algorithm>
#include <ranges>

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QVector>

namespace logviewer
{
class RealListData
{
 public:
  bool parse(const QStringList &toks);

  QVector<double> &data();
  const QVector<double> &data() const;

  double &operator[](qsizetype i);
  const double &operator[](qsizetype i) const;

  virtual constexpr qsizetype cols() const = 0;

 protected:
  QVector<double> m_data;
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

  double &operator[](Index idx);
  const double &operator[](Index idx) const;

 protected:
  QVector<double> m_data;
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

  double &operator[](Index idx);

  const double &operator[](Index idx) const;

 protected:
  QVector<double> m_data;
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

  double &operator[](Index idx);
  const double &operator[](Index idx) const;

 protected:
  QVector<double> m_data;
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

  double &data(Index idx);
  const double &data(Index idx) const;

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

class LogReader : public QObject
{
  Q_OBJECT

 public slots:
  void do_parse(const QString &filename);

 signals:
  void started();
  void line_parsed(QSharedPointer<BaseRecord>);
  void finished();
  void error(const QString &);

 protected:
  QSharedPointer<BaseRecord> parse_line(const QString &line);
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

 signals:
  void started();
  void line_parsed(qsizetype);
  void finished();
  void error(const QString &);

 public slots:
  void parse(const QString &filename);
  void clear();

 protected slots:
  void on_line_parsed(QSharedPointer<BaseRecord> ptr);

 protected:
  QVector<QSharedPointer<BaseRecord>> m_records;
  QThread m_reader_thread;
  LogReader *m_reader;
};

}  // namespace logviewer
