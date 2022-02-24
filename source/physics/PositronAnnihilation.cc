// ----------------------------------------------------------------------------
// petalosim | PositronAnnihilation.cc
//
// This class implements the process of pair production with
// non collinearity for gammas.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PositronAnnihilation.h"

#include <G4UnitsTable.hh>
#include <globals.hh>
#include <G4PhysicalConstants.hh>
#include <Randomize.hh>
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4eeToTwoGammaModel.hh>

#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>

PositronAnnihilation::PositronAnnihilation(const G4String& name)
  :  G4VEmProcess(name)
  {
    //   theGamma = G4Gamma::Gamma();
    //    theElectron = G4Electron::Electron();
    SetCrossSectionType(fEmDecreasing);
    SetBuildTableFlag(false);
    SetStartFromNullFlag(false);
    //SetSecondaryParticle(theGamma);
    SetProcessSubType(fAnnihilation);
    enableAtRestDoIt = true;
    mainSecondaries = 2;
    //  fEntanglementModelID = G4PhysicsModelCatalog::GetModelID("model_GammaGammaEntanglement");
    //  G4cout << "Constructor" << G4endl;
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
//
// Note : Effects due to binding of atomic electrons are negliged.

{
   fParticleChange.InitializeForPostStep(aTrack);

   fParticleChange.SetNumberOfSecondaries(2) ;

   G4double r  = CLHEP::RandGauss::shoot(0.,0.0011); // hard coded value: it gives
   // a distribution of deviation of collinearity with 0.5 degrees FWHM.

   G4double E1 = electron_mass_c2 + r;
   G4double E2 = electron_mass_c2 - r;

   G4double DeltaTeta = 2*r/electron_mass_c2;

   G4double cosTeta = G4RandFlat::shoot(-1.0, 1.0);
   G4double sinTeta = sqrt(1.-cosTeta*cosTeta);
   G4double Phi     = twopi * G4UniformRand() ;
   G4double Phi1     = (twopi * G4UniformRand())/2. ;
   G4ThreeVector Direction (sinTeta*cos(Phi), sinTeta*sin(Phi), cosTeta);


   G4ThreeVector DirectionPhoton (sin(DeltaTeta)*cos(Phi1),sin(DeltaTeta)*sin(Phi1),cos(DeltaTeta));
   DirectionPhoton.rotateUz(Direction);


   fParticleChange.AddSecondary( new G4DynamicParticle (G4Gamma::Gamma(),
                                                        DirectionPhoton, E1) );
   fParticleChange.AddSecondary( new G4DynamicParticle (G4Gamma::Gamma(),
                                                        -Direction, E2) );

   fParticleChange.ProposeLocalEnergyDeposit(0.);

   // Kill the incident positron
   //
   fParticleChange.ProposeTrackStatus( fStopAndKill );

   return &fParticleChange;

}
