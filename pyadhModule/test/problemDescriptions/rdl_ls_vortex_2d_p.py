from pyadh import *
from pyadh.default_p import *
from math import *
from vortex import *

## \page Tests Test Problems 
# \ref ls_vortex_2d_p.py "Linear advection of a circular level set function in an oscillating vortex velocity field"
# 

##\ingroup test
# \file la_vortex_2d_p.py
# @{
#  \brief Conservative linear advection of a circle signed distance function
#  in a oscillating vortex velocity field.
#  
# \f{eqnarray*}
# \phi_t + \nabla \cdot (\vec u \phi) &=& 0 \\ 
# \Omega &=& [0,1] \times [0,1] \\
#  u^{x} &=& \cos(\pi t/8)\sin(2\pi y)\sin^2(\pi x) \\
#  u^{y} &=& -\cos(\pi t/8)\sin(2\pi x)\sin^{2}(\pi y) \\
#  \phi^{0} &=& \left(x-\frac{1}{2}\right)^2 + \left(y-\frac{3}{4}\right)^2 - 0.15^2
# \f}
# The solution should return to the initial condition at \f$T=8\f$.
# Outflow boundaries are applied on \f$\partial \Omega\f$.
# 
#
# \image html  save_la_vortex_2d_dgp2_exact.jpg "exact solution, T=8.0"
# \image latex save_la_vortex_2d_dgp2_exact.eps "exact solution, T=8.0"
# \image html  save_la_vortex_2d_dgp2_phi.jpg "RKDG P^2 solution, Cr=0.1, L^2 error= 7.84e-3"
# \image latex save_la_vortex_2d_dgp2_phi.eps "RKDG $P^2$ solution, Cr=0.1, $L^2$ error= 7.84e-3"
#

class OscillatingVortex2D:
    def __init__(self):
        self.radius = 0.15
        self.xc=0.5
        self.yc=0.75
    def uOfXT(self,x,t):
        return self.radius - math.sqrt((x[0]-self.xc)**2 + (x[1]-self.yc)**2)
        
analyticalSolution = {0:OscillatingVortex2D()}

class UnitSquareVortex(TransportCoefficients.TC_base):
    from pyadh.ctransportCoefficients import unitSquareVortexEvaluate
    def __init__(self):
        mass={0:{0:'linear'}}
        advection={0:{0:'linear'}}
        diffusion={}
        potential={}
        reaction={}
        hamiltonian={}
        TransportCoefficients.TC_base.__init__(self,
                                             1,
                                             mass,
                                             advection,
                                             diffusion,
                                             potential,
                                             reaction,
                                             hamiltonian)
        self.modelIndex = 0
        self.vofModelIndex = 1
    def attachModels(self,modelList):
        self.model = modelList[self.modelIndex]
        self.vofModel = modelList[self.vofModelIndex]
    def preStep(self,t,firstStep=False):
        self.model.calculateCoefficients()
        self.vofModel.q[('m',0)][:]=0.0
        for eN in range(self.model.q[('u',0)].shape[0]):
            for k in range(self.model.q[('u',0)].shape[1]):
                if self.model.q[('u',0)][eN,k] > 0.0:
                    self.vofModel.q[('m',0)][eN,k]=1.0
        self.vofModel.timeIntegration.m_last[0][:]  = self.vofModel.q[('m',0)]
        print "==============================Air volume-ls-pre", Norms.scalarDomainIntegral(self.model.q['abs(det(J))'],
                                                                                            self.model.elementQuadratureWeights[('u',0)],
                                                                                            self.vofModel.q[('m',0)])
        copyInstructions = {}
        return copyInstructions
    def postStep(self,t,firstStep=False):
        self.model.calculateCoefficients()
        self.vofModel.q[('m',0)][:]=0.0
        for eN in range(self.model.q[('u',0)].shape[0]):
            for k in range(self.model.q[('u',0)].shape[1]):
                if self.model.q[('u',0)][eN,k] > 0.0:
                    self.vofModel.q[('m',0)][eN,k]= 1.0            
        print "==============================Air volume-ls-post", Norms.scalarDomainIntegral(self.model.q['abs(det(J))'],
                                                                                             self.model.elementQuadratureWeights[('u',0)],
                                                                                             self.vofModel.q[('m',0)])
        self.model.calculateCoefficients()
        copyInstructions = {}
        return copyInstructions
    def evaluate(self,t,c):
        self.unitSquareVortexEvaluate(t,
                                      c['x'],
                                      c[('u',0)],
                                      c[('m',0)],c[('dm',0,0)],
                                      c[('f',0)],c[('df',0,0)])

coefficients = UnitSquareVortex()

coefficients.variableNames=['u']

#now define the Dirichlet boundary conditions

def getDBC(x):
    pass
    #if (x[1] == 0.0):
    #    return lambda x,t: 0.0
    #if (x[0] == 0.0 or
    #    x[0] == 1.0 or
    #    x[1] == 0.0 or
    #    x[1] == 1.0):
    #    return lambda x,t: 0.0
def zeroInflow(x):
    if (x[0] == 0.0 and x[1] <= 0.5):
        return lambda x,t: 0.0
    if (x[0] == 1.0 and x[1] >= 0.5):
        return lambda x,t: 0.0
    if (x[1] == 0.0 and x[0] >= 0.5):
        return lambda x,t: 0.0
    if (x[1] == 1.0 and x[0] <= 0.5):
        return lambda x,t: 0.0
    
dirichletConditions = {0:getDBC}

initialConditions  = {0:analyticalSolution[0]}

fluxBoundaryConditions = {0:'outFlow'}

def zeroadv(x):
    return lambda x,t: 0.0
advectiveFluxBoundaryConditions =  {}
#advectiveFluxBoundaryConditions =  {0:zeroadv}


diffusiveFluxBoundaryConditions = {0:{}}

## @}
