#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

#define DLL_EXPORT
#include "bwin_utils.h"

#define HANDS_ROWS (13 + 13 + 12 + 11 + 10 + 9 + 8 + 7 + 6 + 5 + 4 + 3 + 2)
#define OPPONENTS_MAX 8

enum table_type
{
	PAIRS,
	XA,
	XK,
	XQ,
	XJ,
	XT,
	X9,
	X8,
	X7,
	X6,
	X5,
	X4,
	X3,
	TABLE_TYPE_NUM
};

enum table_start_ix
{
	IX_PAIRS = 0,
	IX_XA = IX_PAIRS + 13 + 1, /* Account for the suited & connected row - the first row in every table after this one */
	IX_XK = IX_XA + 13,
	IX_XQ = IX_XK + 12,
	IX_XJ = IX_XQ + 11,
	IX_XT = IX_XJ + 10,
	IX_X9 = IX_XT + 9,
	IX_X8 = IX_X9 + 8,
	IX_X7 = IX_X8 + 7,
	IX_X6 = IX_X7 + 6,
	IX_X5 = IX_X6 + 5,
	IX_X4 = IX_X5 + 4,
	IX_X3 = IX_X4 + 3
};

static unsigned char table_start_indexes[] =
{
	IX_PAIRS,
	IX_XA,
	IX_XK,
	IX_XQ,
	IX_XJ,
	IX_XT,
	IX_X9,
	IX_X8,
	IX_X7,
	IX_X6,
	IX_X5,
	IX_X4,
	IX_X3
};

static unsigned char hand_percent[HANDS_ROWS * OPPONENTS_MAX] =
{

	85, 73, 64, 56, 49, 43, 39, 31, // AA 
	82, 69, 58, 50, 43, 38, 33, 29,
	80, 65, 54, 45, 38, 33, 28, 25,
	78, 61, 49, 40, 34, 29, 25, 22,
	75, 58, 45, 36, 30, 25, 22, 19,
	72, 54, 41, 33, 26, 22, 19, 17,
	69, 50, 38, 29, 24, 20, 18, 16,
	66, 46, 34, 26, 21, 19, 16, 15,
	63, 43, 32, 25, 20, 17, 15, 14,
	60, 40, 29, 22, 19, 16, 14, 13,
	57, 37, 25, 21, 17, 15, 14, 13,
	54, 34, 24, 19, 16, 15, 14, 13,
	50, 31, 22, 18, 16, 14, 13, 13, // 22

	67, 51, 41, 35, 31, 28, 25, 23, // AKs 
	65, 48, 39, 32, 28, 24, 22, 19, // AK
	64, 47, 37, 30, 26, 23, 20, 18,
	64, 46, 35, 29, 24, 21, 18, 16,
	63, 44, 34, 28, 23, 20, 17, 15,
	61, 42, 31, 25, 20, 17, 15, 13,
	60, 41, 30, 24, 19, 16, 14, 12,
	59, 39, 28, 22, 18, 15, 13, 11,
	58, 38, 28, 21, 17, 14, 12, 11,
	58, 38, 28, 21, 17, 14, 12, 11,
	56, 37, 27, 21, 17, 14, 12, 11,
	56, 36, 26, 20, 17, 14, 12, 10,
	55, 35, 25, 20, 16, 14, 12, 10,

	63, 47, 38, 33, 28, 25, 23, 20, // KQs
	61, 44, 35, 29, 25, 21, 19, 17, // KQ
	61, 43, 34, 28, 24, 20, 18, 16,
	60, 42, 33, 27, 22, 19, 17, 15,
	58, 40, 30, 24, 20, 17, 14, 12,
	56, 37, 27, 21, 17, 15, 13, 11,
	55, 36, 26, 21, 17, 14, 12, 10,
	54, 35, 25, 20, 16, 13, 11, 10,
	53, 34, 25, 19, 15, 13, 11, 10,
	52, 33, 23, 18, 15, 12, 11, 9,
	51, 32, 23, 18, 14, 12, 10, 9,
	50, 31, 22, 17, 14, 12, 10, 9,

	60, 44, 36, 30, 26, 23, 21, 19, // QJs
	58, 41, 33, 27, 23, 20, 17, 15, // QJ
	57, 40, 31, 26, 22, 19, 16, 14,
	56, 38, 29, 23, 19, 16, 14, 12,
	54, 35, 26, 21, 17, 14, 12, 11,
	52, 33, 24, 19, 15, 13, 11, 9,
	51, 32, 23, 18, 14, 12, 10, 9,
	50, 31, 20, 17, 14, 12, 10, 9,
	49, 30, 21, 16, 13, 11, 9, 8,
	48, 29, 21, 16, 13, 11, 9, 8,
	47, 28, 20, 15, 12, 10, 9, 8,

	58, 42, 34, 29, 25, 22, 20, 18, // JTs
	55, 39, 31, 25, 22, 19, 16, 15, // JT
	53, 37, 28, 23, 19, 16, 14, 12,
	52, 34, 26, 20, 17, 14, 12, 11,
	50, 32, 24, 18, 15, 12, 11, 9,
	48, 30, 21, 17, 13, 11, 9, 8,
	47, 29, 21, 16, 13, 11, 9, 8,
	46, 28, 20, 15, 12, 10, 9, 8,
	45, 27, 19, 15, 12, 10, 8, 7,
	44, 26, 18, 14, 11, 9, 8, 7,

	54, 39, 31, 26, 23, 20, 18, 16, // T9s
	52, 32, 28, 23, 19, 16, 14, 13, // T9
	50, 34, 25, 20, 17, 14, 13, 11,
	48, 31, 23, 18, 15, 13, 11, 10,
	46, 29, 21, 17, 13, 11, 10, 8,
	44, 27, 19, 15, 12, 10, 8, 7,
	43, 26, 19, 14, 12, 10, 8, 7,
	42, 26, 18, 14, 11, 9, 8, 7,
	42, 25, 17, 13, 11, 9, 8, 7,

	51, 36, 29, 24, 20, 18, 16, 15, // 98S
	48, 33, 25, 20, 17, 14, 12, 11, // 98
	47, 31, 23, 18, 16, 13, 11, 10,
	45, 29, 21, 17, 14, 11, 10, 9,
	43, 27, 19, 15, 12, 10, 9, 7,
	41, 25, 17, 13, 11, 9, 7, 6,
	40, 24, 17, 13, 10, 8, 7, 6,
	39, 23, 16, 12, 10, 8, 7, 6,

	48, 34, 27, 22, 19, 17, 15, 14, // 87s
	46, 31, 23, 19, 15, 13, 12, 10, // 87
	44, 29, 21, 17, 14, 12, 10, 9,
	42, 27, 19, 15, 12, 11, 9, 8,
	40, 24, 18, 13, 11, 9, 8, 7,
	38, 22, 16, 12, 10, 8, 7, 6,
	37, 22, 15, 11, 9, 8, 6, 6,

	46, 32, 25, 21, 18, 16, 14, 13,  // 76s
	43, 29, 22, 17, 14, 12, 11, 10,	 // 76
	41, 27, 20, 16, 13, 11, 10, 9,
	38, 25, 18, 14, 11, 10, 9, 8,
	37, 22, 16, 12, 10, 8, 7, 6,
	35, 20, 14, 11, 9, 7, 6, 5,

	43, 30, 24, 20, 17, 15, 14, 13, // 65s
	40, 27, 20, 16, 13, 12, 10, 9,  // 65
	38, 25, 18, 14, 12, 10, 9, 8,
	36, 23, 16, 13, 11, 9, 8, 7,
	34, 21, 15, 11, 9, 8, 7, 6,

	41, 29, 23, 19, 17, 15, 14, 13, // 54s
	38, 25, 19, 15, 13, 11, 10, 9,  // 54
	36, 23, 17, 14, 11, 10, 9, 8,
	34, 21, 15, 12, 10, 9, 8, 7,

	38, 26, 20, 17, 15, 13, 12, 11,  // 43s
	34, 22, 16, 13, 11, 9, 8, 8,	 // 43
	33, 21, 15, 12, 10, 8, 7, 7,

	35, 24, 18, 15, 13, 12, 11, 10, // 32s
	31, 20, 14, 11, 9, 8, 7, 6	    // 32

};

char card_to_num(char c)
{
	int cc;
	cc = toupper(c);

	if (cc <= '9' && cc >= '2')
		return cc - '0';
	else
		switch (cc)
	{
		case 'A':
			return 14;
		case 'K':
			return 13;
		case 'Q':
			return 12;
		case 'J':
			return 11;
		case 'T':
			return 10;
		default:
			break;
	}

	return -1;
}

char card_diff(char c1, char c2)
{
	unsigned char _c1 = card_to_num(c1);
	unsigned char _c2 = card_to_num(c2);

	if (_c1 == -1 || _c2 == -1)
		return -1;

	return abs(_c1 - _c2);
}

int card_range(char *t, int opponents, int *range)
{
	size_t len = strlen(t), type, table_ix = 0, table_offset = 0, s_defined = 0;
	char c1, c2, suited;

	if (len != 2 && len != 3 || opponents < 1 || opponents > 8 || !range)
		return 1;

	c1 = t[0], c2 = t[1];

	if (card_to_num(c1) < 2 || card_to_num(c2) < 2)
		return 1;

	if (card_to_num(c2) > card_to_num(c1))
	{
		char tmp = c1;
		c1 = c2;
		c2 = tmp;
	}

	/* Table type (enum) */
	type = (c1 == c2 ? PAIRS : card_diff('A', c1) + 1);

	if (type < PAIRS || type > X3)
		return 1;

	/* Check if suited - PAIRS are never suited */
	suited = ((t[2] == 's' || t[2] == 'S') && (type != PAIRS) ? 1 : 0);

	/* Get base index */
	table_ix = table_start_indexes[type];

	if (type == PAIRS)
		table_ix += card_diff('A', c1);
	else
		table_ix += (card_diff(c1, c2) - 1);

	/* Row -1 is for connected and suited cards */
	if (s_defined = (suited && (card_diff(c1, c2) == 1) ? 1 : 0))
		--table_ix;

	*range = hand_percent[table_ix * OPPONENTS_MAX + opponents - 1];

	/* If the cards are not connected, but suited, if there's 1 opponent add 2%, else 3% */
	if (suited && !s_defined)
	{
		if (opponents == 1)
			*range += 2;
		else
			*range += 3;
	}

	return 0;
}