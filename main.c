#include <stdio.h>
#include "markov.h"


int main()
{
	char buff[100000];
	scanf("%100000s", buff);
	char* str = add2markov(buff);

	if (str == 0)
	{
		fprintf(stderr, "String must contains only |.");
		return -1;
	}

	printf("%s\n", str);
	free(str);
	return 0;
}
