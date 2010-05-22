from pyadh import *
from pyadh.default_n import *
from poisson_1c_het_3d_p import *

parallel = False#True

timeIntegration = NoIntegration
nDTout = 1


femSpaces = {0:DG_AffineLinearOnSimplexWithNodalBasis}


elementQuadrature =SimplexGaussQuadrature(nd,4)
elementBoundaryQuadrature = SimplexGaussQuadrature(nd-1,4)

nn = 5#11
nLevels = 1

subgridError = None

shockCapturing = None

numericalFluxType = Diffusion_LDG#Advection_DiagonalUpwind_Diffusion_IIPG

multilevelNonlinearSolver  = Newton

levelNonlinearSolver = Newton

fullNewtonFlag = True

tolFac = 0.01

nl_atol_res = 1.0e-8

matrix = SparseMatrix

if parallel:
    multilevelLinearSolver = PETSc#LU
    #for petsc do things lie
    #"-ksp_type cg -pc_type asm -pc_asm_type basic -ksp_atol  1.0e-10 -ksp_rtol 1.0e-10 -ksp_monitor_draw" or
    #-pc_type lu -pc_factor_mat_solver_package
    #can also set -pc_asm_overlap 2 with default asm type (restrict)
    levelLinearSolver = PETSc#LU#MGM#PETSc#
    #pick number of layers to use in overlap 
    nLayersOfOverlapForParallel = 1
    #type of partition
    parallelPartitioningType = MeshParallelPartitioningTypes.element
    #parallelPartitioningType = MeshParallelPartitioningTypes.node

else:
    multilevelLinearSolver = LU#NI#MGM

    levelLinearSolver = LU#MGM


linTolFac = 0.001

conservativeFlux = {0:'dg-point-eval'} 
