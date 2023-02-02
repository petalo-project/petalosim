// ----------------------------------------------------------------------------
// petalosim | SiPMHamamatsuBlue.cc
//
// 6x6 mm2 Hamamatsu Blue SiPM geometry.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef SIPM_pet_Blue_H
#define SIPM_pet_Blue_H

#include "nexus/GeometryBase.h"
#include <G4ThreeVector.hh>

class G4GenericMessenger;

using namespace nexus;

class SiPMHamamatsuBlue : public GeometryBase
{
public:
  /// Constructor
  SiPMHamamatsuBlue();
  /// Destructor
  ~SiPMHamamatsuBlue();

  /// Return dimensions of the SiPM
  //G4ThreeVector GetDimensions() const;

  /// Invoke this method to build the volumes of the geometry
  void Construct();

  void SetVisibility(G4bool vis);
  void SetSensorDepth(G4int sensor_depth);
  void SetMotherDepth(G4int mother_depth);
  void SetNamingOrder(G4int naming_order);
  void SetBoxGeom(G4int box_geom);

private:
  G4bool visibility_;

  G4int sensor_depth_;
  G4int mother_depth_;
  G4int naming_order_;
  G4int box_geom_;
};

inline void SiPMHamamatsuBlue::SetVisibility(G4bool vis)
{
  visibility_ = vis;
}

inline void SiPMHamamatsuBlue::SetSensorDepth(G4int sensor_depth)
{
  sensor_depth_ = sensor_depth;
}

inline void SiPMHamamatsuBlue::SetMotherDepth(G4int mother_depth)
{
  mother_depth_ = mother_depth;
}

inline void SiPMHamamatsuBlue::SetNamingOrder(G4int naming_order)
{
  naming_order_ = naming_order;
}

inline void SiPMHamamatsuBlue::SetBoxGeom(G4int box_geom)
{
  box_geom_ = box_geom;
}

#endif
