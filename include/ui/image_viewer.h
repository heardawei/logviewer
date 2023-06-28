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
  Q_PROPERTY(bool loop READ loop WRITE set_loop NOTIFY loop_changed)
  Q_PROPERTY(qsizetype index READ index NOTIFY index_changed)

  ImageViewer(QWidget *parent = nullptr);
  virtual ~ImageViewer();

  QStringList images() const;
  qsizetype index() const;
  bool loop() const;

 signals:
  void loop_changed(bool);
  void index_changed(int);

 public slots:
  void set_loop(bool loop);
  void set_images(QStringList images);

 private slots:
  void on_next_image_requested();
  void set_index(qsizetype index);

 private:
  QStringList m_images;
  QList<int> m_ms_intervals;
  bool m_loop{true};
  qsizetype m_index{};
  QTimer m_timer;
};

}  // namespace logviewer
