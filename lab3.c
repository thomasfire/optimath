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
#else
#define debugf(fmt, ...) do{}while(0)
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
#ifndef NDEBUG
    uint32_t count_loops = 0;
#endif
    while (stop - start > EPSILON) {
#ifndef NDEBUG
        count_loops++;
#endif
        buffer = (start + stop) / 2;
        if(FUNC(stop) > FUNC(buffer))
            start = buffer;
        else
            stop = buffer;
    }
#ifndef NDEBUG
    debugf("Bisection counted loops, with 2 F(x) calls on each: %u\n", count_loops);
#endif
    return (start + stop) / 2;
}

// по ощущениям херня какая-то - циклов больше, и результат другой
double golden_ratio_method(const double search_begin, const double search_end) {
    double start = search_begin,
    stop = search_end,
    buffer_start, buffer_stop;
#ifndef NDEBUG
    uint32_t count_loops = 0;
#endif
    while (fabs(stop - start) > EPSILON) {
#ifndef NDEBUG
        count_loops++;
#endif
        double buff_diff = (stop - start) / GOLDEN_RATIO;
        buffer_start = stop - buff_diff;
        buffer_stop = start + buff_diff;
        if(FUNC(buffer_start) <= FUNC(buffer_stop))
            start = buffer_start;
        else
            stop = buffer_stop;
    }
#ifndef NDEBUG
debugf("Golden ratio counted loops, with 2 F(x) calls on each: %u\n", count_loops);
#endif
    return (start + stop) / 2;
}

int main(int argc, const char *argv[]) {
    printf(SEARCH_TASK"\n");
    printf("Bisection method: min = %f;\n\n", bisection_method(SEARCH_BEGIN, SEARCH_END));
    printf("Golden ratio method: min = %f;\n\n", golden_ratio_method(SEARCH_BEGIN, SEARCH_END));
    return 0;
}