#include "Python.h"
#include "numpy/arrayobject.h"
#include "ellam.h"
#include "superluWrappersModule.h"

#define ND(p) ((PyArrayObject *)p)->nd
#define SHAPE(p) ((PyArrayObject *)p)->dimensions
#define DDATA(p) ((double *) (((PyArrayObject *)p)->data))
#define IDATA(p) ((int *) (((PyArrayObject *)p)->data))
#define CSRVAL(p) ((double*)((SparseMatrix*)p)->A.nzval)

#define ND(p) ((PyArrayObject *)p)->nd
#define SHAPE(p) ((PyArrayObject *)p)->dimensions
#define DDATA(p) ((double *) (((PyArrayObject *)p)->data))
#define IDATA(p) ((int *) (((PyArrayObject *)p)->data))
#define CSRVAL(p) ((double*)((SparseMatrix*)p)->A.nzval)

static PyObject* cellam_updateOldMass_weak(PyObject* self,
					   PyObject* args)
{
  int nSpace,            
    nDOF_test_element, 
    nElements_global,  
    nNodes_global,
    nNodes_element,
    nElementBoundaries_element,
    nQuadraturePoints_element;
  PyObject *nodeArray,
    *elementNodesArray,
    *elementNeighborsArray,
    *elementBoundaryOuterNormalsArray,
    *dV,
    *x_track,
    *t_track,
    *element_track,
    *flag_track,
    *u_l2g, 
    *q_m_last,
    *elementResidual_u; 

  if (!PyArg_ParseTuple(args,
                        "iiiiiiiOOOOOOOOOOOO",/*iiO",*/
			&nSpace,
			&nDOF_test_element,
			&nElements_global,
			&nNodes_global,
			&nNodes_element,
			&nElementBoundaries_element,
			&nQuadraturePoints_element,
			&nodeArray,
			&elementNodesArray,
			&elementNeighborsArray,
			&elementBoundaryOuterNormalsArray,
			&dV,
			&x_track,
			&t_track,
			&element_track,
			&flag_track,
			&u_l2g, 
			&q_m_last,
			&elementResidual_u))

    return NULL;
  
  updateOldMass_weak(nSpace,
		     nDOF_test_element,
		     nElements_global,
		     nNodes_global,
		     nNodes_element,
		     nElementBoundaries_element,
		     nQuadraturePoints_element,
		     DDATA(nodeArray),
		     IDATA(elementNodesArray),
		     IDATA(elementNeighborsArray),
		     DDATA(elementBoundaryOuterNormalsArray),
		     DDATA(dV),
		     DDATA(x_track),
		     DDATA(t_track),
		     IDATA(element_track),
		     IDATA(flag_track),
		     IDATA(u_l2g), 
		     DDATA(q_m_last),
		     DDATA(elementResidual_u));

  Py_INCREF(Py_None); 
  return Py_None;
}

static PyObject* cellam_evaluateSolutionAtTrackedPoints(PyObject* self,
							PyObject* args)
{
  int nSpace,            
    nDOF_trial_element, 
    nPoints_tracked,
    nElements_global,
    nNodes_global,
    nNodes_element,
    nElementBoundaries_element;
  PyObject *nodeArray,
    *elementNodesArray,
    *elementNeighborsArray,
    *elementBoundaryOuterNormalsArray,
    *x_track,
    *t_track,
    *element_track,
    *flag_track,
    *u_l2g, 
    *u_dof,
    *u_x_track;

  if (!PyArg_ParseTuple(args,
                        "iiiiiiiOOOOOOOOOOO",
			&nSpace,
			&nDOF_trial_element,
			&nPoints_tracked,
			&nElements_global,
			&nNodes_global,
			&nNodes_element,
			&nElementBoundaries_element,
			&nodeArray,
			&elementNodesArray,
			&elementNeighborsArray,
			&elementBoundaryOuterNormalsArray,
			&x_track,
			&t_track,
			&element_track,
			&flag_track,
			&u_l2g, 
			&u_dof,
			&u_x_track))
    return NULL;
  
  evaluateSolutionAtTrackedPoints(nSpace,
				  nDOF_trial_element,
				  nPoints_tracked,
				  nElements_global,
				  nNodes_global,
				  nNodes_element,
				  nElementBoundaries_element,
				  DDATA(nodeArray),
				  IDATA(elementNodesArray),
				  IDATA(elementNeighborsArray),
				  DDATA(elementBoundaryOuterNormalsArray),
				  DDATA(x_track),
				  DDATA(t_track),
				  IDATA(element_track),
				  IDATA(flag_track),
				  IDATA(u_l2g), 
				  DDATA(u_dof),
				  DDATA(u_x_track)); 
  Py_INCREF(Py_None); 
  return Py_None;
}

static PyObject* cellam_updateExteriorOutflowBoundaryFlux(PyObject* self,
							  PyObject* args)
{
  double dtnp1;
  int nSpace,
    nDOF_test_element,
    nQuadraturePoints_elementBoundary,
    nExteriorElementBoundaries_global;
  PyObject *exteriorElementBoundariesArray,
    *elementBoundaryElementsArray,
    *elementBoundaryLocalElementBoundariesArray,
    *ebqe_velocity_ext,
    *ebqe_n_ext,
    *ebqe_outflow_flux_last,
    *u_test_dS_ext,
    *ebqe_u,
    *u_l2g,
    *ebqe_outflow_flux;
  PyObject* q_elementResidual_u;

  if (!PyArg_ParseTuple(args,
                        "diiiiOOOOOOOOOOO",
			&dtnp1,
			&nSpace,
			&nDOF_test_element,
			&nQuadraturePoints_elementBoundary,
			&nExteriorElementBoundaries_global,
			&exteriorElementBoundariesArray,
			&elementBoundaryElementsArray,
			&elementBoundaryLocalElementBoundariesArray,
			&ebqe_velocity_ext,
			&ebqe_n_ext,
			&ebqe_outflow_flux_last,
			&u_test_dS_ext,
			&ebqe_u,
			&u_l2g,
			&ebqe_outflow_flux,
			&q_elementResidual_u)) 
			
    return NULL;

  updateExteriorOutflowBoundaryFlux(dtnp1,
				    nSpace,
				    nDOF_test_element,
				    nQuadraturePoints_elementBoundary,
				    nExteriorElementBoundaries_global,
				    IDATA(exteriorElementBoundariesArray),
				    IDATA(elementBoundaryElementsArray),
				    IDATA(elementBoundaryLocalElementBoundariesArray),
				    DDATA(ebqe_velocity_ext),
				    DDATA(ebqe_n_ext),
				    DDATA(ebqe_outflow_flux_last),
				    DDATA(u_test_dS_ext),
				    DDATA(ebqe_u),
				    IDATA(u_l2g),
				    DDATA(ebqe_outflow_flux),
				    DDATA(q_elementResidual_u)); 

  Py_INCREF(Py_None); 
  return Py_None;

} 

static PyObject* cellam_updateExteriorOutflowBoundaryFluxInGlobalResidual(PyObject* self,
									  PyObject* args)
{
  double dtnp1;
  int nSpace,
    nDOF_test_element,
    nQuadraturePoints_elementBoundary,
    nExteriorElementBoundaries_global;
  PyObject *exteriorElementBoundariesArray,
    *elementBoundaryElementsArray,
    *elementBoundaryLocalElementBoundariesArray,
    *ebqe_velocity_ext,
    *ebqe_n_ext,
    *ebqe_outflow_flux_last,
    *u_test_dS_ext,
    *ebqe_u,
    *u_l2g,
    *ebqe_outflow_flux;
  int offset_u,stride_u;
  PyObject* q_elementResidual_u,
    *globalResidual;

  if (!PyArg_ParseTuple(args,
                        "diiiiOOOOOOOOOOiiOO",
			&dtnp1,
			&nSpace,
			&nDOF_test_element,
			&nQuadraturePoints_elementBoundary,
			&nExteriorElementBoundaries_global,
			&exteriorElementBoundariesArray,
			&elementBoundaryElementsArray,
			&elementBoundaryLocalElementBoundariesArray,
			&ebqe_velocity_ext,
			&ebqe_n_ext,
			&ebqe_outflow_flux_last,
			&u_test_dS_ext,
			&ebqe_u,
			&u_l2g,
			&ebqe_outflow_flux,
			&offset_u, &stride_u, 
			&q_elementResidual_u, 
			&globalResidual))
    return NULL;

  updateExteriorOutflowBoundaryFluxInGlobalResidual(dtnp1,
						    nSpace,
						    nDOF_test_element,
						    nQuadraturePoints_elementBoundary,
						    nExteriorElementBoundaries_global,
						    IDATA(exteriorElementBoundariesArray),
						    IDATA(elementBoundaryElementsArray),
						    IDATA(elementBoundaryLocalElementBoundariesArray),
						    DDATA(ebqe_velocity_ext),
						    DDATA(ebqe_n_ext),
						    DDATA(ebqe_outflow_flux_last),
						    DDATA(u_test_dS_ext),
						    DDATA(ebqe_u),
						    IDATA(u_l2g),
						    DDATA(ebqe_outflow_flux),
						    offset_u,stride_u, 
						    DDATA(q_elementResidual_u), 
						    DDATA(globalResidual));

  Py_INCREF(Py_None); 
  return Py_None;

} 


static PyObject* cellam_updateExteriorOutflowBoundaryFluxGlobalJacobian(PyObject* self,
									PyObject* args)
{
  double dtnp1;
  int nSpace,
    nDOF_test_element,
    nDOF_trial_element,
    nQuadraturePoints_elementBoundary,
    nExteriorElementBoundaries_global;
  PyObject *exteriorElementBoundariesArray,
    *elementBoundaryElementsArray,
    *elementBoundaryLocalElementBoundariesArray,
    *ebqe_velocity_ext,
    *ebqe_n_ext,
    *ebqe_outflow_flux_last,
    *u_test_dS_ext,
    *ebqe_u,
    *u_trial_ext,
    *csrRowIndeces_u_u, *csrColumnOffsets_u_u,
    *csrColumnOffsets_eb_u_u,
    *globalJacobian;


  if (!PyArg_ParseTuple(args,
                        "diiiiiOOOOOOOOOOOOO",
			&dtnp1,
			&nSpace,
			&nDOF_test_element,
			&nDOF_trial_element,
			&nQuadraturePoints_elementBoundary,
			&nExteriorElementBoundaries_global,
			&exteriorElementBoundariesArray,
			&elementBoundaryElementsArray,
			&elementBoundaryLocalElementBoundariesArray,
			&ebqe_velocity_ext,
			&ebqe_n_ext,
			&ebqe_outflow_flux_last,
			&u_test_dS_ext,
			&ebqe_u,
			&u_trial_ext,
			&csrRowIndeces_u_u,
			&csrColumnOffsets_u_u,
			&csrColumnOffsets_eb_u_u,
			&globalJacobian))
    return NULL;

  updateExteriorOutflowBoundaryFluxGlobalJacobian(dtnp1,
						  nSpace,
						  nDOF_test_element,
						  nDOF_trial_element,
						  nQuadraturePoints_elementBoundary,
						  nExteriorElementBoundaries_global,
						  IDATA(exteriorElementBoundariesArray),
						  IDATA(elementBoundaryElementsArray),
						  IDATA(elementBoundaryLocalElementBoundariesArray),
						  DDATA(ebqe_velocity_ext),
						  DDATA(ebqe_n_ext),
						  DDATA(ebqe_outflow_flux_last),
						  DDATA(u_test_dS_ext),
						  DDATA(ebqe_u),
						  DDATA(u_trial_ext),
						  IDATA(csrRowIndeces_u_u),
						  IDATA(csrColumnOffsets_u_u),
						  IDATA(csrColumnOffsets_eb_u_u),
						  CSRVAL(globalJacobian));
  
  Py_INCREF(Py_None); 
  return Py_None;

} 

static PyObject* cellam_updateExteriorOutflowBoundaryFluxJacobian(PyObject* self,
								  PyObject* args)
{
  double dtnp1;
  int nSpace,
    nDOF_test_element,
    nDOF_trial_element,
    nQuadraturePoints_elementBoundary,
    nExteriorElementBoundaries_global;
  PyObject *exteriorElementBoundariesArray,
    *elementBoundaryElementsArray,
    *elementBoundaryLocalElementBoundariesArray,
    *ebqe_velocity_ext,
    *ebqe_n_ext,
    *ebqe_outflow_flux_last,
    *u_test_dS_ext,
    *ebqe_u,
    *u_trial_ext,
    *fluxJacobian;


  if (!PyArg_ParseTuple(args,
                        "diiiiiOOOOOOOOOO",
			&dtnp1,
			&nSpace,
			&nDOF_test_element,
			&nDOF_trial_element,
			&nQuadraturePoints_elementBoundary,
			&nExteriorElementBoundaries_global,
			&exteriorElementBoundariesArray,
			&elementBoundaryElementsArray,
			&elementBoundaryLocalElementBoundariesArray,
			&ebqe_velocity_ext,
			&ebqe_n_ext,
			&ebqe_outflow_flux_last,
			&u_test_dS_ext,
			&ebqe_u,
			&u_trial_ext,
			&fluxJacobian))
    return NULL;

  updateExteriorOutflowBoundaryFluxJacobian(dtnp1,
					    nSpace,
					    nDOF_test_element,
					    nDOF_trial_element,
					    nQuadraturePoints_elementBoundary,
					    nExteriorElementBoundaries_global,
					    IDATA(exteriorElementBoundariesArray),
					    IDATA(elementBoundaryElementsArray),
					    IDATA(elementBoundaryLocalElementBoundariesArray),
					    DDATA(ebqe_velocity_ext),
					    DDATA(ebqe_n_ext),
					    DDATA(ebqe_outflow_flux_last),
					    DDATA(u_test_dS_ext),
					    DDATA(ebqe_u),
					    DDATA(u_trial_ext),
					    DDATA(fluxJacobian));
  
  Py_INCREF(Py_None); 
  return Py_None;

} 

static PyObject* cellam_markInflowBoundaryPoints(PyObject* self,
						 PyObject* args)
{
  int nSpace;

  double tn,tnp1,t;

  int nExteriorElementBoundaries_global,
    nQuadraturePoints_elementBoundary;

  PyObject *exteriorElementBoundariesArray,
    *elementBoundaryElementsArray,
    *elementBoundaryLocalElementBoundariesArray,
    *elementBoundaryOuterNormalsArray,
    *ebqe_x,
    *ebqe_n_ext,
    *ebqe_velocity_ext_last,
    *ebqe_velocity_ext,
    *isDOFBoundary_u,
    *isFluxBoundary_u,
    *element_track,
    *flag_track;

  if (!PyArg_ParseTuple(args,
                        "idddiiOOOOOOOOOOO",
			&nSpace,
			&tn,
			&tnp1,
			&t,
			&nExteriorElementBoundaries_global,
			&nQuadraturePoints_elementBoundary,
			&exteriorElementBoundariesArray,
			&elementBoundaryElementsArray,
			&elementBoundaryLocalElementBoundariesArray,
			&ebqe_x,
			&ebqe_n_ext,
			&ebqe_velocity_ext_last,
			&ebqe_velocity_ext,
			&isDOFBoundary_u,
			&isFluxBoundary_u,
			&element_track,
			&flag_track))
    return NULL;

  markInflowBoundaryPoints(nSpace,
			   tn,
			   tnp1,
			   t,
			   nExteriorElementBoundaries_global,
			   nQuadraturePoints_elementBoundary,
			   IDATA(exteriorElementBoundariesArray),
			   IDATA(elementBoundaryElementsArray),
			   IDATA(elementBoundaryLocalElementBoundariesArray),	
			   DDATA(ebqe_x),
			   DDATA(ebqe_n_ext),
			   DDATA(ebqe_velocity_ext_last),
			   DDATA(ebqe_velocity_ext),
			   IDATA(isDOFBoundary_u),
			   IDATA(isFluxBoundary_u),
			   IDATA(element_track),
			   IDATA(flag_track));
  Py_INCREF(Py_None); 
  return Py_None;
}

static PyObject* cellam_accumulateInflowFluxInGlobalResidual(PyObject* self,
							     PyObject* args)
{
  int nSpace,
    nDOF_test_element;

  double tp,timeWeight;

  int nElements_global,
    nNodes_global,
    nNodes_element,
    nElementBoundaries_element,
    offset_u,stride_u,
    nExteriorElementBoundaries_global,
    nQuadraturePoints_elementBoundary;

  PyObject *nodeArray,
    *elementNodesArray,
    *elementNeighborsArray,
    *exteriorElementBoundariesArray,
    *elementBoundaryElementsArray,
    *elementBoundaryLocalElementBoundariesArray, 
    *elementBoundaryOuterNormalsArray,
    *dS,
    *x_track_ext,
    *t_track_ext,
    *element_track_ext,
    *flag_track_ext,
    *u_l2g,
    *u_dof,
    *elementResidual_u, 
    *globalResidual,
    *sdInfo_u_rowptr,
    *sdInfo_u_colind,
    *isFluxBoundary_u,
    *ebqe_bc_flux_u_ext;

  if (!PyArg_ParseTuple(args,
			"iiiiiiiiOOOOOOOddOOOOOOOOOOiiOOO",
			&nSpace,
			&nDOF_test_element,
			&nElements_global,
			&nNodes_global,
			&nNodes_element,
			&nElementBoundaries_element,
			&nExteriorElementBoundaries_global,
			&nQuadraturePoints_elementBoundary,
			&nodeArray,
			&elementNodesArray,
			&elementNeighborsArray, 
			&exteriorElementBoundariesArray,
			&elementBoundaryElementsArray,
			&elementBoundaryLocalElementBoundariesArray,
			&elementBoundaryOuterNormalsArray,
			&tp,
			&timeWeight,
			&dS,
			&x_track_ext,
			&t_track_ext,
			&element_track_ext,
			&flag_track_ext,
			&u_l2g, 
			&u_dof,
			&elementResidual_u, 
			&sdInfo_u_rowptr,
			&sdInfo_u_colind,
			&offset_u,
			&stride_u, 
			&isFluxBoundary_u,
			&ebqe_bc_flux_u_ext,
			&globalResidual))
    return NULL;

  accumulateInflowFluxInGlobalResidual(nSpace,
				       nDOF_test_element,
				       nElements_global,
				       nNodes_global,
				       nNodes_element,
				       nElementBoundaries_element,
				       nExteriorElementBoundaries_global,
				       nQuadraturePoints_elementBoundary,
				       DDATA(nodeArray),
				       IDATA(elementNodesArray),
				       IDATA(elementNeighborsArray), 
				       IDATA(exteriorElementBoundariesArray),
				       IDATA(elementBoundaryElementsArray),
				       IDATA(elementBoundaryLocalElementBoundariesArray),
				       DDATA(elementBoundaryOuterNormalsArray),
				       tp,
				       timeWeight,
				       DDATA(dS),
				       DDATA(x_track_ext),
				       DDATA(t_track_ext),
				       IDATA(element_track_ext),
				       IDATA(flag_track_ext),
				       IDATA(u_l2g), 
				       DDATA(u_dof),
				       DDATA(elementResidual_u), 
				       IDATA(sdInfo_u_rowptr),
				       IDATA(sdInfo_u_colind),
				       offset_u,
				       stride_u, 
				       IDATA(isFluxBoundary_u),
				       DDATA(ebqe_bc_flux_u_ext),
				       DDATA(globalResidual));

  Py_INCREF(Py_None); 
  return Py_None;
 
}

static PyObject* cellam_accumulateInflowFlux(PyObject* self,
					     PyObject* args)
{
  int nSpace,
    nDOF_test_element;

  double tp,timeWeight;

  int nElements_global,
    nNodes_global,
    nNodes_element,
    nElementBoundaries_element,
    nExteriorElementBoundaries_global,
    nQuadraturePoints_elementBoundary;

  PyObject *nodeArray,
    *elementNodesArray,
    *elementNeighborsArray,
    *exteriorElementBoundariesArray,
    *elementBoundaryElementsArray,
    *elementBoundaryLocalElementBoundariesArray, 
    *elementBoundaryOuterNormalsArray,
    *dS,
    *x_track_ext,
    *t_track_ext,
    *element_track_ext,
    *flag_track_ext,
    *u_l2g,
    *u_dof,
    *elementResidual_u, 
    *sdInfo_u_rowptr,
    *sdInfo_u_colind,
    *isFluxBoundary_u,
    *ebqe_bc_flux_u_ext;

  if (!PyArg_ParseTuple(args,
			"iiiiiiiiOOOOOOOddOOOOOOOOOOOO",
			&nSpace,
			&nDOF_test_element,
			&nElements_global,
			&nNodes_global,
			&nNodes_element,
			&nElementBoundaries_element,
			&nExteriorElementBoundaries_global,
			&nQuadraturePoints_elementBoundary,
			&nodeArray,
			&elementNodesArray,
			&elementNeighborsArray, 
			&exteriorElementBoundariesArray,
			&elementBoundaryElementsArray,
			&elementBoundaryLocalElementBoundariesArray,
			&elementBoundaryOuterNormalsArray,
			&tp,
			&timeWeight,
			&dS,
			&x_track_ext,
			&t_track_ext,
			&element_track_ext,
			&flag_track_ext,
			&u_l2g, 
			&u_dof,
			&elementResidual_u, 
			&sdInfo_u_rowptr,
			&sdInfo_u_colind,
			&isFluxBoundary_u,
			&ebqe_bc_flux_u_ext))
    return NULL;

  accumulateInflowFlux(nSpace,
		       nDOF_test_element,
		       nElements_global,
		       nNodes_global,
		       nNodes_element,
		       nElementBoundaries_element,
		       nExteriorElementBoundaries_global,
		       nQuadraturePoints_elementBoundary,
		       DDATA(nodeArray),
		       IDATA(elementNodesArray),
		       IDATA(elementNeighborsArray), 
		       IDATA(exteriorElementBoundariesArray),
		       IDATA(elementBoundaryElementsArray),
		       IDATA(elementBoundaryLocalElementBoundariesArray),
		       DDATA(elementBoundaryOuterNormalsArray),
		       tp,
		       timeWeight,
		       DDATA(dS),
		       DDATA(x_track_ext),
		       DDATA(t_track_ext),
		       IDATA(element_track_ext),
		       IDATA(flag_track_ext),
		       IDATA(u_l2g), 
		       DDATA(u_dof),
		       DDATA(elementResidual_u), 
		       IDATA(sdInfo_u_rowptr),
		       IDATA(sdInfo_u_colind),
		       IDATA(isFluxBoundary_u),
		       DDATA(ebqe_bc_flux_u_ext));

  Py_INCREF(Py_None); 
  return Py_None;
 
}


static PyObject* cellam_tagNegligibleIntegrationPoints(PyObject* self,
						       PyObject* args)
{
  int nPoints;
  double zeroTol;
  PyObject *x,*u,*flag_track;

  if (!PyArg_ParseTuple(args,"idOOO",
			&nPoints,
			&zeroTol,
			&x,
			&u,
			&flag_track))
    return NULL;

  tagNegligibleIntegrationPoints(nPoints,
				 zeroTol,
				 DDATA(x),
				 DDATA(u),
				 IDATA(flag_track));

  
  Py_INCREF(Py_None); 
  return Py_None;
 
}
static PyObject* cellam_calculateSlumpedMassApproximation1d(PyObject* self,
							    PyObject* args)
{
  PyObject *u_l2g,
    *elementNeighborsArray,
    *u_dof,*u_dof_limit,
    *dm,*w,*v,
    *dV,*rhs,
    *theta,
    *slumpedMassMatrixCorrection,
    *elementResidual; 

  if (!PyArg_ParseTuple(args,
                        "OOOOOOOOOOOO",
			&u_l2g,
			&elementNeighborsArray,
			&u_dof,
			&u_dof_limit,
			&dm,
			&w,
			&v,
			&dV,
			&rhs,
			&elementResidual,
			&theta,
			&slumpedMassMatrixCorrection))
    return NULL;
  
  calculateSlumpedMassApproximation1d(SHAPE(u_l2g)[0],
				      SHAPE(elementNeighborsArray)[1],
				      SHAPE(dm)[1],
				      SHAPE(v)[2],
				      SHAPE(w)[2],
				      IDATA(u_l2g),
				      IDATA(elementNeighborsArray),
				      DDATA(u_dof),
				      DDATA(u_dof_limit),
				      DDATA(dm),
				      DDATA(w),
				      DDATA(v),
				      DDATA(dV),
				      DDATA(rhs),
				      DDATA(elementResidual),
				      DDATA(theta),
				      DDATA(slumpedMassMatrixCorrection));

  Py_INCREF(Py_None); 
  return Py_None;
}
static PyObject* cellam_calculateSlumpedMassApproximation2d(PyObject* self,
							    PyObject* args)
{
  PyObject *u_l2g,
    *elementNeighborsArray,
    *u_dof,*u_dof_limit,
    *dm,*w,*v,
    *dV,*rhs,
    *theta,
    *slumpedMassMatrixCorrection,
    *elementResidual; 

  if (!PyArg_ParseTuple(args,
                        "OOOOOOOOOOOO",
			&u_l2g,
			&elementNeighborsArray,
			&u_dof,
			&u_dof_limit,
			&dm,
			&w,
			&v,
			&dV,
			&rhs,
			&elementResidual,
			&theta,
			&slumpedMassMatrixCorrection))
    return NULL;
  
  calculateSlumpedMassApproximation2d(SHAPE(u_l2g)[0],
				      SHAPE(elementNeighborsArray)[1],
				      SHAPE(dm)[1],
				      SHAPE(v)[2],
				      SHAPE(w)[2],
				      IDATA(u_l2g),
				      IDATA(elementNeighborsArray),
				      DDATA(u_dof),
				      DDATA(u_dof_limit),
				      DDATA(dm),
				      DDATA(w),
				      DDATA(v),
				      DDATA(dV),
				      DDATA(rhs),
				      DDATA(elementResidual),
				      DDATA(theta),
				      DDATA(slumpedMassMatrixCorrection));

  Py_INCREF(Py_None); 
  return Py_None;
}
static PyObject* cellam_updateElementJacobianWithSlumpedMassApproximation(PyObject* self,
									  PyObject* args)
{
  PyObject *theta,
    *elementJacobian; 

  if (!PyArg_ParseTuple(args,
                        "OO",
			&theta,
			&elementJacobian))
    return NULL;
  
  updateElementJacobianWithSlumpedMassApproximation(SHAPE(elementJacobian)[0],
						    SHAPE(elementJacobian)[2],
						    SHAPE(elementJacobian)[1],
						    DDATA(theta),
						    DDATA(elementJacobian));

  Py_INCREF(Py_None); 
  return Py_None;
}

static PyObject* cellam_calculateBerzinsSlumpedMassApproximation1d(PyObject* self,
								  PyObject* args)
{
  PyObject *u_l2g,
    *elementNeighborsArray,
    *u_dof,*u_dof_limit,
    *dm,*w,*v,
    *dV,*rhs,
    *slumpedMassMatrixCorrection,
    *elementResidual; 

  if (!PyArg_ParseTuple(args,
                        "OOOOOOOOOOO",
			&u_l2g,
			&elementNeighborsArray,
			&u_dof,
			&u_dof_limit,
			&dm,
			&w,
			&v,
			&dV,
			&rhs,
			&elementResidual,
			&slumpedMassMatrixCorrection))
    return NULL;
  
  calculateBerzinsSlumpedMassApproximation1dv2(SHAPE(u_l2g)[0],
					    SHAPE(elementNeighborsArray)[1],
					    SHAPE(dm)[1],
					    SHAPE(v)[2],
					    SHAPE(w)[2],
					    IDATA(u_l2g),
					    IDATA(elementNeighborsArray),
					    DDATA(u_dof),
					    DDATA(u_dof_limit),
					    DDATA(dm),
					    DDATA(w),
					    DDATA(v),
					    DDATA(dV),
					    DDATA(rhs),
					    DDATA(elementResidual),
					    DDATA(slumpedMassMatrixCorrection));

  Py_INCREF(Py_None); 
  return Py_None;
}

static PyObject* cellam_updateElementJacobianWithSlumpedMassCorrection(PyObject* self,
								       PyObject* args)
{
  PyObject *elementMassMatrixCorrection,
    *elementJacobian; 

  if (!PyArg_ParseTuple(args,
                        "OO",
			&elementMassMatrixCorrection,
			&elementJacobian))
    return NULL;
  
  updateElementJacobianWithSlumpedMassCorrection(SHAPE(elementJacobian)[0],
						 SHAPE(elementJacobian)[2],
						 SHAPE(elementJacobian)[1],
						 DDATA(elementMassMatrixCorrection),
						 DDATA(elementJacobian));

  Py_INCREF(Py_None); 
  return Py_None;
}

static PyMethodDef cellamMethods[] = {
 { "updateOldMass_weak",
    cellam_updateOldMass_weak,
   METH_VARARGS, 
   "update ellam element residual with mass accumulation from previous time level"},
 { "evaluateSolutionAtTrackedPoints",
    cellam_evaluateSolutionAtTrackedPoints,
   METH_VARARGS, 
   "evaluate solution at tracked points"},
 { "updateExteriorOutflowBoundaryFlux",
    cellam_updateExteriorOutflowBoundaryFlux,
   METH_VARARGS, 
   "approximate outflow boundary integral using trapezoidal rule"},
 { "updateExteriorOutflowBoundaryFluxInGlobalResidual",
    cellam_updateExteriorOutflowBoundaryFluxInGlobalResidual,
   METH_VARARGS, 
   "approximate outflow boundary integral using trapezoidal rule directly in global residual"},
 { "updateExteriorOutflowBoundaryFluxJacobian",
   cellam_updateExteriorOutflowBoundaryFluxJacobian,
   METH_VARARGS, 
   "update jacobian to reflext approximate outflow boundary integral using trapezoidal rule"},
 { "updateExteriorOutflowBoundaryFluxGlobalJacobian",
   cellam_updateExteriorOutflowBoundaryFluxGlobalJacobian,
   METH_VARARGS, 
   "update global jacobian to reflext approximate outflow boundary integral using trapezoidal rule"},
 { "markInflowBoundaryPoints",
   cellam_markInflowBoundaryPoints,
   METH_VARARGS, 
   "mark points that need to be integrated for inflow boundary"},
 { "accumulateInflowFlux",
   cellam_accumulateInflowFlux,
   METH_VARARGS, 
   "apply inflow boundary contribution to local element residual "},
 { "accumulateInflowFluxInGlobalResidual",
   cellam_accumulateInflowFluxInGlobalResidual,
   METH_VARARGS, 
   "apply inflow boundary contribution to residual "},
 { "tagNegligibleIntegrationPoints",
   cellam_tagNegligibleIntegrationPoints,
   METH_VARARGS, 
   "tag integration points with magnitude less than tolerance "},
 { "calculateSlumpedMassApproximation1d",
    cellam_calculateSlumpedMassApproximation1d,
   METH_VARARGS, 
   "calculate 1d slumping approximation from Russell and Binning and apply to residual"},
 { "calculateSlumpedMassApproximation2d",
    cellam_calculateSlumpedMassApproximation2d,
   METH_VARARGS, 
   "calculate 2d slumping approximation for just local mass matrix and apply to residual"},
 { "calculateBerzinsSlumpedMassApproximation1d",
    cellam_calculateBerzinsSlumpedMassApproximation1d,
   METH_VARARGS, 
   "calculate 1d slumping approximation from Berzins and apply to residual"},
 { "updateElementJacobianWithSlumpedMassApproximation",
    cellam_updateElementJacobianWithSlumpedMassApproximation,
   METH_VARARGS, 
   "update jacobian matrix to account for slumping"},
 { "updateElementJacobianWithSlumpedMassCorrection",
    cellam_updateElementJacobianWithSlumpedMassCorrection,
   METH_VARARGS, 
   "update jacobian matrix to account for slumping"},
 { NULL,NULL,0,NULL}
};

extern "C"
{
PyMODINIT_FUNC initcellam(void)
{
  PyObject *m,*d;
  m = Py_InitModule("cellam", cellamMethods);
  d = PyModule_GetDict(m);
  import_array();
}
}//extern "C"
