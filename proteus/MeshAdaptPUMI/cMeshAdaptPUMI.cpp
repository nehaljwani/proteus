#include <gmi_mesh.h>
#include <gmi_sim.h>
#include <ma.h>
#include <apfMDS.h>
#include <PCU.h>
#include <SimUtil.h>
#include <SimModel.h>

#include "MeshAdaptPUMI.h"

MeshAdaptPUMIDrvr::MeshAdaptPUMIDrvr(double Hmax, double Hmin, int NumIter)
{
  PCU_Comm_Init();
  PCU_Protect();
  Sim_readLicenseFile(0);
  SimModel_start();
  numVar=0;
  hmin=Hmin; hmax=Hmax;
  numIter=NumIter;
  nAdapt=0;
  if(PCU_Comm_Self()==0)
     printf("Setting hmax=%lf, hmin=%lf, numIters(meshadapt)=%d\n",
       hmax, hmin, numIter);
  global[0] = global[1] = global[2] = global[3] = 0;
  local[0] = local[1] = local[2] = local[3] = 0;
  solution = 0;
  size_iso = 0;
  size_scale = 0;
  size_frame = 0;
  err_reg = 0;
  gmi_register_mesh();
  gmi_register_sim();
}

MeshAdaptPUMIDrvr::~MeshAdaptPUMIDrvr()
{
  freeField(solution);
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

int MeshAdaptPUMIDrvr::AdaptPUMIMesh()
{
  get_local_error();
  //CalculateAnisoSizeField();

  for (int d = 0; d <= m->getDimension(); ++d)
    freeNumbering(local[d]);
  /// Adapt the mesh
  freeField(size_iso);
  ma::Input* in = ma::configure(m, size_scale, size_frame);
  ma::validateInput(in);
  in->shouldRunPreParma = true;
  in->shouldRunMidParma = true;
  in->shouldRunPostParma = true;
  in->maximumIterations = numIter;
  in->shouldSnap = false;
  in->shouldFixShape = false;
  in->goodQuality = 0.01; //added
  std::cout<<"Starting (numIter "<<numIter<<")"<<std::endl;
  apf::writeVtkFiles("size", m);
  ma::adapt(in);
  std::cout<<"Finished "<<std::endl;
  freeField(size_frame);
  freeField(size_scale);
  m->verify();
  apf::writeVtkFiles("pumi_adapt", m);
  nAdapt++; //counter for number of adapt steps
  return 0;
}
