// ----------------------------------------------------------------------------
// petalosim | SiPMFBKVUV.cc
//
// 3x3 mm2 FBK SiPM geometry.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef SIPM_pet_FBK_new_H
#define SIPM_pet_FBK_new_H

#include "nexus/GeometryBase.h"
#include <G4ThreeVector.hh>

class G4GenericMessenger;

using namespace nexus;

class SiPMFBKVUV : public GeometryBase
{
public:
  /// Constructor
  SiPMFBKVUV();
  /// Destructor
  ~SiPMFBKVUV();

  /// Invoke this method to build the volumes of the geometry
  void Construct();

  void SetVisibility(G4bool vis);
  void SetPDE(G4double eff);
  void SetSensorDepth(G4int sensor_depth);
  void SetMotherDepth(G4int mother_depth);
  void SetNamingOrder(G4int naming_order);
  void SetBoxGeom(G4int box_geom);

private:
  G4bool visibility_;

  // PDE for the sensor
  G4double eff_;

  G4int sensor_depth_;
  G4int mother_depth_;
  G4int naming_order_;
  G4int box_geom_;
};

inline void SiPMFBKVUV::SetVisibility(G4bool vis)
{
  visibility_ = vis;
}

inline void SiPMFBKVUV::SetPDE(G4double eff)
{
  eff_ = eff;
}

inline void SiPMFBKVUV::SetSensorDepth(G4int sensor_depth)
{
  sensor_depth_ = sensor_depth;
}

inline void SiPMFBKVUV::SetMotherDepth(G4int mother_depth)
{
  mother_depth_ = mother_depth;
}

inline void SiPMFBKVUV::SetNamingOrder(G4int naming_order)
{
  naming_order_ = naming_order;
}

inline void SiPMFBKVUV::SetBoxGeom(G4int box_geom)
{
  box_geom_ = box_geom;
}

#endif
