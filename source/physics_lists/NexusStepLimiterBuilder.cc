// $Id$
//
//---------------------------------------------------------------------------
//
// ClassName:   G4StepLimiterBuilder
//
// Author:      V.Ivanchenko 24.11.2004
//
// Modified:


#include "NexusStepLimiterBuilder.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(NexusStepLimiterBuilder);


NexusStepLimiterBuilder::NexusStepLimiterBuilder(const G4String& name)
   :  G4VPhysicsConstructor(name)
{
  fStepLimiter = new G4StepLimiter();
  fUserSpecialCuts = new G4UserSpecialCuts();
}


NexusStepLimiterBuilder::~NexusStepLimiterBuilder()
{
  delete fStepLimiter;
  delete fUserSpecialCuts;
}


void NexusStepLimiterBuilder::ConstructParticle()
{}


void NexusStepLimiterBuilder::ConstructProcess()
{
  theParticleIterator->reset();

  while ((*theParticleIterator)()) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4double charge = particle->GetPDGCharge();

    // if(!particle->IsShortLived()) {
    //   if (charge != 0.0) {
    // 	// All charged particles should have a step limiter
    // 	// to make sure that the steps do not get too long.
    // 	pmanager->AddDiscreteProcess(fStepLimiter);
    // 	pmanager->AddDiscreteProcess(fUserSpecialCuts);
    //   } else {
    // 	// Energy cuts for all other neutral particles
    // 	pmanager->AddDiscreteProcess(fUserSpecialCuts);
    //   }
    if (particle->GetParticleName()=="e-") {
       pmanager->AddDiscreteProcess(fStepLimiter);
	pmanager->AddDiscreteProcess(fUserSpecialCuts);
    }

  }
}



