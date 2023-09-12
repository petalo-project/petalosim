// ----------------------------------------------------------------------------
// petalosim | SiPMHamamatsuVUV.cc
//
// 6x6 mm2 VUV Hamamatsu SiPM geometry.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef SIPM_pet_VUV_new_H
#define SIPM_pet_VUV_new_H

#include "PetaloUtils.h"
#include "nexus/GeometryBase.h"
#include <G4ThreeVector.hh>

class G4GenericMessenger;

using namespace nexus;

class SiPMHamamatsuVUV : public GeometryBase
{
public:
  /// Constructor
  SiPMHamamatsuVUV();
  /// Destructor
  ~SiPMHamamatsuVUV();

  /// Invoke this method to build the volumes of the geometry
  void Construct();

  void SetVisibility(G4bool vis);
  void SetSensorDepth(G4int sensor_depth);
  void SetMotherDepth(G4int mother_depth);
  void SetNamingOrder(G4int naming_order);
  void SetBoxConf(petit_conf box_conf);

private:
  G4bool visibility_;

  G4int sensor_depth_;
  G4int mother_depth_;
  G4int naming_order_;
  petit_conf box_conf_;
};

inline void SiPMHamamatsuVUV::SetVisibility(G4bool vis)
{
  visibility_ = vis;
}

inline void SiPMHamamatsuVUV::SetSensorDepth(G4int sensor_depth)
{
  sensor_depth_ = sensor_depth;
}

inline void SiPMHamamatsuVUV::SetMotherDepth(G4int mother_depth)
{
  mother_depth_ = mother_depth;
}

inline void SiPMHamamatsuVUV::SetNamingOrder(G4int naming_order)
{
  naming_order_ = naming_order;
}

inline void SiPMHamamatsuVUV::SetBoxConf(petit_conf box_conf)
{
  box_conf_ = box_conf;
}

#endif
