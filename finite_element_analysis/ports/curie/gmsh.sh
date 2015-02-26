#! /bin/bash

. ~/.bash_profile
module unload c/intel/12.1.7.256  fortran/intel/12.1.7.256  c++/intel/12.1.7.256  
module unload mkl
module load cmake/2.8.3
module load c++/gnu/4.5.1 

cmake \
 -DCMAKE_SHARED_LINKER_FLAGS="-L$WORKDIR/local-gcc45/lib;-L$MPI_ROOT/lib" \
 -DCMAKE_Fortran_COMPILER="/usr/local/gcc-4.5.1/bin/gfortran" \
 -DCMAKE_INSTALL_PREFIX=$WORKDIR/local-gcc45 \
 -DENABLE_PETSC=OFF \
 $SCRATCHDIR/Gmsh/gmsh-2.6.0-source/

#make all lib shared
