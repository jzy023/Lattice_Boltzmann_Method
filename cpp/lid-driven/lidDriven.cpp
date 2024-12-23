#include "lidDriven.hpp"


// constructors
lidDriven::lidDriven()
:
    ny_(66),
    nx_(66),
    ulid_(0.1),
    re_(1000.0)
{
    init();
}


lidDriven::lidDriven
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
lidDriven::~lidDriven(){}


// private methods
void lidDriven::init()
{
    std::cout << "Initializing system...\n";

    itr_ = 0;
    ux_.resize(ny_);
    uy_.resize(ny_);
    usqr_.resize(ny_);
    umag_.resize(ny_);
    rho_.resize(ny_);

    f_.resize(ny_);
    f0_.resize(ny_);
    feq_.resize(ny_);

    // #pragma omp parallel for
    for (int i = 0; i < ny_; i++)
    {
        ux_[i].resize(nx_);
        uy_[i].resize(nx_);
        usqr_[i].resize(nx_);
        umag_[i].resize(nx_);
        rho_[i].assign(nx_, 1.0);

        f_[i].resize(nx_);
        f0_[i].resize(nx_);
        feq_[i].resize(nx_);

        for (int j = 0; j < nx_; j++)
        {
            f_[i][j] = {4./9., 1./9., 1./9., 1./9., 1./9., 1./36., 1./36., 1./36., 1./36.};
            f0_[i][j] = f_[i][j];
            feq_[i][j].resize(Q);
        }
    }

    umagMax_ = ulid_;
    visc_ = umagMax_ * (ny_ - 1) / re_;    // <--  needs to be updated with cursor velocity
    tau_ = (6 * visc_ + 1) / 2;         // <--  needs to be updated with FPS

    std::cout << "domain size: " << nx_ <<std::endl;
    std::cout << "viscosity: " << visc_ <<std::endl;
    std::cout << "time constant: " << tau_ <<std::endl;
}


void lidDriven::calcTau()
{
    visc_ = umagMax_ * (ny_ - 1) / re_;    // <--  needs to be updated with cursor velocity
    tau_ = (6 * visc_ + 1) / 2;         // <--  needs to be updated with FPS
    // std::cout << std::fixed << std::setprecision(4) << "\t>>> tau: " << tau_;
}


void lidDriven::equilibrium()
{
    // #pragma omp parallel for collapse(2)
    for (int i = 0; i < ny_; i++)
    {
        for (int j = 0; j < nx_; j++)
        {
            float uxSqr = ux_[i][j] * ux_[i][j];
            float uySqr = uy_[i][j] * uy_[i][j];
            float uxy = ux_[i][j] + uy_[i][j];
            float uxy_sq = uxy * uxy;

            float r = rho_[i][j];
            feq_[i][j][C] = (4./9.) * r * (1 - 1.5 * usqr_[i][j]);
            feq_[i][j][E] = (1./9.) * r * (1 + 3 * ux_[i][j] + 4.5 * uxSqr - 1.5 * usqr_[i][j]);
            feq_[i][j][N] = (1./9.) * r * (1 + 3 * uy_[i][j] + 4.5 * uySqr - 1.5 * usqr_[i][j]);
            feq_[i][j][W] = (1./9.) * r * (1 - 3 * ux_[i][j] + 4.5 * uxSqr - 1.5 * usqr_[i][j]);
            feq_[i][j][S] = (1./9.) * r * (1 - 3 * uy_[i][j] + 4.5 * uySqr - 1.5 * usqr_[i][j]);
            feq_[i][j][NE] = (1./36.) * r * (1 + 3 * uxy + 4.5 * uxy_sq - 1.5 * usqr_[i][j]);
            feq_[i][j][NW] = (1./36.) * r * (1 + 3 * (uy_[i][j] - ux_[i][j]) + 4.5 * (uy_[i][j] - ux_[i][j]) * (uy_[i][j] - ux_[i][j]) - 1.5 * usqr_[i][j]);
            feq_[i][j][SW] = (1./36.) * r * (1 + 3 * (-uxy) + 4.5 * uxy_sq - 1.5 * usqr_[i][j]);
            feq_[i][j][SE] = (1./36.) * r * (1 + 3 * (-uy_[i][j] + ux_[i][j]) + 4.5 * (-uy_[i][j] + ux_[i][j]) * (-uy_[i][j] + ux_[i][j]) - 1.5 * usqr_[i][j]);
        }
    }
}


void lidDriven::boundaryConditions()
{
    // #pragma omp parallel for
    for (int i = 0; i < ny_; i++)
    {
        // Left wall
        f_[i][0][E] = f_[i][0][W];
        f_[i][0][NE] = f_[i][0][SW];
        f_[i][0][SE] = f_[i][0][NW];

        // Right wall
        f_[i][nx_-1][W] = f_[i][nx_-1][E];
        f_[i][nx_-1][NW] = f_[i][nx_-1][SE];
        f_[i][nx_-1][SW] = f_[i][nx_-1][NE];
    }

    // #pragma omp parallel for
    for (int j = 0; j < nx_; j++)
    {   
        // Buttom wall
        f_[0][j][N] = f_[0][j][S];
        f_[0][j][NE] = f_[0][j][SW];
        f_[0][j][NW] = f_[0][j][SE];
        // Top wall
        for (int q = 0; q < Q; q++)
        {
            f_[ny_-1][j][q] = feq_[ny_-1][j][q];
        }
    }
}


void lidDriven::collideStream()
{
    // for (int i = 1; i < ny_-1; i++)
    // {
    //     for (int j = 1; j < nx_-1; j++)
    //     {
    //         std::cout << std::setw(8) << std::setprecision(2) << f_[i][j][1] << " ";
    //     }
    //     std::cout << "\n";
    // }
    // std::cout << "\n";

    // Colliding
    for (int i = 1; i < ny_-1; i++)
    {
        for (int j = 1; j < nx_-1; j++)
        {
            for (int q = 0; q < Q; q++)
            {
                f_[i][j][q] += -(1. / tau_) * (f_[i][j][q] - feq_[i][j][q]);
            }
            // std::cout << std::setw(8) << std::setprecision(2) << f0_[i][j][1] << " ";
        }
        // std::cout << "\n";
    }
    // std::cout << "================================================================\n";

    // Streaming
    f0_.swap(f_);

    // for (int i = 1; i < ny_-1; i++)
    // {
    //     for (int j = 1; j < nx_-1; j++)
    //     {
    //         std::cout << std::setw(8) << std::setprecision(2) << f0_[i][j][1] << " ";
    //     }
    //     std::cout << "\n";
    // }
    // std::cout << "\n";

    // East propagation
    for (int i = 0; i < ny_; i++)
    {
        for (int j = 0; j < nx_-1; j++)
        {
            f_[i][j+1][E] = f0_[i][j][E];
        }
    }
    // North propagation
    for (int i = 0; i < ny_-1; i++)
    {
        for (int j = 0; j < nx_; j++)
        {
            f_[i+1][j][N] = f0_[i][j][N];
        }      
    }
    // West propagation
    for (int i = 0; i < ny_; i++)
    {
        for (int j = 0; j < nx_-1; j++)
        {
            f_[i][j][W] = f0_[i][j+1][W];
        }      
    }
    // South propagation
    for (int i = 0; i < ny_-1; i++)
    {
        for (int j = 0; j < nx_; j++)
        {
            f_[i][j][S] = f0_[i+1][j][S];
        }      
    }
    // North East propagation
    for (int i = 0; i < ny_-1; i++)
    {
        for (int j = 0; j < nx_-1; j++)
        {
            f_[i+1][j+1][NE] = f0_[i][j][NE];
        }      
    }
    // North West propagation
    for (int i = 0; i < ny_-1; i++)
    {
        for (int j = 0; j < nx_-1; j++)
        {
            f_[i+1][j][NW] = f0_[i][j+1][NW];
        }      
    }
    // South East propagation
    for (int i = 0; i < ny_-1; i++)
    {
        for (int j = 0; j < nx_-1; j++)
        {
            f_[i][j+1][SE] = f0_[i+1][j][SE];
        }      
    }
    // South West propagation
    for (int i = 0; i < ny_-1; i++)
    {
        for (int j = 0; j < nx_-1; j++)
        {
            f_[i][j][SW] = f0_[i+1][j+1][SW];
        }      
    }
}




// methods
void lidDriven::macroscopic()
{
    umagMax_ = 0.0;
    // umagMax_ = ulid_;
    // umagMax_ = 0.95*umagMax_;
    
    // #pragma omp parallel for collapse(2)
    for (int i = 0; i < ny_; i++)
    {
        for (int j = 0; j < nx_; j++)
        {
            rho_[i][j] = 0;
            for (int q = 0; q < Q; q++)
            {
                rho_[i][j] += f_[i][j][q];
            }
            ux_[i][j] = (f_[i][j][E] - f_[i][j][W] + f_[i][j][NE] + f_[i][j][SE] - f_[i][j][NW] - f_[i][j][SW]) / rho_[i][j];
            uy_[i][j] = (f_[i][j][N] - f_[i][j][S] + f_[i][j][NE] + f_[i][j][NW] - f_[i][j][SE] - f_[i][j][SW]) / rho_[i][j];
            ux_[ny_-1][j] = ulid_;
            uy_[ny_-1][j] = 0;

            usqr_[i][j] = ux_[i][j] * ux_[i][j] + uy_[i][j] * uy_[i][j];
            umag_[i][j] = std::sqrt(usqr_[i][j]);

            // rewrite umagMax for tau calculation
            umagMax_ = (umag_[i][j] >= umagMax_) * umag_[i][j] + (umag_[i][j] < umagMax_) * umagMax_;
        }
    }
}


void lidDriven::update()
{
    calcTau();
    equilibrium();
    boundaryConditions();
    collideStream();
    
    itr_++;
}


void lidDriven::addCursorVel
(
    std::tuple<int, int, float, float>& cursorVel
)
{
    int x = std::get<0>(cursorVel);
    int y = std::get<1>(cursorVel);
    float ux = std::get<2>(cursorVel);
    float uy = std::get<3>(cursorVel);

    x = (x < 2) * 2 + (x >= 2 && x <= nx_-3) * x + (x > nx_-3) * (nx_-3);
    y = (y < 2) * 2 + (y >= 2 && y <= ny_-3) * y + (y > ny_-3) * (ny_-3);

    ux_[y+1][x-1] += 1./36. * ux;  ux_[y+1][x] += 1./9. * ux;  ux_[y+1][x+1] += 1./36. * ux;
    ux_[y][x-1]   += 1./9.  * ux;  ux_[y][x]   += 4./9. * ux;  ux_[y][x+1]   += 1./9.  * ux;
    ux_[y-1][x-1] += 1./36. * ux;  ux_[y-1][x] += 1./9. * ux;  ux_[y-1][x+1] += 1./36. * ux;
    
    uy_[y+1][x-1] += 1./36. * uy;  uy_[y+1][x] += 1./9. * uy;  uy_[y+1][x+1] += 1./36. * uy;
    uy_[y][x-1]   += 1./9.  * uy;  uy_[y][x]   += 4./9. * uy;  uy_[y][x+1]   += 1./9.  * uy;
    uy_[y-1][x-1] += 1./36. * uy;  uy_[y-1][x] += 1./9. * uy;  uy_[y-1][x+1] += 1./36. * uy;

    float umagInput = std::sqrt(ux * ux + uy * uy);
    umagMax_ = (umagInput >= umagMax_) * umagInput + (umagInput < umagMax_) * umagMax_;
    // std::cout << " umagMax: " << umagMax_;
}


void lidDriven::mkdirDataOutput
(std::string dataOutputDir)
{
    dataOutputDir_ = dataOutputDir;
    // make a dir with drwxr-xr-x
    mkdir(dataOutputDir_.c_str(), 0755);
}


void lidDriven::wrtieDataCSV(int sampleFreq)
{
    std::stringstream filenameStream;
    filenameStream << dataOutputDir_ << "/"
                   << std::setfill('0') << std::setw(4) // Width of 4, fill with '0'
                   << (itr_ / sampleFreq) << ".csv";

    std::string filename_ = filenameStream.str();



    // Open the file with the formatted filename
    std::ofstream log;
    log.open(filename_, std::ios::out);
    log << "x,y,ux,uy,umag\n";

    for (int y = 0; y < ny_; y++)
        for (int x = 0; x < nx_; x++) {
        {
            log << std::setprecision(15) <<
                   x << ","    << 
                   y << ","    << "0," <<
                   ux_[y][x]   << "," <<
                   uy_[y][x]   << "," <<
                   umag_[y][x] << "\n";
        }
    }

    log.close();
}
