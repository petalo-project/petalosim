// ----------------------------------------------------------------------------
// petalosim | PetaloUtils.h
//
// Functions used in more than one class in the code.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include <G4ThreeVector.hh>


#ifndef PET_UTILS_H
#define PET_UTILS_H

// Given an angle and a vector, the function returns a unitary vector whose
// scalar product with the first vector is equal to the cosine of the angle
G4ThreeVector GenerateNonCollinearVector(G4ThreeVector dir, G4double angle);

// Returns the momentum of the second gamma and the energies of both gammas
// produced in the annihilation of a positron in a body tissue
std::tuple<G4ThreeVector, G4double, G4double> CalculateNonCollinearKinematicInBodyTissue(G4ThreeVector dir);


#endif
