// ----------------------------------------------------------------------------
// petalosim | set-up_filter_transmitance.h
//
// This class is used to obtain the neutral VUV filter transmitance. It is a
// simple set-up with only the filter and a panel to detect photons. 
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef FILTER_TRANS_H
#define FILTER_TRANS_H

#include "nexus/GeometryBase.h"

class G4GenericMessenger;
class G4LogicalVolume;
class G4VPhysicalVolume;

using namespace nexus;

class filter_transmitance : public GeometryBase
{

public:
  // Constructor
  filter_transmitance();
  //Destructor
  ~filter_transmitance();

  /// Generate a vertex within a given region of the geometry
  G4ThreeVector GenerateVertex(const G4String &region) const;

private:
  void Construct();

  G4bool visibility_;
  
  G4ThreeVector specific_vertex_;

 
  /// Messenger for the definition of control commands
  G4GenericMessenger* msg_;

};

#endif
