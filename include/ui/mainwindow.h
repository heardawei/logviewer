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
  Image *m_img1{};
  Image *m_img2{};
  Plotter *m_plotter1{};
  Plotter *m_plotter2{};

  // QWidget interface
  protected:
  virtual void resizeEvent(QResizeEvent *event) override;
};

}  // namespace logviewer
