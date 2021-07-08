# miniRT
An accelerated mini raytracer for 42 and the 42 world

The main objective of this work was to implement a whitted style raytracer.  
Regarding the other miniRT you could find on github this work originality and **main contribution** is the implementation of an accelerated tree structure **(BVH)**.  
The rendering should be way faster than any other miniRT (so far)   
# Features
    whitted style raytracing
    Multi-thread
    BVH-acceleration
    super-sampling
    Blin-Phong lighting
    Reflection
    Multiple coloring modes:  0:classic 
                              1:normal coloring 
                              2:checkerbord coloring 3 -> 5: different perlin noise generatade coloring
                              6: sphere image mapping
    Bump mapping
    skyboxing
    camera wasd-Move
    camera orientation mouse clic move
    image save
    ...

# ToDo
  a proper Readme  
  explication about .rt files  
# Image sample

![alt text](https://github.com/LesChats/miniRT/blob/master/images/birds.jpg)
![alt text](https://github.com/LesChats/miniRT/blob/master/images/reflexion.jpg)
![alt text](https://github.com/LesChats/miniRT/blob/master/images/dragon.jpg)
![alt text](https://github.com/LesChats/miniRT/blob/master/images/coumponds.jpg)
![alt text](https://github.com/LesChats/miniRT/blob/master/images/cat.jpg)
![alt text](https://github.com/LesChats/miniRT/blob/master/images/test.jpg)

# Requirements for Linux

    MinilibX only support TrueColor visual type (8,15,16,24 or 32 bits depth)
    gcc
    make
    X11 include files (package xorg)
    XShm extension must be present (package libxext-dev)
    Utility functions from BSD systems - development files (package libbsd-dev)
    e.g. sudo apt-get install gcc make xorg libxext-dev libbsd-dev (Debian/Ubuntu)
    
    more info: https://github.com/42Paris/minilibx-linux
# Warning
    
    The projet is faaaaaar from perfect, you may encounter bugs playing whit it do not copy past ;)
