// ----------------------------------------------------------------------------
// petalosim | PetaloStackingAction.cc
//
// This class is an example of how to implement a stacking action, if needed.
// At the moment, it is not used in the NEXT simulations.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetaloStackingAction.h"

#include "nexus/FactoryBase.h"


REGISTER_CLASS(PetaloStackingAction, G4UserStackingAction)

PetaloStackingAction::PetaloStackingAction(): G4UserStackingAction()
{
}



PetaloStackingAction::~PetaloStackingAction()
{
}



G4ClassificationOfNewTrack
PetaloStackingAction::ClassifyNewTrack(const G4Track* /*track*/)
{
  return fUrgent;
}



void PetaloStackingAction::NewStage()
{
  return;
}



void PetaloStackingAction::PrepareNewEvent()
{
  return;
}
