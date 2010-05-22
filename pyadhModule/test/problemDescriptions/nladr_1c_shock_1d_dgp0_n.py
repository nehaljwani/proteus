from pyadh import *
from pyadh.default_n import *
from nladr_1c_shock_1d_p import *

timeIntegration = BackwardEuler
#timeIntegration = ForwardEuler
#timeIntegration = ForwardEuler_A

DT=None
#DT=0.1
runCFL = 0.9

femSpaces = {0:DG_Constants}

elementQuadrature = SimplexGaussQuadrature(nd,1)

elementBoundaryQuadrature = SimplexGaussQuadrature(nd-1,1)

nn=3
nLevels = 5
nDTout = 100

subgridError = None

#numericalFluxType = Advection_DiagonalUpwind
numericalFluxType = Advection_DiagonalUpwind_Diffusion_IIPG

shockCapturing = None

multilevelNonlinearSolver  = NLNI

levelNonlinearSolver = Newton

nonlinearSmoother = NLGaussSeidel

fullNewtonFlag = True

tolFac = 0.01

nl_atol_res = 1.0e-8

matrix = SparseMatrix

multilevelLinearSolver = LU

levelLinearSolver = LU

linearSmoother = GaussSeidel

linTolFac = 0.001

conservativeFlux = None

archiveFlag = ArchiveFlags.EVERY_USER_STEP
