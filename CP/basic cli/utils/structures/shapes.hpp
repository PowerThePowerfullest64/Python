#pragma once

#include <optional>

#include "utils/structures/circle.hpp"
#include "utils/structures/rectangle.hpp"
#include "utils/structures/line.hpp"

#include "utils/math/vec2.hpp"
#include "utils/structures/hit.hpp"

// Collision between a Point(Vec2) and a Line. Returns a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Vec2<T>& point, const Line<T>& line, T buffer = 0.1) {
    float lineLength = line.Length();

    float d1 = point.DistanceTo(line.from);
    float d2 = point.DistanceTo(line.to);

    if (d1 + d2 >= lineLength - buffer && d1 + d2 <= lineLength + buffer)
        return Hit<T>(point);

    return std::nullopt;
}

// Collision between a Line and a Point(Vec2). Returns a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Line<T>& line, const Vec2<T>& point, T buffer = 0.1) {
    return Collision(point, line, buffer);
}

// Collision between a Point(Vec2) and a Rectangle. Returns a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Vec2<T>& point, const Rectangle<T>& rectangle) {
    if (point.x >= rectangle.Left() && point.y >= rectangle.Top() && point.x < rectangle.Right() && point.y < rectangle.Bottom())
        return Hit<T>(point);
    
    return std::nullopt;
}

// Collision between a Rectangle and a Point(Vec2). Returns a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Rectangle<T>& rectangle, const Vec2<T>& point) {
    return Collision(point, rectangle);
}

// Collision between a Point(Vec2) and a Circle. Returns a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Vec2<T>& point, const Circle<T>& circle) {
    if (point.DistanceTo(circle.pos) < circle.radius)
        return Hit<T>(point);
    
    return std::nullopt;
}

// Collision between a Circle and a Point(Vec2). Returns a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Circle<T>& circle, const Vec2<T>& point) {
    return Collision(point, circle);
}

// Collision between a Line and a Line. Returns a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Line<T>& lineA, const Line<T>& lineB) {
    // Calculate the distance to intersection point.
    T uA = ((lineB.to.x - lineB.from.x) * (lineA.from.y - lineB.from.y) - (lineB.to.y - lineB.from.y) * (lineA.from.x - lineB.from.x)) / ((lineB.to.y - lineB.from.y) * (lineA.to.x - lineA.from.x) - (lineB.to.x - lineB.from.x)*(lineA.to.y - lineA.from.y));
    T uB = ((lineA.to.x - lineA.from.x) * (lineA.from.y-lineB.from.y) - (lineA.to.y - lineA.from.y) * (lineA.from.x - lineB.from.x)) / ((lineB.to.y - lineB.from.y) * (lineA.to.x - lineA.from.x) - (lineB.to.x - lineB.from.x)*(lineA.to.y - lineA.from.y));

    // If uA and uB are between 0-1, the lines are colliding.
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        T intersectionX = lineA.from.x + (uA * (lineA.to.x - lineA.from.x));
        T intersectionY = lineA.from.y + (uA * (lineA.to.y - lineA.from.y));

        return Hit<T>({intersectionX, intersectionY});
    }

    // No hit.
    return std::nullopt;
}

// Collision between a Line and a Rectangle. Does not return a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Line<T>& line, const Rectangle<T>& rectangle) {
    bool left = Collision(line, rectangle.LeftLine()).has_value();
    bool right = Collision(line, rectangle.RightLine()).has_value();
    bool top = Collision(line, rectangle.TopLine()).has_value();
    bool bottom = Collision(line, rectangle.BottomLine()).has_value();

    if (left || right || top || bottom) // Maybe at detection to where the hit(s) happened, but that would require potentially return multiple...
        return Hit<T>();

    if (Collision(line.from, rectangle).has_value())
        return Hit<T>();
    if (Collision(line.to, rectangle).has_value())
        return Hit<T>();
    
    // No hit.
    return std::nullopt;
}

// Collision between a Rectangle and a Line. Does not return a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Rectangle<T>& rectangle, const Line<T>& line) {
    return Collision(line, rectangle);
}

// Collision between a Line and a Circle. Does not return a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Line<T>& line, const Circle<T>& circle) {
    bool inside1 = Collision(line.from, circle).has_value();
    bool inside2 = Collision(line.to, circle).has_value();

    if (inside1 || inside2)
        return Hit<T>();
    
    float len = line.Length();

    float dot = ( ((circle.pos.x - line.from.x)*(line.to.x-line.from.x)) + ((circle.pos.y - line.from.y)*(line.to.y - line.from.y)) ) / (len * len);

    Vec2<T> closestPoint = {
        line.from.x + (dot * (line.to.x-line.from.x)),
        line.from.y + (dot * (line.to.y-line.from.y))
    };

    bool onSegment = Collision(closestPoint, line).has_value();

    if (!onSegment)
        return std::nullopt;
    
    float dist = circle.pos.DistanceTo(closestPoint);

    if (dist < circle.radius)
        return Hit<T>();

    return std::nullopt;
}

// Collision between a Circle and a Line. Does not return a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Circle<T>& circle, const Line<T>& line) {
    return Collision(line, circle);
}

// Collision between a Rectangle and a Circle. Does not return a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Rectangle<T>& rectangle, const Circle<T>& circle) {
    bool colliding = false;

    colliding = (
        Collision(rectangle.LeftLine(), circle).has_value() ||
        Collision(rectangle.RightLine(), circle).has_value() ||
        Collision(rectangle.TopLine(), circle).has_value() ||
        Collision(rectangle.BottomLine(), circle).has_value()
    );

    if (colliding)
        return Hit<T>();

    if (Collision(circle.pos, rectangle).has_value())
        return Hit<T>();
    
    return std::nullopt;
}

// Collision between a Circle and a Rectangle. Does not return a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Circle<T>& circle, const Rectangle<T>& rectangle) {
    return Collision(rectangle, circle);
}

// Collision between a Rectangle and a Rectangle. Does not return a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Rectangle<T>& rectangleA, const Rectangle<T>& rectangleB) {
    if (rectangleA.Left() < rectangleB.Right() &&
        rectangleA.Right() > rectangleB.Left() &&
        rectangleA.Top() < rectangleB.Bottom() &&
        rectangleA.Bottom() > rectangleB.Top()) {
            return Hit<T>();
        }
    
    return std::nullopt;
}

// Collision between a Circle and a Circle. Does not return a hit position.
template<typename T>
std::optional<Hit<T>> Collision(const Circle<T>& circleA, const Circle<T>& circleB) {
    T radiusSum = circleA.radius + circleB.radius;

    if (circleA.pos.DistanceToSq(circleB.pos) <= radiusSum * radiusSum)
        return Hit<T>();
    
    return std::nullopt;
}
