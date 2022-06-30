// ----------------------------------------------------------------------------
// petalosim | JaszczakPhantom.h
//
// This class implements the geometry of a Jaszczak phantom, filled with water.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef JASZCZAK_PHANTOM_H
#define JASZCZAK_PHANTOM_H

#include "nexus/GeometryBase.h"

class G4Material;

namespace nexus
{
  class SpherePointSampler;
  class CylinderPointSampler2020;
}

using namespace nexus;

class JaszczakPhantom: public GeometryBase
{

 public:
  JaszczakPhantom();
  ~JaszczakPhantom();

  void Construct();

 private:

  void BuildRods(unsigned long n, G4double r, G4double z_pos,
                 G4LogicalVolume* mother_logic, G4Material* mat) const;

  SpherePointSampler* spheric_gen_;
  CylinderPointSampler2020* cyl_gen_;

  G4double cylinder_inner_diam_;
  G4double cylinder_height_;
  G4double cylinder_thickn_;

  G4double sphere1_d_;
  G4double sphere2_d_;
  G4double sphere3_d_;
  G4double sphere4_d_;
  G4double sphere5_d_;
  G4double sphere6_d_;
  G4double sphere_height_;

  G4double rod1_d_;
  G4double rod2_d_;
  G4double rod3_d_;
  G4double rod4_d_;
  G4double rod5_d_;
  G4double rod6_d_;
  G4double rod_height_;

};

#endif
