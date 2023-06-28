#include "ui/series.h"

// #include <algorithm>
// #include <concepts>
// #include <ranges>

// #include "util/algo_util.h"

namespace logviewer
{
LineSeries::LineSeries(QObject *parent)
    : QLineSeries(parent)
{
}

LineSeries::~LineSeries() {}

ScatterSeries::ScatterSeries(QObject *parent)
   : QScatterSeries(parent)
{
}

ScatterSeries::~ScatterSeries() {}

}  // namespace logviewer
