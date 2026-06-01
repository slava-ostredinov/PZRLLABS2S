#pragma once

#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <random>

/**
 * @brief Структура, представляющая точку на плоскости.
 */
struct Point2D {
    double x, y;

    /**
     * @brief Оператор сравнения на равенство двух точек.
     * 
     * @param other Другая точка.
     * @return true, если точки равны.
     * @return false, если точки не равны.
     */
    bool operator==(const Point2D& other) const {
	    if (x == other.x && y == other.y)
		    return true;
	    else 
		    return false;
			/// Необходимо реализовать самостоятельно
    }

    /**
     * @brief Оператор сравнения на неравенство двух точек.
     * 
     * @param other Другая точка.
     * @return true, если точки не равны.
     * @return false, если точки равны.
     */
    bool operator!=(const Point2D& other) const {
        return !(*this == other);
    }
};

/**
 * @brief Структура, представляющая отрезок на плоскости.
 */
struct LineSegment {
    Point2D start; ///< Начальная точка отрезка.
    Point2D end;   ///< Конечная точка отрезка.
};

/**
 * @brief Структура, представляющая окружность.
 */
struct Circle {
    Point2D center; ///< Центр окружности.
    double radius;  ///< Радиус окружности.

    /**
     * @brief Проверяет, является ли окружность допустимой.
     * 
     * @return true, если радиус >= 0.
     * @return false, если радиус < 0.
     */
    bool isValid() const;
    /**
     * @brief Проверяет, содержится ли точка внутри окружности.
     * 
     * @param point Точка для проверки.
     * @return true, если точка находится внутри окружности.
     * @return false, если точка находится вне окружности.
     */
    bool contains(const Point2D& point) const;
    /**
     * @brief Проверяет, содержится ли отрезок полностью внутри окружности.
     * 
     * @param segment Отрезок для проверки.
     * @return true, если отрезок полностью содержится в окружности.
     * @return false, если отрезок выходит за пределы окружности.
     */
    bool contains(const LineSegment& segment) const;
    /**
     * @brief Проверяет, содержится ли весь отрезок внутри окружности, включая его середину.
     * 
     * @param segment Отрезок для проверки.
     * @return true, если отрезок полностью содержится в окружности.
     * @return false, если отрезок выходит за пределы окружности.
     */
};

/**
 * @brief Находит минимальную окружность, содержащую все заданные отрезки.
 * 
 * @param segments Вектор отрезков.
 * @return Минимальная окружность, содержащая все отрезки.
 */
Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment>& segments);

// Вспомогательные функции для реализации

/**
 * @brief Вычисляет квадрат расстояния между двумя точками.
 * 
 * @param p1 Первая точка.
 * @param p2 Вторая точка.
 * @return Квадрат расстояния между точками.
 */
double distanceSquared(const Point2D& p1, const Point2D& p2);

/**
 * @brief Вычисляет расстояние между двумя точками.
 * 
 * @param p1 Первая точка.
 * @param p2 Вторая точка.
 * @return Расстояние между точками.
 */
double distance(const Point2D& p1, const Point2D& p2);

/**
 * @brief Создает окружность по двум точкам (используя диаметр).
 * 
 * @param p1 Первая точка.
 * @param p2 Вторая точка.
 * @return Окружность, построенная по диаметру.
 */
Circle circleFromDiameter(const Point2D& p1, const Point2D& p2);

/**
 * @brief Создает окружность, проходящую через три точки.
 * 
 * @param p1 Первая точка.
 * @param p2 Вторая точка.
 * @param p3 Третья точка.
 * @return Окружность, проходящая через три точки.
 */
Circle circleFrom3Points(const Point2D& p1, const Point2D& p2, const Point2D& p3);

/**
 * @brief Рекурсивный алгоритм Вельцля для нахождения минимальной окружности.
 * 
 * @param points Точки, которые нужно обработать.
 * @param boundary Точки, лежащие на границе окружности.
 * @param n Количество оставшихся точек.
 * @return Минимальная окружность, содержащая все точки.
 */
Circle minCircleHelper(std::vector<Point2D>& points, std::vector<Point2D> boundary, size_t n);

/**
 * @brief Нахождение минимальной окружности для множества точек.
 * 
 * @param points Вектор точек.
 * @return Минимальная окружность, содержащая все точки.
 */
Circle minCircle(std::vector<Point2D> points);

/**
 * @brief Извлекает все точки из отрезков (включая середины).
 * 
 * @param segments Вектор отрезков.
 * @return Вектор точек.
 */
std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment>& segments);

/**
 * @brief Находит середину отрезка.
 * 
 * @param segment Отрезок.
 * @return Точка, являющаяся серединой отрезка.
 */
Point2D findMidPoint(const LineSegment& segment);

