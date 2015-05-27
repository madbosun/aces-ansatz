acesinit
========

aces4 initialization program.
Preprocessing program for aces4 [https://github.com/UFParLab/aces4].

It reads in a file containing the molecule, quantum mechanical method and basis set and creates an initialization file.

Prerequisites
-------------
```
C, C++ & Fortran Compiler
cmake 
```

To Compile
----------
```BASH
mkdir OBJECTS
cd BUILD
cmake ../
make 
```
This should create the ```acesansatz``` executable.

To Use
------
Copy all the files in ```test/``` and the ```acesansatz``` executable to a separate directory. This folder should contain these files:
```BASH
<molecule_and_keyword_file>
<basis file>
```
The moleucle definitions file contains the molecule configuration and other configuration parameters.
The basi file contains basis set information.

In this directory, run the acesansatz file.
The generated ```data.dat``` binary file contains initialization data for aces4.

