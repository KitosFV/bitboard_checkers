#include<fstream>
#include<iostream>
#include<stdio.h>
#include"board.h"
#include"parser.h"
using namespace std;

void play();
void fileW();
void printFileMenu();
void printPlayMenu();
Board board;

int main(){
	b2p_init();
	system("clear");

	//cout << board;

	printf("Enter file name: ");
	char *name;
	scanf("%[^\n]", name);
	FILE* f = fopen(name, "r");
	if (!f){
        f = fopen(name, "w");
		fclose(f);
		write_fen(board, name);
    }else{
		fclose(f);
	}///ifdef


	ifstream in(name);
	string fen;
	getline(in, fen); 

	board = parse_fen(fen);

	fileW();

	printf("Good bye\n");
	return 0;
}

void fileW(){
	bool ext = false;
	while(true){
		if(ext)break;
		system("clear");// cls or clear checking
		cout << board;
		cout << endl;
		printFileMenu();

		int cmd;
		cin >> cmd;
		system("clear");
		switch(cmd){
			case 0:
				printf("Enter new file name: ");
				char name[256];
				fflush(stdin);
				// getch();
				cin >> name;
				// printf("%s", name);
				write_fen(board, name);
				ext = true;
				printf("Thanks\n");
				exit(0);
				break;
			case 1:
				ext = true;
				printf("Thanks\n");
				exit(0);
				break;
			case 2:
				play();
				break;
			default: 
				printf("Wrong command\n");
				fflush(stdin);
				cin.get();
				break;
		}
	}
}

void play(){
	bool ext = false;
	while(true){
		if(ext)break;
		system("clear");
		cout << board;
		printPlayMenu();

		int cmd;
		fflush(stdin);
		scanf("%d", &cmd);
		ext = false;
		vector<Turn> t; 
		switch(cmd){
			case(0):
				fileW();
				break;
			case(1): 
				printf("Enter checker id: ");
				char id[2];
				cin >> id;
				t = turns(board,grid2bit(id));
				while(access(board,grid2bit(id))!=2||t.size()==0){
					printf("Failed to access or no steps\nEnter checker id: ");
					cin >> id;
					t = turns(board,grid2bit(id));
				}

				int mov;

				printf("Available moves:\n");

				for(int i=0;i<t.size();i++){
					cout << i+1 << ". ";
					for(auto j:t[i].wayPos){
						char a[2];
						bit2grid(j, a);
						cout << a << " ";
					}
					cout << endl;
				}
						
				printf("Enter number of move: ");
				while(true){
					cin >> mov;
					if(mov-1<t.size()&&mov>0){
						board = t[mov-1].cb;
						board.turn = (turn_e)((board.turn+1)%2);
						break;
					}
				}
				
				fflush(stdin);
				cin.get();
				break;
			default:
				printf("Wrong command\n");
				fflush(stdin);
				cin.get();
		}
	}
}

void printFileMenu(){
	printf("0. Save as\n");
	printf("1. Exit\n");
	printf("2. Play\n");
}

void printPlayMenu(){
	printf("Currently %s\n",(board.turn)?"Blacks":"Whites");
	printf("0. Exit to file menu\n");
	printf("1. Choose checker\n");
}