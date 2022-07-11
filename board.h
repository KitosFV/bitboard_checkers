#pragma once
#include<stdint.h>
#include<vector>
#include<bitset>
#include<ostream>
#include<sstream>
#include<algorithm>
#include<cstring>
#include<queue>
#include<iostream>

#define BORDER_DOWN !(bit2pos(i)>28)
#define BORDER_TOP !(bit2pos(i)<5)
#define BORDER_RIGHT !((bit2pos(i)-1)%8==0)
#define BORDER_LEFT !(bit2pos(i)%8==0)

enum turn_e{WHITE = 0, BLACK};

struct Board{
	uint32_t board[4] = {0x061C71C1,
						 0,
						 0xE1820E38,
						 0};
	turn_e turn = WHITE; 
};

struct Turn{
	std::vector<int> wayPos;
	int attackC;
	Board cb;
};
std::vector<Turn> makeStep(Board b, int bit, Turn turn);
std::vector<std::vector<int>> axises(const Board &f, int bit);
std::ostream& operator << (std::ostream &, const Board &);
std::ostream& operator << (std::ostream &, const Turn &);

int grid2bit(const char *id);
void bit2grid(const int& bit, char *a);
int access(const Board &f, int bit);

int dir2shift(const int& bit, const int& dir, const int& n);
std::vector<Turn> turns(Board f,int);

uint32_t every(Board b);
uint32_t colored(Board b);