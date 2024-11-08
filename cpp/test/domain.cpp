#include "domain.hpp"


// constructors
domain::domain()
:
    ny_(66),
    nx_(66),
    ulid_(0.1),
    re_(1000.0)
{
    init();
}


domain::domain
(
    int size,
    float uLid,
    float re
):
    ny_(size),
    nx_(size),
    ulid_(uLid),
    re_(re)
{
    init();
}


// destructor
domain::~domain(){}


// private methods
void domain::init()
{
    std::cout << "Initializing system...\n";

    nodes.reserve(ny_);

    itr_ = 0;
    
    // uy_.reserve(ny_);
    // ux_.reserve(ny_);
    // usqr_.reserve(ny_);
    // umag_.reserve(ny_);
    // rho_.reserve(ny_);

    // f_.reserve(ny_);
    // f0_.reserve(ny_);
    // feq_.reserve(ny_);


    // #pragma omp parallel for
    for (int j = 0; j < ny_; j++)
    {
        nodes[j].reserve(nx_);

        // f_[j].reserve(nx_);
        // f0_[j].reserve(nx_);
        // feq_[j].reserve(nx_);

        // rho_[j].assign(nx_, 1.0);
        // uy_[j].reserve(nx_);
        // ux_[j].reserve(nx_);
        // usqr_[j].reserve(nx_);
        // umag_[j].reserve(nx_);

        for (int i = 0; i < nx_; i++)
        {
            nodes[j].emplace_back(j, i);

            // f_[j][i] = {4./9., 1./9., 1./9., 1./9., 1./9., 1./36., 1./36., 1./36., 1./36.};
            // f0_[j][i] = f_[j][i];
            // feq_[j][i].reserve(Q);
        }
    }

    umagMax_ = ulid_;
    visc_ = umagMax_ * (ny_ - 1) / re_;    // <--  needs to be updated with cursor velocity
    tau_ = (6 * visc_ + 1) / 2;         // <--  needs to be updated with FPS
    std::cout << "domain size: " << nx_ <<std::endl;
    std::cout << "viscosity: " << visc_ <<std::endl;
    std::cout << "time constant: " << tau_ <<std::endl;

    checkBound();
}


// void domain::boundaryConditions()
// {
//     // #pragma omp parallel for
//     for (int i = 0; i < ny_; i++)
//     {
//         // Left wall
//         f_[i][0][E] = f_[i][0][W];
//         f_[i][0][NE] = f_[i][0][SW];
//         f_[i][0][SE] = f_[i][0][NW];

//         // Right wall
//         f_[i][nx_-1][W] = f_[i][nx_-1][E];
//         f_[i][nx_-1][NW] = f_[i][nx_-1][SE];
//         f_[i][nx_-1][SW] = f_[i][nx_-1][NE];
//     }

//     // #pragma omp parallel for
//     for (int j = 0; j < nx_; j++)
//     {   
//         // Buttom wall
//         f_[0][j][N] = f_[0][j][S];
//         f_[0][j][NE] = f_[0][j][SW];
//         f_[0][j][NW] = f_[0][j][SE];
//         // Top wall
//         for (int q = 0; q < Q; q++)
//         {
//             f_[ny_-1][j][q] = feq_[ny_-1][j][q];
//         }
//     }
// }


// void domain::collideStream()
// {
//     // Colliding
//     // #pragma omp parallel for collapse(2)
//     for (int i = 1; i < ny_-1; i++)
//     {
//         for (int j = 1; j < nx_-1; j++)
//         {
//             for (int q = 0; q < Q; q++)
//             {
//                 f_[i][j][q] += -(1. / tau_) * (f_[i][j][q] - feq_[i][j][q]);
//             }
//         }
//     }

//     // Streaming
//     f0_.swap(f_);

//     // East propagation
//     // #pragma omp parallel for collapse(2)
//     for (int i = 0; i < ny_; i++)
//     {
//         for (int j = 0; j < nx_-1; j++)
//         {
//             f_[i][j+1][E] = f0_[i][j][E];
//         }
//     }
//     // North propagation
//     // #pragma omp parallel for collapse(2)
//     for (int i = 0; i < ny_-1; i++)
//     {
//         for (int j = 0; j < nx_; j++)
//         {
//             f_[i+1][j][N] = f0_[i][j][N];
//         }      
//     }
//     // West propagation
//     // #pragma omp parallel for collapse(2)
//     for (int i = 0; i < ny_; i++)
//     {
//         for (int j = 0; j < nx_-1; j++)
//         {
//             f_[i][j][W] = f0_[i][j+1][W];
//         }      
//     }
//     // South propagation
//     // #pragma omp parallel for collapse(2)
//     for (int i = 0; i < ny_-1; i++)
//     {
//         for (int j = 0; j < nx_; j++)
//         {
//             f_[i][j][S] = f0_[i+1][j][S];
//         }      
//     }
//     // North East propagation
//     // #pragma omp parallel for collapse(2)
//     for (int i = 0; i < ny_-1; i++)
//     {
//         for (int j = 0; j < nx_-1; j++)
//         {
//             f_[i+1][j+1][NE] = f0_[i][j][NE];
//         }      
//     }
//     // North West propagation
//     // #pragma omp parallel for collapse(2)
//     for (int i = 0; i < ny_-1; i++)
//     {
//         for (int j = 0; j < nx_-1; j++)
//         {
//             f_[i+1][j][NW] = f0_[i][j+1][NW];
//         }      
//     }
//     // South East propagation
//     // #pragma omp parallel for collapse(2)
//     for (int i = 0; i < ny_-1; i++)
//     {
//         for (int j = 0; j < nx_-1; j++)
//         {
//             f_[i][j+1][SE] = f0_[i+1][j][SE];
//         }      
//     }
//     // South West propagation
//     // #pragma omp parallel for collapse(2)
//     for (int i = 0; i < ny_-1; i++)
//     {
//         for (int j = 0; j < nx_-1; j++)
//         {
//             f_[i][j][SW] = f0_[i+1][j+1][SW];
//         }      
//     }
// }



// void domain::update()
// {
//     // std::cout << ">>> time iteration:" << itr_;

//     calcTau();
//     equilibrium();
//     boundaryConditions();
//     collideStream();
    
//     itr_++;
// }


// void domain::addCursorVel
// (
//     std::tuple<int, int, float, float>& cursorVel
// )
// {
//     int x = std::get<0>(cursorVel);
//     int y = std::get<1>(cursorVel);
//     float ux = std::get<2>(cursorVel);
//     float uy = std::get<3>(cursorVel);

//     // std::cout << " x: " << x << " y: " << y << std::endl;

//     x = (x < 2) * 2 + (x >= 2 && x <= nx_-3) * x + (x > nx_-3) * (nx_-3);
//     y = (y < 2) * 2 + (y >= 2 && y <= ny_-3) * y + (y > ny_-3) * (ny_-3);

//     // std::cout << " ux: " << ux_[y][x];
//     // ux_[y][x] += ux;
//     // uy_[y][x] += uy;
//     // std::cout << " ux: " << ux_[y][x] << std::endl;

//     ux_[y+1][x-1] += 1./36. * ux;  ux_[y+1][x] += 1./9. * ux;  ux_[y+1][x+1] += 1./36. * ux;
//     ux_[y][x-1]   += 1./9.  * ux;  ux_[y][x]   += 4./9. * ux;  ux_[y][x+1]   += 1./9.  * ux;
//     ux_[y-1][x-1] += 1./36. * ux;  ux_[y-1][x] += 1./9. * ux;  ux_[y-1][x+1] += 1./36. * ux;
    
//     uy_[y+1][x-1] += 1./36. * uy;  uy_[y+1][x] += 1./9. * uy;  uy_[y+1][x+1] += 1./36. * uy;
//     uy_[y][x-1]   += 1./9.  * uy;  uy_[y][x]   += 4./9. * uy;  uy_[y][x+1]   += 1./9.  * uy;
//     uy_[y-1][x-1] += 1./36. * uy;  uy_[y-1][x] += 1./9. * uy;  uy_[y-1][x+1] += 1./36. * uy;

//     // ux_[y+1][x-1] = 1./36. * ux;  ux_[y+1][x] = 1./9. * ux;  ux_[y+1][x+1] = 1./36. * ux;
//     // ux_[y][x-1]   = 1./9.  * ux;  ux_[y][x]   = 4./9. * ux;  ux_[y][x+1]   = 1./9.  * ux;
//     // ux_[y-1][x-1] = 1./36. * ux;  ux_[y-1][x] = 1./9. * ux;  ux_[y-1][x+1] = 1./36. * ux;
    
//     // uy_[y+1][x-1] = 1./36. * uy;  uy_[y+1][x] = 1./9. * uy;  uy_[y+1][x+1] = 1./36. * uy;
//     // uy_[y][x-1]   = 1./9.  * uy;  uy_[y][x]   = 4./9. * uy;  uy_[y][x+1]   = 1./9.  * uy;
//     // uy_[y-1][x-1] = 1./36. * uy;  uy_[y-1][x] = 1./9. * uy;  uy_[y-1][x+1] = 1./36. * uy;


//     float umagInput = std::sqrt(ux * ux + uy * uy);
//     umagMax_ = (umagInput >= umagMax_) * umagInput + (umagInput < umagMax_) * umagMax_;
//     std::cout << " umagMax: " << umagMax_;
// }



void domain::checkBound()
{
    bool isNeighborOut_;
    // to avoid indexing out of domain, currently only works for rectangular domain
    for (int iy = 0; iy < ny_; iy++)
    {
        for (int ix = 0; ix < nx_; ix++)
        {
            for (int n = 0; n < 8; n++)
            {
                neighborY_ = nodes[iy][ix].neibors[n][0]; 
                neighborX_ = nodes[iy][ix].neibors[n][1]; 

                isNeighborOut_ = (neighborY_ < 0) || (neighborY_ >= ny_) 
                              || (neighborX_ < 0) || (neighborX_ >= nx_);
                              // || (nodes[neighborY_][neighborX_].Type() == -1);
                              // TODO: maybe add check if Type!=fluid to make it more general

                if (isNeighborOut_)
                {
                    nodes[iy][ix].neibors[n][0] = iy;
                    nodes[iy][ix].neibors[n][1] = ix;
                }

                // nodes[iy][ix].neibors[n][0] = (nodes[iy][ix].neibors[n][0] < 0)*0
                //                             + (nodes[iy][ix].neibors[n][0] > (ny_-1))*(ny_-1)
                //                             + ((ny_-1) >= nodes[iy][ix].neibors[n][0] && nodes[iy][ix].neibors[n][0] >= 0)*nodes[iy][ix].neibors[n][0];

                // nodes[iy][ix].neibors[n][1] = (nodes[iy][ix].neibors[n][1] < 0)*0
                //                             + (nodes[iy][ix].neibors[n][1] > (nx_-1))*(nx_-1)
                //                             + ((nx_-1) >= nodes[iy][ix].neibors[n][1] && nodes[iy][ix].neibors[n][1] >= 0)*nodes[iy][ix].neibors[n][1];
            }
        }
    }
}



void domain::printTypes()
{
    // for (int iy = 0; iy < ny_; iy++)
    // {
    //     for (int ix = 0; ix < nx_; ix++)
    //     {
    //         std::cout << "(" << iy << "," << ix << ") ";
    //     }
    //     std::cout << "\n";
    // }
    // std::cout << std::endl;

    for (int iy = 0; iy < ny_; iy++)
    {
        for (int ix = 0; ix < nx_; ix++)
        {
            if (nodes[iy][ix].Type() == 1)
            {
                std::cout << "  ";
            }
            else if(nodes[iy][ix].Type() == -1)
            {
                std::cout << " x";
            }
            else
            {
                std::cout << " o";
            }
              
            // std::cout << nodes[iy][ix].Type() << " ";
            // std::cout << "(" << nodes[iy][ix].y << "," << nodes[iy][ix].x << ") ";
            // std::cout << "(" << nodes[iy][ix].neibors[0][0] << "," << nodes[iy][ix].neibors[0][1] << ") ";
            // std::cout << "(" << nodes[iy][ix].neibors[0][0] << "," << nodes[iy][ix].neibors[0][1] << ") ";
        }
        std::cout << "\n";
    }


}


void domain::setWall
(
    int yMin, 
    int xMin, 
    int yMax, 
    int xMax
)
{
    // check if min and max are legal
    if (yMax < yMin || xMax < xMin)
    {        
        std::cerr << "minus entrance larger than max entrance\n";
        abort();
    }

    if (yMin < 0 || yMax > (ny_-1))
    {        
        std::cout << "WARNING: entrance [yMin, yMax]: (" << yMin << "," << yMax
                  << ") are out of bound of: ["          << 0    << "," << (ny_-1) << "] for setWall()\n";
    }

    if (xMin < 0 || xMax > (nx_-1))
    {        
        std::cout << "WARNING: entrance [xMin, xMax]: (" << xMin << "," << xMax
                  << ") are out of bound of: ["          << 0    << "," << (nx_-1) << "] for setWall()\n";
    }

    // check to avoid out of bound
    yMin = (yMin < 0)*0
         + (yMin > (ny_-1))*(ny_-1)
         + (0 <= yMin && yMin <= (ny_-1))*yMin;

    xMin = (xMin < 0)*0
         + (xMin > (nx_-1))*(nx_-1)
         + (0 <= xMin && xMin <= (nx_-1))*xMin;

    yMax = (yMax < 0)*0
         + (yMax > (ny_-1))*(ny_-1)
         + (0 <= yMax && yMax <= (ny_-1))*yMax;

    xMax = (xMax < 0)*0
         + (xMax > (nx_-1))*(nx_-1)
         + (0 <= xMax && xMax <= (nx_-1))*xMax;

    // std::cout << "yMin: " << yMin << ", xMin: " << xMin << ", "
    //           << "yMax: " << yMax << ", xMax: " << xMax << "\n";

    // set bound
    for (int iy = yMin; iy < yMax+1; iy++)
    {
        for (int ix = xMin; ix < xMax+1; ix++)
        {
            // std::cout << "setting bound at (" << iy << "," << ix << ")\n";
            nodes[iy][ix].setWall();
        }
    }
}


void domain::setConstVel
(
    int yMin, 
    int xMin, 
    int yMax, 
    int xMax, 
    float ux, 
    float uy
)
{
    // check if min and max are legal
    if (yMax < yMin || xMax < xMin)
    {        
        std::cerr << "minus entrance larger than max entrance\n";
        abort();
    }

    if (yMin < 0 || yMax > (ny_-1))
    {        
        std::cout << "WARNING: entrance [yMin, yMax]: (" << yMin << "," << yMax
                  << ") are out of bound of: ["          << 0    << "," << (ny_-1) << "] for setConstVel()\n";
    }

    if (xMin < 0 || xMax > (nx_-1))
    {        
        std::cout << "WARNING: entrance [xMin, xMax]: (" << xMin << "," << xMax
                  << ") are out of bound of: ["          << 0    << "," << (nx_-1) << "] for setConstVel()\n";
    }

    // check to avoid out of bound
    yMin = (yMin < 0)*0
         + (yMin > (ny_-1))*(ny_-1)
         + (0 <= yMin && yMin <= (ny_-1))*yMin;

    xMin = (xMin < 0)*0
         + (xMin > (nx_-1))*(nx_-1)
         + (0 <= xMin && xMin <= (nx_-1))*xMin;

    yMax = (yMax < 0)*0
         + (yMax > (ny_-1))*(ny_-1)
         + (0 <= yMax && yMax <= (ny_-1))*yMax;

    xMax = (xMax < 0)*0
         + (xMax > (nx_-1))*(nx_-1)
         + (0 <= xMax && xMax <= (nx_-1))*xMax;

    // set bound
    for (int iy = yMin; iy < yMax+1; iy++)
    {
        for (int ix = xMin; ix < xMax+1; ix++)
        {
            nodes[iy][ix].setVel(ux, uy);
        }
    }
}



void domain::update()
{
    for (int iy = 0; iy < ny_; iy++)
    {
        for (int ix = 0; ix < nx_; ix++)
        {
            // skip bound and voids
            if ((nodes[iy][ix].Type() == -1) ||
                (nodes[iy][ix].Type() == -2))
            {
                continue;
            }


            // calc equilibrium
            nodes[iy][ix].equilibrium();

            // boundary
            // nodes[iy][ix].forceBoundaries();
            for (int q = 0; q < 8; q++)
            {
                neighborY_ = nodes[iy][ix].neibors[q][0]; // Y-coord of the neighboring particle on direction q
                neighborX_ = nodes[iy][ix].neibors[q][1]; // X-coord of the neighboring particle on direction q
                // in checkBound(), neighbor nodes' coords would set to local coords if they are edge or bound 
                isNeighborBound_ = (nodes[neighborY_][neighborX_].Type() != 1);

                if (isNeighborBound_)
                {
                    oppositeIdx_ = oppositeIdxF[q];
                    (*nodes[neighborY_][neighborX_].setF())[oppositeIdx_] = nodes[neighborY_][neighborX_].F()[q];
                }   
            }

            // add source term
            if ((iy == ny_-1) && (0 < ix) && (ix < nx_-1))
            {
                nodes[iy][ix].setVel(ulid_, 0.0);

                // for (int q = 0; q < 9; q++)
                // {
                //     if (/* condition */)
                //     {
                //         continue;
                //     }
                    
                //     (*nodes[iy][ix].setF())[q] = nodes[iy][ix].Feq()[q];
                // }
            }
            

            // colliding
            if ( (0 < iy) && (0 < ix) && (iy < ny_-1) && (ix < nx_-1))
            {
                nodes[iy][ix].colliding(tau_);
            }

            // streaming
            for (int q = 0; q < 8; q++)
            {
                neighborY_ = nodes[iy][ix].neibors[q][0]; // Y-coord of the neighboring particle on direction q
                neighborX_ = nodes[iy][ix].neibors[q][1]; // X-coord of the neighboring particle on direction q

                if(neighborY_ != iy && neighborX_ != ix && nodes[neighborY_][neighborX_].Type() == 1) {
                    (*nodes[iy][ix].setF())[q] = nodes[neighborY_][neighborX_].F0()[q];
                }
                // std::cout << "(" << neighborY_ << "," << neighborX_ << ") ";
                
                // std::cout << nodes[neighborY_][neighborX_].Type() << " "; 

                // if(nodes[neighborY_][neighborX_].Type() == !1) 
                // {
                //     std::cout << q << " "; 
                // }
                // else
                // {
                //     std::cout << q;
                // }
            }
            // std::cout << ", ";

            // macroscopic
            nodes[iy][ix].macroscopic();

        }
        // std::cout << std::endl;
    }
}



