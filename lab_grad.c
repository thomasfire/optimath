//
// Created by thomasf on 2021/11/14.
//

// find extremum f(x1, x2) = 4*x1 + 6*x2 - 2*x1^2 - 2*x1*x2 - 2*x2^2

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#ifndef NDEBUG
#define debugf(fmt, ...) printf(fmt, ## __VA_ARGS__)
#define dcounter_init(_name) uint32_t _name = 0
#define dcounter_inc(_name) _name++
#else
#define debugf(fmt, ...) do{}while(0)
#define dcounter_init(_name) do{}while(0)
#define dcounter_inc(_name) do{}while(0)
#endif

#define SEARCH_TASK "f(x1, x2) = 4*x1 + 6*x2 - 2*x1^2 - 2*x1*x2 - 2*x2^2, epsilon = 0.05;"

#define EPSILON      (0.05)
#define STEP_INIT    (0.25)
#define STEP_FACTOR  (15.0/16)

typedef struct {
    double x1;
    double x2;
} vec2t;

#define FUNC(_v) ({ \
    double _t1 = ((_v).x1), _t2 = ((_v).x2); \
    2 * (_t1*(2 - _t1) + _t2*(3 - _t1 - _t2)); \
})

// 4 - 4*x1 - 2*x2
#define DFUNC1(_v) ({ \
    double _t1 = ((_v).x1), _t2 = ((_v).x2); \
    4 * (1 - _t1) - 2*_t2;\
})

// 6 - 2*x1 - 4*x2
#define DFUNC2(_v) ({ \
    double _t1 = ((_v).x1), _t2 = ((_v).x2); \
    2 * (3 - _t1 - 2*_t2);\
})

#define GRAD(_v, _s) ({ \
    __auto_type _step = (_s); \
    vec2t _vec = (_v);     \
    (vec2t) { .x1 = _vec.x1 + DFUNC1(_vec) * _step, .x2 = _vec.x2 + DFUNC2(_vec) * _step }; \
})

#define AGRAD(_v, _s) ({ \
    __auto_type _step = (_s); \
    vec2t _vec = (_v);     \
    (vec2t) { .x1 = _vec.x1 - DFUNC1(_vec) * _step, .x2 = _vec.x2 - DFUNC2(_vec) * _step }; \
})

vec2t min_by_grad_boost(const vec2t initial_vec) {
    double prev = FUNC(initial_vec), curr = prev;
    double step = STEP_INIT;
    vec2t buffer = initial_vec;
    dcounter_init(count_loops);
    do {
        dcounter_inc(count_loops);
        prev = curr;
        buffer = AGRAD(buffer, step);
        curr = FUNC(buffer);
        step *= STEP_FACTOR;
    } while (fabs(curr - prev) >= EPSILON);
    debugf("Min by gradient boost counted loops, with 1 F(x, y) and 1 AGRAD() calls on each: %u\n", count_loops);
    return  buffer;
}

vec2t max_by_grad_boost(const vec2t initial_vec) {
    double prev = FUNC(initial_vec), curr = prev;
    double step = STEP_INIT;
    vec2t buffer = initial_vec;
    dcounter_init(count_loops);
    do {
        dcounter_inc(count_loops);
        prev = curr;
        buffer = GRAD(buffer, step);
        curr = FUNC(buffer);
        step *= STEP_FACTOR;
    } while (fabs(curr - prev) >= EPSILON);
    debugf("Max by gradient boost counted loops, with 1 F(x, y) and 1 AGRAD() calls on each: %u\n", count_loops);
    return  buffer;
}

int main() {
    const vec2t vec = {0, 0};
    vec2t buffer;
    printf(SEARCH_TASK"\n");
    buffer = min_by_grad_boost(vec);
    printf("Min by gradient boost method: min = (%f, %f), f = %f;\n\n", buffer.x1, buffer.x2, FUNC(buffer));
    buffer = max_by_grad_boost(vec);
    printf("Max by gradient boost method: min = (%f, %f), f = %f;\n\n", buffer.x1, buffer.x2, FUNC(buffer));
    return 0;
}