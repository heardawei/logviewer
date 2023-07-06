#include "ui/image_viewer.h"

#include <algorithm>
#include <filesystem>
#include <ranges>
#include <utility>

#include <QFileInfo>

namespace logviewer
{

Image::Image(QWidget *parent)
{
  setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
}

Image::~Image() {}

const QString &Image::filename() const { return m_filename; }

void Image::filename(QString filename)
{
  setPixmap(QPixmap(filename));
  setScaledContents(true);
  m_filename = std::move(filename);
}

ImageViewer::ImageViewer(QWidget *parent)
    : Image(parent)
{
  connect(
      &m_timer, &QTimer::timeout, this, &ImageViewer::on_next_image_requested, Qt::ConnectionType::DirectConnection);
}

ImageViewer::~ImageViewer() {}

QStringList ImageViewer::images() const { return m_images; }

qsizetype ImageViewer::index() const { return m_index; }

void ImageViewer::set_images(QStringList images)
{
  auto parse_file_time = [](const auto &img)
  { return QFileInfo(img).completeBaseName().toDouble(); };
  m_images = std::move(images);
  m_image_times = std::views::all(m_images) |
                  std::views::transform(parse_file_time) |
                  std::ranges::to<QVector<double>>();
  set_index(0);
}

void ImageViewer::play_until(double time)
{
  m_play_until_time = time;

  auto next = index() + 1;

  if (next >= m_images.size())
  {  // 已显示最后一张图片
    return;
  }

  if (m_image_times[next] > m_play_until_time)
  {  // 没有更多图片可供显示
    return;
  }

  qDebug() << "预备显示图片: " << m_images[next] << ", 直到: " << m_play_until_time;

  set_index(next);
}

void ImageViewer::on_next_image_requested()
{
  auto next = index() + 1;

  if (next >= m_images.size())
  {  // 已显示最后一张图片
    m_timer.stop();
    return;
  }

  if (m_image_times[next] > m_play_until_time)
  {  // 没有更多图片可供显示
    m_timer.stop();
    return;
  }

  qDebug() << "显示图片: " << m_images[next] << ", 直到: " << m_play_until_time;

  set_index(next);
}

void ImageViewer::set_index(qsizetype index)
{
  m_index = index;
  filename(m_images[m_index]);
  emit index_changed(m_index);
}

}  // namespace logviewer
