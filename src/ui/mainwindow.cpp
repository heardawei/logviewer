#include "ui/mainwindow.h"

#include <algorithm>
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
    , m_img1(new Image)
    , m_img2(new Image)
    , m_plotter1(new Plotter)
    , m_plotter2(new Plotter)
{
  auto test_pb = new QPushButton("测试数据");
  auto open_log_pb = new QPushButton("打开日志文件");
  auto refresh_pb = new QPushButton("刷新");

  auto header_layout = new QHBoxLayout;
  header_layout->addWidget(test_pb);
  header_layout->addWidget(open_log_pb);
  header_layout->addWidget(refresh_pb);
  header_layout->addStretch();

  auto body_layout = new QGridLayout;
  body_layout->addWidget(m_img1, 0, 0 /* , Qt::AlignCenter */);
  body_layout->addWidget(m_img2, 0, 1 /* , Qt::AlignCenter */);
  body_layout->addWidget(m_plotter1, 1, 0);
  body_layout->addWidget(m_plotter2, 1, 1);
  body_layout->setRowStretch(0, 50);
  body_layout->setRowStretch(1, 50);
  body_layout->setColumnStretch(0, 50);
  body_layout->setColumnStretch(1, 50);

  auto central_layout = new QVBoxLayout;
  central_layout->addLayout(header_layout);
  central_layout->addLayout(body_layout);

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
}

MainWindow::~MainWindow() {}

void MainWindow::on_open_file_clicked()
{
  qDebug() << "开始选择日志文件";
  QSettings settings(QSettings::Scope::SystemScope);
  auto filename = QFileDialog::getOpenFileName(
      this, "请选择日志文件", settings.value("cache/logdir").toString());
  if (filename.isEmpty())
  {
    qDebug() << "取消选择日志文件";
    return;
  }
  qDebug() << "已选择日志文件: " << filename;

  settings.setValue("cache/logdir", QDir(filename).dirName());

  reload_file(filename);
}

void MainWindow::reload_file(const QString &filename)
{
  m_plotter1->clear();
  m_plotter2->clear();

  // 窗口标题
  setWindowFilePath(filename);

  // TODO(ldw)

  qDebug() << "TODO(ldw) 读取日志";
}

void MainWindow::generate_samples()
{
  generate_img_samples();
  generate_plotter_samples();
  generate_traj_samples();
}

void MainWindow::generate_img_samples()
{
  m_img1->setPixmap(QPixmap("C:/Users/lidawei/Desktop/补卡截图/v.png"));
  m_img1->setScaledContents(true);
  m_img2->setPixmap(QPixmap("C:/Users/lidawei/Desktop/补卡截图/v.png"));
  m_img2->setScaledContents(true);
}

void MainWindow::generate_plotter_samples()
{
  std::random_device r;
  std::default_random_engine e(r());

  constexpr auto samples = 100;

  QList<QPointF> samples_t_gb_x;
  QList<QPointF> samples_t_gb_y;
  QList<QPointF> samples_t_gb_z;
  QList<QPointF> samples_t_ba_x;
  QList<QPointF> samples_t_ba_y;
  QList<QPointF> samples_t_ba_z;

  auto base_time = QDateTime::currentMSecsSinceEpoch();

  std::ranges::transform(std::views::iota(base_time),
                         std::views::iota(0, samples),
                         std::back_inserter(samples_t_gb_x),
                         [&](qint64 t, int)
                         {
                           std::uniform_real_distribution<qreal> u(0, 50);
                           return QPointF(static_cast<qreal>(t), u(e));
                         });

  std::ranges::transform(std::views::iota(base_time),
                         std::views::iota(0, samples),
                         std::back_inserter(samples_t_gb_y),
                         [&](qint64 t, int)
                         {
                           std::uniform_real_distribution<qreal> u(40, 100);
                           return QPointF(static_cast<qreal>(t), u(e));
                         });

  std::ranges::transform(std::views::iota(base_time),
                         std::views::iota(0, samples),
                         std::back_inserter(samples_t_gb_z),
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

  m_plotter1->create_series("timestamp-gb.x", samples_t_gb_x);
  m_plotter1->create_series("timestamp-gb.y", samples_t_gb_y);
  m_plotter1->create_series("timestamp-gb.z", samples_t_gb_z);
  m_plotter1->create_series("timestamp-ba.x", samples_t_ba_x);
  m_plotter1->create_series("timestamp-ba.y", samples_t_ba_y);
  m_plotter1->create_series("timestamp-ba.z", samples_t_ba_z);
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
