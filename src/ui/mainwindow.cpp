#include "ui/mainwindow.h"

#include <algorithm>
#include <filesystem>
#include <random>
#include <ranges>

#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedLayout>
#include <QtWidgets/QVBoxLayout>

namespace logviewer
{
namespace
{
void SetImage(Image *img, const QString &filename)
{
  img->setPixmap(QPixmap(filename));
  img->setScaledContents(true);
  img->setSizePolicy(QSizePolicy::Policy::Ignored,
                     QSizePolicy::Policy::Ignored);
}
}  // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_img1(new Image)
    , m_img2(new Image)
    , m_plotter1(new LinePlotter)
    , m_plotter2(new LinePlotter)
{
  auto test_pb = new QPushButton("测试数据");
  auto open_log_pb = new QPushButton("打开日志文件");
  auto open_img1_pb = new QPushButton("打开图片1");
  auto open_img2_pb = new QPushButton("打开图片2");
  auto refresh_pb = new QPushButton("刷新");

  auto header_layout = new QHBoxLayout;
  header_layout->addWidget(test_pb);
  header_layout->addWidget(open_log_pb);
  header_layout->addWidget(open_img1_pb);
  header_layout->addWidget(open_img2_pb);
  header_layout->addWidget(refresh_pb);
  header_layout->addStretch();

  auto header = new QWidget;
  header->setLayout(header_layout);

  auto body_layout = new QGridLayout;
  body_layout->addWidget(m_img1, 0, 0, Qt::AlignCenter);
  body_layout->addWidget(m_img2, 0, 1, Qt::AlignCenter);
  body_layout->addWidget(m_plotter1, 1, 0, Qt::AlignCenter);
  body_layout->addWidget(m_plotter2, 1, 1, Qt::AlignCenter);
  body_layout->setRowStretch(0, 50);
  body_layout->setRowStretch(1, 50);
  body_layout->setColumnStretch(0, 50);
  body_layout->setColumnStretch(1, 50);

  auto body = new QWidget;
  body->setLayout(body_layout);

  auto central_layout = new QVBoxLayout;
  central_layout->addWidget(header);
  central_layout->addWidget(body);

  auto central = new QWidget;
  central->setLayout(central_layout);

  setCentralWidget(central);

  connect(test_pb,
          &QPushButton::clicked,
          this,
          [=]()
          {
            m_plotter1->clear();
            m_plotter2->clear();
            generate_samples();
          });

  connect(open_log_pb,
          &QPushButton::clicked,
          this,
          &MainWindow::on_open_file_clicked);

  connect(open_img1_pb,
          &QPushButton::clicked,
          this,
          &MainWindow::on_open_img1_clicked);

  connect(open_img2_pb,
          &QPushButton::clicked,
          this,
          &MainWindow::on_open_img2_clicked);
}

MainWindow::~MainWindow() {}

void MainWindow::set_t_bg_x_points(const QList<QPointF> &pts)
{
  if (pts.empty())
  {
    return;
  }
  m_plotter1->create_series("t-bg.x", pts);
  qDebug() << "t-bg.x: " << pts.size() << "points";
}

void MainWindow::set_t_bg_y_points(const QList<QPointF> &pts)
{
  if (pts.empty())
  {
    return;
  }
  m_plotter1->create_series("t-bg.y", pts);
  qDebug() << "t-bg.y: " << pts.size() << "points";
}

void MainWindow::set_t_bg_z_points(const QList<QPointF> &pts)
{
  if (pts.empty())
  {
    return;
  }
  m_plotter1->create_series("t-bg.z", pts);
  qDebug() << "t-bg.z: " << pts.size() << "points";
}

void MainWindow::set_t_ba_x_points(const QList<QPointF> &pts)
{
  if (pts.empty())
  {
    return;
  }
  m_plotter1->create_series("t-ba.x", pts);
  qDebug() << "t-ba.x: " << pts.size() << "points";
}

void MainWindow::set_t_ba_y_points(const QList<QPointF> &pts)
{
  if (pts.empty())
  {
    return;
  }
  m_plotter1->create_series("t-ba.y", pts);
  qDebug() << "t-ba.y: " << pts.size() << "points";
}

void MainWindow::set_t_ba_z_points(const QList<QPointF> &pts)
{
  if (pts.empty())
  {
    return;
  }
  m_plotter1->create_series("t-ba.z", pts);
  qDebug() << "t-ba.z: " << pts.size() << "points";
}

void MainWindow::set_t_px_py_points(const QList<QPointF> &pts)
{
  if (pts.empty())
  {
    return;
  }
  m_plotter2->create_series("p.x-p.y", pts);
  qDebug() << "p.x-p.y: " << pts.size() << "points";
}

void MainWindow::on_open_file_clicked()
{
  qDebug() << "开始选择日志文件";
  QSettings settings(QSettings::Format::IniFormat,
                     QSettings::Scope::SystemScope,
                     "Alibaba",
                     "LogViewer");
  qDebug() << "使用缓存: " << settings.fileName()
           << " cache/logdir = " << settings.value("cache/logdir");

  auto filename = QFileDialog::getOpenFileName(
      this, "请选择日志文件", settings.value("cache/logdir").toString());
  if (filename.isEmpty())
  {
    qDebug() << "取消选择日志文件";
    return;
  }
  qDebug() << "已选择日志文件: " << filename;

  settings.setValue(
      "cache/logdir",
      QString::fromStdString(std::filesystem::path(filename.toStdString())
                                 .parent_path()
                                 .generic_string()));
  qDebug() << "更新缓存: " << settings.fileName() << " cache/logdir = "
           << QString::fromStdString(
                  std::filesystem::path(filename.toStdString())
                      .parent_path()
                      .generic_string());

  reload_file(filename);
}

void MainWindow::on_open_img1_clicked()
{
  qDebug() << "开始选择图片1";
  QSettings settings(QSettings::Format::IniFormat,
                     QSettings::Scope::SystemScope,
                     "Alibaba",
                     "LogViewer");
  auto filename = QFileDialog::getOpenFileName(
      this, "请选择图片1", settings.value("cache/img1dir").toString());
  if (filename.isEmpty())
  {
    qDebug() << "取消选择图片1";
    return;
  }
  qDebug() << "已选择图片1: " << filename;

  settings.setValue(
      "cache/img1dir",
      QString::fromStdString(std::filesystem::path(filename.toStdString())
                                 .parent_path()
                                 .generic_string()));
  qDebug() << "更新缓存: " << settings.fileName() << " cache/img1dir = "
           << QString::fromStdString(
                  std::filesystem::path(filename.toStdString())
                      .parent_path()
                      .generic_string());

  SetImage(m_img1, filename);
}

void MainWindow::on_open_img2_clicked()
{
  qDebug() << "开始选择图片2";
  QSettings settings(QSettings::Format::IniFormat,
                     QSettings::Scope::SystemScope,
                     "Alibaba",
                     "LogViewer");
  auto filename = QFileDialog::getOpenFileName(
      this, "请选择图片2", settings.value("cache/img2dir").toString());
  if (filename.isEmpty())
  {
    qDebug() << "取消选择图片2";
    return;
  }
  qDebug() << "已选择图片2: " << filename;

  settings.setValue(
      "cache/img2dir",
      QString::fromStdString(std::filesystem::path(filename.toStdString())
                                 .parent_path()
                                 .generic_string()));
  qDebug() << "更新缓存: " << settings.fileName() << " cache/img2dir = "
           << QString::fromStdString(
                  std::filesystem::path(filename.toStdString())
                      .parent_path()
                      .generic_string());

  SetImage(m_img2, filename);
}

void MainWindow::reload_file(const QString &filename)
{
  m_plotter1->clear();
  m_plotter2->clear();

  // 窗口标题
  setWindowFilePath(filename);

  emit open_log(filename);
}

void MainWindow::generate_samples()
{
  generate_img_samples();
  generate_plotter_samples();
  generate_traj_samples();
}

void MainWindow::generate_img_samples()
{
  SetImage(m_img1, "C:/Users/lidawei/Desktop/补卡截图/v.png");
  SetImage(m_img2, "C:/Users/lidawei/Desktop/补卡截图/v.png");
}

void MainWindow::generate_plotter_samples()
{
  std::random_device r;
  std::default_random_engine e(r());

  constexpr auto samples = 100;

  QList<QPointF> samples_t_bg_x;
  QList<QPointF> samples_t_bg_y;
  QList<QPointF> samples_t_bg_z;
  QList<QPointF> samples_t_ba_x;
  QList<QPointF> samples_t_ba_y;
  QList<QPointF> samples_t_ba_z;

  auto base_time = QDateTime::currentMSecsSinceEpoch();

  std::ranges::transform(std::views::iota(base_time),
                         std::views::iota(0, samples),
                         std::back_inserter(samples_t_bg_x),
                         [&](qint64 t, int)
                         {
                           std::uniform_real_distribution<qreal> u(0, 50);
                           return QPointF(static_cast<qreal>(t), u(e));
                         });

  std::ranges::transform(std::views::iota(base_time),
                         std::views::iota(0, samples),
                         std::back_inserter(samples_t_bg_y),
                         [&](qint64 t, int)
                         {
                           std::uniform_real_distribution<qreal> u(40, 100);
                           return QPointF(static_cast<qreal>(t), u(e));
                         });

  std::ranges::transform(std::views::iota(base_time),
                         std::views::iota(0, samples),
                         std::back_inserter(samples_t_bg_z),
                         [&](qint64 t, int)
                         {
                           std::uniform_real_distribution<qreal> u(90, 150);
                           return QPointF(static_cast<qreal>(t), u(e));
                         });

  std::ranges::transform(std::views::iota(base_time),
                         std::views::iota(0, samples),
                         std::back_inserter(samples_t_ba_x),
                         [&](qint64 t, int)
                         {
                           std::uniform_real_distribution<qreal> u(140, 200);
                           return QPointF(static_cast<qreal>(t), u(e));
                         });

  std::ranges::transform(std::views::iota(base_time),
                         std::views::iota(0, samples),
                         std::back_inserter(samples_t_ba_y),
                         [&](qint64 t, int)
                         {
                           std::uniform_real_distribution<qreal> u(190, 250);
                           return QPointF(static_cast<qreal>(t), u(e));
                         });

  std::ranges::transform(std::views::iota(base_time),
                         std::views::iota(0, samples),
                         std::back_inserter(samples_t_ba_z),
                         [&](qint64 t, int)
                         {
                           std::uniform_real_distribution<qreal> u(240, 300);
                           return QPointF(static_cast<qreal>(t), u(e));
                         });

  m_plotter1->create_series("t-bg.x", samples_t_bg_x);
  m_plotter1->create_series("t-bg.y", samples_t_bg_y);
  m_plotter1->create_series("t-bg.z", samples_t_bg_z);
  m_plotter1->create_series("t-ba.x", samples_t_ba_x);
  m_plotter1->create_series("t-ba.y", samples_t_ba_y);
  m_plotter1->create_series("t-ba.z", samples_t_ba_z);
}

void MainWindow::generate_traj_samples()
{
  std::random_device r;
  std::default_random_engine e(r());

  constexpr auto samples = 5;
  QList<QPointF> samples_px_py;

  std::ranges::transform(
      std::views::iota(0),
      std::views::iota(0, samples),
      std::back_inserter(samples_px_py),
      [&](int i, int j)
      {
        std::uniform_real_distribution<qreal> di(i * 10, i * 10 + 10);
        std::uniform_real_distribution<qreal> dj(j * 2, j * 2 + 2);
        return QPointF(di(e), dj(e));
      });

  qDebug() << samples_px_py;

  m_plotter2->create_series("p.x-p.y", samples_px_py);
}

}  // namespace logviewer
