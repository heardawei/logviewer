#pragma once

#include <QtCore/QObject>
#include <QtCore/QStringList>

namespace logviewer
{
namespace proc
{
class ImageProc : public QObject
{
  Q_OBJECT
 public:
  ImageProc(QObject *parent = nullptr);
  virtual ~ImageProc();

  QStringList images() const;

 signals:
  void load_finished(bool);

 public slots:
  void clear();
  void load(const QString &dirname);

 private:
  QStringList m_images;
};

}  // namespace proc

}  // namespace logviewer
