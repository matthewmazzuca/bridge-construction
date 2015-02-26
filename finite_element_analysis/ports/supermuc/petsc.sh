#! /bin/bash

prefix=/usr
export PETSC_DIR=`pwd`
./configure --prefix=$WORK/petsc/3.4.2 \
    --with-shared-libraries=1 \
    --with-debugging=1 \
    --with-mpiexec=$MPI_BASE/bin/mpiexec \
    --with-mpi=1 \
    --with-mpi-dir=$MPI_BASE \
    --with-blas-lapack-dir=/lrz/sys/intel/mkl110u1/ \
    --download-scalapack=$packagesBaseDir/petsc/ExternalPackages/SCALAPACK-1.7.tar.gz \
    --download-blacs=$packagesBaseDir/petsc/ExternalPackages/blacs-dev.tar.gz \
    --download-mumps=$packagesBaseDir/petsc/ExternalPackages/MUMPS_4.10.0-p3.tar.gz \
    --download-ml=$packagesBaseDir/petsc/ExternalPackages/ml-6.2-win.tar.gz


#    --download-ptscotch=$WORK/sources/scotch_6.0.0_esmumps.tar.gz \
