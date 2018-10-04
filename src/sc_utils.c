#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

void print(const char *comment, const char *b, const char *e) {
	const char *c = NULL;
	printf("%s", comment);
	for (c = b; c < e; c++)
		printf("%c", *c);
	printf("\n");
}

int equation(double *result, const char* s, const char* f);

int task_pri(int *pri, const char task) {
	int _pri;
	switch (task) {
	case '+':
	case '-':
		_pri = 1;
		break;
	case '*':
	case '/':
		_pri = 0;
		break;
	default:
		return EDOM;
	}
	if (NULL != pri)
		*pri = _pri;
	return 0;
}

int arg(const char **b, const char **e, char* task, const char **in_brd,
		const char **ex_brd, const char *s, const char *f) {
	const char *c = s;
	int brk = 0;

	if (s >= f)
		return EINVAL;

	if ('(' == *c) {
		for (brk = 0; c < f; ++c) {
			if ('(' == *c)
				brk++;
			else if (')' == *c)
				brk--;
			if (!brk)
				break;
		}

		if (brk)
			return EDOM;

		*b = s + 1;
		*e = c;

		c++;

		*in_brd = c;
		*ex_brd = c;

		if (c >= f)
			return 0;

		*task = *c;
		*ex_brd = c + 1;

		if (*ex_brd >= f)
			*ex_brd = f;

		return 0;

	}

	while (c < f && (('0' <= *c && '9' >= *c) || *c == ',' || *c == '.'))
		c++;

	*e = c;
	*b = s;
	*ex_brd = c;
	*in_brd = c;

	if (c < f) {
		*task = *c;
		*ex_brd = c + 1;
	}

	return 0;
}

int calulate(double *result, double a_sign, char op, const char *as,
		const char *af, const char *bs, const char *bf) {
	int ret = 0;
	double a = 0, b = 0;

	if ( NULL == as || NULL == af || NULL == bs || NULL == bf)
		return EINVAL;

	ret = equation(&a, as, af);
	if (0 != ret)
		return ret;
	ret = equation(&b, bs, bf);
	if (0 != ret)
		return ret;

	switch (op) {
	case '+':
		*result = a_sign * a + b;
		break;
	case '-':
		*result = a_sign * a - b;
		break;
	case '*':
		*result = a_sign * a * b;
		break;
	case '/':
		if (!b)
			return ERANGE;
		*result = a_sign * a / b;
		break;
	default:
		return EDOM;
	}
	return 0;

}

int equation(double *result, const char* s, const char* f) {
	char op_a = 0, op_b = 0;

	int op_a_p = 0, op_b_p = 0;

	const char *as = NULL, *af = NULL, *bs = NULL, *bf = NULL;

	const char *in_bd_a = NULL, *ex_bd_a = NULL, *in_bd_b = NULL, *ex_bd_b = NULL;

	int ret = 0;
	double a_sign = 1;

	if (NULL == result)
		return EINVAL;

	if (s >= f) {
		*result = 0;
		return 0;
	}

	if ('-' == *s) {
		a_sign = -1;
		as = s + 1;
	} else
		as = s;

	ret = arg(&as, &af, &op_a, &in_bd_a, &ex_bd_a, as, f);

	if (0 != ret)
		return ret;

	if (ex_bd_a >= f) {

		if (in_bd_a != af)
		{
			ret = equation(result, as, af);
			*result*=a_sign;
			return ret;
		}

		if (1 == sscanf(s, "%lf", result))
			return 0;

		return EDOM;
	}

	ret = task_pri(&op_a_p, op_a);

	if (0 != ret)
		return ret;

	do {
		ret = arg(&bs, &bf, &op_b, &in_bd_b, &ex_bd_b, ex_bd_a, f);

		if (0 != ret)
			return ret;

		if (ex_bd_b >= f)
			break;

		ret = task_pri(&op_b_p, op_b);

		if (0 != ret)
			return ret;

		bs = ex_bd_a;
		bf = f;

		if (op_b_p < op_a_p)
			break;

		as = s;
		a_sign = 1;
		af = in_bd_b;
		ex_bd_a = ex_bd_b;
		op_a_p = op_b_p;
		op_a = op_b;

	} while (1);

	return calulate(result, a_sign, op_a, as, af, bs, bf);

}

int is_suitable(char c) {
	if (isdigit(c))
		return 1;

	if ('(' == c || ')' == c || '.' == c) 
		return 1;

	if (0 == task_pri(NULL, c))
		return 1;

	return 0;

}

int test(char **dst_b, char **dst_e, const char *src) {

	char *dst_i = NULL;
	const char *src_i = NULL;
	if (NULL == dst_b || NULL == src)
		return EINVAL;

	*dst_b = calloc(strlen(src) + 1, sizeof(char));

	if ( NULL == dst_b)
		return ENOMEM;

	for (dst_i = *dst_b, src_i = src; *src_i; src_i++) {
		if (isspace(*src_i))
			continue;
			
		if ( ',' == *src_i ) {
			*dst_i = '.';
			dst_i++;
			continue;
		}

		if (is_suitable(*src_i)) {
			*dst_i = *src_i;
			dst_i++;
			continue;
		}

		if (*src_i)
			return EDOM;

	}

	*dst_e = dst_i;

	return 0;

}

int solve(double *result, const char* eq) {
	int ret;
	char *b = NULL;
	char *e = NULL;
	ret = test(&b, &e, eq);

	if (0 != ret)
		goto End;

	ret = equation(result, b, e);

	*result = (double) round(*result * 100.) / 100.;

	End:

	if (NULL != b)
		free(b);

	return ret;
}
