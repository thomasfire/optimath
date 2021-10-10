//
// Created by thomasf on 2021/10/07.
//

/*
 * Решить задачу четырьмя методами: методом половинного деления, методом золотого сечения,
 * методом хорд и методом Ньютона. По 3 шагов каждого метода выполнить вручную +
 * написать программу по каждому методу на одном из языков программирования.
 * */

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

#define SEARCH_TASK "f(x) = x^5 - 5x^3 + 10x^2 - 5x, [a, b] = [-3, -2], epsilon = 0.05;"
#define SEARCH_BEGIN (-3.0)
#define SEARCH_END   (-2.0)

#define EPSILON      (0.05)
#define GOLDEN_RATIO (1.618033989)

/// :))))
#define FUNC(x)     ({ \
    __auto_type t = (x); \
    ((((t*t) - 5) * (t) + 10) * (t) - 5)*(t); \
})

#define DFUNC(x) ({ \
    __auto_type t = (x); \
    ((5*(t*t) - 15) * (t) + 20) * (t) - 5; \
})

double bisection_method(const double search_begin, const double search_end) {
    double start = search_begin,
            stop = search_end,
            buffer;
    dcounter_init(count_loops);
    while (stop - start > EPSILON) {
        dcounter_inc(count_loops);
        buffer = (start + stop) / 2;
        if (FUNC(stop) < FUNC(buffer))
            start = buffer;
        else
            stop = buffer;
    }

    debugf("Bisection counted loops, with 2 F(x) calls on each: %u\n", count_loops);

    return (start + stop) / 2;
}

// по ощущениям херня какая-то - циклов больше
double golden_ratio_method(const double search_begin, const double search_end) {
    double start = search_begin,
            stop = search_end,
            buffer_start, buffer_stop;
    dcounter_init(count_loops);
    while (fabs(stop - start) > EPSILON) {
        dcounter_inc(count_loops);
        double buff_diff = (stop - start) / GOLDEN_RATIO;
        buffer_start = stop - buff_diff;
        buffer_stop = start + buff_diff;
        if (FUNC(buffer_start) >= FUNC(buffer_stop))
            start = buffer_start;
        else
            stop = buffer_stop;
    }

    debugf("Golden ratio counted loops, with 2 F(x) calls on each: %u\n", count_loops);

    return (start + stop) / 2;
}

double secant_method(const double search_begin, const double search_end) {
    double start = search_begin,
            stop = search_end,
            buffer_start, buffer_stop;
    dcounter_init(count_loops);
    while (fabs(stop - start) > EPSILON) {
        dcounter_inc(count_loops);
        buffer_start = FUNC(start);
        buffer_stop = FUNC(stop);
        if (buffer_stop < buffer_start)
            start = stop - (stop - start) * buffer_stop / (buffer_stop - buffer_start);
        else
            stop = start - (start - stop) * buffer_start / (buffer_start - buffer_stop);
    }

    debugf("Secant method counted loops, with 2 F(x) calls on each: %u\n", count_loops);

    return (start + stop) / 2;
}

double newton_method(const double search_begin, const double search_end) {
    double buffer_start = search_begin,
           buffer_x = (search_begin + search_end) / 2;
    dcounter_init(count_loops);

    while (fabs(buffer_x - buffer_start) > EPSILON) {
        dcounter_inc(count_loops);
        buffer_start = buffer_x;
        buffer_x = buffer_x - FUNC(buffer_x) / DFUNC(buffer_x);
    }

    debugf("Newton's method counted loops, with 1 F(x) and 1 DF(x) calls on each: %u\n", count_loops);

    return buffer_x;
}

int main(int argc, const char *argv[]) {
    printf(SEARCH_TASK"\n");
    printf("Bisection method: min = %f;\n\n", bisection_method(SEARCH_BEGIN, SEARCH_END));
    printf("Golden ratio method: min = %f;\n\n", golden_ratio_method(SEARCH_BEGIN, SEARCH_END));
    printf("Secant method: min = %f;\n\n", secant_method(SEARCH_BEGIN, SEARCH_END));
    printf("Newton's method: min = %f;\n\n", newton_method(SEARCH_BEGIN, SEARCH_END));
    return 0;
}