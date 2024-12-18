#include "enhanced_framework.h"

using namespace std;


class Board3x3 : public Board<char> {
protected:
    char winner;
public:
    Board3x3() : Board<char>(){}
    bool is_valid_move(const int &x, const int &y) override;
    bool is_game_won() override;
    bool is_game_draw() override;
    void declare_winner() override;
};

bool Board3x3::is_valid_move(const int &x, const int &y) {
    return (this->is_move_in_boundaries(x, y) and this->is_cell_available(x, y));
}

bool all_are_x(const vector<char> &v){

    if(v[0] != 'X')
        return false;
    return (v[0] == v[1] and v[1] == v[2]);
}


bool all_are_o(const vector<char> &v){
    if(v[0] != 'O')
        return false;
    return (v[0] == v[1] and v[1] == v[2]);
}

bool Board3x3::is_game_won() {
    int Xcount{0};
    Xcount += this->count_vertical_valids(3, all_are_x);
    Xcount += this->count_horizontal_valids(3, all_are_x);
    Xcount += this->count_diagonal_valids(3, all_are_x);

    int Ocount{0};
    Ocount += this->count_vertical_valids(3, all_are_o);
    Ocount += this->count_horizontal_valids(3, all_are_o);
    Ocount += this->count_diagonal_valids(3, all_are_o);

    if(Xcount > 0){
        winner = 'X';
        return true;
    }
    else if(Ocount > 0){
        winner = 'O';
        return true;
    }
    else{
        return false;
    }

}

bool Board3x3::is_game_draw() {
    return (n_moves == 9);
}

void Board3x3::declare_winner() {
    cout << "Player : " << winner << " has won!" << endl;
}

class move_style3x3 : public move_style<char> {
public:
    move_style3x3(shared_ptr<Board<char>> ptr) : move_style<char>(ptr){}
    void make_move(char symbol) override;
};



void move_style3x3::make_move(char symbol) {
    int x, y;
    cout << "enter a move (x, y): ";
    while(cin >> x >> y){
        if(boardPtr->is_valid_move(x, y)){
            boardPtr->update_board(x, y, symbol);
            return;
        }
        cout << "please enter a valid move\n";
    }

}


int main(void){
    Board3x3 myboard;
    shared_ptr<Board<char>> boardptr(&myboard);

    move_style3x3 human_style(boardptr);
    shared_ptr<move_style<char>> styleptr(&human_style);

    Player<char> player1("islam", 'X', styleptr);
    Player<char> player2("ahmed", 'O', styleptr);

    shared_ptr<Player<char>> player1ptr(&player1);
    shared_ptr<Player<char>> player2ptr(&player2);


    vector<shared_ptr<Player<char>>> players;
    players.push_back(player1ptr);
    players.push_back(player2ptr);

    GameManager<char> mygame(boardptr, players);
    mygame.run();


}


