#include "node.hpp"


// constructors
node::node()
:
    type_(1),
    fixVel_(false),
    y(0),
    x(0)
{
    // std::cout << "constructing node at x: " << x << ", y: " << y << std::endl;
    // NW = {y+1, x-1}; NE = {y+1, x+1};
    // W =  {y, x-1};   E =  {y, x+1};
    // SW = {y-1, x-1}; SE = {y-1, x+1};

    neibors[0][0] = y;   neibors[0][1] = x+1; // E
    neibors[1][0] = y+1; neibors[1][1] = x;   // N
    neibors[2][0] = y;   neibors[2][1] = x-1; // W
    neibors[3][0] = y-1; neibors[3][1] = x;   // S

    neibors[4][0] = y+1; neibors[4][1] = x+1; // NE
    neibors[5][0] = y+1; neibors[5][1] = x-1; // NW
    neibors[6][0] = y-1; neibors[6][1] = x-1; // SW
    neibors[7][0] = y-1; neibors[7][1] = x+1; // SE
}

node::node
(
    int y, 
    int x
)
:
    type_(1),
    fixVel_(false),
    y(y),
    x(x)
{
    // std::cout << "constructing node at x: " << x << ", y: " << y << std::endl;
    // NW = {y+1, x-1}; NE = {y+1, x+1};
    // W =  {y, x-1};   E =  {y, x+1};
    // SW = {y-1, x-1}; SE = {y-1, x+1};

    neibors[0][0] = y;   neibors[0][1] = x+1; // E
    neibors[1][0] = y+1; neibors[1][1] = x;   // N
    neibors[2][0] = y;   neibors[2][1] = x-1; // W
    neibors[3][0] = y-1; neibors[3][1] = x;   // S

    neibors[4][0] = y+1; neibors[4][1] = x+1; // NE
    neibors[5][0] = y+1; neibors[5][1] = x-1; // NW
    neibors[6][0] = y-1; neibors[6][1] = x-1; // SW
    neibors[7][0] = y-1; neibors[7][1] = x+1; // SE
}



node::~node()
{
    // std::cout << "node destroyed" << std::endl;
}


