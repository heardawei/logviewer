#pragma once

#include <QtCore/QPointF>
#include <QtWidgets/QMainWindow>

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
  void set_img1_files(QStringList imgs);
  void set_img2_files(QStringList imgs);

 private slots:
  void on_open_file_clicked();
  void on_open_img1_clicked();
  void on_open_img2_clicked();
  void reload_file(const QString &filename);

 private:
  void generate_samples();
  void generate_img_samples();
  void generate_plotter_samples();
  void generate_traj_samples();

 private:
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
};

}  // namespace logviewer
