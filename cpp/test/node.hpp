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


    std::array<float, 9> f_ = {};
    std::array<float, 9> f0_ = {};
    std::array<float, 9> feq_ = {};

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
    const float Type() { return type_; }

    const float Rho() { return rho_; }

    const float Ux() { return ux_; }

    const float Uy() { return uy_; }

    const float Umag() { return umag_; }

    const float UmagMax() { return umagMax_; }

    const std::array<float, 9>& F() { return f_; }

    const std::array<float, 9>& F0() { return f0_; }

    const std::array<float, 9>& Feq() { return feq_; }


    // setters
    void setFluid() { type_ = 1; }
    
    void setWall() { type_ = -1; }

    void setVoid() { type_ = -2; }

    void setVel( float ux, float uy) { ux_ = ux; uy_ = uy; }

    std::array<float, 9>* setF() { return &f_; }

    std::array<float, 9>* setF0() { return &f0_; }

    std::array<float, 9>* setFeq() { return &feq_; }

    // void setF(int i, float fi) { f_[i] = fi; }
    // void setF0(int i, float fi) { f0_[i] = fi; }
    // void setFeq(int i, float fi) { feq_[i] = fi; }


    // methods
    void equilibrium();
    
    void colliding(float tau);

    void macroscopic();
    

    // void update();
    
    // void addCursorVel(std::tuple<int, int, float, float>& cursorVel);
    // // void clear();

    // void setUlid(float ulid) { ulid_ = ulid; }

};