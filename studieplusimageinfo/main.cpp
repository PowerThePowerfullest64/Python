// ============================================================================
//
//    ____  _                                          _
//   / ___|| |__   _____      _____ __ _ ___  ___     | |
//   \___ \| '_ \ / _ \ \ /\ / / __/ _` / __|/ _ \    | |
//    ___) | | | | (_) \ V  V / (_| (_| \__ \  __/    |_|
//   |____/|_| |_|\___/ \_/\_/ \___\__,_|___/\___|    (_)
//
//   Three tiny terminal demos in one file. It is a program and a poster.
//
//   Build:  g++ -std=c++17 -O2 -o showcase showcase.cpp
//   Run:    ./showcase mandel    zooming Mandelbrot set
//           ./showcase life      Conway's Game of Life with a glider gun
//           ./showcase donut     the classic spinning ASCII donut
//
//   Needs a terminal that understands ANSI escape codes (basically all of
//   them) and a window of at least 80x32 characters.
//
//   Why this file exists: it is stored as black and white pixels in an image,
//   one bit per cell, 8 bits per character. If you can read this, the
//   decoder survived the JPEG compression. Congratulations.
//
// ============================================================================

#include <algorithm>
#include <chrono>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>
#include <vector>

// ----------------------------------------------------------------------------
// Small helpers
// ----------------------------------------------------------------------------

static void sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

static void clear_screen() { std::fputs("\x1b[2J\x1b[H", stdout); }
static void hide_cursor()  { std::fputs("\x1b[?25l", stdout); }
static void show_cursor()  { std::fputs("\x1b[?25h", stdout); }

// ----------------------------------------------------------------------------
// Demo 1: Mandelbrot zoom
//
// For every character cell we pick a point c in the complex plane and iterate
// z = z*z + c. If z escapes (|z| > 2) quickly, the point is "outside" and gets
// a light character. Points that never escape are "inside" and get '@'.
// ----------------------------------------------------------------------------

static void mandel() {
    const char* ramp = " .:-=+*#%@";
    const int W = 78, H = 30, max_iter = 80;

    // A point on the boundary of the set that stays interesting when zoomed.
    const double cx = -0.7436447860, cy = 0.1318252536;
    double zoom = 1.0;

    clear_screen();
    for (int frame = 0; frame < 70; ++frame) {
        const double scale = 3.0 / zoom;
        std::string out = "\x1b[H";

        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                // Terminal cells are about twice as tall as they are wide,
                // so the vertical span is stretched to keep the picture round.
                const double re = cx + (x - W / 2.0) / W * scale;
                const double im = cy + (y - H / 2.0) / H * scale * 0.82;

                const std::complex<double> c(re, im);
                std::complex<double> z(0.0, 0.0);
                int i = 0;
                while (i < max_iter && std::norm(z) < 4.0) {
                    z = z * z + c;
                    ++i;
                }
                out += (i == max_iter) ? '@' : ramp[i * 9 / max_iter];
            }
            out += '\n';
        }

        std::fputs(out.c_str(), stdout);
        std::printf("frame %2d   zoom x%.1f\n", frame + 1, zoom);
        zoom *= 1.16;
        sleep_ms(70);
    }
}

// ----------------------------------------------------------------------------
// Demo 2: Conway's Game of Life
//
// The rules:
//   - a live cell with 2 or 3 live neighbours survives
//   - a dead cell with exactly 3 live neighbours becomes alive
//   - everything else dies or stays dead
//
// The world wraps around at the edges (a torus). We seed it with the Gosper
// glider gun, which fires a new glider every 30 generations, forever.
// ----------------------------------------------------------------------------

static int neighbours(const std::vector<std::string>& g, int x, int y) {
    const int h = static_cast<int>(g.size());
    const int w = static_cast<int>(g[0].size());
    int n = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            const int nx = (x + dx + w) % w;
            const int ny = (y + dy + h) % h;
            if (g[ny][nx] == 'O') ++n;
        }
    }
    return n;
}

static void life() {
    const int W = 78, H = 30;
    std::vector<std::string> g(H, std::string(W, ' '));

    const char* gun[] = {
        "........................O...........",
        "......................O.O...........",
        "............OO......OO............OO",
        "...........O...O....OO............OO",
        "OO........O.....O...OO..............",
        "OO........O...O.OO....O.O...........",
        "..........O.....O.......O...........",
        "...........O...O....................",
        "............OO......................",
    };

    const int ox = 4, oy = 3;
    for (int r = 0; r < 9; ++r) {
        const int len = static_cast<int>(std::strlen(gun[r]));
        for (int c = 0; c < len; ++c) {
            if (gun[r][c] == 'O') g[oy + r][ox + c] = 'O';
        }
    }

    clear_screen();
    for (int gen = 0; gen < 300; ++gen) {
        std::string out = "\x1b[H";
        int alive = 0;
        for (const auto& row : g) {
            out += row;
            out += '\n';
            alive += static_cast<int>(std::count(row.begin(), row.end(), 'O'));
        }
        std::fputs(out.c_str(), stdout);
        std::printf("generation %3d   alive %4d\n", gen, alive);

        std::vector<std::string> next = g;
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                const int n = neighbours(g, x, y);
                if (g[y][x] == 'O') {
                    next[y][x] = (n == 2 || n == 3) ? 'O' : ' ';
                } else {
                    next[y][x] = (n == 3) ? 'O' : ' ';
                }
            }
        }
        g = next;
        sleep_ms(50);
    }
}

// ----------------------------------------------------------------------------
// Demo 3: the spinning donut
//
// A torus is swept out by two angles: i goes around the tube, j goes around
// the ring. Each surface point is rotated by A and B, projected to 2D, and
// shaded by how much its normal faces a light. A z-buffer (the 1/z value D)
// makes sure that nearer points hide farther ones.
// ----------------------------------------------------------------------------

static void donut() {
    const int W = 80, H = 24;
    const char* shades = ".,-~:;=!*#$@";
    float A = 0.0f, B = 0.0f;

    clear_screen();
    for (int frame = 0; frame < 500; ++frame) {
        std::vector<char>  buf(W * H, ' ');
        std::vector<float> zbuf(W * H, 0.0f);

        for (float j = 0.0f; j < 6.28f; j += 0.07f) {
            for (float i = 0.0f; i < 6.28f; i += 0.02f) {
                const float c = std::sin(i), d = std::cos(j);
                const float e = std::sin(A), f = std::sin(j);
                const float g = std::cos(A), h = d + 2.0f;
                const float D = 1.0f / (c * h * e + f * g + 5.0f);
                const float l = std::cos(i), m = std::cos(B), n = std::sin(B);
                const float t = c * h * g - f * e;

                const int x = static_cast<int>(40 + 30 * D * (l * h * m - t * n));
                const int y = static_cast<int>(12 + 15 * D * (l * h * n + t * m));
                const int o = x + W * y;
                const int N = static_cast<int>(
                    8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n));

                if (y > 0 && y < H && x > 0 && x < W && D > zbuf[o]) {
                    zbuf[o] = D;
                    buf[o] = shades[std::min(std::max(N, 0), 11)];
                }
            }
        }

        std::string out = "\x1b[H";
        for (int y = 0; y < H; ++y) {
            out.append(buf.begin() + y * W, buf.begin() + (y + 1) * W);
            out += '\n';
        }
        std::fputs(out.c_str(), stdout);

        A += 0.04f;
        B += 0.02f;
        sleep_ms(30);
    }
}

// ----------------------------------------------------------------------------
// Entry point
// ----------------------------------------------------------------------------

static void usage(const char* prog) {
    std::printf("usage: %s mandel | life | donut\n", prog);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    const std::string mode = argv[1];
    hide_cursor();

    if (mode == "mandel") {
        mandel();
    } else if (mode == "life") {
        life();
    } else if (mode == "donut") {
        donut();
    } else {
        show_cursor();
        usage(argv[0]);
        return 1;
    }

    show_cursor();
    return 0;
}