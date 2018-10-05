/* calc.h Calculator internals 
 */

#define STACK_SIZE 16
#define REG_SIZE 4 

struct calc_state {
    double stack[STACK_SIZE];
    double reg[REG_SIZE];
    int n_stack; 
};

/* init_calc Initialize calculator
 */
void init_calc(struct calc_state * cs);

/* process_token Process the stack in the way specified by the token.
 * Otherwise, try to convert it to a double and add it to the stack.
 * If that fails, return -1.
 *
 * Input: char * token = Pointer to a token string.
 *        struct calc_state * cs = The stack to process. 
 * Output: 0 if token subroutine completed normally.
 *        -1 if token did not match any subroutine, or stack processing failed.
 */
int process_token(char * token, struct calc_state * cs);
