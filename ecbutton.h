

#ifndef     ECBUTTON_H_
#define     ECBUTTON_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct buttons_debouncing {
        uint8_t depressed;
        uint8_t previous;
};


struct buttons {
    struct buttons_debouncing debounce;
    struct {
        uint8_t current;
        uint8_t previous; 
    }level;
    uint32_t change;
    uint32_t tick;
};

struct encoder {
    struct buttons A;
    struct buttons B;
    struct buttons S;
};
enum {
    ROTARY_CW = 1,
    ROTARY_CCW = 2,
};
enum {
    BUTTON_NO_CHANGE = 0,
    BUTTON_CHANGE_STATE = 1,
    BUTTON_KEEP = 2,
    BUTTON_DEPRESSED = 1 << 4,
    BUTTON_RELEASED = 1 << 5,
    BUTTON_SHORT = 1 << 6,
    BUTTON_LONG = 1 << 7,
    BUTTON_LONG_LONG = 1 << 8,
};


#define TIME_CONFIG          {300, 300, 600, 600, 1000, 1200}


int16_t ecbutton(struct encoder *encoder, const uint8_t A, const uint8_t B, const uint8_t S);

static inline uint8_t ecbutton_switch(struct encoder *encoder){
    return encoder->S.debounce.depressed ? BUTTON_DEPRESSED : BUTTON_RELEASED;
}


#ifdef __cplusplus
}
#endif
#endif



