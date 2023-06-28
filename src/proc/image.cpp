#include "proc/image.h"

#include <ranges>

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QDir>

namespace logviewer
{
namespace proc
{

ImageProc::ImageProc(QObject *parent)
    : QObject(parent)
{
}

ImageProc::~ImageProc() {}

QStringList ImageProc::images() const { return m_images; }

void ImageProc::clear() { m_images.clear(); }

void ImageProc::load(const QString &dirname)
{
  using ResultType = std::pair<bool, QStringList>;
  auto load_impl = [](const QString &dirname)
  {
    QDir dir(dirname);
    if (!dir.exists())
    {
      return ResultType(false, QStringList{});
    }
    auto fileinfos =
        dir.entryInfoList({"*.jpg"}, QDir::Filter::Files, QDir::SortFlag::Name);
    auto filenames = std::views::transform(fileinfos,
                                           [](const QFileInfo &info) {
                                             return info.absoluteFilePath();
                                           }) |
                     std::ranges::to<QStringList>();
    return ResultType(true, filenames);
  };

  auto future = QtConcurrent::run(load_impl, dirname);
  auto watcher = new QFutureWatcher<ResultType>;
  connect(watcher,
          &QFutureWatcher<ResultType>::finished,
          this,
          [=]()
          {
            auto [success, images] = watcher->result();
            m_images = std::move(images);
            emit load_finished(success);
          });
  watcher->setFuture(future);
}

}  // namespace proc

}  // namespace logviewer
