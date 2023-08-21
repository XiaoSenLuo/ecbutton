
#ifdef __cplusplus
extern "C" {
#endif

#include "ecbutton.h"


static uint16_t button_debounce(struct buttons_debouncing * const debounce, const uint16_t pin_level);

uint16_t button_debounce(struct buttons_debouncing * const debounce, const uint16_t pin_level){
    uint32_t current = 0;
    uint32_t tmp = 0;

    current = pin_level;
    tmp = debounce->depressed;
    debounce->depressed |= (debounce->previous & current);
    debounce->depressed &= (debounce->previous | current);
    debounce->previous = current;
    tmp ^= debounce->depressed;

    return tmp ? BUTTON_CHANGE_STATE : BUTTON_NO_CHANGE;
}

int16_t ecbutton(struct encoder *encoder, const uint8_t A, const uint8_t B, const uint8_t S){
    int16_t ro = 0;

    encoder->S.tick += 1;
    encoder->A.tick = encoder->S.tick;
    encoder->B.tick = encoder->S.tick;
    if(button_debounce(&encoder->A.debounce, A)){   /// changed
        encoder->A.level.previous = encoder->A.level.current;
        encoder->A.level.current = A;
        // encoder->A.change += 1;
        
        if(encoder->A.debounce.depressed){    /// depressed

        }else{    /// released
            if(encoder->A.level.current ^ encoder->B.level.current){    /// 顺时针
                ro |= ROTARY_CW;
            }else{    /// 逆时针
                ro |= ROTARY_CCW;
            }
        }
    }
    if(button_debounce(&encoder->B.debounce, B)){   /// changed
        encoder->B.level.previous = encoder->B.level.current;
        encoder->B.level.current = B;
        // encoder->B.change += 1;

        if(encoder->B.debounce.depressed){    /// depressed

        }else{    /// released

        }
    }else{

    }

    if(button_debounce(&encoder->S.debounce, S)){   /// changed
        encoder->S.level.previous = encoder->S.level.current;
        encoder->S.level.current = S;
        uint16_t time[] = TIME_CONFIG;
        if(encoder->S.debounce.depressed){      /// depressed
            encoder->S.change = encoder->S.tick;
        }else{
            encoder->S.change = encoder->S.tick - encoder->S.change;
            if((encoder->S.change <= time[0])){
                /// 点击
                ro |= BUTTON_RELEASED;
            }
            else if((encoder->S.change > time[1]) && (encoder->S.change <= time[2])){
                /// 短按
                ro |= BUTTON_SHORT;
            }else if((encoder->S.change > time[3]) && (encoder->S.change <= time[4])){
                /// 长按
                ro |= BUTTON_LONG;
            }else if(encoder->S.change > time[5]){
                ro |= BUTTON_LONG_LONG;
            }
        }
    }else{
        if(encoder->S.debounce.depressed){
            // encoder->S.change = encoder->S.tick - encoder->S.change;
            // if((encoder->S.change <= 500)){
            //     /// 点击
            //     // ro |= BUTTON_RELEASED;
            //     encoder->S.change = encoder->S.tick - encoder->S.change;
            // }else if((encoder->S.change > 500) && (encoder->S.change <= 1000)){
            //     /// 短按
            //     ro |= BUTTON_SHORT;
            // }else if((encoder->S.change > 1500) && (encoder->S.change <= 3000)){
            //     /// 长按
            //     ro |= BUTTON_LONG;
            // }else if(encoder->S.change > 3500){
            //     ro |= BUTTON_LONG_LONG;
            // }else{

            // }
        }
    }
    return ro & ROTARY_CCW ? ro * -1 : ro;
}










#ifdef __cplusplus
}
#endif
