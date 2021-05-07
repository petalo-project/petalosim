// ----------------------------------------------------------------------------
// petalosim | PetaloRunAction.cc
//
// This is the default run action of the NEXT simulations.
// A message at the beginning and at the end of the simulation is printed.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetaloRunAction.h"

#include "nexus/FactoryBase.h"

#include <G4Run.hh>


REGISTER_CLASS(PetaloRunAction, G4UserRunAction)

PetaloRunAction::PetaloRunAction(): G4UserRunAction()
{
}



PetaloRunAction::~PetaloRunAction()
{
}



void PetaloRunAction::BeginOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
}


void PetaloRunAction::EndOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " end." << G4endl;
}
