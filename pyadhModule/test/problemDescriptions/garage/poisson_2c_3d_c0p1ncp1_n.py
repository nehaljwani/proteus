from pyadh import *
from pyadh.default_n import *
from poisson_2c_3d_p import *

timeIntegration = NoIntegration

nDTout = 1

femSpaces = {0:C0_AffineLinearOnSimplexWithNodalBasis,1:NC_AffineLinearOnSimplexWithNodalBasis}

#try to pick terms specifically for components and equation terms
elementQuadrature = {'default':SimplexGaussQuadrature(nd,3),
                     ('a',1,0):SimplexGaussQuadrature(nd,1), #pick midpoint quad for P1_NC components
                     ('a',1,1):SimplexGaussQuadrature(nd,1),
                     ('r',1):SimplexGaussQuadrature(nd,1),
                     ('f',1):SimplexGaussQuadrature(nd,1)}
elementBoundaryQuadrature = {'default':SimplexGaussQuadrature(nd-1,3),
                             ('a',1,0):SimplexGaussQuadrature(nd-1,1), #pick midpoint quad for P1_NC components
                             ('a',1,1):SimplexGaussQuadrature(nd-1,1),
                             ('r',1):SimplexGaussQuadrature(nd-1,1),
                             ('f',1):SimplexGaussQuadrature(nd-1,1)}


nn = 3
nLevels = 2

subgridError = None

shockCapturing = None

multilevelNonlinearSolver  = Newton

levelNonlinearSolver = Newton

fullNewtonFlag = True

tolFac = 0.01

atol = 1.0e-8

matrix = SparseMatrix

multilevelLinearSolver = LU

levelLinearSolver = LU

linTolFac = 0.001

conservativeFlux = {0:'pwl',1:'p1-nc'}
