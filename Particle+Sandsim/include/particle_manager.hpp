#pragma once

#include <vector>
#include <cmath>

#include "utils/graphics/renderer.hpp"
#include "utils/math/random.hpp"

class ParticleManager {
public:
    void Update() {
        for (int i = 0; i < AmountOfParticles(); i++) {
            double ax = 0.0, ay = 0.0;

            for (int j = 0; j < AmountOfParticles(); j++) {
                // If same particle then skip it.
                if (i == j) continue;
                
                double dist;
                double distx;
                double disty;

                distx = x[j] - x[i];
                disty = y[j] - y[i];

                dist = std::sqrt(distx*distx + disty*disty);
                if (dist == 0.0) continue; // ABORT!!
                
                double direction = std::atan2(disty, distx);
                double aForceX = std::cos(direction) / dist;
                double aForceY = std::sin(direction) / dist;
                
                double rForceX = 0.0;
                double rForceY = 0.0;
                
                if (dist <= prRadius) {
                    rForceX = (prRadius - dist) / prRadius * -aForceX;
                    rForceY = (prRadius - dist) / prRadius * -aForceY;
                }

                ax += aForceX * ATR_FORCE;
                ay += aForceY * ATR_FORCE;
            }

            vx[i] += ax;
            vy[i] += ay;
            x[i] += static_cast<int>(vx[i]);
            y[i] += static_cast<int>(vy[i]);
        }
    }

    void Draw() const {
        for (int i = 0; i < AmountOfParticles(); i++) {
            Renderer::DrawShape(Circle<double>(x[i], y[i], pvRadius), pColor);
        }
    }

    void AddParticle(double _x, double _y) {
        x.push_back(_x);
        y.push_back(_y);
        vx.push_back(0.0);
        vy.push_back(0.0);
    }

    void AddParticle() {
        x.push_back(Random::RangeI32(0, 1600));
        y.push_back(Random::RangeI32(0, 900));
        vx.push_back(0.0);
        vy.push_back(0.0);
    }

    int AmountOfParticles() const {
        return x.size();
    }

    void Clear() {
        x.clear();
        y.clear();
        vx.clear();
        vy.clear();
    }
    
private:
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> vx;
    std::vector<double> vy;

    static constexpr int pvRadius = 4;
    static constexpr int prRadius = 12;
    static constexpr Color pColor = Colors::RED;
    static constexpr double ATR_FORCE = 0.05;
    static constexpr double REP_FORCE = 0.75;
};