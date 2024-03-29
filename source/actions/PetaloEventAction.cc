// ----------------------------------------------------------------------------
// petalosim | PetaloEventAction.cc
//
// This is the default event action of the PETALO simulations. Only events with
// deposited energy larger than 0 are saved in the nexus output file.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetaloEventAction.h"
#include "PetaloPersistencyManager.h"

#include "nexus/Trajectory.h"
#include "nexus/FactoryBase.h"

#include <G4Event.hh>
#include <G4VVisManager.hh>
#include <G4Trajectory.hh>
#include <G4GenericMessenger.hh>
#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4HCtable.hh>
#include <globals.hh>

using namespace nexus;

REGISTER_CLASS(PetaloEventAction, G4UserEventAction)

PetaloEventAction::PetaloEventAction(): G4UserEventAction(), nevt_(0),
  nupdate_(10), min_energy_(0.), max_energy_(DBL_MAX)
{
  msg_ = new G4GenericMessenger(this, "/Actions/PetaloEventAction/");

  G4GenericMessenger::Command &thresh_cmd =
      msg_->DeclareProperty("min_energy", min_energy_,
                            "Minimum deposited energy to save the event to file.");
  thresh_cmd.SetParameterName("min_energy", true);
  thresh_cmd.SetUnitCategory("Energy");
  thresh_cmd.SetRange("min_energy>0.");

  G4GenericMessenger::Command &max_energy_cmd =
      msg_->DeclareProperty("max_energy", max_energy_,
                            "Maximum deposited energy to save the event to file.");
  max_energy_cmd.SetParameterName("max_energy", true);
  max_energy_cmd.SetUnitCategory("Energy");
  max_energy_cmd.SetRange("max_energy>0.");

  PetaloPersistencyManager* pm = dynamic_cast<PetaloPersistencyManager*>
    (G4VPersistencyManager::GetPersistencyManager());

  pm->SaveNumbOfInteractingEvents(true);
}

PetaloEventAction::~PetaloEventAction()
{
}

void PetaloEventAction::BeginOfEventAction(const G4Event* /*event*/)
{
  // Print out event number info
  if ((nevt_ % nupdate_) == 0)
  {
    G4cout << " >> Event no. " << nevt_ << G4endl;
    if (nevt_ == (10 * nupdate_))
      nupdate_ *= 10;
  }
}

void PetaloEventAction::EndOfEventAction(const G4Event* event)
{
  nevt_++;

  // Determine whether total energy deposit in ionization sensitive
  // detectors is above threshold
  if (min_energy_ >= 0.)
  {

    // Get the trajectories stored for this event and loop through them
    // to calculate the total energy deposit

    G4double edep = 0.;

    G4TrajectoryContainer* tc = event->GetTrajectoryContainer();
    if (tc) {
      for (unsigned int i = 0; i < tc->size(); ++i) {
        Trajectory* trj = dynamic_cast<Trajectory *>((*tc)[i]);
        edep += trj->GetEnergyDeposit();
      }
    }

    PetaloPersistencyManager* pm =
      dynamic_cast<PetaloPersistencyManager*>(G4VPersistencyManager::GetPersistencyManager());

    if (!event->IsAborted() && edep > 0)
    {
      pm->InteractingEvent(true);
    }
    else
    {
      pm->InteractingEvent(false);
    }
    if (!event->IsAborted() && edep > min_energy_ && edep < max_energy_)
    {
      pm->StoreCurrentEvent(true);
    }
    else
    {
      pm->StoreCurrentEvent(false);
    }
  }
}
