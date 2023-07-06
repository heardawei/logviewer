#include "data/record.h"

#include <format>
#include <ranges>

#include <QtCore/QtDebug>

namespace logviewer
{

namespace
{
double to_double(const std::string &str) { return std::stod(str); }
}  // namespace

bool RealListData::parse(std::span<std::string> toks)
{
  if (toks.size() != cols())
  {
    return false;
  }

  m_data = std::views::all(toks) | std::views::transform(to_double) |
           std::ranges::to<std::vector<double>>();

  return true;
}

std::vector<double> &RealListData::data() { return m_data; }

const std::vector<double> &RealListData::data() const { return m_data; }

double &RealListData::operator[](size_t i) { return m_data[i]; }

const double &RealListData::operator[](size_t i) const { return m_data[i]; }

size_t IMUInputData::cols() const { return std::to_underlying(Index::MAX); }

double &IMUInputData::operator[](Index idx)
{
  return m_data[std::to_underlying(idx)];
}

const double &IMUInputData::operator[](Index idx) const
{
  return m_data[std::to_underlying(idx)];
}

size_t OdomInputData::cols() const { return std::to_underlying(Index::MAX); }

double &OdomInputData::operator[](Index idx)
{
  return m_data[std::to_underlying(idx)];
}

const double &OdomInputData::operator[](Index idx) const
{
  return m_data[std::to_underlying(idx)];
}

size_t CameraInputData::cols() const { return std::to_underlying(Index::MAX); }

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

size_t Quantity::cols() const { return std::to_underlying(Index::MAX); }

size_t StateQuantityCovariance::cols() const { return 18 * 18; }

bool BaseRecord::parse(std::span<std::string> toks)
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
  size_t offset = 0;
  size_t count = input_data_cols();
  if (!parse_input_data(toks.subspan(offset, count)))
  {
    return false;
  }

  // 解析真实量
  offset += count;
  count = m_real_quantity.cols();
  if (!m_real_quantity.parse(toks.subspan(offset, count)))
  {
    return false;
  }

  // 解析状态量
  offset += count;
  count = m_state_quantity.cols();
  if (!m_state_quantity.parse(toks.subspan(offset, count)))
  {
    return false;
  }

  // 解析状态量协方差
  offset += count;
  count = m_state_quantity_covariance.cols();
  // TODO(ldw): optimiz
  // if (!m_state_quantity_covariance.parse(toks.subspan(offset, count)))
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

size_t IMURecord::input_data_cols() const { return m_input_data.cols(); }

bool IMURecord::parse_input_data(std::span<std::string> toks)
{
  return m_input_data.parse(toks);
}

BaseRecord::SensorType OdomRecord::sensor_type() { return SensorType::ODOM; }

size_t OdomRecord::input_data_cols() const { return m_input_data.cols(); }

bool OdomRecord::parse_input_data(std::span<std::string> toks)
{
  return m_input_data.parse(toks);
}

BaseRecord::SensorType CameraRecord::sensor_type()
{
  return SensorType::CAMERA;
}

size_t CameraRecord::input_data_cols() const { return m_input_data.cols(); }

bool CameraRecord::parse_input_data(std::span<std::string> toks)
{
  return m_input_data.parse(toks);
}

BaseRecord::SensorType GNSSRecord::sensor_type() { return SensorType::GNSS; }

size_t GNSSRecord::input_data_cols() const { return m_input_data.cols(); }

bool GNSSRecord::parse_input_data(std::span<std::string> toks)
{
  return m_input_data.parse(toks);
}

}  // namespace logviewer
