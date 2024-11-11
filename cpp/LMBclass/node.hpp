#pragma once


#include "includes.hpp"


class node 
{
private:
    // constant 2D spacial info
    // const int C = 0;
    // const int E  = 1,  N = 2,  W = 3,  S = 4;
    // const int NE = 5, NW = 6, SW = 7, SE = 8;
    // const int Q = 9;
    
    int type_; // (-2: void; -1: boundary; 1: fluid)
    bool fixVel_;

    // parameters
    
    float ulid_;
    float re_;

    // calculated parameters
    float visc_;
    float tau_;
    float rho_;
    
    float umagMax_;

    float ux_;
    float uy_;
    float usqr_;
    float umag_;

    //                         E      N      W      S      NE      NW      SW      SE      C
    //                         0      1      2      3      4       5       6       7       8         
    std::array<float, 9> f_ = {1./9., 1./9., 1./9., 1./9., 1./36., 1./36., 1./36., 1./36., 4./9.};
    // std::array<float, 9> f_ = {};
    std::array<float, 9> f0_ = f_;
    std::array<float, 9> feq_ = f_;

    // private methods


public:
    // constructors
    node();

    node
    (
        int y,
        int x
    );

    // destructor
    ~node();

    //
    // location
    const int y; 
    const int x;

    std::array<std::array<int, 2>, 8> neibors = {};

    // std::array<int, 2> NE = {};
    // std::array<int, 2> NW = {};
    // std::array<int, 2> E = {};
    // std::array<int, 2> W = {}; 
    // std::array<int, 2> SE = {};
    // std::array<int, 2> SW = {};
        
    // getters
    const float Type() const { return type_; }

    const float Rho() const { return rho_; }

    const float Ux() const { return ux_; }

    const float Uy() const { return uy_; }

    const float Umag() const { return umag_; }

    const float UmagMax() const { return umagMax_; }

    const std::array<float, 9>& F() const { return f_; }

    const std::array<float, 9>& F0() const { return f0_; }

    const std::array<float, 9>& Feq() const { return feq_; }


    // setters
    void setNodeFluid() { type_ = 1; }
    
    void setNodeWall() { type_ = -1; }

    void setNodeVoid() { type_ = -2; }

    void setNodeVel( float ux, float uy) { ux_ = ux; uy_ = uy; }

    std::array<float, 9>* setF() { return &f_; }

    std::array<float, 9>* setF0() { return &f0_; }

    std::array<float, 9>* setFeq() { return &feq_; }


    // methods
    void equilibrium();
    
    void colliding(float tau);

    void macroscopic();
    

};