#ifndef ENHANCED_FRAMEWORK_H
#define ENHANCED_FRAMEWORK_H
//Islam Waleed Salah
//20230062
//S15
//( وَقُلْ رَبِّ زِدْنِي عِلْمًا )


#import <string> //use import instead of include
#import <vector>
#import <iostream>
#import <iomanip>
#import <memory>
#import <limits>

using namespace std;


template <typename T>
class Board {
protected:
    int rows, columns;
    vector<vector<T>> board; //simply using 2D vector would make things much much more easier
    int n_moves = 0;
    virtual bool is_move_in_boundaries(const int &x, const int &y);
    int countsoutheast(int sequence_length, bool (*Is_valid) (const vector<T> &v));
    int countsouthwest(int sequence_length, bool (*Is_valid) (const vector<T> &v));

public:

    Board(int r = 3, int c = 3); //default value for cells is always zero

///    virtual bool update_board(int x, int y, T symbol) = 0; //it's not a good practice for a function to do "more than one thing"
///    the funciton update board simply both validates and updates at the same time
    virtual bool is_valid_move(const int &x, const int &y) = 0;

    virtual bool is_cell_available(const int &x, const int &y){return (board[x][y] == 0);}

    /// removing the noisy comments is generally better :
    virtual void update_board(const int &x, const int &y, const T &symbol);

    virtual void display_board();

    virtual bool is_game_won() = 0; //changed the names a little bit to be more intuitive and meaningful

    virtual bool is_game_draw() = 0;

    virtual void declare_winner(string name1, string name2) = 0;

    ///the following three functions just accumulate the row, column, diagonal of a certain size, and lets the logic done over them to the programmer
    ///as he is going to use a function for that, and give the function pointer as a parametar to the function
    int count_vertical_valids(int sequence_length, bool (*Is_valid) (const vector<T> &v) );
    int count_horizontal_valids(int sequence_length, bool (*Is_valid) (const vector<T> &v) );
    int count_diagonal_valids(int sequence_length, bool (*Is_valid) (const vector<T> &v) );

    T get_cell(const int &x, const int &y) {return board[x][y];}

    int get_n_moves() const {return n_moves;} ///it's good to make getters to avoid vanditta access to the variables

    int get_rows() const {return  rows;}

    int get_columns() const {return columns;}

    virtual ~Board(); //it's always better to make a virtual destructor to avoid memory leak
};

//putting the implementation just down the class made more sense for me during the assignment

template <typename T>
Board<T>::Board(int r, int c){
    rows = r;
    columns = c;
//    board(r, vector<T>(c, 0));
    board = vector<vector<T>>(r, vector<T>(c, 0));
}

template <typename T>
bool Board<T>::is_move_in_boundaries(const int &x, const int &y) {
    bool x_in_boundaries = (x >= 0) and (x < rows);
    bool y_in_boundaries = (y >= 0) and (y < columns);
    return(x_in_boundaries and y_in_boundaries);
}


template <typename T>
void Board<T>::update_board(const int &x, const int &y, const T &symbol) {
    board[x][y] = toupper(symbol);

    bool is_undo_move = (symbol == 0); //because we will want to undo a move in the AI
    if(not is_undo_move)
        n_moves++;
    else
        n_moves--;
}


template <typename T>
void Board<T>::display_board() {
    for (int i = 0; i < this->rows; i++) {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++) {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << this->board[i][j] << " |";
        }
        cout << endl;
        //print a seperation line :
        for (int j = 0; j < 9 * rows; ++j)
            cout << '-';
    }
    cout << endl;
}

template <typename T>
int Board<T>::count_vertical_valids(int sequence_length, bool (*Is_valid) (const vector<T> &v) ) {
    vector<T> line;
    int row_max = this->rows - sequence_length + 1;
    int counter{0};
    for (int i = 0; i < row_max; ++i) {
        for (int j = 0; j < this->columns; ++j) {
            for (int k = 0; k < sequence_length; ++k) {
                line.push_back(this->board[i + k][j]);
            }
            counter += Is_valid(line);
            line.clear();
        }
    }

    return counter;
}
// if you want to check for a wind in a simple 3 * 3 XO game, just use the function and if the answer is bigger than 0, then it's a win

/*
 * X O O X
 * X     X
 * X O O X
 * X O O X
 *  regarding for O only :
    if sequence length is 3, output should be 4
 * */



template <typename T>
int Board<T>::count_horizontal_valids(int sequence_length, bool (*Is_valid) (const vector<T> &v) ) {
    vector<T> line;
    int column_max = this->columns - sequence_length + 1;
    int counter{0};
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < column_max; ++j) {
            for (int k = 0; k < sequence_length; ++k) {
                line.push_back(this->board[i][j + k]);
            }
            counter += Is_valid(line);
            line.clear();
        }
    }

    return counter;
}
/*
 * X O O X
 * X     X
 * X O O O
 *   O O X
 *   regarding for O only :
    if sequence length is 2, output should be 4
 * */


template <typename T>
int Board<T>::count_diagonal_valids(int sequence_length, bool (*Is_valid) (const vector<T> &v) ) {
    if(sequence_length > this->rows or sequence_length > this->columns)
        return 0;

    int counter{0};
    counter += countsoutheast(sequence_length, Is_valid);
    counter += countsouthwest(sequence_length, Is_valid);

    return counter;
}
/*
 * X O O X
 * X X   X
 * O X O O
 *   O O X
 *     X O
    regarding for X only :
    when sequence length is 2, it should return 3
 * */




template <typename T>
int Board<T>::countsoutheast(int sequence_length, bool (*Is_valid) (const vector<T> &v)){
    vector<T> line;
    int row_max = this->rows - sequence_length + 1;
    int column_max = this->columns - sequence_length + 1;
    int counter{0};

    for (int i = 0; i < row_max; ++i) {
        for (int j = 0; j < column_max; ++j) {
            for (int k = 0; k < sequence_length; ++k) {
                line.push_back(this->board[i + k][j + k]);
            }
            counter += Is_valid(line);
            line.clear();
        }
    }

    return counter;

}


template <typename T>
int Board<T>::countsouthwest(int sequence_length, bool (*Is_valid) (const vector<T> &v)){
    vector<T> line;
    int row_max = this->rows - sequence_length + 1;
    int counter{0};

    //count for south-west
    for (int i = 0; i < row_max; ++i) {
        for (int j = sequence_length - 1; j < this->columns; ++j) {
            for (int k = 0; k < sequence_length; ++k) {
                line.push_back(this->board[i + k][j - k]);
            }
            counter += Is_valid(line);
            line.clear();
        }
    }
    return counter;

}


template <typename T>
Board<T>::~Board(){

};





template <typename T>
class move_style { ///it's always better to avoid inheritance as much as possible, so we always use player, but inject the dependency over move_style in the constructor
protected:
    shared_ptr<Board<T>> boardPtr;
public:
    move_style(shared_ptr<Board<T>> bPtr){boardPtr = bPtr;}
    virtual void make_move(T symbol) = 0;
};



template <typename T>
class random_style : public move_style<T>{
public:
    random_style(shared_ptr<Board<T>> bPtr) : move_style<T>(bPtr){
        srand(time(nullptr)); // to make it actually random
    }
    void make_move(T symbol) override;
};



template <typename T>
void random_style<T>::make_move(T symbol) {
    int x = this->boardPtr->get_rows() + 1; //dummy initial values
    int y = this->boardPtr->get_columns() + 1;

    while(not this->boardPtr->is_valid_move(x, y)){
        x = rand() % this->boardPtr->get_rows();
        y = rand() % this->boardPtr->get_columns();
    }

    this->boardPtr->update_board(x, y, symbol);
}


template <typename T>
class AIstyle : public move_style<T>{
public:
    AIstyle(shared_ptr<Board<T>> bPtr) : move_style<T>(bPtr){}
    void make_move(T symbol) override;
    int calculateMinMax(T s, bool isMaximizing);
};


template <typename T>
void AIstyle<T>::make_move(T symbol) {

    // First, check if we can win in the next move
    for (int i = 0; i < this->boardPtr->get_rows(); ++i) {
        for (int j = 0; j < this->boardPtr->get_columns(); ++j) {
            if (this->boardPtr->is_valid_move(i, j)) {
                this->boardPtr->update_board(i, j, symbol);
                if (this->boardPtr->is_game_won()) {
                    return;
                }
                this->boardPtr->update_board(i, j, 0); // Undo move
            }
        }
    }

    T opponentSymbol = (symbol == 'X') ? 'O' : 'X';
    // Second, check if the opponent can win in their next move and block them
    for (int i = 0; i < this->boardPtr->get_rows(); ++i) {
        for (int j = 0; j < this->boardPtr->get_columns(); ++j) {
            if (this->boardPtr->is_valid_move(i, j)) {
                this->boardPtr->update_board(i, j, opponentSymbol);

                if (this->boardPtr->is_game_won()){
                    this->boardPtr->update_board(i, j, 0); //undo the opponent symbol
                    this->boardPtr->update_board(i, j, symbol); //block them
                    return;
                }

                this->boardPtr->update_board(i, j, 0); // Undo move
            }
        }
    }

    int bestValue = std::numeric_limits<int>::min();
    std::pair<int, int> bestMove = {-1, -1};
    // If no immediate win use MinMax to find the best move
    for (int i = 0; i < this->boardPtr->get_rows(); ++i) {
        for (int j = 0; j < this->boardPtr->get_columns(); ++j) {
            if (this->boardPtr->is_valid_move(i, j)) {
                this->boardPtr->update_board(i, j, symbol);
                int moveValue = calculateMinMax(symbol, false);
                this->boardPtr->update_board(i, j, 0); // Undo move
                if (moveValue > bestValue) {
                    bestMove = {i, j};
                    bestValue = moveValue;
                }
            }
        }
    }

    this->boardPtr->update_board(bestMove.first, bestMove.second, symbol);
    return;
}

// Minimax algorithm to evaluate the board
template <typename T>
int AIstyle<T>::calculateMinMax(T s, bool isMaximizing) {
    if (this->boardPtr->is_game_won()) {
        return isMaximizing ? -1 : 1;
    } else if (this->boardPtr->is_game_draw()) {
        return 0;
    }

    int bestValue = isMaximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    T opponentSymbol = (s == 'X') ? 'O' : 'X';

    for (int i = 0; i < this->boardPtr->get_rows(); ++i) {
        for (int j = 0; j < this->boardPtr->get_columns(); ++j) {
            if (this->boardPtr->is_valid_move(i, j)) {
                this->boardPtr->update_board(i, j, s);
                int value = calculateMinMax(opponentSymbol, !isMaximizing);
                this->boardPtr->update_board(i, j, 0); // Undo move

                if (isMaximizing) {
                    bestValue = std::max(bestValue, value);
                }else{
                    bestValue = std::min(bestValue, value);
                }
            }
        }
    }

    return bestValue;
}



template <typename T>
class Player { //player is not responsible for making a valid move
protected:
    string name;
    T symbol;
    shared_ptr<move_style<T>> move_stylePtr;
public:
    /// Two constructors to initiate players
    /// Give the player a symbol to use in playing
    /// It can be X, O, or others
    /// Optionally, you can give them a name
    Player(string n, T symbol, shared_ptr<move_style<T>> mv);
    Player(T symbol, shared_ptr<move_style<T>> mv); // For computer players

    void play(){move_stylePtr->make_move(symbol);}

    T getsymbol() {return symbol;}
    string getname(){return name;}


    ~Player();
};





using namespace std;
// Constructor for Player with a name and symbol
template <typename T>
Player<T>::Player(std::string n, T symbol, shared_ptr<move_style<T>> mv) {
    this->name = n;
    this->symbol = symbol;
    this->move_stylePtr = mv;
}

// Constructor for Player with only a symbol (e.g., for computer players)
template <typename T>
Player<T>::Player(T symbol, shared_ptr<move_style<T>> mv) {
    this->name = "Computer";
    this->symbol = symbol;
    this->move_stylePtr = mv;
}

template <typename T>
Player<T>::~Player(){

}


template <typename T>
class GameManager { //the only responsibillity is to run
private:
    shared_ptr<Board<T>> boardPtr;
    vector<shared_ptr<Player<T>>> players;
public:
    GameManager(shared_ptr<Board<T>> bPtr, vector<shared_ptr<Player<T>>> p);

    void run();

    virtual ~GameManager(){}
};

template <typename T>
GameManager<T>::GameManager(shared_ptr<Board<T>> bPtr, vector<shared_ptr<Player<T>>> p) {
    boardPtr = bPtr;
    players.push_back((shared_ptr<Player<T>>)p[0]);
    players.push_back((shared_ptr<Player<T>>)p[1]);
}

template <typename T>
void GameManager<T>::run() {


    boardPtr->display_board();

    while (true) {
        for (int i : {0, 1}) {
            players[i]->play(); //it's his reponsibillity to make a valid move
            boardPtr->display_board();
            /// the previous version supposed that always if a win happens, the last one to play is the winner, which is not always the case
            if (boardPtr->is_game_won()) {
                boardPtr->declare_winner(players[i]->getname(), players[!i]->getname());
                return;
            }
            if (boardPtr->is_game_draw()) {
                cout << "Draw!\n";
                return;
            }
        }
    }
}


#endif //ENHANCED_FRAMEWORK_H
