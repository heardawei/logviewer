#pragma once

#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedLayout>
#include <QtWidgets/QStackedWidget>

namespace logviewer
{

class Image : public QLabel
{
  Q_OBJECT
 public:
  Image(QWidget *parent = nullptr);
  virtual ~Image();

  const QString &filename() const;

 public slots:
  void filename(QString filename);

 private:
  QString m_filename;
};

class ImageViewer : public Image
{
  Q_OBJECT
 public:
  Q_PROPERTY(qsizetype index READ index NOTIFY index_changed)

  ImageViewer(QWidget *parent = nullptr);
  virtual ~ImageViewer();

  QStringList images() const;
  qsizetype index() const;

 signals:
  void index_changed(int);

 public slots:
  void set_images(QStringList images);
  void play_until(double time_ms);

 private slots:
  void on_next_image_requested();
  void set_index(qsizetype index);

 private:
  QStringList m_images;
  QVector<double> m_image_times;
  qsizetype m_index{};
  QTimer m_timer;
  double m_play_until_time{};
};

}  // namespace logviewer
