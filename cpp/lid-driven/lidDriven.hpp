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


class lidDriven
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
    float umagMax_;
    float visc_, tau_;
    std::vector<std::vector<float>> ux_;
    std::vector<std::vector<float>> uy_;
    std::vector<std::vector<float>> usqr_;
    std::vector<std::vector<float>> umag_;
    std::vector<std::vector<float>> rho_;
    std::vector<std::vector<std::vector<float>>> f_;
    std::vector<std::vector<std::vector<float>>> f0_;
    std::vector<std::vector<std::vector<float>>> feq_;

    // private methods
    void init();

    void calcTau();

    void equilibrium();

    void boundaryConditions();

    void collideStream();


public:
    // constructors
    lidDriven();

    lidDriven
    (
        int size,
        float uLid,
        float re
    );

    // destructor
    ~lidDriven();
        
    // getters
    const std::vector<std::vector<float>>& getRho(){ return rho_; }

    const std::vector<std::vector<float>>& getUx(){ return ux_; }

    const std::vector<std::vector<float>>& getUy(){ return uy_; }

    const std::vector<std::vector<float>>& getUmag(){ return umag_; }

    const float getUmagMax(){ return umagMax_; }

    const int getNx() { return nx_; }

    const int getNy() { return ny_; }

    // methods
    void macroscopic();

    void update();
    
    void addCursorVel(std::tuple<int, int, float, float>& cursorVel);

    void setUlid(float ulid) { ulid_ = ulid; }

};