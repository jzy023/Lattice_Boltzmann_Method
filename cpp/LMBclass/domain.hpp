/* -------------- Gneral Info -------------- //

Description:
    This is a 2D lid -driven simlation using 
Lattice Boltzmann Method.

Parameters:




Geometric Setup:

    5(NW)  1(N)  4(NE)
        \   |   /
         \  |  /
    2(W)-- 8(C) --0(E)
         /  |  \
        /   |   \
    6(SW)  3(S)  7(SE)

// ------------------------------------------ */ 

#pragma once


#include "includes.hpp"
#include "node.hpp"


class domain
{
private:

    // constant 2D spacial info
    const std::array<int, 9> oppositeIdxF = 
    {
        2,  // -> 0 (E)
        3,  // -> 1 (N)
        0,  // -> 2 (W)
        1,  // -> 3 (S)
        6,  // -> 4 (NE)
        7,  // -> 5 (NW)
        4,  // -> 6 (SW)
        5,  // -> 7 (SE)
        8   // -> 8 (C)
    };

    const std::array<std::string, 9> nameF = 
    {
        "E",
        "N",
        "W",
        "S",
        "NE",
        "NW",
        "SW",
        "SE",
        "C"
    };

    // helper variables
    int oppositeIdx_;
    int neighborY_;
    int neighborX_;
    bool isNeighborBound_;
    
    // time stepping info
    int itr_;

    // parameters
    int ny_, nx_;
    float ulid_;
    float re_;

    // calculated parameters
    float visc_, tau_;
    float umagMax_;

    //
    std::vector<std::vector<node>> nodes;

    // private methods
    void init();

    void checkBound();

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


    // void printUmag()
    // {
    //     for (int iy = 0; iy < ny_; iy++)
    //     {
    //         // int iy = 3;
    //         for (int ix = 0; ix < nx_; ix++)
    //         {
    //             std::cout << std::setw(8) << std::setprecision(2) << nodes[iy][ix].Umag() << " ";
    //             // std::cout << std::setw(4) << std::setprecision(2) << nodes[iy][ix].F()[0] << " ";
    //         }
    //         std::cout << "\n";
    //     }
    // }
        

    // getters
    const int Nx() { return nx_; }

    const int Ny() { return ny_; }

    const std::vector<std::vector<node>>& Nodes() const { return nodes; }

    // methods

    void printTypes();

    void setBcWall(int yMin, int xMin, int yMax, int xMax);

    void setBcConstVel(int yMin, int xMin, int yMax, int xMax, float ux, float uy);

    void update();



    
};