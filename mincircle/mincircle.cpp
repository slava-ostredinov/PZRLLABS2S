#include "mincircle.h"

bool Circle::isValid() const {
	if (radius >= 0) 
		return true;
	else 
		return false;
}

bool Circle::contains(const Point2D& point) const {
	if (!isValid()) 
		return false;
	
	if (distance(point, center) <= radius) 
		return true;
	else 
		return false;
}

bool Circle::contains(const LineSegment& segment) const {
	if (contains(segment.start) && contains(segment.end)) 
		return true;
	else 
		return false;
}

double distanceSquared(const Point2D& p1, const Point2D& p2) {
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	return dx*dx + dy*dy;
}

double distance(const Point2D& p1, const Point2D& p2) {
	return sqrt(distanceSquared(p1, p2));
}

Circle circleFromDiameter(const Point2D& pointA, const Point2D& pointB) {
	double rad = distance(pointA, pointB) / 2.0;
	Point2D midPoint = findMidPoint({pointA, pointB});
	return Circle{midPoint, rad};
}

Point2D findMidPoint(const LineSegment& seg) {
	double midX = (seg.start.x + seg.end.x) / 2.0;
	double midY = (seg.start.y + seg.end.y) / 2.0;
	return Point2D{midX, midY};
}

Circle circleFrom3Points(const Point2D& pt1, const Point2D& pt2, const Point2D& pt3) {
	double a = pt2.x - pt1.x;
	double b = pt2.y - pt1.y;
	double c = pt3.x - pt1.x;
	double d = pt3.y - pt1.y;
	double e = a * (pt1.x + pt2.x) + b * (pt1.y + pt2.y);
	double f = c * (pt1.x + pt3.x) + d * (pt1.y + pt3.y);
	double g = 2.0 * (a * (pt3.y - pt2.y) - b * (pt3.x - pt2.x));
	
	if (g == 0.0) 
		return Circle{{0, 0}, 0};
	else {
		double centerX = (d * e - b * f) / g;
		double centerY = (a * f - c * e) / g;
		double rad = distance({centerX, centerY}, pt1);
		return Circle{{centerX, centerY}, rad};
	}
}

std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment>& segments) {
	std::vector<Point2D> extractedPoints;
	for (size_t idx = 0; idx < segments.size(); ++idx)
	{
		extractedPoints.push_back(segments[idx].start);
		extractedPoints.push_back(segments[idx].end);
		extractedPoints.push_back(findMidPoint(segments[idx]));
	}
	return extractedPoints;
}

Circle minCircleHelper(std::vector<Point2D>& points, std::vector<Point2D> boundaryPoints, size_t pointCount) {
	if (boundaryPoints.size() == 3) 
		return circleFrom3Points(boundaryPoints[0], boundaryPoints[1], boundaryPoints[2]);
	
	if (pointCount == 0) {
		switch (boundaryPoints.size()) {
			case 2:
				return circleFromDiameter(boundaryPoints[0], boundaryPoints[1]);
			case 1:
				return Circle{boundaryPoints[0], 0};
			default:
				return Circle{{0, 0}, 0};
		}
	}
	
	Circle currentCircle = minCircleHelper(points, boundaryPoints, pointCount - 1);
	
	if (currentCircle.contains(points[pointCount - 1])) 
		return currentCircle;
	
	boundaryPoints.push_back(points[pointCount - 1]);
	return minCircleHelper(points, boundaryPoints, pointCount - 1);
}

Circle minCircle(std::vector<Point2D> points) {
	return minCircleHelper(points, {}, points.size());
}

Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment>& segments) {
	std::vector<Point2D> allPoints = extractPointsFromSegments(segments);
	return minCircle(allPoints);
}
