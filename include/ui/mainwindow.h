#pragma once

#include <QtCore/QPointF>
#include <QtCore/QSharedPointer>
#include <QtCore/QTimer>
#include <QtCore/QVector>
#include <QtWidgets/QMainWindow>

#include "data/record.h"
#include "ui/image_viewer.h"
#include "ui/plotter.h"

namespace logviewer
{
class MainWindow : public QMainWindow
{
  Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  using GetPlotDataHandler =
      std::function<bool(double, QVector<BaseRecordPtr> &)>;

  void register_handler(GetPlotDataHandler handler)
  {
    m_handler = std::move(handler);
  }

 signals:
  void open_log(const QString &filename);
  void open_img1_dir(const QString &dir);
  void open_img2_dir(const QString &dir);

 public slots:
  void set_t_bg_x_points(const QVector<double> &x, const QVector<double> &y);
  void set_t_bg_y_points(const QVector<double> &x, const QVector<double> &y);
  void set_t_bg_z_points(const QVector<double> &x, const QVector<double> &y);
  void set_t_ba_x_points(const QVector<double> &x, const QVector<double> &y);
  void set_t_ba_y_points(const QVector<double> &x, const QVector<double> &y);
  void set_t_ba_z_points(const QVector<double> &x, const QVector<double> &y);
  void set_t_px_py_points(const QVector<double> &x, const QVector<double> &y);

  void add_t_bg_x_points(double x, double y);
  void add_t_bg_y_points(double x, double y);
  void add_t_bg_z_points(double x, double y);
  void add_t_ba_x_points(double x, double y);
  void add_t_ba_y_points(double x, double y);
  void add_t_ba_z_points(double x, double y);
  void add_t_px_py_points(double x, double y);

  void add_t_bg_x_points(const QVector<double> &x, const QVector<double> &y);
  void add_t_bg_y_points(const QVector<double> &x, const QVector<double> &y);
  void add_t_bg_z_points(const QVector<double> &x, const QVector<double> &y);
  void add_t_ba_x_points(const QVector<double> &x, const QVector<double> &y);
  void add_t_ba_y_points(const QVector<double> &x, const QVector<double> &y);
  void add_t_ba_z_points(const QVector<double> &x, const QVector<double> &y);
  void add_t_px_py_points(const QVector<double> &x, const QVector<double> &y);

  void play_images_until(double time);

  void set_img1_files(QStringList imgs);
  void set_img2_files(QStringList imgs);

 private slots:
  void on_open_file_clicked();
  void on_open_img1_clicked();
  void on_open_img2_clicked();
  void on_play_clicked();
  void reload_file(const QString &filename);
  void enable_interaction();
  void disable_interaction();

 private:
  void generate_samples();
  void generate_img_samples();
  void generate_plotter_samples();
  void generate_traj_samples();

 private:
  QTimer m_timer;
  ImageViewer *m_img1{};
  ImageViewer *m_img2{};
  Plotter *m_plotter1{};
  Plotter *m_plotter2{};
  QCPGraph *m_t_bg_x{};
  QCPGraph *m_t_bg_y{};
  QCPGraph *m_t_bg_z{};
  QCPGraph *m_t_ba_x{};
  QCPGraph *m_t_ba_y{};
  QCPGraph *m_t_ba_z{};
  QCPGraph *m_px_py{};
  QPushButton *open_log_pb{};
  QPushButton *open_img1_pb{};
  QPushButton *open_img2_pb{};
  QPushButton *play_pb{};
  GetPlotDataHandler m_handler;
};

}  // namespace logviewer
