# pluto-rpn
RPN calculator inside an F91-W.  Interface proof-of-concept. 

Usage
-----

 1. Enter Morse code '.', '-' into STDIN to build a token. 
 2. Use character 'x' to end a group of Morse code. 
 3. Press return to submit a line of ".-x"'s
 4. When an empty group is submitted, the buffer is processed by the calculator.

Example
-------

    Input: "....-x.x..---x-..x-x...--x-----x\n"

The above represents entering the string "4e2dt30" into the buffer, but 
not yet submitting to the calculator. 

We can view the current buffer by inputting the control character 'v' 
(which doesn't add to the buffer):

    Input: "...-x\n"

If we decide the last buffer input (here a '0') was a mistake, we can 
delete it using the control character 'w' by inputting:

    Input: ".--x\n"

Now the string in the buffer is "4e2dt3".

    Input: "x\n" 

This 'x' ends a blank group, which submits the buffer to calculator 
processing. As per the definition of `process_token in `calc.c`, the 
calculator interprets the buffer input "4e2dt3" as the double 4.2e-3, and 
adds it to the RPN calculator stack. 

The next line submits the token "exp" into to the calculator, causing the 
calculator to process the top of the stack with the math function `exp()`:

    Input: ".x-..-x.--.xx\n"

The next line submits the token "p" to the calculator, which causes it to 
print the top few entries in the stack:

    Input: ".--.xx\n"

For a complete list of calculator functions see the body of `process_token` 
in `calc.c`
 
