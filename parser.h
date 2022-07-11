#pragma once
#include"board.h"
#include<string>

Board parse_fen(std::string fen);
void write_fen(const Board &b, const char* name);

int pos2bit(int);
int bit2pos(int);
void b2p_init();