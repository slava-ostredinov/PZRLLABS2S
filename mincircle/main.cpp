#include "mincircle.h"
#include <iostream>

int main() {
    LineSegment segment1 = {{0, 0}, {4, 0}};
    LineSegment segment2 = {{2, 0}, {2, 3}};

    std::vector<LineSegment> segments = {segment1, segment2};

    Circle result = MinimumEnclosingCircleForSegments(segments);

    std::cout << "Центр: (" << result.center.x << ", " << result.center.y << ")" << std::endl;
    std::cout << "Радиус: " << result.radius << std::endl;

    return 0;
}
