/* -------------- Gneral Info -------------- //

Description:
    This is a 2D lid -driven simlation using 
Lattice Boltzmann Method.

Parameters:




Geometric Setup:

    6(NW)  2(N)  5(NE)
        \   |   /
         \  |  /
    3(W)-- 0(C) --1(E)
         /  |  \
        /   |   \
    7(SW)  4(S)  8(SE)

// ------------------------------------------ */ 

#pragma once


#include "includes.hpp"
#include "node.hpp"


class domain
{
private:

    // constant 2D spacial info
    const int C = 0;
    const int E  = 1,  N = 2,  W = 3,  S = 4;
    const int NE = 5, NW = 6, SW = 7, SE = 8;
    const int Q = 9;
    
    // time stepping info
    int itr_;

    // parameters
    int ny_, nx_;
    float ulid_;
    float re_;

    // calculated parameters
    float visc_, tau_;
    float umagMax_;

    // std::vector<std::vector<float>> ux_;
    // std::vector<std::vector<float>> uy_;
    // std::vector<std::vector<float>> usqr_;
    // std::vector<std::vector<float>> umag_;
    // std::vector<std::vector<float>> rho_;

    // std::vector<std::vector<float>> marker_;

    // std::vector<std::vector<std::vector<float>>> f_;
    // std::vector<std::vector<std::vector<float>>> f0_;
    // std::vector<std::vector<std::vector<float>>> feq_;

    // private methods
    void init();

    void calcTau();

    void equilibrium();

    void boundaryConditions();

    void collideStream();


public:
    // constructors
    domain();

    domain
    (
        int size,
        float uLid,
        float re
    );

    // destructor
    ~domain();


    std::vector<std::vector<node>> nodes;

    void checkBound();

    void printTypes();

    void setBound(int yMin, int xMin, int yMax, int xMax);

    void setConstVel(int yMin, int xMin, int yMax, int xMax, float ux, float uy);

    void update();

    // void setF()
    // {
    //     for (int iy = 0; iy < ny_; iy++)
    //     {
    //         for (int ix = 0; ix < nx_; ix++)
    //         {
    //             (*nodes[iy][ix].setF())[0] = 1.0;
    //         }
    //     }
    // }


    // void printF()
    // {
    //     for (int iy = 0; iy < ny_; iy++)
    //     {
    //         for (int ix = 0; ix < nx_; ix++)
    //         {
    //             std::cout << (*nodes[iy][ix].setF())[0] << " ";
    //         }
    //         std::cout << "\n";
    //     }
    // }
        

    // // getters
    // const std::vector<std::vector<float>>& getRho(){ return rho_; }

    // const std::vector<std::vector<float>>& getUx(){ return ux_; }

    // const std::vector<std::vector<float>>& getUy(){ return uy_; }

    // const std::vector<std::vector<float>>& getUmag(){ return umag_; }

    // const float getUmagMax(){ return umagMax_; }

    // const int getNx() { return nx_; }

    // const int getNy() { return ny_; }

    // // methods
    
    // void addCursorVel(std::tuple<int, int, float, float>& cursorVel);
    // // void clear();

    // void setUlid(float ulid) { ulid_ = ulid; }





    
};