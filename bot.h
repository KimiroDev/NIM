#pragma once

#include "variables.h"

short cols()
{
	short nrc = 0;
	for (int i = 1; i <= 5; ++i)
	{
		nrc += (nrCoins[i] > 0);
	}
	return nrc;
}

short sum()
{
	short s = 0;
	for (int i = 1; i <= 5; ++i)
	{
		s += nrCoins[i];
	}
	return s;
}

void play(int& to_pile, int& to_coin)
{
	if (difficulty == 1)
	{
		while (!nrCoins[to_pile])
			to_pile = rand() % 5 + 1;
		to_coin = rand() % nrCoins[to_pile] + 1;
	}
	else if (difficulty == 3)
	{
		int xsum;
		if (cols() == 1)
		{
			for (size_t i = 1; i <= 5; i++)
			{
				if (nrCoins[i] > 0)
					to_pile = i;
				to_coin = 1;
			}
		}
		else
		{
			int xsum = nrCoins[1];
			for (size_t i = 2; i < 6; i++)
			{
				xsum = xsum ^ nrCoins[i];
			}
			if (xsum != 0)
			{
				for (size_t i = 1; i < 6; i++)
				{
					if (((nrCoins[i] ^ xsum) < nrCoins[i]) && nrCoins[i] > 0)
					{
						to_pile = i;
						to_coin = (nrCoins[i] ^ xsum) + 1;
						break;
					}
				}
			}
			else
			{
				while (!nrCoins[to_pile])
					to_pile = rand() % 5 + 1;
				to_coin = rand() % nrCoins[to_pile] + 1;
			}
		}
	}
	else
	{
		if (cols() == 1)
		{
			int i = 1;
			while (!nrCoins[i])
			{
				++i;
			}
			to_pile = i;
			to_coin = nrCoins[i];
		}
		else if (sum() % 2 == 1)
		{
			int i = 1;
			while (nrCoins[i] < 2 && i < 5)
			{
				++i;
			}
			if (nrCoins[i] >= 2)
			{
				to_pile = i;
				to_coin = nrCoins[i] - 1;
			}
			else
			{
				while (!nrCoins[to_pile])
					to_pile = rand() % 5 + 1;
				to_coin = rand() % nrCoins[to_pile] + 1;
			}
		}
		else
		{
			int i = 1;
			while (nrCoins[i] < 1 && i < 5)
			{
				++i;
			}
			to_pile = i;
			to_coin = nrCoins[i];
		}
	}
}

