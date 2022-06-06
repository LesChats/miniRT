# miniRT
An accelerated mini raytracer for 42 and the 42 world

The main objective of this work was to implement a whitted style raytracer.  
Regarding the other miniRT that you could find on github this work originality and **main contribution** is the implementation of an accelerated three structure **(namely :BVH)**.  
Thus the rendering should be way faster than any other miniRT (so far)   
# Features
    whitted style raytracing
    Multi-thread
    BVH-acceleration
    super-sampling
    Blin-Phong lighting
    Reflection
    Multiple coloring modes:  0:classic 
                              1:normal coloring 
                              2:checkerbord coloring 
                              3 -> 5: different perlin noise generated coloring
                              6: sphere image mapping
    Bump mapping
    skyboxing
    camera wasd-Move
    camera orientation mouse clic move
    image save
    ...

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
  
# usage
    ./miniRT (.rt file) [skymap directory] [--save]
    exemples:
        ./miniRT scenes/reflexion.rt skymaps/creek/
        -- will display the scene reflection.rt whiht the skymap creek
      
        ./miniRT scenes/dragon2.rt skymaps/darkness/ --save
        -- will display the scene dragon2.rt whiht the skymap darkness and save the result to image

        ./miniRT scenes/onlycat.rt
        -- whill display the scene onlycat.rt

# .rt file basic configutation
    * Resolution:
        R 1920 1080
            - identifier: R
            - x render size
            - r render size
    * Ambient lightning:
        A 0.2 255,255,255
            - identifier: A
            - ambient lighting ratio in range [0.0,1.0]: 0.2
            - R,G,B colors in range 0-255: 255, 255, 255
    * Camera:
        c  -50.0,0,20  0,0,1  70
            - identifier: c
            - 3d normalized orientation vector. In range -1,1 for each x,y,z axis: 0.0,0.0,1.0
            - FOV : Horizontal field of view in degrees in range [0,180
    * Ligth:
        l  -40.0,50.0,0.0 0.6  10.0.255
            - identifier: l
            - ×,y,z coordinates of the light point: 0.0,0.0,20.6
            - the light brightness ratio in range 0.0,1.0: 0.6
            - R,G,B colors in range [0-255]: 10, 0, 255
    * Sphere:
        sp  0.0,0.0,20.6  12.6  10,0.255
            - identifier: sp
            - ×,y,z coordinates of the sphere center: 0.0,0.0,20.6
            - the sphere diameter: 12.6
            - R,G,B colors in range [0-255]: 10, 0, 255
    * Plane:
        pl  0.0,0.0,-10.0  0.0,1.0,0.0  0,0,225
            - identifier: pl
            - x,y,z coordinates: 0.0,0.0,-10.0
            - 3d normalized orientation vector. In range [-1,1] for each x,y,z axis: 0.0,0.0,1.0
            - R,G,B colors in range [0-255]: 0, 0, 255
    * Square:
        sq  0.0,0.0,20.6  1.0,0.0,0.0  12.6  255,0,255
            - identifier: sq
            - x,y,z coordinates of the square center: 0.0,0.0,20.6
            - 3d normalized orientation vector. In range [-1,1] for each x,y,z axis: 1.0,0.0,0.0
            - side size: 12.6
            - R,G,B colors in range [0-255]: 255, 0, 255
    * Cylinder:
        cy 50.0,0.0,20.6  0.0,0.0,1.0  10,0,255  14.2 21.42
            - identifier: cy
            - x,y,z coordinates: 50.0,0.0,20.6
            - 3d normalized orientation vector. In range (-1,1] for each x,y,z axis: 0.0,0.0,1.0
            - the cylinder diameter: 14.2
            - the cylinder height: 21.42
            - R.G.B colors in range [0,255]: 10, 0, 255
    * Triangle:
        tr 10.0,20.0,10.0  10.0,10.0,20.0  20.0,10.0,10.0  0,0,255
            - identifier: tr
            - ×,y,z coordinates of the first point: 10.0,20.0,10.0
            - ×,y,z coordinates of the second point: 10.0,10.0,20.0
            - ×,y,z coordinates of the third point: 20.0,10.0,10.0
            - R,G,B colors in range 0,255: 0, 255, 255

# .rt file special material configuration
    expemple 1: sp	-2.5,-0.5,-10	2	255,0,0         2 4 90 0.1 100 100,0,123  0.5
        after the color argument:
            - material type, range [0 2]: 2
                0: mate material
                1: specular material
                2: reflexive matrial
                
            - color mode, range [0, 6]: 4
                0: only color
                1: normale rainbow color
                2: checkboard coloring
                3: marble coloring
                4: turbulance effect coloring
                5: wood effect coloring
                6: normal mapping whit image
                
            - speculatity coeficient: 90
            
            - reflexion coeficient: 0.1
            
            - coeficient used for special effect coloring: 100
            
            - r,g,b for special effect coloring: (100,0,123)
            
            - bump maping coeficient: 0.5
            
            - path to file for normal mapping (only for sphere) : ''
    
# Warning
    
    The projet is faaaaaar from perfect, you may encounter bugs  while playing whit it so do not do brainless copy past 😂
