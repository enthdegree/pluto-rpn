#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "calc.h"

/* init_calc Initialize calculator
*/
void init_calc(struct calc_state * cs) {
	for(int idx=0;idx<STACK_SIZE;idx++) (*cs).stack[idx] = NAN;
	for(int idx=0;idx<REG_SIZE;idx++) (*cs).reg[idx] = NAN;
	memset((*cs).token, '\0', TOKEN_SIZE*sizeof(char));
	(*cs).tidx = 0;
	(*cs).n_stack = 0;
	return;
}

/* input_to_calc Input a character to the calculator
 * Control chars:
 *      ' ', 'v', 'w'
 *
 * Input: cs = calculator to input to
 *         c = char to add to the token
 */
void input_to_calc(struct calc_state * cs, char c) {
	// ' ' causes the current string in `token` to be processed
	if(' ' == c) {
		process_token((*cs).token, cs);
		memset((*cs).token, '\0', TOKEN_SIZE*sizeof(char));
		(*cs).tidx = 0;
	}
	// 'v', as in 'view', is a control character to replay the current input
	else if('v' == c) { 
		printf("%s_\n", (*cs).token);
	}
	// 'w', as in 'wipe', is a control character to delete the last typed char
	else if('w' == c) {
		if((*cs).tidx>0) (*cs).tidx = (*cs).tidx-1;
		(*cs).token[(*cs).tidx] = '\0';
	} 
	else { 
		(*cs).token[(*cs).tidx] = c;
		(*cs).token[(*cs).tidx+1] = '\0';
		(*cs).tidx = ((*cs).tidx+1)%(TOKEN_SIZE-1);
	}
}

/* daprint print the first 5 of a list of doubles
*/
void daprint(double * da) {
	printf("\n");
	for(int idx=0;idx<5;idx++) printf("%f,\n",da[idx]);
	printf("...\n");
}

/* str_rep Replace all instances of a with b in str (stop at null)
 * Input: str = initial string
 * rep = replacement location
 * a = original char
 * b = thing to replace a with
 */
void str_rep(char * str, char * rep, char a, char b) {
	int nlen = strlen(str);
	for(int idx=0;idx<nlen;idx++) {
		if(a == str[idx]) rep[idx] = b; 
		else rep[idx] = str[idx];
	}
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
int process_token(char * token, struct calc_state * cs) {
#define NS (*cs).n_stack
#define S (*cs).stack
#define R (*cs).reg
#define CHECK_STACK(x) if((NS<x) || \
		((0==x) && (STACK_SIZE==x))) {\
	printf("BAD STACK\n");\
	return -1; \
} 
#define PUSH(x) S[NS] = x; \
						NS = NS + 1;
#define POP S[NS-1] = NAN; \
					  NS = NS-1;

printf("Token: \"%s\"\n", token);

if(strcmp(token, "") == 0) return 0;
/* -----------------------
 * Stack, register control
 * ----------------------- */
// Delete last thing on stack
else if(strcmp(token, "x") == 0) { 
	CHECK_STACK(2)
		POP	
		return 0;
}
// Save to register 
/* Usage: X, to be saved, is on the top of the stack.
 *        Add register index A to the top of the stack.
 *        Run "z" to save X to register A.
 */
else if(strcmp(token, "z") == 0) { 
	CHECK_STACK(2)
		int idx = round(S[NS-1]);
	POP
		if(!((0<=idx)&&(idx<REG_SIZE))) { 
			printf("BAD REGISTER\n");
			return -1;
		}
	double d = S[NS-2];
	R[idx] = d;
	POP
		return 0;
}
// Yank from register
/* Usage: X, to be yanked, is in register A.
 *        Add register index A to the top of the stack.
 *        Run "y" to replace the top of the stack with contents of register A.
 */
else if(strcmp(token, "y") == 0) { 
	CHECK_STACK(1)
		int idx = round(S[NS-1]);
	POP
		if(!((0<=idx)&&(idx<REG_SIZE))) { 
			printf("BAD REGISTER\n");
			return -1;
		}
	PUSH(R[idx])
		return 0;
}
// Print stack
else if(strcmp(token, "p") == 0) { 
	printf("Stack: ");
	daprint(S);
	return 0;
}

/* ---------------
 * Basic operators
 * --------------- */
// Addition
else if(strcmp(token, "a") == 0) { 
	CHECK_STACK(2)
		S[NS-2] = S[NS-1] + S[NS-2];
	POP
		return 0;
} 
// Negation
else if(strcmp(token, "t") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = -S[NS-1];
	return 0;
}
// Multiplication
else if(strcmp(token, "m") == 0) { 
	CHECK_STACK(2)
		S[NS-2] = S[NS-1]*S[NS-2];
	POP
		return 0;
}
// Division
/* Usage: A is on top, above B. 
 * Run "d" to pop A,B, push B/A.
 */
else if(strcmp(token, "d") == 0) { 
	CHECK_STACK(2)
		S[NS-2] = S[NS-1]/S[NS-2];
	POP
		return 0;
}
// Abs
else if(strcmp(token, "ab") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = abs(S[NS-1]);
	return 0;
}

/* ---------------------
 * Exponential functions
 * --------------------- */
// Power
/* Usage: A is on top, above B. 
 * Run "d" to pop A,B, push B^A.
 */
else if(strcmp(token, "po") == 0) { 
	CHECK_STACK(2)
		S[NS-2] = pow(S[NS-2],S[NS-1]);
	POP
		return 0;
}
// Exp
else if(strcmp(token, "exp") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = exp(S[NS-1]);
	return 0;
}
// Square root
else if(strcmp(token, "sr") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = sqrt(S[NS-1]);
	return 0;
}
// Natural logarithm
else if(strcmp(token, "ln") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = log(S[NS-1]);
	return 0;
}
// Log base-10
else if(strcmp(token, "lg") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = log10(S[NS-1]);
	return 0;
}

/* -----------------------
 * Trigonometric functions
 * ----------------------- */
// Sine
else if(strcmp(token, "sn") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = sin(S[NS-1]);
	return 0;
}
// Cosine 
else if(strcmp(token, "cs") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = cos(S[NS-1]);
	return 0;
}
// Tangent 
else if(strcmp(token, "tn") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = tan(S[NS-1]);
	return 0;
}
// Arcsine 
else if(strcmp(token, "asn") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = asin(S[NS-1]);
	return 0;
}
// Arccosine 
else if(strcmp(token, "acs") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = acos(S[NS-1]);
	return 0;
}
// Arctangent 
else if(strcmp(token, "atn") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = atan(S[NS-1]);
	return 0;
}

/* -------
 * Entropy
 * ------- */
// Binary Shannon entropy (bits)
else if(strcmp(token, "h") == 0) { 
	CHECK_STACK(1)
		if((S[NS-1]<0) || (S[NS-1]>1)) {
			S[NS-1] = NAN;
			return 0;
		}
	if((S[NS-1]==0) || (S[NS-1]==1)) {
		S[NS-1] = 0;
		return 0;
	}
	S[NS-1] = (1/log(2))* \
			  -S[NS-1]*log(S[NS-1]) \
			  -(1-S[NS-1])*log(1-S[NS-1]);
	return 0;
}
// Log base-2  
else if(strcmp(token, "lg2") == 0) { 
	CHECK_STACK(1)
		S[NS-1] = log(S[NS-1])/log(2);
	return 0;
}

/* ---------
 * Constants
 * --------- */
// Euler's constant
else if(strcmp(token, "e") == 0) { 
	CHECK_STACK(0)
		PUSH(M_E)        
		return 0;
}
// Pi
else if(strcmp(token, "pi") == 0) { 
	CHECK_STACK(0)
		PUSH(M_PI)        
		return 0;
}

// Try and convert to double, add to stack.
/* Usage: Enter a number in any format detected by strtod( )
 * 'e' represents a decimal point '.' 
 * 'd' represents scientific notation exponent
 * 't' represents a negative sign '-'
 * 
 * 4.2e-3 would be entered as "4e2dt3"
 */
char td[strlen(token)];
str_rep(token, td, 'e', '.');    
str_rep(td, td, 'd', 'e');    
str_rep(td, td, 't', '-');    

char * cfail;
double dc;
dc = strtod(td, &cfail);
if(td == cfail) return -1;
else {
	CHECK_STACK(0)
		PUSH(dc)
		return 0;
}
}

