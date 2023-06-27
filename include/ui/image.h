#pragma

#include <QtWidgets/QLabel>

namespace logviewer
{
class Image : public QLabel
{
 public:
  Image(QWidget *parent = nullptr);
  virtual ~Image();
};

}  // namespace logviewer
