#pragma once

class Particle {
public:
    double x, y;
    double vx = 0.0, vy = 0.0;

    Particle(double _x, double _y) {
        x = _x, y = _y;
    }
};