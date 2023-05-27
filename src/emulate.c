#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

int currAddress = 0x0;

/*registers*/

struct pState {
	bool N;
	bool Z;
	bool C;
	bool V;
}

/* Private functions */

static void inc_PC (){
	currAddress += 4;
};

static void readInstruction ( int objectCode ) {
}

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}
