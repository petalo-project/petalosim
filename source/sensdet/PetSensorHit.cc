// ----------------------------------------------------------------------------
// petalosim | PetSensorHit.cc
//
// This class describes the charge detected by a photosensor.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetSensorHit.h"


G4Allocator<PetSensorHit> PetSensorHitAllocator;



PetSensorHit::PetSensorHit():
  G4VHit(), counts_(0), sns_id_(-1.)
{
}



PetSensorHit::PetSensorHit(G4int id, const G4ThreeVector& position):
  G4VHit(), counts_(0), sns_id_(id), position_(position)
{
}



PetSensorHit::~PetSensorHit()
{
}



PetSensorHit::PetSensorHit(const PetSensorHit& other): G4VHit()
{
  *this = other;
}



const PetSensorHit& PetSensorHit::operator=(const PetSensorHit& other)
{
  sns_id_    = other.sns_id_;
  position_  = other.position_;
  counts_    = other.counts_;
  phot_      = other.phot_;

  return *this;
}



G4int PetSensorHit::operator==(const PetSensorHit& other) const
{
  return (this==&other) ? 1 : 0;
}



void PetSensorHit::AddPhoton(G4double time, G4int track_id)
{
  phot_[time] = track_id;
}

