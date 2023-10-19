// ----------------------------------------------------------------------------
// petalosim | PetIonizationSD.h
//
// This class is the sensitive detector that creates ionization hits.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PET_IONIZATION_SD_H
#define PET_IONIZATION_SD_H

#include <G4VSensitiveDetector.hh>
#include "nexus/IonizationHit.h"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

  /// Sensitive detector to create ionization hits

  class PetIonizationSD: public G4VSensitiveDetector
  {
  public:
    /// Constructor
    PetIonizationSD(const G4String& sdname);
    /// Destructor
    virtual ~PetIonizationSD();

    /// A hit collection created by this sensitive detector is attached
    /// in this method to the G4HCofThisEvent object.
    virtual void Initialize(G4HCofThisEvent*);

    void EndOfEvent(G4HCofThisEvent*);

    /// Return the unique name of the hits collection created
    /// by this sensitive detector. This will be used by the persistency
    /// manager to fetch the collection from the G4HCofThisEvent object.
    static G4String GetCollectionUniqueName();

    void IncludeInTotalEnergyDeposit(G4bool);

  private:
    ///
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

  private:
    nexus::IonizationHitsCollection* IHC_;
    G4String det_name_;
    G4bool include_;
  };

  inline void PetIonizationSD::IncludeInTotalEnergyDeposit(G4bool inc)
  { include_ = inc; }

#endif
