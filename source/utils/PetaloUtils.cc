// ----------------------------------------------------------------------------
// petalosim | PetaloUtils.cc
//
// Functions used in more than one class in the code.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetaloUtils.h"

#include <Randomize.hh>

#include <CLHEP/Units/PhysicalConstants.h>

using namespace CLHEP;

G4ThreeVector GenerateNonCollinearVector(G4ThreeVector dir, G4double angle)
{
  G4double Phi = (twopi * G4UniformRand())/2. ;
  G4ThreeVector non_coll_dir(sin(angle)*cos(Phi), sin(angle)*sin(Phi),
                             cos(angle));
  non_coll_dir.rotateUz(dir.unit());
  return non_coll_dir;
}

std::tuple<G4ThreeVector, G4double, G4double> CalculateNonCollinearKinematicInBodyTissue(G4ThreeVector dir)
{
  G4double r = G4RandGauss::shoot(0., 0.00099); // It gives
  // a distribution of deviation of collinearity with 0.54 degrees FWHM.
  // Value taken from "Limit of Spatial Resolution in FDG-PET due to
  // Annihilation Photon Non-Collinearity", Shibuya, K. et al.
  // DOI: 10.1007/978-3-540-36841-0_411

  G4double DeltaTheta = 2*r/electron_mass_c2;
  G4ThreeVector dir2  = GenerateNonCollinearVector(dir, DeltaTheta);

  G4double e1 = electron_mass_c2 + r;
  G4double e2 = electron_mass_c2 - r;

  return std::make_tuple(dir2, e1, e2);
}
