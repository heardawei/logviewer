#include "log/log.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QtDebug>

namespace logviewer
{

Log::Log() {}

Log::~Log() {}

bool Log::parse(const QString &filename)
{
  QFile fin(filename);
  if (!fin.open(QIODeviceBase::OpenModeFlag::ReadOnly))
  {
    qDebug() << "打开文件失败: " << filename << ", 原因: " << fin.errorString();
    return false;
  }

  QTextStream sin(&fin);

  while (true)
  {
    const auto &line = sin.readLine();
    if (line.isEmpty())
    {
      break;
    }
    parse_line(line);
  }

  return true;
}

bool Log::parse_line(const QString &line)
{
  auto ptr = Record::parse(line.split(','));
  if (!ptr)
  {
    return false;
  }

  m_records.emplace_back(std::move(ptr));
  return true;
}

}  // namespace logviewer
