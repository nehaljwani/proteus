#include <gmi_mesh.h>
#include <gmi_sim.h>
#include <ma.h>
#include <maShape.h>
#include <apfMDS.h>
#include <PCU.h>
#include <SimUtil.h>
#include <SimModel.h>

#include "MeshAdaptPUMI.h"

MeshAdaptPUMIDrvr::MeshAdaptPUMIDrvr(double Hmax, double Hmin, int NumIter,
    const char* sfConfig)
{
  m = 0;
  PCU_Comm_Init();
  PCU_Protect();
  Sim_readLicenseFile(0);
  SimModel_start();
  hmin=Hmin; hmax=Hmax;
  numIter=NumIter;
  nAdapt=0;
  if(PCU_Comm_Self()==0)
     printf("MeshAdapt: Setting hmax=%lf, hmin=%lf, numIters(meshadapt)=%d\n",
       hmax, hmin, numIter);
  global[0] = global[1] = global[2] = global[3] = 0;
  local[0] = local[1] = local[2] = local[3] = 0;
  size_iso = 0;
  size_scale = 0;
  size_frame = 0;
  err_reg = 0;
  gmi_register_mesh();
  gmi_register_sim();
  approximation_order = 2;
  integration_order = 3;//approximation_order * 2;
  exteriorGlobaltoLocalElementBoundariesArray = NULL;
  size_field_config = sfConfig;
}

MeshAdaptPUMIDrvr::~MeshAdaptPUMIDrvr()
{
  freeField(size_iso);
  freeField(size_scale);
  freeField(size_frame);
  SimModel_stop();
  Sim_unregisterAllKeys();
}

int MeshAdaptPUMIDrvr::loadModelAndMesh(const char* modelFile, const char* meshFile)
{
  m = apf::loadMdsMesh(modelFile, meshFile);
  m->verify();
  comm_size = PCU_Comm_Peers();
  comm_rank = PCU_Comm_Self();
  return 0;
}

int MeshAdaptPUMIDrvr::adaptPUMIMesh()
{
  if (size_field_config == "farhad")
    calculateAnisoSizeField();
  else if (size_field_config == "alvin")
    get_local_error();
  else {
    std::cerr << "unknown size field config " << size_field_config << '\n';
    abort();
  }
  //m->destroyTag(fluxtag[1]); m->destroyTag(fluxtag[2]); m->destroyTag(fluxtag[3]);
  delete [] exteriorGlobaltoLocalElementBoundariesArray;
  exteriorGlobaltoLocalElementBoundariesArray = NULL;
  assert(size_iso == 0);
  for (int d = 0; d <= m->getDimension(); ++d)
    freeNumbering(local[d]);
  /// Adapt the mesh
  ma::Input* in = ma::configure(m, size_scale, size_frame);
  ma::validateInput(in);
  in->shouldRunPreParma = true;
  in->shouldRunMidParma = true;
  in->shouldRunPostParma = true;
  in->maximumIterations = numIter;
  in->shouldSnap = false;
  in->shouldFixShape = true;
  double mass_before = getTotalMass();
  ma::adapt(in);
  freeField(size_frame);
  freeField(size_scale);
  m->verify();
  double mass_after = getTotalMass();
  std::cout<<std::setprecision(15)<<"Before "<<mass_before<<" After "<<mass_after<<" diff "<<mass_after-mass_before<<std::endl;
  nAdapt++; //counter for number of adapt steps
  return 0;
}

double MeshAdaptPUMIDrvr::getMinimumQuality()
{
  ma::SizeField* isf = new ma::IdentitySizeField(m);
  apf::MeshIterator* it = m->begin(m->getDimension());
  apf::MeshEntity* e;
  double minq = 1;
  while ((e = m->iterate(it)))
    minq = std::min(minq, ma::measureElementQuality(m, isf, e));
  m->end(it);
  delete isf;
  return PCU_Min_Double(minq);
}

double MeshAdaptPUMIDrvr::getTotalMass()
{
  apf::Field* voff = m->findField("vof");
  assert(voff);
  apf::MeshEntity* e;
  apf::MeshIterator* it = m->begin(m->getDimension());
  double mass = 0.0;
  while ((e = m->iterate(it))) {
    apf::MeshElement* elem = apf::createMeshElement(m,e);
    apf::Element* voff_elem = apf::createElement(voff, elem);
    int int_order = 4;
    for(int l = 0; l < apf::countIntPoints(elem, int_order); ++l) {
      apf::Vector3 qpt;
      apf::getIntPoint(elem,int_order,l,qpt);
      double vof_val = apf::getScalar(voff_elem,qpt);
      double rho_val = getMPvalue(vof_val,rho[0],rho[1]);
      double weight = apf::getIntWeight(elem,int_order,l);
      apf::Matrix3x3 J;
      apf::getJacobian(elem,qpt,J); //evaluate the Jacobian at the quadrature point
      double Jdet = apf::getJacobianDeterminant(J,m->getDimension());
      mass += rho_val*weight*Jdet;
    }
    apf::destroyElement(voff_elem);
    apf::destroyMeshElement(elem);
  }
  m->end(it);
  return mass;
}
