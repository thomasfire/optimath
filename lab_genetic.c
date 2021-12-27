//
// Created by thomasf on 2021/12/27.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static uint16_t node_map[5][5] = {
        {0, 4, 6, 2, 9},
        {4, 0, 3, 2, 9},
        {6, 3, 0, 5, 9},
        {2, 2, 5, 0, 8},
        {9, 9, 9, 8, 0},
};

#define POPULATION (4)
#define PMUT (0.01)
#define EPOCH (4)
#define WLEN (5)

#define FUNC(_arr, _size) ({ \
   uint16_t _buff = 0;   \
   for (unsigned _i=1;_i<(_size);_i++) { \
        _buff += node_map[(_arr)[_i-1]][(_arr)[_i]];                      \
   }                     \
   _buff + node_map[(_arr)[0]][(_arr)[(_size)-1]];\
})

#define GENERATE(_target) do{ \
     uint16_t _d[WLEN] = {0 , 1, 2, 3, 4}; \
     for (uint16_t _i = 4; _i > 0; --_i){\
        uint16_t _j = rand() % (_i+1);\
        uint16_t _temp = _d[_i];\
        _d[_i] = _d[_j];\
        _d[_j] = _temp;\
    }                        \
     for (uint16_t _i = 0; _i < WLEN; _i++)  \
        (_target)[_i] = _d[_i];\
}while(0)

#define MUTATE(_target) do{ \
       uint16_t _tmp = 0;                     \
       uint16_t _l = rand() % WLEN,\
                _r = rand() % WLEN;\
       _tmp = (_target)[_l]; \
       (_target)[_l] = (_target)[_r];\
       (_target)[_r] = _tmp;\
}while(0)

#define DOES_MUTATE() ({ \
    uint32_t _d = 1 /  PMUT; \
    uint32_t _n = rand() % _d; \
    !_n;\
})

#define pprint(_arr) do{ printf("%hu %hu %hu %hu %hu", (_arr)[0], (_arr)[1], (_arr)[2], (_arr)[3], (_arr)[4]); }while(0)

#define CACHE_PATHS(_arr, _tar) do { for (uint16_t _ic = 0; _ic < POPULATION; _ic++) { \
    (_tar)[_ic] = FUNC((_arr)[_ic], WLEN); \
}}while(0)


static inline void cross(const uint16_t *lhs, const uint16_t *rhs, uint16_t *ltarget, uint16_t *rtarget) {
    uint16_t lsplit = rand() % WLEN,
            rsplit = rand() % WLEN;
    if (lsplit > rsplit) {
        uint16_t tmp = lsplit;
        lsplit = rsplit;
        rsplit = tmp;
    } else if (lsplit == rsplit) {
        if (lsplit == 0) rsplit += 2;
        else if (lsplit == 4) lsplit -= 2;
        else {
            lsplit--;
            rsplit++;
        }
    }
    memset(ltarget, -1, WLEN * sizeof(*rtarget));
    memset(rtarget, -1, WLEN * sizeof(*rtarget));
    uint16_t lbits = 0, rbits = 0;
    for (uint16_t i = lsplit; i < rsplit; i++) {
        ltarget[i] = rhs[i];
        lbits |= (1 << ltarget[i]);
        rtarget[i] = lhs[i];
        rbits |= (1 << rtarget[i]);
    }
    for (uint16_t i = 0; i < WLEN; i++) {
        if (ltarget[i] != (uint16_t) - 1) continue;
        uint16_t tmp = 0;
        tmp = lhs[i];
        while ((1 << tmp) & lbits) tmp = (tmp + 1) % WLEN;
        lbits |= (1 << tmp);
        ltarget[i] = tmp;

        tmp = rhs[i];
        while ((1 << tmp) & rbits) tmp = (tmp + 1) % WLEN;
        rbits |= (1 << tmp);
        rtarget[i] = tmp;
    }
}

static uint16_t path_len_cache[WLEN] = {0};

int comp(const void *lhs, const void *rhs) {
    return path_len_cache[*(uint16_t *) lhs] - path_len_cache[*(uint16_t *) rhs];
}

int main() {
    srand(time(NULL));
    uint16_t current[POPULATION][WLEN] = {0};
    for (uint16_t i = 0; i < POPULATION; i++) {
        GENERATE((current)[i]);
    }
    CACHE_PATHS(current, path_len_cache);

    for (uint16_t ep = 0; ep < EPOCH; ep++) {
        uint16_t rating[POPULATION] = {0, 1, 2, 3};
        qsort(rating, POPULATION, sizeof(uint16_t), comp);
        printf("Parents rating: {%hu, %hu, %hu, %hu}\n", rating[0], rating[1], rating[2], rating[3]);
        for (uint16_t i = 0; i < POPULATION; i++) {
            pprint(current[i]);
            printf(" (%hu)\n", path_len_cache[i]);
        }
        uint16_t buffer[POPULATION][WLEN] = {0};
        cross(current[rating[0]], current[rating[1]], buffer[0], buffer[1]);
        cross(current[rating[2]], current[rating[3]], buffer[2], buffer[3]);
        for (uint16_t i = 0; i < POPULATION; i++) {
            if (DOES_MUTATE()) {
                MUTATE(current[i]);
            }
        }
        CACHE_PATHS(buffer, path_len_cache);
        uint16_t rating_buff[POPULATION] = {0, 1, 2, 3};
        qsort(rating_buff, POPULATION, sizeof(uint16_t), comp);

        printf("Childs rating: {%hu, %hu, %hu, %hu}\n", rating_buff[0], rating_buff[1], rating_buff[2], rating_buff[3]);
        for (uint16_t i = 0; i < POPULATION; i++) {
            pprint(buffer[i]);
            printf(" (%hu)\n", path_len_cache[i]);
        }

        // копіюємо самих найуспішнiших
        memcpy(current[rating[2]], buffer[rating_buff[0]], sizeof(current[0]));
        memcpy(current[rating[3]], buffer[rating_buff[1]], sizeof(current[0]));

        CACHE_PATHS(current, path_len_cache);
        printf("\n\n");
    }


}
