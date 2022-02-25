// ----------------------------------------------------------------------------
// nexus | PetaloUtils.h
//
// Functions used in more than one class in the code.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include <G4ThreeVector.hh>


#ifndef PET_UTILS_H
#define PET_UTILS_H

// Returns a unit vector in a direction at a given angle from a given direction
G4ThreeVector GenerateNonCollinearVector(G4ThreeVector dir, G4double angle);

// Returns the momentum of the second gamma and the energies of both gammas
// produced in the annihilation of a positron in a body tissue
std::tuple<G4ThreeVector, G4double, G4double> CalculateNonCollinearKinematicInBodyTissue(G4ThreeVector dir);


#endif
