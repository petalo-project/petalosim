// ----------------------------------------------------------------------------
// petalosim | PositronAnnihilation.h
//
// This class implements the process of pair production with
// non collinearity for gammas.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef POS_ANNIHIL_H
#define POS_ANNIHIL_H

#include <G4VEmProcess.hh>

class G4ParticleDefinition;

class PositronAnnihilation : public G4VEmProcess

{
 public:

  PositronAnnihilation(const G4String& name = "pos_annihil");
  ~PositronAnnihilation() override;

  G4bool IsApplicable(const G4ParticleDefinition& p) final;

  G4VParticleChange* AtRestDoIt(const G4Track& aTrack,
                                const G4Step& aStep) override;

  G4double AtRestGetPhysicalInteractionLength(const G4Track& track,
                                              G4ForceCondition* condition
                                               ) override;

  PositronAnnihilation & operator=(const PositronAnnihilation &right) = delete;
  PositronAnnihilation(const PositronAnnihilation&) = delete;

  protected:

  void InitialiseProcess(const G4ParticleDefinition*) override;

  private:

  G4bool isInitialised = false;
};

#endif
