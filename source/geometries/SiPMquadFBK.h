// ----------------------------------------------------------------------------
// petalosim | SiPMquadFBK.h
//
// Specific SiPM geometry proposed by FBK.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef SIPM_quad_FBK_H
#define SIPM_quad_FBK_H

#include "nexus/GeometryBase.h"
#include <G4ThreeVector.hh>

class G4GenericMessenger;

using namespace nexus;

class SiPMquadFBK : public GeometryBase
{
public:
  /// Constructor
  SiPMquadFBK();
  /// Destructor
  ~SiPMquadFBK();

  /// Invoke this method to build the volumes of the geometry
  void Construct();

private:
  // Visibility of the tracking plane
  G4bool visibility_;

  // Optical properties to be used for epoxy
  G4double refr_index_;

  // PDE for the sensor
  G4double eff_;

  // Messenger for the definition of control commands
  G4GenericMessenger *msg_;
};

#endif
