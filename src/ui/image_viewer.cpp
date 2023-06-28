#include "ui/image_viewer.h"

#include <algorithm>
#include <filesystem>
#include <ranges>
#include <utility>

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
  m_timer.setSingleShot(true);
  connect(
      &m_timer, &QTimer::timeout, this, &ImageViewer::on_next_image_requested);
}

ImageViewer::~ImageViewer() {}

QStringList ImageViewer::images() const { return m_images; }

qsizetype ImageViewer::index() const { return m_index; }

bool ImageViewer::loop() const { return m_loop; }

void ImageViewer::set_loop(bool loop)
{
  m_loop = loop;
  emit loop_changed(m_loop);
}

void ImageViewer::set_images(QStringList images)
{
  if (images.empty())
  {
    return;
  }
  auto filename_to_time = [](const QString &filename)
  {
    std::filesystem::path filepath(filename.toStdString());
    auto stem = QString::fromStdString(filepath.stem().generic_string());
    return stem.toDouble();
  };
  auto prev_time = filename_to_time(images.front());
  auto calc_interval = [&](qreal next_time)
  {
    auto interval = std::max(next_time - prev_time, 0.0);
    prev_time = next_time;
    return std::round(interval * 1000);
  };
  m_ms_intervals.reserve(images.size());

  m_ms_intervals = std::views::all(images.sliced(1)) |
                   std::views::transform(filename_to_time) |
                   std::views::transform(calc_interval) |
                   std::ranges::to<QList<int>>();
  m_images = std::move(images);

  Q_ASSERT_X(m_ms_intervals.size() + 1 == m_images.size(),
             __FUNCTION__,
             "n张图片必须有n-1个间隔");

  set_index(0);

  m_timer.start(1);
}

void ImageViewer::on_next_image_requested()
{
  auto index = property("index").toLongLong();
  qDebug() << "开始显示第" << index << "张图片: " << m_images[index];

  filename(m_images[index]);  // 设置图片

  if (index < m_ms_intervals.size())
  {
    qDebug() << "下一张图片将在 " << m_ms_intervals[index] << "ms 后开始播放";
    set_index(index + 1);
    m_timer.start(m_ms_intervals[index]);
  }
  else
  {  // 由于n张图片，只有n-1个间隔，"这是最后一张图片"
    if (loop())
    {
      qDebug() << "这是最后一张图片，下一轮将在 1000ms 后从头播放";
      set_index(0);
      m_timer.start(1000);  // 最后一张图片默认播放1000ms
    }
    else
    {
      qDebug() << "这是最后一张图片，播放停止";
    }
  }
}

void ImageViewer::set_index(qsizetype index)
{
  m_index = index;
  emit index_changed(m_index);
}

}  // namespace logviewer
