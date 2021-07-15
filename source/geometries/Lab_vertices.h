// ----------------------------------------------------------------------------
// petalosim | Lab_vertices.h
//
// This class consists of two LXe cells placed opposite to each other.
// A file is expected to be read with the positions of the vertices of the gamma
// interactions in each cell.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef LAB_V_H
#define LAB_V_H

#include "nexus/GeometryBase.h"

#include <TFile.h>

class G4GenericMessenger;
class Pet2boxes;
class PetLYSObox;
class PetLXeCell;
class PetLYSOCell;

using namespace nexus;

class Lab_vertices : public GeometryBase
{
public:
  /// Constructor
  Lab_vertices();
  /// Destructor
  ~Lab_vertices();

  /// Return vertex within region <region> of the chamber
  virtual G4ThreeVector GenerateVertex(const G4String &region) const;

  virtual void Construct();

private:
  /// Messenger for the definition of control commands
  G4GenericMessenger *msg_;

  PetLXeCell *module_;

  TFile *file_;
  G4float px1_, py1_, pz1_, px2_, py2_, pz2_;
  mutable G4int index_;
  mutable std::vector<G4ThreeVector> vertices1_;
  mutable std::vector<G4ThreeVector> vertices2_;

  G4int starting_point_;
  G4String filename_;
  G4String type_;
};

#endif
