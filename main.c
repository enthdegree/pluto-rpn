/* 
 * Morse code RPN calculator. 
 *
 */
#include <stdio.h>
#include <string.h>

#include "mc.h"
#include "calc.h"

#define TOKEN_SIZE 32

int main() {
    struct calc_state cs;
    init_calc(&cs);

    char t[TOKEN_SIZE];
    memset(t, '\0', TOKEN_SIZE);
    int idx_t = 0;
    while(1) {
        char c = mc_listen();
        if(' ' == c) {
            process_token(t, &cs);
            t[0] = '\0';
            idx_t = 0;
        }
        // 'v', as in 'view', is a control character to replay the current input
        else if('v' == c) {
            printf("%s_\n", t);
        }
        // 'w', as in 'wipe', is a control character to delete the last typed char
        else if('w' == c) {
            if(idx_t > 0) idx_t = idx_t-1;
            t[idx_t] = '\0';
        }
        else { 
            t[idx_t] = c;
            t[idx_t+1] = '\0';
            idx_t = (idx_t+1)%(TOKEN_SIZE-1);
        }
    }
}
