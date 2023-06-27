#pragma once

#include <QtWidgets/QMainWindow>

#include "ui/image.h"
#include "ui/plotter.h"

namespace logviewer
{
class MainWindow : public QMainWindow
{
 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  void on_open_file_clicked();
  void reload_file(const QString &filename);
  void generate_samples();
  void generate_img_samples();
  void generate_plotter_samples();
  void generate_traj_samples();

 private:
  Image *m_img1{};
  Image *m_img2{};
  Plotter *m_plotter1{};
  Plotter *m_plotter2{};
};

}  // namespace logviewer
