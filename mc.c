#include <stdio.h>
#include <string.h>

#include "mc.h"

#define CLEAR_BUFFER {\
	memset(b, '\0', 6);\
	idx = 0;\
	printf("Buffer: %s\n",b);\
	continue;\
	}

/* mc_dec Decode a Morse code string
 * Input: b = Null-terminated string containing up to 5 '.' and '-'
 * Output: c = Character b represents, or '\0' if not a Morse code. 
 */
char mc_dec(char b[6]) {
	const char DEC_KEY[] = "\0 etianmsurwdkgohvf\0l\0pjbxcyzq\0\0\x35\x34\0\x33\0\0\0\x32\0\0\0\0\0\0\0\x31\x36\0\0\0\0\0\0\0\x37\0\0\0\x38\0\x39\x30\0\0\0\0\0\0\0\0\0\0\0\0?\0\0\0\0\0\0\0\0.\0\0\0\0\0\0\0\0\0\0\0-\0\0\0\0\0\0\0\0\0!\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	int pos = 1;
	for(int idx=0; idx<6; idx++) {
		if('.' == b[idx]) pos = 2*pos;
		else if('-' == b[idx]) pos = 2*pos+1;
		else if(!b[idx]) break;
	}
	return DEC_KEY[pos];
}

/* mc_listen Listen for Morse code from STDIN.
 * Listens to STDIN, putting each '.' (DOT) and '-' (DASH) into a 5-char-long buffer. 
 * When the buffer is full, the next '.' or '-' clears the buffer.
 * The Morse-coded-character is terminated by 'x'.
 * Once 'x' is input to STDIN:
 *	If the current buffer is empty, ' ' is returned.
 *	If the current buffer represents a character in Morse code, this character is returned. 
 *	If the current buffer is full but does not represent a Morse code character, the buffer is cleared.
 *
 * Inputs: None.
 * Output: char representing a decoding of a Morse code.
 */
char mc_listen() {
	char b[6];
	memset(b, '\0', 6);
	int idx = 0;
	while(1) {
		char in = getchar();
		if(('.' == in)||
		   ('-' == in)) {
			if(idx == 6) CLEAR_BUFFER
			b[idx] = in;
			idx = (idx+1)%6;
			printf("Buffer: %s\n",b);
		}
		else if('x' == in) {
			char c = mc_dec(b);
			if(!c) CLEAR_BUFFER
			return c;
		}
	}
}


