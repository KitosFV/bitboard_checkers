#include"board.h"
#include"parser.h"

int dir2shift(const int& bit, const int& dir, const int& n){
    return (bit+n*((dir<2)?1:-1)*((dir%2)?7:1)+n*32)%32;
}

uint32_t every(Board b){
    uint32_t ans = 0;
    for(auto i:b.board)ans |= i;
    return ans;
}

uint32_t colored(Board b){// returns opposition
    return ((b.turn!=BLACK)
    ?b.board[2*BLACK]|b.board[2*BLACK+1]
    :b.board[2*WHITE]|b.board[2*WHITE+1]);
}

int subarr(const std::vector<int> &a, const std::vector<int> &b){
    if(a.size()<b.size())return -1;
    for(int i=0;i<=a.size()-b.size();i++){
        bool eq = true;
        for(int j=i;j<i+b.size();j++){
            if(a.at(j)!=b.at(j-i)){
                eq = false;
                break;
            }
        }
        if(eq)return i;
    }
    return -1;
}

std::ostream& operator << (std::ostream &os, const Board &f){
    std::vector<std::string> b;
    b.resize(33);

    for(int i=0;i<32;i++){
        b[bit2pos(i)] = " ";
        if(f.board[2*WHITE]&(1<<i))
            b[bit2pos(i)] = u8"\u25CF";
            
        if(f.board[2*WHITE+1]&(1<<i))
            b[bit2pos(i)] = u8"\u25BC";

        if(f.board[2*BLACK]&(1<<i))
            b[bit2pos(i)] = u8"\u25CB";

        if(f.board[2*BLACK+1]&(1<<i))
            b[bit2pos(i)] = u8"\u25BD";
    }
    os<<"\u250f\u2501\u2533\u2501\u2533\u2501\u2533\u2501\u2533\u2501\u2533\u2501\u2533\u2501\u2533\u2501\u2513\n";
    for(int i=7;i>=0;i--){
        os << "\u2503";
        if(i%2)os << " \u2503";
        for(int j=i*4+1;j<=(1+i)*4;j++){
            os<<b[j];
            if((i%2==0)||!(j==(1+i)*4)){
                os<<"\u2503 \u2503";
            }else if(j==(1+i)*4)os<<"\u2503";
            
        }
        os << i+1;
        if(i)os<<"\n\u2523\u2501\u254b\u2501\u254b\u2501\u254b\u2501\u254b\u2501\u254b\u2501\u254b\u2501\u254b\u2501\u252b\n";
    }
    os<<"\n\u2517\u2501\u253B\u2501\u253B\u2501\u253B\u2501\u253B\u2501\u253B\u2501\u253B\u2501\u253B\u2501\u251B\n";
    os <<" a b c d e f g h\n";
    
    return os;
}

void bit2grid(const int& bit, char *a){
    std::stringstream ss;
    int pos = bit2pos(bit)-1;
    int row = pos/4+1;
    int col = 2*(pos%4)+(row-1)%2;
    ss << (char)('a'+col) << row;
    ss >> a;
}

int grid2bit(const char *id){
    std::stringstream ss;
    int a,bb;char b;
    ss << id;ss >> b >> a;
    b-='a';
    bb = (int)b/2;
    return pos2bit(4*(a-1)+bb+1);
}

std::ostream& operator << (std::ostream &os, const Turn &b){
    for(auto i:b.wayPos){
        char *a;
        bit2grid(i,a); 
        os << a << " ";
    }
    return os;
}

int access(const Board &f, int bit){
    if((f.board[f.turn*2]|f.board[2*f.turn+1])&(1<<bit))return 2;
    else if(colored(f)&(1<<bit))return 1;
    else return 0;
}

std::vector<std::vector<int>> axises(const Board &f, int bit){
    std::vector<std::vector<int>> ways(4);
    fflush(stdout);
    for(int i=(bit+1)%32;BORDER_TOP&&BORDER_LEFT;i=(i+1)%32)// 0 quart cycle
        ways[0].push_back(access(f,i));
    for(int i=(bit+7)%32;BORDER_TOP&&BORDER_RIGHT;i=(i+7)%32)// 1 quart cycle
        ways[1].push_back(access(f,i));
    for(int i=(bit+31)%32;BORDER_RIGHT&&BORDER_DOWN;i=(i+31)%32)// 2 quart cycle
        ways[2].push_back(access(f,i));
    for(int i=(bit+25)%32;BORDER_LEFT&&BORDER_DOWN;i=(i+25)%32)// 3 quart cycle
        ways[3].push_back(access(f,i));
    fflush(stdout);
    return ways;
}

//done: oneStep makes new board after bitting one figure
Board oneStep(const Board &f, const int &bit, const int &dir, const int &n, const bool &isKing){
    Board new_board = f;
    new_board.board[2*((f.turn+1)%2)] =
     ~(~new_board.board[2*((f.turn+1)%2)] | (1 << dir2shift(bit,dir,n)));
    new_board.board[2*((f.turn+1)%2)+1] =
     ~(~new_board.board[2*((f.turn+1)%2)+1] | (1 << dir2shift(bit,dir,n)));
    new_board.board[2*f.turn+((isKing)?1:0)] ^= (1 << dir2shift(bit,dir,n+1)) | (1 << bit);
    if(bit2pos(dir2shift(bit,dir,n))>28 && f.turn==WHITE){
        new_board.board[WHITE] = ~(~new_board.board[WHITE] | (1 << dir2shift(bit,dir,n+1)));
        new_board.board[WHITE+1] |= (1 << dir2shift(bit,dir,n+1));
    }else if(bit2pos(dir2shift(bit,dir,n))<5 && f.turn==BLACK){
        new_board.board[BLACK] = ~(~new_board.board[BLACK] | (1 << dir2shift(bit,dir,n+1)));
        new_board.board[BLACK+1] |= (1 << dir2shift(bit,dir,n+1));
    }
    return new_board;
}

std::vector<Turn> turns(Board f,int bit){
    std::vector<Turn> t;
    auto axis = axises(f,bit);
    bool isKing = f.board[2*f.turn+1]&(1<<bit);
    //to use links
    if (!isKing){
        for(int i = 0;i<2;i++)// Pustyja poli
            if (axis[2*f.turn+i].size() > 0 && axis[2*f.turn+i][0] == 0){
                Board new_board = f;
                new_board.board[2*f.turn] ^= ((1 << dir2shift(bit,2*f.turn+i,1)) + (1 << bit));
                if(bit2pos(dir2shift(bit,i,1))>28 && f.turn==WHITE){
                    new_board.board[WHITE] = ~(~new_board.board[WHITE] | (1 << dir2shift(bit,i,1)));
                    new_board.board[WHITE+1] |= (1 << dir2shift(bit,i,1));
                }else if(bit2pos(dir2shift(bit,i,1))<5 && f.turn==BLACK){
                     new_board.board[BLACK] = ~(~new_board.board[BLACK] | (1 << dir2shift(bit,i,1)));
                    new_board.board[BLACK+1] |= (1 << dir2shift(bit,i,1));
                }
                t.push_back({{bit,dir2shift(bit,2*f.turn+i,1)},0, new_board});
            } 

        for(int i=0;i<4;i++)
            if (axis[i].size() >= 2 && subarr(axis[i],std::vector<int>({1,0}))==0){
                Board new_board = oneStep(f, bit, i, 1, false);
                auto move = makeStep(new_board,dir2shift(bit,i,2),{{bit,dir2shift(bit,i,2)},1,new_board});
                t.insert(std::end(t), std::begin(move), std::end(move));
            }
    }else{
        for(int i = 0;i<4;i++){//king free cells
            if (axis[i].size()>0){
                for(int j=0;axis[i][j]==0;j++){
                    Board new_board = f;
                    new_board.board[2*f.turn+1]^=(1 << dir2shift(bit,i,j+1)) + (1 << bit);
                    t.push_back({{bit,dir2shift(bit,i,(j+1))},0, new_board});
                }
            } 
        }

        for(int i=0;i<4;i++){//king attacks
            if(auto dir = i; axis[dir].size() >= 2){
                if(int index = subarr(axis[i],std::vector<int>({1,0})); index!=-1){
                    bool eq = true;
                    for(int j = 0;j<index;j++)
                        if(axis[i][j]!=0){
                            eq = false;
                            break;
                    }
                    if(eq){
                        Board new_board = oneStep(f,bit,dir,index+1, true);
                        Turn turn={};
                        turn.wayPos.push_back(bit);
                        turn.wayPos.push_back(dir2shift(bit,i,index+2));
                        turn.attackC=1;
                        turn.cb = new_board;
                        auto move = makeStep(new_board,dir2shift(bit,i,index+2),turn);
                        t.insert(std::end(t), std::begin(move), std::end(move));
                    }
                }
            }
        }
    }
    return t;
}

std::vector<Turn> makeStep(Board b, int bit, Turn turn){ // retrun all recursive attacks
    std::vector<Turn> ret;
    auto axis = axises(b,bit);
    bool isKing = b.board[2*b.turn+1]&(1<<bit);
    
    if (!isKing){ // if not king moves
        for(int i=0;i<4;i++){
            if (axis[i].size() >= 2 && subarr(axis[i],std::vector<int>({1,0}))==0){
                Board new_board = oneStep(b, bit, i, 1, isKing);
                auto new_turn = turn;
                new_turn.wayPos.push_back(dir2shift(bit,i,2));
                new_turn.attackC++;
                new_turn.cb = new_board;

                auto t = makeStep(new_board,dir2shift(bit,i,2),new_turn);
                if (t.size()==1 && t[0].wayPos==new_turn.wayPos){
                    ret.push_back(new_turn);
                }else{ // if no more attacks from this bit available
                    ret.insert(std::end(ret), std::begin(t), std::end(t));
                }
            }
        }
    }else{
        for(int i=0;i<4;i++){
            if(int index = subarr(axis[i],std::vector<int>({1,0})); index!=-1){
                bool eq = true;
                for(int j = 0;j<index;j++)
                    if(axis[i][j]!=0){
                        eq = false;
                        break;
                    }
                if(eq){
                    Board new_board = oneStep(b, bit, i, index+1, isKing);
                    auto new_turn = turn;
                    new_turn.wayPos.push_back(dir2shift(bit,i,index+2));
                    new_turn.attackC++;
                    new_turn.cb = new_board;

                    auto t = makeStep(new_board,dir2shift(bit,i,index+2),new_turn);

                    if (t.size()==1 && t[0].wayPos==new_turn.wayPos){
                        ret.push_back(new_turn);
                    }else{ // if no more attacks from this bit available
                        ret.insert(std::end(ret), std::begin(t), std::end(t));
                    }
                }
            }
        }
    }

    if(ret.size()==0){
        ret.push_back(turn);
    }

    return ret;
}