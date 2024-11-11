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

    for (int j = 0; j < ny_; j++)
    {
        nodes[j].reserve(nx_);

        for (int i = 0; i < nx_; i++)
        {
            nodes[j].emplace_back(j, i);
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
            }
        }
    }
}



void domain::printTypes()
{
    for (int iy = 0; iy < ny_; iy++)
    {
        for (int ix = 0; ix < nx_; ix++)
        {
            if (nodes[iy][ix].Type() == 1)
            {
                std::cout << " ~";
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


void domain::setBcWall
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
                  << ") are out of bound of: ["          << 0    << "," << (ny_-1) << "] for setBcWall()\n";
    }

    if (xMin < 0 || xMax > (nx_-1))
    {        
        std::cout << "WARNING: entrance [xMin, xMax]: (" << xMin << "," << xMax
                  << ") are out of bound of: ["          << 0    << "," << (nx_-1) << "] for setBcWall()\n";
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
            nodes[iy][ix].setNodeWall();
        }
    }
}


void domain::setBcConstVel
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
                  << ") are out of bound of: ["          << 0    << "," << (ny_-1) << "] for setBcConstVel()\n";
    }

    if (xMin < 0 || xMax > (nx_-1))
    {        
        std::cout << "WARNING: entrance [xMin, xMax]: (" << xMin << "," << xMax
                  << ") are out of bound of: ["          << 0    << "," << (nx_-1) << "] for setBcConstVel()\n";
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
            nodes[iy][ix].setNodeVel(ux, uy);
        }
    }
}



void domain::update()
{
    // #pragma omp parallel for collapse(2)
    // #pragma omp for schedule(dynamic)
    for (int iy = 0; iy < ny_; iy++)
    {
        for (int ix = 0; ix < nx_; ix++)
        {
            // - skip bound and voids
            if ((nodes[iy][ix].Type() == -1) ||
                (nodes[iy][ix].Type() == -2))
            {
                continue;
            }


            // - macroscopic
            nodes[iy][ix].macroscopic();


            // - add source term
            if ((iy == ny_-1) && (0 < ix) && (ix < nx_-1))
            {
                nodes[iy][ix].setNodeVel(ulid_, 0.0);

                for (int q = 0; q < 9; q++)
                {   
                    (*nodes[iy][ix].setF0())[q] = nodes[iy][ix].Feq()[q];
                }
            }


            // - calc equilibrium
            nodes[iy][ix].equilibrium();


            // - colliding
            if ( (0 < iy) && (0 < ix) && (iy < ny_-1) && (ix < nx_-1))
            {
                nodes[iy][ix].colliding(tau_);
            }
            // else
            // {
            //     for (int q = 0; q < 9; q++)
            //     {
            //         (*nodes[iy][ix].setF0())[q] = nodes[iy][ix].F()[q];
            //     }
            // }
        }
    }

    // #pragma omp parallel for collapse(2)
    // #pragma omp for schedule(dynamic)
    for (int iy = 0; iy < ny_; iy++)
    {
        for (int ix = 0; ix < nx_; ix++)
        {
            // - streaming
            for (int q = 0; q < 8; q++)
            {
                neighborY_ = nodes[iy][ix].neibors[q][0]; // Y-coord of the neighboring particle on direction q
                neighborX_ = nodes[iy][ix].neibors[q][1]; // X-coord of the neighboring particle on direction q

                // if((neighborY_ != iy || neighborX_ != ix) && nodes[neighborY_][neighborX_].Type() == 1)
                if(nodes[neighborY_][neighborX_].Type() == 1)
                {
                    (*nodes[neighborY_][neighborX_].setF())[q] = nodes[iy][ix].F0()[q];
                }
            }


            // - boundary
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
                    (*nodes[iy][ix].setF())[oppositeIdx_] = nodes[iy][ix].F()[q];
                }   
            }
        }
    }
}



