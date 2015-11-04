from math import *
import proteus.MeshTools
from proteus import Domain
from proteus.default_n import *
from proteus import Profiling
from proteus.MeshAdaptPUMI import MeshAdaptPUMI
from tables import *

Profiling.verbose=True
Profiling.logLevel=7
#Profiling.logAllProcesses=True
#Profiling.openLog("proteus.log",7)

#  Discretization -- input options
Refinement = 100 #45min on a single core for spaceOrder=1, useHex=False
genMesh=True
useOldPETSc=False
useSuperlu=True
spaceOrder = 1
useHex     = False
useRBLES   = 0.0
useMetrics = 1.0
applyCorrection=True
useVF = 1.0
useOnlyVF = False
redist_Newton = False
useRANS = 0 # 0 -- None
            # 1 -- K-Epsilon
            # 2 -- K-Omega
gatherAtClose=True
# Input checks
if spaceOrder not in [1,2]:
    print "INVALID: spaceOrder" + spaceOrder
    sys.exit()

if useRBLES not in [0.0, 1.0]:
    print "INVALID: useRBLES" + useRBLES
    sys.exit()

if useMetrics not in [0.0, 1.0]:
    print "INVALID: useMetrics"
    sys.exit()

#  Discretization
nd = 3
if spaceOrder == 1:
    hFactor=1.0
    if useHex:
	 basis=C0_AffineLinearOnCubeWithNodalBasis
         elementQuadrature = CubeGaussQuadrature(nd,2)
         elementBoundaryQuadrature = CubeGaussQuadrature(nd-1,2)
    else:
    	 basis=C0_AffineLinearOnSimplexWithNodalBasis
         elementQuadrature = SimplexGaussQuadrature(nd,3)
         elementBoundaryQuadrature = SimplexGaussQuadrature(nd-1,3)
elif spaceOrder == 2:
    hFactor=0.5
    if useHex:
	basis=C0_AffineLagrangeOnCubeWithNodalBasis
        elementQuadrature = CubeGaussQuadrature(nd,4)
        elementBoundaryQuadrature = CubeGaussQuadrature(nd-1,4)
    else:
	basis=C0_AffineQuadraticOnSimplexWithNodalBasis
        elementQuadrature = SimplexGaussQuadrature(nd,4)
        elementBoundaryQuadrature = SimplexGaussQuadrature(nd-1,4)

# Domain and mesh
L = (0.584,0.146,0.70)
he = L[0]/float(4*Refinement-1)
#he*=0.5
#he = L[0]/40.0
#he*=0.5#128
#he*=0.5#1024
quasi2D = False#True

if quasi2D:
    L = (L[0],he,L[2])

# Domain and mesh
nLevels = 1
parallelPartitioningType = proteus.MeshTools.MeshParallelPartitioningTypes.element
#parallelPartitioningType = proteus.MeshTools.MeshParallelPartitioningTypes.node
nLayersOfOverlapForParallel = 0
structured=False
if useHex:
    nnx=4*Refinement+1
    nny=1*Refinement+1
    nnz=2*Refinement+1
    if quas2D:
        nny=2
    hex=True
    domain = Domain.RectangularDomain(L)
else:
    boundaries=['bottom','top','front','back','left','right']
    boundaryTags=dict([(key,i+1) for (i,key) in enumerate(boundaries)])
    if structured:
        nnx=4*Refinement
        nny=2*Refinement
    else:
        domain = Domain.PUMIDomain() #initialize the domain
        #Following sets list of face tags of geometric model as mapped from boundary Tags,
        #meaning if faceList=[[2,4],[1]] and boundaries=['left','right'], then faces with geometry tags 2 and 4 are set as 'left'
        #and face with geometric tag 4 is set as 'right'
        #The order of boundaries list is important because the last ones take precendence over the first ones,
        #which means that the geometric edge or vertex which lies on 2 or more geometric faces will be set with the boundaries tag of
        #the geomtric face which is latter in the order (email: chitak2@rpi.edu for any questions)
        domain.faceList=[[3],[5],[1],[6],[2],[4]]
        #set max edge length, min edge length, number of meshadapt iterations and initialize the MeshAdaptPUMI object
        he = 0.018
        #these are now inputs to the numerics
        adaptMesh = True
        adaptMesh_nSteps = 10
        adaptMesh_numIter = 2
        #
        domain.PUMIMesh=MeshAdaptPUMI.MeshAdaptPUMI(hmax=0.08, hmin=he, numIter=adaptMesh_numIter)
        #read the geometry and mesh
        comm = Comm.init()
        model_dir = "%s-Proc" % comm.size()
        case_mesh = "Dambreak_coarse.smb"
        input_mesh = "%s/%s" % (model_dir,case_mesh)
        domain.PUMIMesh.loadModelAndMesh("Dambreak.smd", input_mesh)

# Time stepping
T=0.35
dt_fixed = 0.01
dt_init = min(0.01*dt_fixed,0.0001)
runCFL=0.9
nDTout = int(round(T/dt_fixed))

# Numerical parameters
ns_forceStrongDirichlet = False#True
if useMetrics:
    ns_shockCapturingFactor  = 0.5
    ns_lag_shockCapturing = True
    ns_lag_subgridError = True
    ls_shockCapturingFactor  = 0.5
    ls_lag_shockCapturing = True
    ls_sc_uref  = 1.0
    ls_sc_beta  = 1.5
    vof_shockCapturingFactor = 0.5
    vof_lag_shockCapturing = True
    vof_sc_uref = 1.0
    vof_sc_beta = 1.5
    rd_shockCapturingFactor  = 0.5
    rd_lag_shockCapturing = False
    epsFact_density    = 1.5
    epsFact_viscosity  = epsFact_curvature  = epsFact_vof = epsFact_consrv_heaviside = epsFact_consrv_dirac = epsFact_density
    epsFact_redistance = 0.33
    epsFact_consrv_diffusion = 10.0
    redist_Newton = False
else:
    ns_shockCapturingFactor  = 0.9
    ns_lag_shockCapturing = True
    ns_lag_subgridError = True
    ls_shockCapturingFactor  = 0.9
    ls_lag_shockCapturing = True
    ls_sc_uref  = 1.0
    ls_sc_beta  = 1.0
    vof_shockCapturingFactor = 0.9
    vof_lag_shockCapturing = True
    vof_sc_uref  = 1.0
    vof_sc_beta  = 1.0
    rd_shockCapturingFactor  = 0.9
    rd_lag_shockCapturing = False
    epsFact_density    = 1.5
    epsFact_viscosity  = epsFact_curvature  = epsFact_vof = epsFact_consrv_heaviside = epsFact_consrv_dirac = epsFact_density
    epsFact_redistance = 0.33
    epsFact_consrv_diffusion = 10.0
    redist_Newton = False
    kappa_shockCapturingFactor = 0.9
    kappa_lag_shockCapturing = True#False
    kappa_sc_uref  = 1.0
    kappa_sc_beta  = 1.0
    dissipation_shockCapturingFactor = 0.9
    dissipation_lag_shockCapturing = True#False
    dissipation_sc_uref  = 1.0
    dissipation_sc_beta  = 1.0

ns_nl_atol_res = max(1.0e-8,0.001*he**2)
vof_nl_atol_res = max(1.0e-8,0.001*he**2)
ls_nl_atol_res = max(1.0e-8,0.001*he**2)
rd_nl_atol_res = max(1.0e-8,0.01*he**2)
mcorr_nl_atol_res = max(1.0e-8,0.001*he**2)
kappa_nl_atol_res = max(1.0e-8,0.001*he**2)
dissipation_nl_atol_res = max(1.0e-8,0.001*he**2)

#turbulence
ns_closure=0 #1-classic smagorinsky, 2-dynamic smagorinsky, 3 -- k-epsilon, 4 -- k-omega
if useRANS == 1:
    ns_closure = 3
elif useRANS == 2:
    ns_closure == 4
# Water
rho_0 = 998.2
nu_0  = 1.004e-6

# Air
rho_1 = 1.205
nu_1  = 1.500e-5

# Surface tension
sigma_01 = 0.0

# Gravity
g = [0.0,0.0,-9.81]

# Initial condition
waterLine_x = 0.146
waterLine_z = 0.292

def signedDistance(x):
    phi_x = x[0]-waterLine_x
    phi_z = x[2]-waterLine_z
    if phi_x < 0.0:
        if phi_z < 0.0:
            return max(phi_x,phi_z)
        else:
            return phi_z
    else:
        if phi_z < 0.0:
            return phi_x
        else:
            return sqrt(phi_x**2 + phi_z**2)
