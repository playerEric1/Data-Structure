#ifndef __BOARD_H_
#define __BOARD_H_
#include <iostream>
#include <string>
#include <cassert>

class Board
{
public:
    //CONSTRUCTORS, taking Player1's token string, Player 2's token string, and the blank string
    Board(std::string _token1, std::string _token2, std::string _blankstr);
    // ACCESSORS
    int numRows() const { return row+1; }
    int numColumns() const { return col+1; }
    int numTokensInRow(const int num) const;
    int numTokensInColumn(const int num) const;

    // MODIFIERS
    std::string insert(const int numcol, const bool player1round);

    //Overload operator
    friend std::ostream &operator<<(std::ostream &output, const Board &D);

    //DESTRUCTOR
    ~Board(){clear();}
    void clear();


private:
    // PRINT HELPER FUNCTIONS Avoid creating too many accessors pass string to ostream
    std::string print() const;

    void growboard(const int numcol);
    bool checkconnect() const;
    // REPRESENTATION
    int row = 4;
    int col = 3; //Minimum row and col numbers
    std::string token1;
    std::string token2;
    std::string blankstr;
    std::string **m_data = new std::string *[4]; //2D array of strings
    int *index = new int[4]{0, 0, 0, 0};         // {4,0,0,0,0} The first digit store row length
};
#endif
