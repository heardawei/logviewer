#pragma once

namespace logviewer
{
class AlgoUtil
{
 public:
  template <typename T>
  static bool update_min_max(const T &v, T &min, T &max)
  {
    bool updated = false;
    if (v < min)
    {
      min = v;
      updated = true;
    }
    if (v > max)
    {
      max = v;
      updated = true;
    }
    return updated;
  }
};
}  // namespace logviewer
