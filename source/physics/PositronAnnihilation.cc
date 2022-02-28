// ----------------------------------------------------------------------------
// petalosim | PositronAnnihilation.cc
//
// This class implements the process of pair production with
// non collinearity for gammas.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PositronAnnihilation.h"
#include "PetaloUtils.h"

#include <G4UnitsTable.hh>
#include <globals.hh>
#include <G4PhysicalConstants.hh>
#include <Randomize.hh>
#include <G4RandomDirection.hh>
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4eeToTwoGammaModel.hh>

#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>

PositronAnnihilation::PositronAnnihilation(const G4String& name)
  :  G4VEmProcess(name)
  {
    SetCrossSectionType(fEmDecreasing);
    SetBuildTableFlag(false);
    SetStartFromNullFlag(false);
    SetProcessSubType(fAnnihilation);
    enableAtRestDoIt = true;
    mainSecondaries = 2;
  }


PositronAnnihilation::~PositronAnnihilation() {}

G4bool PositronAnnihilation::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Positron::Positron());
}

G4double PositronAnnihilation::AtRestGetPhysicalInteractionLength(const G4Track&, G4ForceCondition* condition)
{
  *condition = NotForced;
  return 0.0;
}

void PositronAnnihilation::InitialiseProcess(const G4ParticleDefinition*)
 {
   if(!isInitialised) {
     isInitialised = true;
     if(nullptr == EmModel(0)) { SetEmModel(new G4eeToTwoGammaModel()); }
     EmModel(0)->SetLowEnergyLimit(MinKinEnergy());
     EmModel(0)->SetHighEnergyLimit(MaxKinEnergy());
     AddEmModel(1, EmModel(0));
   }
 }

G4VParticleChange* PositronAnnihilation::AtRestDoIt(const G4Track& aTrack,
                                                    const G4Step&  )
//
// Performs the e+ e- annihilation when both particles are assumed at rest.
// It generates two photons almost back-to-back, according to a given
// spread in the angle between the photons. The energy of the photons also
// deviates from the electron_mass, due to Doppler shift.
// The angular distribution is isotropic.
// Caveat: the positron annihilates always at rest, therefore, the 4-momentum
// is not conserved.
//
// Note : Effects due to binding of atomic electrons are negliged.

{
   fParticleChange.InitializeForPostStep(aTrack);

   fParticleChange.SetNumberOfSecondaries(2) ;

   auto  dir1          = G4RandomDirection();
   auto [dir2, e1, e2] = CalculateNonCollinearKinematicInBodyTissue(dir1);

   fParticleChange.AddSecondary(new G4DynamicParticle (G4Gamma::Gamma(),
                                                       dir1, e1) );
   fParticleChange.AddSecondary(new G4DynamicParticle (G4Gamma::Gamma(),
                                                       -dir2, e2) );

   fParticleChange.ProposeLocalEnergyDeposit(0.);

   // Kill the incident positron
   fParticleChange.ProposeTrackStatus( fStopAndKill );

   return &fParticleChange;

}
