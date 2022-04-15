#include <stdio.h>
#include "markov.h"


int main()
{
	char buff[100000];
	scanf("%s", buff);
	char* str = add2markov(buff);
	printf("%s\n", str);
	free(str);
	return 0;
}
