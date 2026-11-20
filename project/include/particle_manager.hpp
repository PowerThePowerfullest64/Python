#pragma once

#include <vector>
#include <cmath>

#include "particle.hpp"

#include "utils/graphics/renderer.hpp"
#include "utils/math/random.hpp"

class ParticleManager {
public:
    ParticleManager() {
        
    }

    void Update() {
        for (auto& p : particles) {
            for (auto& other : particles) {
                // If same particle then skip it.
                if (&p == &other) continue;
                
                double dist;
                double distx;
                double disty;

                distx = other.x - p.x;
                disty = other.y - p.y;
            
                dist = std::sqrt(distx*distx + disty*disty);
                if (dist == 0.0) continue; // DO NOT DIVIDE BY ZERO!!!!

                double direction = std::atan2(disty, distx);
                double forceX = std::cos(direction) / dist;
                double forceY = std::sin(direction) / dist;

                p.vx += forceX * G;
                p.vy += forceY * G;
            }
            
            p.x += p.vx;
            p.y += p.vy;
        }
    }

    void Draw() const {
        for (const Particle& p : particles) {
            Renderer::DrawShape(Circle<double>(p.x, p.y, pRadius), pColor);
        }
    }

    void AddParticle(double x, double y) {
        particles.push_back(Particle(x, y));
    }

    void AddParticle() {
        particles.push_back(Particle(Random::RangeI(0, 1600), Random::RangeI(0, 900)));
    }

    int AmountOfParticles() {
        return particles.size();
    }

    void Clear() {
        particles.clear();
    }
    
private:
    std::vector<Particle> particles;
    static constexpr int pRadius = 4;
    static constexpr Color pColor = Colors::RED;
    static constexpr double G = 0.05;
};