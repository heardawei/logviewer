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
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_img1(new ImageViewer)
    , m_img2(new ImageViewer)
    , m_plotter1(new Plotter)
    , m_plotter2(new Plotter)
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

  connect(test_pb, &QPushButton::clicked, this, [=]() { generate_samples(); });

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

void MainWindow::set_t_bg_x_points(const QVector<double> &x,
                                   const QVector<double> &y)
{
  if (x.empty() || y.empty())
  {
    return;
  }
  qDebug() << "t-bg.x: " << x.size() << " points";
  m_plotter1->create_graph("t-bg.x", x, y);
}

void MainWindow::set_t_bg_y_points(const QVector<double> &x,
                                   const QVector<double> &y)
{
  if (x.empty() || y.empty())
  {
    return;
  }
  qDebug() << "t-bg.y: " << x.size() << " points";
  m_plotter1->create_graph("t-bg.y", x, y);
}

void MainWindow::set_t_bg_z_points(const QVector<double> &x,
                                   const QVector<double> &y)
{
  if (x.empty() || y.empty())
  {
    return;
  }
  qDebug() << "t-bg.z: " << x.size() << " points";
  m_plotter1->create_graph("t-bg.z", x, y);
}

void MainWindow::set_t_ba_x_points(const QVector<double> &x,
                                   const QVector<double> &y)
{
  if (x.empty() || y.empty())
  {
    return;
  }
  qDebug() << "t-ba.x: " << x.size() << " points";
  m_plotter1->create_graph("t-ba.x", x, y);
}

void MainWindow::set_t_ba_y_points(const QVector<double> &x,
                                   const QVector<double> &y)
{
  if (x.empty() || y.empty())
  {
    return;
  }
  qDebug() << "t-ba.y: " << x.size() << " points";
  m_plotter1->create_graph("t-ba.y", x, y);
}

void MainWindow::set_t_ba_z_points(const QVector<double> &x,
                                   const QVector<double> &y)
{
  if (x.empty() || y.empty())
  {
    return;
  }
  qDebug() << "t-ba.z: " << x.size() << " points";
  m_plotter1->create_graph("t-ba.z", x, y);
}

void MainWindow::set_t_px_py_points(const QVector<double> &x,
                                    const QVector<double> &y)
{
  if (x.empty() || y.empty())
  {
    return;
  }
  qDebug() << "p.x-p.y: " << x.size() << " points";
  m_plotter2->create_graph("p.x-p.y", x, y);
}

void MainWindow::set_img1_files(QStringList imgs)
{
  if (imgs.empty())
  {
    return;
  }
  qDebug() << "imgs1: " << imgs.size() << " files";
  m_img1->set_images(imgs);
}

void MainWindow::set_img2_files(QStringList imgs)
{
  if (imgs.empty())
  {
    return;
  }
  qDebug() << "imgs1: " << imgs.size() << " files";
  m_img2->set_images(imgs);
}

void MainWindow::on_open_file_clicked()
{
  qDebug() << "开始选择日志文件";
  QSettings settings(QSettings::Format::IniFormat,
                     QSettings::Scope::SystemScope,
                     "Alibaba",
                     "LogViewer");

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

  reload_file(filename);
}

void MainWindow::on_open_img1_clicked()
{
  qDebug() << "开始选择图片1文件夹";
  QSettings settings(QSettings::Format::IniFormat,
                     QSettings::Scope::SystemScope,
                     "Alibaba",
                     "LogViewer");
  auto dir = QFileDialog::getExistingDirectory(
      this, "请选择图片1文件夹", settings.value("cache/img1dir").toString());
  if (dir.isEmpty())
  {
    qDebug() << "取消选择图片1文件夹";
    return;
  }
  qDebug() << "已选择图片1文件夹: " << dir;

  settings.setValue(
      "cache/img1dir",
      QString::fromStdString(std::filesystem::path(dir.toStdString())
                                 .parent_path()
                                 .generic_string()));

  emit open_img1_dir(dir);
}

void MainWindow::on_open_img2_clicked()
{
  qDebug() << "开始选择图片2文件夹";
  QSettings settings(QSettings::Format::IniFormat,
                     QSettings::Scope::SystemScope,
                     "Alibaba",
                     "LogViewer");
  auto dir = QFileDialog::getExistingDirectory(
      this, "请选择图片2文件夹", settings.value("cache/img2dir").toString());
  if (dir.isEmpty())
  {
    qDebug() << "取消选择图片2文件夹";
    return;
  }
  qDebug() << "已选择图片2文件夹: " << dir;

  settings.setValue(
      "cache/img2dir",
      QString::fromStdString(std::filesystem::path(dir.toStdString())
                                 .parent_path()
                                 .generic_string()));

  emit open_img2_dir(dir);
}

void MainWindow::reload_file(const QString &filename)
{
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
  m_img1->set_images({
      "C:/Users/lidawei/Desktop/补卡截图/a.png",
      "C:/Users/lidawei/Desktop/补卡截图/v.png",
  });
  m_img2->set_images({
      "C:/Users/lidawei/Desktop/补卡截图/a.png",
      "C:/Users/lidawei/Desktop/补卡截图/v.png",
  });
}

void MainWindow::generate_plotter_samples()
{
  std::random_device r;
  std::default_random_engine e(r());

  constexpr auto samples = 100;

  std::uniform_real_distribution<double> u_t_bg_x(0, 50);
  std::uniform_real_distribution<double> u_t_bg_y(40, 100);
  std::uniform_real_distribution<double> u_t_bg_z(90, 150);
  std::uniform_real_distribution<double> u_t_ba_x(140, 200);
  std::uniform_real_distribution<double> u_t_ba_y(190, 250);
  std::uniform_real_distribution<double> u_t_ba_z(240, 300);
  // auto base_time = QDateTime::currentMSecsSinceEpoch();

  auto times =
      std::views::iota(0, samples) |
      std::views::transform([](qint64 v) { return static_cast<double>(v); }) |
      std::ranges::to<QVector<double>>();

  auto samples_t_bg_x =
      std::views::iota(0, samples) |
      std::views::transform([&](int) { return u_t_bg_x(e); }) |
      std::ranges::to<QVector<double>>();

  auto samples_t_bg_y =
      std::views::iota(0, samples) |
      std::views::transform([&](int) { return u_t_bg_y(e); }) |
      std::ranges::to<QVector<double>>();

  auto samples_t_bg_z =
      std::views::iota(0, samples) |
      std::views::transform([&](int) { return u_t_bg_z(e); }) |
      std::ranges::to<QVector<double>>();

  auto samples_t_ba_x =
      std::views::iota(0, samples) |
      std::views::transform([&](int) { return u_t_ba_x(e); }) |
      std::ranges::to<QVector<double>>();

  auto samples_t_ba_y =
      std::views::iota(0, samples) |
      std::views::transform([&](int) { return u_t_ba_y(e); }) |
      std::ranges::to<QVector<double>>();

  auto samples_t_ba_z =
      std::views::iota(0, samples) |
      std::views::transform([&](int) { return u_t_ba_z(e); }) |
      std::ranges::to<QVector<double>>();

  m_plotter1->create_graph("t-bg.x", times, samples_t_bg_x);
  m_plotter1->create_graph("t-bg.y", times, samples_t_bg_y);
  m_plotter1->create_graph("t-bg.z", times, samples_t_bg_z);
  m_plotter1->create_graph("t-ba.x", times, samples_t_ba_x);
  m_plotter1->create_graph("t-ba.y", times, samples_t_ba_y);
  m_plotter1->create_graph("t-ba.z", times, samples_t_ba_z);
}

void MainWindow::generate_traj_samples()
{
  std::random_device r;
  std::default_random_engine e(r());

  constexpr auto samples = 5;

  auto times =
      std::views::iota(0, samples) |
      std::views::transform([](qint64 v) { return static_cast<double>(v); }) |
      std::ranges::to<QVector<double>>();

  auto samples_px =
      std::views::iota(0, samples) |
      std::views::transform(
          [&](int i)
          {
            std::uniform_real_distribution<double> u_x(i * 10, i * 10 + 10);
            return u_x(e);
          }) |
      std::ranges::to<QVector<double>>();

  auto samples_py =
      std::views::iota(0, samples) |
      std::views::transform(
          [&](int i)
          {
            std::uniform_real_distribution<double> u_y(i * 2, i * 2 + 2);
            return u_y(e);
          }) |
      std::ranges::to<QVector<double>>();

  m_plotter2->create_graph("p.x-p.y", samples_px, samples_py);
}

}  // namespace logviewer
