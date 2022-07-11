#include "parser.h"
#include<regex>
#include<vector>
#include<map>
#include<sstream>
#include<iostream>
#include<fstream>

using namespace std;

const vector<int> bits = {0, 6, 12,18,
						  	 7, 13,19,25,
						  8, 14,20,26,
						     15,21,27,1,
						  16,22,28,2,
						     23,29, 3,9,
						  24,30, 4,10,
						     31,5, 11,17};
map<int,int> b2p;//vector

void b2p_init(){
	for(int i=0;i<32;i++){
		b2p[i+1] = bits[i];
	}
}

int pos2bit(int pos){
	return b2p[pos];//
}

int bit2pos(int bit){
	for (auto it = b2p.begin(); it != b2p.end(); it++)
    	if (it->second == bit){
			return it->first;
			break;
		}
	std::cerr << "AHTUNG bit2pos returned pizdec!" << bit << std::endl;
	return -1;
}

const regex prefix = regex("\\[FEN \\\"([WB]:)");
const regex suffix = regex("\"\\]");

void write_fen(const Board &f, const char* name="./file.txt"){
	stringstream ss;
	ofstream out(name);
	ss << "[FEN \"" << ((f.turn)?"B":"W") << ":W";
	bitset<32> w = bitset<32>(f.board[0]);
	bitset<32> wk = bitset<32>(f.board[1]);
	bitset<32> b = bitset<32>(f.board[2]);
	bitset<32> bk = bitset<32>(f.board[3]);
	for(int i=0;i<32;i++){
		if(w[i])ss<<bit2pos(i)<<",";
		if(wk[i])ss<<"K"<<bit2pos(i)<<",";
	}
	ss << ":B";
	for(int i=0;i<32;i++){
		if(b[i])ss<<bit2pos(i)<<",";
		if(bk[i])ss<<"K"<<bit2pos(i)<<",";
	}
	string s = ss.str();
	s.erase(s.size()-1);
	s += "\"]";
	out << s;
}

Board parse_fen(string fen){
    Board board = {0,0,0,0};

	match_results<string::const_iterator> _turn;
	regex_search(fen,_turn,regex("\"([WB])"));
	board.turn = (_turn.str()[1]=='W')?WHITE:BLACK;

    string _fen = regex_replace(regex_replace(fen, prefix, ""),suffix,"");
    vector<string> side;
    {
	    auto i = _fen.find(":");
	    side = {_fen.substr(0U, i), _fen.substr(i + 1)};
    }
	
	if(side[0][0]!='W')swap(side[0],side[1]);
	
	for(int i=0;i<2;i++){
		side[i] = side[i].substr(1);
		stringstream s(side[i]);
		while(true){
			string op;
			int k = 2*i;
			getline(s,op,',');
			if (op.empty()) break;
			if(op[0]=='K'){
				op = op.substr(1);
				k++;
			}
			int pos = stoi(op);
			board.board[k] += 1 << pos2bit(pos);
		}
	}
    return board;
}