#include "sc_utils.h"
#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	double result = 0;

	int ret = 0;

	if (argc<2)
	{
		printf("Place equation in the first param.");
		return 1;
	}

	ret = solve(&result, argv[1]);

	switch (ret) {
	case 0:
		printf("%.2f", result);
		break;
	case EDOM:
		printf("Wrong equation.");
		break;
	case EINVAL:
		printf("Internal error.");
		break;
	case ENOMEM:
		printf("Not enough memory.");
		break;
	case ERANGE:
		printf("Dividing by zero.");
		break;
	default:
		printf("Unknown error.");
	}

	printf("\n");
	return ret;
}
