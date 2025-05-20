#ifndef _SHOCKWAVE__H
#define _SHOCKWAVE__H

#include "graphics.h"
#include "entities.h"
#include <cmath>

struct ShockWave {
    int x, y;
    int r;
    int maxR;
    int thickness;
    int speed;
    bool active;

    ShockWave(int _x, int _y, int _maxR, int _thickness, int _speed)
     : x(_x), y(_y), r(0), maxR(_maxR), thickness(_thickness),
       speed(_speed), active(true)
    {}

    void update() {
        r += speed;
        if (r > maxR) active = false;
    }

    void render(Graphics graphics) const {
        for (int dr = 0; dr < thickness; ++dr)
            graphics.drawCircle(x, y, r + dr);
    }

    bool hits(const Zombie& z) const {
        float zx = z.xZombie + z.width / 2.0f;
        float zy = z.yZombie + z.height / 2.0f;
        float dx = zx - x, dy = zy - y;
        float dist = sqrt(dx*dx + dy*dy);
        return (dist <= r + thickness);
    }
};

#endif // _SHOCKWAVE__H
