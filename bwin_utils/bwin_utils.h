#ifndef BWIN_UTILS_H
#define BWIN_UTILS_H

#ifdef DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif


DECLDIR int card_range(char *t, int opponents, int *range);
DECLDIR unsigned char card_to_num(char c);

#endif BWIN_UTILS_H