/* 
 * Morse code RPN calculator. 
 *
 */
#include <stdio.h>
#include <string.h>

#include "mc.h"
#include "calc.h"


int main() {
    struct calc_state cs;
    struct mc_buffer mcb;
    init_calc(&cs);
    init_mc_buffer(&mcb);

    while(1) {
        char c = getchar();
	    if('x' == c ) {
            char d;
            mc_writechar(&mcb, &d);
            input_to_calc(&cs, d);
        }
        else mc_read(&mcb, c);
    }
}
