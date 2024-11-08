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


// methods
void node::equilibrium()
{
    float uxSqr = ux_ * ux_;
    float uySqr = uy_ * uy_;
    float uxy = ux_ + uy_;
    float uxy_sq = uxy * uxy;
    
    /*E */  feq_[0] = (1./9.) * rho_ * (1 + 3 * ux_ + 4.5 * uxSqr - 1.5 * usqr_);
    /*N */  feq_[1] = (1./9.) * rho_ * (1 + 3 * uy_ + 4.5 * uySqr - 1.5 * usqr_);
    /*W */  feq_[2] = (1./9.) * rho_ * (1 - 3 * ux_ + 4.5 * uxSqr - 1.5 * usqr_);
    /*S */  feq_[3] = (1./9.) * rho_ * (1 - 3 * uy_ + 4.5 * uySqr - 1.5 * usqr_);
    /*NE*/  feq_[4] = (1./36.) * rho_ * (1 + 3 * uxy + 4.5 * uxy_sq - 1.5 * usqr_);
    /*NW*/  feq_[5] = (1./36.) * rho_ * (1 + 3 * (uy_ - ux_) + 4.5 * (uy_ - ux_) * (uy_ - ux_) - 1.5 * usqr_);
    /*SW*/  feq_[6] = (1./36.) * rho_ * (1 + 3 * (-uxy) + 4.5 * uxy_sq - 1.5 * usqr_);
    /*SE*/  feq_[7] = (1./36.) * rho_ * (1 + 3 * (-uy_ + ux_) + 4.5 * (-uy_ + ux_) * (-uy_ + ux_) - 1.5 * usqr_);
    /*C */  feq_[8] = (4./9.) * rho_ * (1 - 1.5 * usqr_);
}


void node::colliding(float tau)
{
    for (int q = 0; q < 9; q++)
    {
        f0_[q] += -(1. / tau) * (f_[q] - feq_[q]);
    }
}


void node::macroscopic()
{
    rho_ = 0;
    for (int q = 0; q < 9; q++)
    {
        rho_ += f_[q];
    }
    ux_ = (f_[0] - f_[2] + f_[4] + f_[7] - f_[5] - f_[6]) / rho_;
    uy_ = (f_[1] - f_[3] + f_[4] + f_[5] - f_[7] - f_[6]) / rho_;
    // update usqr and umag
    usqr_ = ux_ * ux_ + uy_ * uy_;
    umag_ = std::sqrt(usqr_);
    // std::cout << feq_[0] - feq_[2] + feq_[4] + feq_[7] - feq_[5] - feq_[6];
}



