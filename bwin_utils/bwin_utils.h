#ifndef BWIN_UTILS_H
#define BWIN_UTILS_H

#ifdef DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*
	Function returns preflop hand range %
	t - pointer to cards setting e.g "ATs"
	opponents - number of opponents (1-8)
	range - return value

	returns 0 on SUCCESS, 1 on FAIL
*/

DECLDIR int card_range(char *t, int opponents, int *range);

/*
	Function returns the card value (2 = 2, A = 14)

	returns >0 on SUCCESS, -1 on FAIL
*/
DECLDIR char card_to_num(char c);

/*
	Function returns the absolute difference in card values e.g. (A, K) = 1, (T, Q) = 2

	returns >0 on SUCCESS, -1 on FAIL
*/
DECLDIR char card_diff(char c1, char c2);

#ifdef __cplusplus
}
#endif

#endif BWIN_UTILS_H