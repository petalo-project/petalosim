// ----------------------------------------------------------------------------
// petalosim | PetSensorsEventAction.cc
//
// This class saves in the nexus output file only events with any detected
// charge by the sensors and deposited energy larger than 0.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetSensorsEventAction.h"
#include "PetaloPersistencyManager.h"

#include "nexus/Trajectory.h"
#include "nexus/FactoryBase.h"
#include "nexus/SensorSD.h"

#include <G4Event.hh>
#include <G4VVisManager.hh>
#include <G4Trajectory.hh>
#include <G4GenericMessenger.hh>
#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4HCtable.hh>
#include <globals.hh>

using namespace nexus;

REGISTER_CLASS(PetSensorsEventAction, G4UserEventAction)

PetSensorsEventAction::PetSensorsEventAction() : G4UserEventAction(),
                                                 nevt_(0),
                                                 nupdate_(10),
                                                 energy_threshold_(0.),
                                                 energy_max_(DBL_MAX),
                                                 min_charge_(0)
{
  msg_ = new G4GenericMessenger(this, "/Actions/PetSensorsEventAction/");

  G4GenericMessenger::Command &thresh_cmd =
      msg_->DeclareProperty("energy_threshold", energy_threshold_,
                            "Minimum deposited energy to save the event to file.");
  thresh_cmd.SetParameterName("energy_threshold", true);
  thresh_cmd.SetUnitCategory("Energy");
  thresh_cmd.SetRange("energy_threshold>0.");

  G4GenericMessenger::Command &max_energy_cmd =
      msg_->DeclareProperty("max_energy", energy_max_,
                            "Maximum deposited energy to save the event to file.");
  max_energy_cmd.SetParameterName("max_energy", true);
  max_energy_cmd.SetUnitCategory("Energy");
  max_energy_cmd.SetRange("max_energy>0.");

  msg_->DeclareProperty("min_charge", min_charge_,
    "Minimum charge detected to save the event to file.");
}

PetSensorsEventAction::~PetSensorsEventAction()
{
}

void PetSensorsEventAction::BeginOfEventAction(const G4Event * /*event*/)
{
  // Print out event number info
  if ((nevt_ % nupdate_) == 0)
  {
    G4cout << " >> Event no. " << nevt_ << G4endl;
    if (nevt_ == (10 * nupdate_))
      nupdate_ *= 10;
  }
}

void PetSensorsEventAction::EndOfEventAction(const G4Event *event)
{
  nevt_++;

  // Determine whether total energy deposit in ionization sensitive
  // detectors is above threshold
  if (energy_threshold_ >= 0.)
  {

    // Get the trajectories stored for this event and loop through them
    // to calculate the total energy deposit

    G4double edep = 0.;

    G4TrajectoryContainer *tc = event->GetTrajectoryContainer();
    if (tc)
    {
      for (unsigned int i = 0; i < tc->size(); ++i)
      {
        Trajectory *trj = dynamic_cast<Trajectory *>((*tc)[i]);
        edep += trj->GetEnergyDeposit();
        // Draw tracks in visual mode
        if (G4VVisManager::GetConcreteInstance())
          trj->DrawTrajectory();
      }
    }

    G4bool any_charge_in_sns = false;
    G4SDManager* sdmgr = G4SDManager::GetSDMpointer();
    G4HCtable* hct = sdmgr->GetHCtable();

    G4HCofThisEvent* hc = event->GetHCofThisEvent();

    // Loop through the hits collections
    for (int i=0; i<hct->entries(); i++) {

      // Collection are identified univocally (in principle) using
      // their id number, and this can be obtained using the collection
      // and sensitive detector names.
      G4String hcname = hct->GetHCname(i);
      G4String sdname = hct->GetSDname(i);
      int hcid = sdmgr->GetCollectionID(sdname+"/"+hcname);

      if (hcname == SensorSD::GetCollectionUniqueName()){
        G4VHitsCollection* SensHits = hc->GetHC(hcid);
        SensorHitsCollection* hits = dynamic_cast<SensorHitsCollection*>(SensHits);
        size_t sns_hit_size = hits->GetSize();
        if (sns_hit_size>0)
        {
          any_charge_in_sns = true;
        }
      }
    }


    PetaloPersistencyManager *pm = dynamic_cast<PetaloPersistencyManager *>(G4VPersistencyManager::GetPersistencyManager());

    // if (edep > _energy_threshold) pm->StoreCurrentEvent(true);
    // else pm->StoreCurrentEvent(false);
    if (!event->IsAborted() && edep > 0)
    {
      pm->InteractingEvent(true);
    }
    else
    {
      pm->InteractingEvent(false);
    }
    if (!event->IsAborted() && any_charge_in_sns && charge_above_th && edep > energy_threshold_ && edep < energy_max_)
    {
      pm->StoreCurrentEvent(true);
    }
    else
    {
      pm->StoreCurrentEvent(false);
    }
  }
}
