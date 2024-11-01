# Project Title

This is a project about Lattice Boltzmann Method. It contains serveral cases built in C++ and Python.


## Table of Contents
- [Installation](#installation)
- [Contents](#content)
- [Usage](#usage)
<!-- - [Features](#features)
- [Contributing](#contributing)
- [License](#license) -->


## Installation

1. Clone the repo
    ```bash
    git clone git@github.com:jzy023/Lattice_Boltzmann_Method.git
   ```

2. Dependency packages

    Update and upgrade Linux/Ubuntu
    ```bash
    sudo apt update
    sudo apt upgrade
    ```

    C++ compiler installation
    ```bash
    sudo apt install g++
    ```

    C++ visualization tools (SDL2, GL)
    ```bash
    sudo apt install libsdl2-dev 
    sudo apt install libgl1-mesa-dev
    ```


    <!-- Python Pip Installs Packages
    ```bash
    sudo apt install python-pip
    sudo apt install python3-pip
    ``` -->


## Contents

C++ programes:
    
1. 2D Lid-Driven Cavity 

![lidDrivenGif](https://raw.githubusercontent.com/jzy023/gifs/main/LBM/lidDrivenLBM.gif)

2. 2D Interactive Domain


Python programes:


## Usage

### C++ programs 

Go to the case folder and compile the executable using 
```bash
make all
```
All builts can be cleaned using 
```bash 
make clean
```
The Makefile contains several build options for both debug and release mode. One can choose the build option that meets their intent. For release mode
there are a few more options like:
```
USE_FAST_MATH -> -ffast-math
```
and 
```
USE_NATIVE_CPU_OPTIMIZATIONS -> -march=native
```
One could run into compiling error if the libraries of SDL2 and GL is not linked correctly. If the SDL2 and GL libraries are not located at the standard locations,
please try adding the following to the include path falgs and the library path flags, where the user have to specifiy where the SDL2 and GL packages are located.
```bash
-I/path/to/SDL2/include 
-I/path/to/OpenGL/include

-L/path/to/SDL2/lib 
-L/path/to/OpenGL/lib
```



