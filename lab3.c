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

#define DDFUNC(_x) ({ \
    __auto_type _t = (_x); \
    ((20 * (_t*_t) - 30) * _t + 20); \
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

// математики понапридумывают херни а нам инженерам разгребать потом приходится
double halves_method(const double search_begin, const double search_end) {
    double start = search_begin,
        stop = search_end,
        buffer, buffer_l, buffer_r;
    dcounter_init(count_loops);
    while (stop - start > EPSILON) {
        dcounter_inc(count_loops);
        buffer = (start + stop) / 2;
        buffer_l = buffer - EPSILON;
        buffer_r = buffer + EPSILON;
        if (FUNC(buffer_l) >= FUNC(buffer_r))
            start = buffer;
        else
            stop = buffer;
    }

    debugf("Halves counted loops, with 2 F(x) calls on each: %u\n", count_loops);

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

double true_golden_ratio_method(double search_begin, double search_end) {
    double lhs  = search_begin + (2 - GOLDEN_RATIO) * (search_end - search_begin);
    double rhs = search_begin + (GOLDEN_RATIO - 1) * (search_end - search_begin);
    dcounter_init(count_loops);
    do {
        dcounter_inc(count_loops);
        if (FUNC(lhs) < FUNC(rhs)) {
            search_end = rhs;
            rhs = lhs;
            lhs = search_begin + (2 - GOLDEN_RATIO) * (search_end - search_begin);
        } else {
            search_begin = lhs;
            lhs = rhs;
            rhs = search_begin + (GOLDEN_RATIO - 1) * (search_end - search_begin);
        }
    } while ((search_end - search_begin) > 2 * EPSILON);
    debugf("True golden ratio counted loops, with 2 F(x) calls on each: %u\n", count_loops);
    return (search_end + search_begin) / 2;
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

double chord_method(const double search_begin, const double search_end)
{
    double x_next = 0, buff_begin = search_begin, buff_end = search_end;
    double tmp, tmp_end, tmp_begin;
    dcounter_init(count_loops);
    do
    {
        dcounter_inc(count_loops);
        tmp = x_next;
        tmp_begin = DFUNC(buff_begin);
        tmp_end = DFUNC(buff_end);
        x_next = buff_end - tmp_end * (buff_begin - buff_end) / (tmp_begin - tmp_end);
        buff_begin = buff_end;
        buff_end = tmp;
    } while (fabs(x_next - buff_end) > EPSILON && buff_begin >= search_end && buff_end <= search_end);
    debugf("Chord method counted loops, with 2 DF(x) calls on each: %u\n", count_loops);
    return x_next;
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

double true_newton_method(const double search_begin, const double search_end) {
    double buffer_start = search_begin, buff_d, buff_dd,
    buffer_x = (search_begin + search_end) / 2;
    dcounter_init(count_loops);
    do {
        dcounter_inc(count_loops);
        buffer_start = buffer_x;
        buff_d = DFUNC(buffer_x);
        buff_dd = DDFUNC(buffer_x);
        buffer_x = buffer_x - buff_d / buff_dd;
    } while (fabs(buff_d) > EPSILON);
    debugf("True newton's method counted loops, with 1 DF(x) and 1 DDF(x) calls on each: %u\n", count_loops);
    return buffer_x;
}

double squared_method(const double search_begin, const double search_end) {
    double x_star = (search_begin + search_end) / 2, x_zero = 0;
    double h = 2 * EPSILON,
           x_s = x_star - h, x_e = x_star + h;
    double f_s, f_e, f_z, a, b;
    dcounter_init(count_loops);
    while (fabs(x_star - x_zero) > EPSILON) {
        dcounter_inc(count_loops);
        x_zero = x_star;
        f_s = FUNC(x_s);
        f_e = FUNC(x_e);
        f_z = FUNC(x_zero);
        a = ((f_e - ((x_e * (f_z - f_s) + x_zero * f_s - x_s * f_z) / (x_zero - x_s)))) / (x_e * (x_e - x_s - x_zero) + x_s * x_zero);
        b = ((f_z - f_s) / (x_zero - x_s)) - (a * (x_s + x_zero));
        x_star = -b / (2 * a);
        x_s = x_star - h;
        x_e = x_star + h;
    }
    debugf("Squared interpolation method counted loops, with 3 F(x) calls on each: %u\n", count_loops);
    return x_star;
}

int main(int argc, const char *argv[]) {
    printf(SEARCH_TASK"\n");
    printf("Bisection method: min = %f;\n\n", bisection_method(SEARCH_BEGIN, SEARCH_END));
    printf("Halves method: min = %f;\n\n", halves_method(SEARCH_BEGIN, SEARCH_END));
    printf("Golden ratio method: min = %f;\n\n", golden_ratio_method(SEARCH_BEGIN, SEARCH_END));
    printf("True golden ratio method: min = %f;\n\n", true_golden_ratio_method(SEARCH_BEGIN, SEARCH_END));
    printf("Secant method: min = %f;\n\n", secant_method(SEARCH_BEGIN, SEARCH_END));
    printf("Chord method: min = %f;\n\n", chord_method(SEARCH_BEGIN, SEARCH_END));
    printf("Newton's method: min = %f;\n\n", newton_method(SEARCH_BEGIN, SEARCH_END));
    printf("True newton's method: min = %f;\n\n", true_newton_method(SEARCH_BEGIN, SEARCH_END));
    printf("Squared interpolation method: min = %f;\n\n", squared_method(SEARCH_BEGIN, SEARCH_END));
    return 0;
}