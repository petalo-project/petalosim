// ----------------------------------------------------------------------------
// petalosim | PetIonizationSD.h
//
// This class is the sensitive detector that creates ionization hits.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetIonizationSD.h"

#include "nexus/IonizationHit.h"
#include "nexus/Trajectory.h"
#include "nexus/TrajectoryMap.h"

#include <G4SDManager.hh>
#include <G4Step.hh>
#include <G4OpticalPhoton.hh>

using namespace nexus;


PetIonizationSD::PetIonizationSD(const G4String& name):
  G4VSensitiveDetector(name), include_(true)
{
  collectionName.insert(GetCollectionUniqueName());
}


PetIonizationSD::~PetIonizationSD()
{
}


G4String PetIonizationSD::GetCollectionUniqueName()
{
  G4String name = "PetIonizationHitsCollection";
  return name;
}


void PetIonizationSD::Initialize(G4HCofThisEvent* hce)
{
  // Create a collection of ionization hits and add it to
  // the collection of hits of the event

  IHC_ =
    new IonizationHitsCollection(SensitiveDetectorName, collectionName[0]);

  G4int hcid =
    G4SDManager::GetSDMpointer()->GetCollectionID(SensitiveDetectorName+"/"+collectionName[0]);
  hce->AddHitsCollection(hcid, IHC_);

}


G4bool PetIonizationSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  G4Track* track = step->GetTrack();

  if (track->GetParticleDefinition() == G4OpticalPhoton::Definition())
    return false;

  G4double edep = step->GetTotalEnergyDeposit();

  // Discard steps where no energy was deposited in the detector
  if (edep <= 0.) return false;

  // Create a hit and set its properties
  IonizationHit* hit = new IonizationHit();
  hit->SetTrackID(step->GetTrack()->GetTrackID());
  hit->SetTime(step->GetTrack()->GetGlobalTime());
  hit->SetEnergyDeposit(edep);
  hit->SetPosition(step->GetPostStepPoint()->GetPosition());

  // Add hit to collection
  IHC_->insert(hit);

  // Add energy deposit to the trajectory associated
  // to the current track
  if (include_) {
    Trajectory* trj =
      (Trajectory*) TrajectoryMap::Get(step->GetTrack()->GetTrackID());
    if (trj) {
      edep += trj->GetEnergyDeposit();
      trj->SetEnergyDeposit(edep);
    }
  }

  return true;
}


void PetIonizationSD::EndOfEvent(G4HCofThisEvent*)
{
}
