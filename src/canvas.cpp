#include "canvas.hpp"

#include <iostream>

namespace sk {

Canvas::Canvas(QObject* parent)
    : QObject{ parent }
{
}

auto Canvas::mousePositionChanged(QPoint const& pos) -> void
{
    std::cout << "Clicked at: (" << pos.x() << ", " << pos.y() << ')'
              << std::endl;
}

} // namespace sk
