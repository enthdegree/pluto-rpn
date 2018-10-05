/* 
 * Morse code RPN calculator. 
 *
 * Usage:
 * Enter morse code '.', '-' into STDIN to build a token. When a space (morse code blank), the last token is processed.
 * Tokens are either processed or converted into double-precision floats through `strtod` 
 *
 */
#include <stdio.h>
#include <string.h>

#include "mc.h"
#include "calc.h"

#define TOKEN_SIZE 32

void doubleprint(double * fa) {
    printf("\n");
    for(int idx=0;idx<5;idx++) printf("%f,\n",fa[idx]);
    printf("...\n");
}

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
            printf("Stack: ");
            doubleprint(cs.stack);
            t[0] = '\0';
            idx_t = 0;
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
