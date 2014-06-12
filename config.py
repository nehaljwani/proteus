import os

PROTEUS_INCLUDE_DIR = os.getenv('PROTEUS_PREFIX')+'/include'
PROTEUS_LIB_DIR = os.getenv('PROTEUS_PREFIX')+'/lib'
PROTEUS_EXTRA_COMPILE_ARGS= ['-DF77_POST_UNDERSCORE','-DUSE_BLAS','-DCMRVEC_BOUNDS_CHECK','-DMV_VECTOR_BOUNDS_CHECK','-DPETSC_INCLUDE_AS_C']
PROTEUS_EXTRA_LINK_ARGS=['-lblas']
PROTEUS_EXTRA_FC_COMPILE_ARGS= ['-Wall']
PROTEUS_EXTRA_FC_LINK_ARGS=['-lblas']

PROTEUS_SUPERLU_INCLUDE_DIR = PROTEUS_INCLUDE_DIR
PROTEUS_SUPERLU_H   = r'"slu_ddefs.h"'
PROTEUS_SUPERLU_LIB_DIR = PROTEUS_LIB_DIR
PROTEUS_SUPERLU_LIB = 'superlu_4.1'

PROTEUS_BLAS_INCLUDE_DIR   = '.'
PROTEUS_BLAS_H     = r'"proteus_blas.h"'
PROTEUS_BLAS_LIB_DIR = '/usr/lib'
PROTEUS_BLAS_LIB   = 'blas'

PROTEUS_LAPACK_INCLUDE_DIR = '.'
PROTEUS_LAPACK_H   = r'"proteus_lapack.h"'
PROTEUS_LAPACK_LIB_DIR = '/usr/lib'
PROTEUS_LAPACK_LIB = 'lapack'
PROTEUS_LAPACK_INTEGER = 'int'

PROTEUS_MPI_INCLUDE_DIR = '.'
PROTEUS_MPI_LIB_DIR = '.'
PROTEUS_MPI_LIBS = ['m']

PROTEUS_TRIANGLE_INCLUDE_DIR = PROTEUS_INCLUDE_DIR
PROTEUS_TRIANGLE_H = r'"triangle.h"'
PROTEUS_TRIANGLE_LIB_DIR = PROTEUS_LIB_DIR
PROTEUS_TRIANGLE_LIB ='tri'

PROTEUS_SCOREC_INCLUDE_DIR = os.getenv('PROTEUS_PREFIX')+'/include'
PROTEUS_SCOREC_LIB_DIR = os.getenv('PROTEUS_PREFIX')+'/lib'
###PROTEUS_SCOREC_EXTRA_LINK_ARGS = ['-lpumi_util', '-lSpaACIS', '-lapf_pumi', '-lapf', '-lpumi_geom', '-lpumi_geom_acis', '-lmeshadapt', '-lpumi_mesh', '-lpcu', '-lzoltan', '-lparmetis', '-lmetis']
###PROTEUS_SCOREC_EXTRA_LINK_ARGS = ['-lpumi_util', '-lapf_pumi', '-lapf', '-lpumi_geom', '-lpumi_geom_sim','-lpumi_mesh', '-lpcu', '-lzoltan', '-lparmetis', '-lmetis']
PROTEUS_SCOREC_EXTRA_LINK_ARGS = ['-lpumi_util', '-lapf_pumi', '-lapf', '-lspr', '-lpumi_geom_sim', '-lmeshadapt', '-lpumi_mesh', '-lpumi_geom', '-lpcu', '-lzoltan', '-lparmetis', '-lmetis']
###PROTEUS_SCOREC_EXTRA_LINK_ARGS = ['-lpumi_util', '-lapf_pumi', '-lapf', '-lpumi_geom', '-lpumi_mesh', '-lpcu', '-lzoltan', '-lparmetis', '-lmetis']
PROTEUS_SCOREC_EXTRA_COMPILE_ARGS=['-DPUMI_PARALLEL','-DMA_PARALLEL','-g']

PROTEUS_DAETK_INCLUDE_DIR = [PROTEUS_INCLUDE_DIR]
PROTEUS_DAETK_LIB_DIR = PROTEUS_LIB_DIR
PROTEUS_DAETK_LIB ='daetk'
PROTEUS_PETSC_LIB_DIRS = []
PROTEUS_PETSC_LIBS = []
PROTEUS_PETSC_INCLUDE_DIRS = []
