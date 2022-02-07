#include <iostream>
#include <string>
#include <cassert>
#include "Board.h"

// the following digits store col length
Board::Board(std::string _token1, std::string _token2, std::string _blankstr)
// cannot take reference here because input may be explicit string
{
    token1 = _token1;
    token2 = _token2;
    blankstr = _blankstr;
}

void Board::clear()
{

    for (int i = 0; i < col + 1; i++)
    {
        if (numTokensInColumn(i) != 0)
        {
            delete[] m_data[i];
        }
    }
    delete[] m_data;
    delete[] index;
}

int Board::numTokensInRow(const int num) const
{
    if (num > row || num < 0)
    {
        return -1; // return -1 if out of border
    }
    // look through index of tokens of each column
    int sum = 0;
    for (int i = 0; i < col; i++)
    {
        if (numTokensInColumn(i) > num)
        {
            sum++;
        }
    }
    return sum;
}

/*numTokensInColumn() also takes a single int, the column number, and returns how many tokens are in that
column (or -1 if it doesn't exist).
*/
int Board::numTokensInColumn(const int num) const
{
    if (num > col || num < 0)
    {
        return -1; // return -1 if out of border
    }
    return index[num];
}

/*place the token and then check if
there is any connected four on the board. It should return a string which is the blank spaces string (. in our
example) if there is no connected four, otherwise it should return the player's token string corresponding to
which player won (R or Y in our example).*/
std::string Board::insert(const int numcol, const bool player1round)
{
    std::string pushback = token2;
    if (player1round)
    {
        pushback = token1; // decide which player's token to store
    }
    growboard(numcol);
    // add 1 to index of column and create new temperary pointer to store it
    std::string *tmp = new std::string[numTokensInColumn(numcol) + 1];

    // copy the content of the pointer if it is not empty
    if (numTokensInColumn(numcol) != 0)
    {
        for (int i = 0; i < numTokensInColumn(numcol); i++)
        {
            tmp[i] = m_data[numcol][i];
        }
        delete[] m_data[numcol];
    }
    m_data[numcol] = tmp;
    m_data[numcol][numTokensInColumn(numcol)] = pushback;
    ++index[numcol];
    if (checkconnect())
    {
        return pushback;
    }
    return blankstr;
}

void Board::growboard(const int numcol)
{
    if (numcol <= col && numTokensInColumn(numcol) > row) // if a token is placed in a column that is already
    // full", the board will simply grow larger, allowing the new token to be placed at the top of the column
    {
        row++;
    }
    if (numcol > col) // if a player tries to place a token in a positive column that
    // doesn't exist yet, the board will grow to the right (column 0 is still the left-most column) to fit
    {

        int *tmp = new int[numcol + 1];
        // copy the whole pointer
        for (int i = 0; i < col + 1; i++)
        {
            tmp[i] = index[i];
        }
        for (int i = col + 1; i < numcol; i++)
        {
            tmp[i] = 0;
        }
        col = numcol;
        delete[] index;
        index = tmp;
        /*
        std::string **tmp2 = new std::string*[numTokensInColumn(numcol) + 1];
            for (int i = 0; i < col+1; i++)
            {
                tmp2[i] = m_data[i];
            }
        
                for (int i = col + 1; i < numcol; i++)
        {
            tmp2[i] = new std::string[1];
    
    }
    delete[] m_data;
    m_data = tmp2;*/
}
}

bool Board::checkconnect() const
{
    // check vertical four
    for (int j = 0; j < col; j++)
    {
        if (numTokensInColumn(j) >= 4)
        { // only look at column with >=4 tokens
            for (int i = 0; i < numTokensInColumn(j) - 3; i++)
            {
                if (m_data[j][i] == m_data[j][i + 1] &&
                    m_data[j][i + 1] == m_data[j][i + 2] &&
                    m_data[j][i + 2] == m_data[j][i + 3])
                {
                    return true;
                }
            }
        }
    }
    // check horizontal four
    for (int i = 0; i < row; i++)
    {
        if (numTokensInRow(i) >= 4)
        { // only look at row with >=4 tokens
            for (int j = 0; j < col - 3; j++)
            {
                if (numTokensInColumn(j) > i &&
                    numTokensInColumn(j + 1) > i &&
                    numTokensInColumn(j + 2) > i &&
                    numTokensInColumn(j + 3) > i &&
                    m_data[j][i] == m_data[j + 1][i] &&
                    m_data[j + 1][i] == m_data[j + 2][i] &&
                    m_data[j + 2][i] == m_data[j + 3][i])
                {
                    return true;
                }
            }
        }
    }
    return false;
}

std::string Board::print() const
{
    std::string out; // string to be returned
    // use a nested loop to print the whole table
    // note that index of i is upsidedown
    for (int i = row; i >= 0; i--)
    {
        for (int j = 0; j <= col; j++)
        {
            // std::cout<<numTokensInRow(i) <<"  "<< numTokensInColumn(j)<<std::endl;
            if (i < numTokensInColumn(j)) // print token if there is one
            {
                out += m_data[j][i] + " "; // m_data points to column first
            }
            else // otherwise print blank string
            {
                out += blankstr + " ";
            }
        }
        out += "\n";
    }
    return out;
}

// Overload operator
std::ostream &operator<<(std::ostream &output, const Board &D)
{
    output << D.print();
    return output;
}