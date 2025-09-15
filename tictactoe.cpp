#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <tuple>

/*
    This is a script that simulates a tic-tac-toe game between the user and the computer.
    For the first version, the AI will make a random move per turn AKA not be smart.

    For latter versions, the AI wil be smart.

    The purpose of this very simple project is to familiarize myself more with C++.
*/
using namespace std;

class Board
{
private:
    string board[3][3] = {
        {"1", "2", "3"},
        {"4", "5", "6"},
        {"7", "8", "9"}};
    vector<int> possibleMoves = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    tuple<int, int, string> recentMove;
    vector<int>::iterator moveIndex(int move)
    {
        int shift;
        for (int i = 0; i < possibleMoves.size(); i++)
        {
            if (possibleMoves[i] == move)
            {
                shift = i;
                break;
            }
        }
        return possibleMoves.begin() + shift;
    }
    string winningSymbol;
    string boardStatus = "in_progress";

public:
    Board() {}

    void insertMove(int move, string symbol)
    {
        possibleMoves.erase(moveIndex(move));
        int row = move / 3;
        int col;
        if (row == 3)
        {
            row--;
            col = 2;
        }
        else if (move == (3 * row))
        {
            row--;
            col = 2;
        }
        else
        {
            col = move - 3 * row - 1;
        }

        recentMove = make_tuple(row, col, symbol);

        board[row][col] = symbol;

        if (!boardComplete())
        {
            if (hasWinner())
            {
                int row, col;
                string symbol;
                tie(row, col, symbol) = recentMove;
                winningSymbol = symbol;
                boardStatus = "complete";
            }
        }
        else
        {
            if (hasWinner())
            {
                int row, col;
                string symbol;
                tie(row, col, symbol) = recentMove;
                winningSymbol = symbol;
                boardStatus = "complete";
            }
            else
            {
                boardStatus = "Draw";
            }
        }
    }

    void displayBoard()
    {
        for (int i = 0; i < 3; i++)
        {
            cout << "|";
            for (int j = 0; j < 3; j++)
            {
                cout << board[i][j] << "|";
            }
            cout << "\n";
        }
    }

    vector<int> getMoves()
    {
        return possibleMoves;
    }

    bool boardComplete()
    {
        if (static_cast<int>(possibleMoves.size()) == 0)
        {
            return true;
        }
        return false;
    }

    tuple<int, int, string> getRecentMove()
    {
        return recentMove;
    }

    bool hasWinner()
    {
        /*
        By having the recent move, we can check if the win condition is satisfied by checking
        if:
            the same row is filled of the same symbol
            the same col is filled of the same symbol
            the diag is filled of the same symbol
            the anti-diag is filled of the same symbol
        */

        bool winnerExists = true;

        int row, col;
        string symbol;
        tie(row, col, symbol) = recentMove;

        // Row check
        for (int j = 1; j < 3; j++)
        {
            if (board[row][j] != board[row][j - 1])
            {
                winnerExists = false;
            }
        }

        if (winnerExists)
        {
            return winnerExists;
        }

        winnerExists = true;
        // col check
        for (int j = 1; j < 3; j++)
        {
            if (board[j][col] != board[j - 1][col])
            {
                winnerExists = false;
            }
        }

        if (winnerExists)
        {
            return winnerExists;
        }

        winnerExists = true;
        // Diag
        for (int j = 1; j < 3; j++)
        {
            if (board[j][j] != board[j - 1][j - 1])
            {
                winnerExists = false;
            }
        }

        if (winnerExists)
        {
            return winnerExists;
        }

        winnerExists = true;
        // anti-diag
        for (int j = 1; j < 3; j++)
        {
            if (board[j][2 - j] != board[j - 1][2 - j + 1])
            {
                winnerExists = false;
            }
        }

        return winnerExists;
    }

    string getWinner()
    {
        return winningSymbol;
    }

    string getStatus()
    {
        return boardStatus;
    }
};

class AI
{
private:
    string symbol;
    default_random_engine generator;

public:
    AI(string Symbol) : generator(random_device{}()), symbol(Symbol) {}

    void makeMove(Board &board)
    {
        vector<int> possibleMoves = board.getMoves();
        int numMoves = static_cast<int>(possibleMoves.size());
        uniform_int_distribution<int> dist(0, numMoves - 1);
        int selectedMove = possibleMoves[dist(generator)];
        board.insertMove(selectedMove, symbol);
    }
};

class Game
{
private:
    tuple<int, int> scoreboard = make_tuple(0, 0);
    bool gameRunning;
    Board tt_board;
    Board &board = tt_board;
    string player;
    string ai;
    AI computer;

public:
    Game(string PlayerSymbol, string AISymbol) : player(PlayerSymbol), ai(AISymbol), computer(ai), gameRunning(true)
    {
    }

    void setGameEnd()
    {
        gameRunning = false;
    }

    bool isGameRunning()
    {
        return gameRunning;
    }

    void displayScoreBoard()
    {
        cout << "\nYou: " << get<0>(scoreboard) << " Computer: " << get<1>(scoreboard) << "\n";
    }

    bool checkBoard(Board &board)
    {
        string boardStatus = board.getStatus();
        cout << boardStatus << "\n";
        if (boardStatus == "complete")
        {
            setGameEnd();
            string winStatus = board.getWinner();
            if (winStatus == player)
            {
                // Update scoreboard
                get<0>(scoreboard) += 1;
                cout << "\nYou've Won!\n";
            }
            else
            {
                // Update scoreboard
                get<1>(scoreboard) += 1;
                cout << "\nComputer Wins!\n";
            }
            displayScoreBoard();
            return true;
        }
        if (boardStatus == "Draw")
        {
            setGameEnd();
            cout << "\nA Draw!\n";
            displayScoreBoard();
            return true;
        }
        return false;
    }

    void runGame()
    {
        // Board &board = tt_board;
        board.displayBoard();
        // Player moves first
        int move;
        cout << "\nEnter board position: ";
        cin >> move;
        board.insertMove(move, player);
        if (checkBoard(board))
        {
            return;
        }
        computer.makeMove(board);
        if (checkBoard(board))
        {
            board.displayBoard();
            return;
        }
    }

    void startNewGame()
    {
        tt_board = Board();
        board = tt_board;
        gameRunning = true;
    }
};

int main()
{
    // Board tt_board;
    // Board &board = tt_board;
    // AI computer;
    // // bool gameRunning = true;

    string player = "X";
    string computer = "O";

    Game ttt(player, computer);

    bool continueRound = true;
    while (continueRound)
    {
        while (ttt.isGameRunning())
        {
            ttt.runGame();
        }
        cout << "\nDo you want to keep playing? (y/n)";
        string answer;
        cin >> answer;
        cout << "\n";
        if (answer == "n")
        {
            continueRound = false;
            break;
        }
        ttt.startNewGame();
    }

    return 0;
}
