#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>

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

 private:
  Plotter *m_plotter1{};
  Plotter *m_plotter2{};
  QVBoxLayout *m_plotters_layout{};
};

}  // namespace logviewer
