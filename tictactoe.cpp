#include <iostream>
#include <string>
#include "ctype.h"
#include <vector>
#include <random>
#include <tuple>

/*
    This is a script that simulates a tic-tac-toe game between the user and the computer.
    This current version uses minimax algorithim to select the best move possible, always resulting in a draw or losses.
*/
using namespace std;

class Board
{

public:
    using Grid = vector<vector<char>>;
    using GridRow = vector<char>;

    Board()
    {
        getMoves();
    }

    void displayBoard()
    {
        for (int i = 0; i < 3; i++)
        {
            cout << "|";
            for (int j = 0; j < 3; j++)
            {
                cout << ticTacToeBoard[i][j] << "|";
            }
            cout << "\n";
        }
    }

    GridRow getMoves()
    {
        GridRow positions = {};
        for (GridRow row : ticTacToeBoard)
        {
            for (char val : row)
            {
                if (isdigit(val))
                {
                    positions.push_back(val);
                }
            }
        }
        return positions;
    }

    void insertMove(char move, char piece)
    {
        for (int i = 0; i < ticTacToeBoard.size(); i++)
        {
            for (int j = 0; j < ticTacToeBoard[i].size(); j++)
            {
                if (ticTacToeBoard[i][j] == move)
                {
                    ticTacToeBoard[i][j] = piece;
                    return;
                }
            }
        }
    }

    bool winnerExists()
    {
        // Check horizontal
        for (GridRow row : ticTacToeBoard)
        {
            char val = row[0];
            bool rowUniform = true;
            for (int i = 1; i < row.size(); i++)
            {
                if (row[i] != val)
                {
                    rowUniform = false;
                }
            }
            if (rowUniform)
            {
                return true;
            }
        }

        // Check Vertical
        for (int j = 0; j < ticTacToeBoard[0].size(); j++)
        {
            char val = ticTacToeBoard[0][j];
            bool colUniform = true;
            for (int i = 1; i < ticTacToeBoard.size(); i++)
            {
                if (ticTacToeBoard[i][j] != val)
                {
                    colUniform = false;
                }
            }
            if (colUniform)
            {
                return true;
            }
        }

        // Check Diagonal
        bool diagUniform = true;
        for (int i = 1; i < ticTacToeBoard.size(); i++)
        {
            if (ticTacToeBoard[i][i] != ticTacToeBoard[0][0])
            {
                diagUniform = false;
            }
        }
        if (diagUniform)
        {
            return true;
        }

        // Check Anti-Diagonal
        bool antiDiagUniform = true;
        for (int i = 1; i < ticTacToeBoard.size(); i++)
        {
            int j = ticTacToeBoard[i].size() - 1 - i;
            if (ticTacToeBoard[i][j] != ticTacToeBoard[0][ticTacToeBoard[0].size() - 1])
            {
                antiDiagUniform = false;
            }
        }
        if (antiDiagUniform)
        {
            return true;
        }

        return false;
    }

    bool boardFull()
    {
        for (GridRow row : ticTacToeBoard)
        {
            for (char val : row)
            {
                if (isdigit(val))
                {
                    return false;
                }
            }
        }
        return true;
    }

    void clear()
    {
        ticTacToeBoard = {{'1', '2', '3'},
                          {'4', '5', '6'},
                          {'7', '8', '9'}};
    }

private:
    Grid ticTacToeBoard = {{'1', '2', '3'},
                           {'4', '5', '6'},
                           {'7', '8', '9'}};
};

class Player
{
private:
    int score;

    tuple<int, char> minimax(Board &state, bool maximizing_player, char move)
    {
        // Check if terminal and respond accordingly
        if (state.winnerExists())
        {
            if (!maximizing_player)
            {
                return make_tuple(1, move);
            }
            else
            {
                return make_tuple(-1, move);
            }
        }

        if (state.boardFull())
        {
            return make_tuple(0, move);
        }

        vector<char> possibleMoves = state.getMoves();

        if (maximizing_player)
        {
            int max_val = -2;
            char bestMove = '0';
            for (char move : possibleMoves)
            {
                Board newState = state;
                newState.insertMove(move, 'X');
                int state_score;
                char chosen_move;
                tie(state_score, chosen_move) = minimax(newState, !maximizing_player, move);
                if (state_score > max_val)
                {
                    max_val = state_score;
                    bestMove = move;
                }
            }
            return make_tuple(max_val, bestMove);
        }
        else
        {
            int min_val = 2;
            char bestMove = '0';
            for (char move : possibleMoves)
            {
                Board newState = state;
                newState.insertMove(move, 'O');
                int state_score;
                char chosen_move;
                tie(state_score, chosen_move) = minimax(newState, !maximizing_player, move);
                if (state_score < min_val)
                {
                    min_val = state_score;
                    bestMove = move;
                }
            }
            return make_tuple(min_val, bestMove);
        }
    }

public:
    Player(char symbol, Board &gameBoard) : piece(symbol), gameBoard(gameBoard), score(0) {}

    int getScore()
    {
        return score;
    }

    void updateScore()
    {
        score++;
    }

    virtual void makeMove()
    {
        int state_score;
        char chosen_move;

        tie(state_score, chosen_move) = minimax(gameBoard, false, '0');
        gameBoard.insertMove(chosen_move, piece);

        // vector<char> availableMoves = gameBoard.getMoves();
        // random_device rd;
        // mt19937 gen(rd());
        // uniform_int_distribution<> distrib(0, availableMoves.size());

        // int randomPosition = distrib(gen);
        // gameBoard.insertMove(availableMoves[randomPosition], piece);
    }

protected:
    char piece;
    Board &gameBoard;
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(char symbol, Board &gameBoard) : Player(symbol, gameBoard) {}

    void makeMove()
    {
        cout << "Please Enter a Board Position: ";
        char pos;
        cin >> pos;
        gameBoard.insertMove(pos, piece);
    }
};

int main()
{

    bool hasGameEnded = false;
    Board gameBoard;
    Board &test = gameBoard;
    HumanPlayer user('X', gameBoard);
    Player computer('O', gameBoard);

    while (!hasGameEnded)
    {
        char answer;
        bool roundOver = false;
        gameBoard.displayBoard();
        user.makeMove();
        if (gameBoard.winnerExists())
        {
            roundOver = true;
            // User Won!
            user.updateScore();
            cout << "You Won!!!" << endl;
        }
        else if (gameBoard.boardFull())
        {
            roundOver = true;
            // A draw
            cout << "A draw!" << endl;
        }
        else
        {
            computer.makeMove();
            if (gameBoard.winnerExists())
            {
                roundOver = true;
                // Computer Won!
                computer.updateScore();
                cout << "AI wins!" << endl;
            }
            else if (gameBoard.boardFull())
            {
                roundOver = true;
                // A draw
                cout << "A draw!" << endl;
            }
            else
            {
                continue;
            }
        }
        cout << "ScoreBoard\nYou: " << user.getScore() << " AI: " << computer.getScore() << endl;
        gameBoard.displayBoard();
        cout << "Would You like to continue playing?: ";
        cin >> answer;
        if (answer != 'y')
        {
            hasGameEnded = true;
        }
        else
        {
            gameBoard.clear();
        }
    }

    return 0;
}
