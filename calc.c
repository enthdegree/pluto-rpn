#include <stdio.h>
#include <string.h>
#include <math.h>

#include "calc.h"

/* init_calc Initialize calculator
 */
void init_calc(struct calc_state * cs) {
	for(int idx=0;idx<STACK_SIZE;idx++) (*cs).stack[idx] = NAN;
	for(int idx=0;idx<REG_SIZE;idx++) (*cs).reg[idx] = NAN;
    (*cs).n_stack = 0;
    return;
}

/* process_token Process the stack in the way specified by the token.
 * Otherwise, try to convert it to a double and add it to the stack.
 * If that fails, return -1.
 *
 * Input: char * token = Pointer to a token string.
 *        struct calc_state * cs = The stack to process. 
 * Output: 0 if token subroutine completed normally.
 *        -1 if token did not match any subroutine, or stack processing failed.
 */
#define NS (*cs).n_stack
#define S (*cs).stack
#define R (*cs).reg
#define CHECK_STACK(x) if((NS<x) || \
        ((0==x) && (STACK_SIZE==x))) {\
    printf("BAD STACK\n");\
    return -1; \
    } 
int process_token(char * token, struct calc_state * cs) {
    printf("Token: \"%s\"\n", token);

    if(strcmp(token, "") == 0) return 0;
    // Negation
    else if(strcmp(token, "n") == 0) { 
        CHECK_STACK(1)
        S[NS-1] = -S[NS-1];
        return 0;
    }
    // Addition
    else if(strcmp(token, "a") == 0) { 
        CHECK_STACK(2)
        S[NS-2] = S[NS-1] + S[NS-2];

        S[NS-1] = NAN;
        NS = NS-1;
        return 0;
    } 
    // Multiplication
    else if(strcmp(token, "m") == 0) { 
        CHECK_STACK(2)
        S[NS-2] = S[NS-1]*S[NS-2];

        S[NS-1] = NAN;
        NS = NS-1;
        return 0;
    }
    // Pi
    else if(strcmp(token, "p") == 0) { 
        CHECK_STACK(0)
        S[NS] = M_PI; 
        NS = NS + 1;
        return 0;
    }
}


