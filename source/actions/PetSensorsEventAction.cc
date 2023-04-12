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
#include "ToFSD.h"

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

REGISTER_CLASS(PetSensorsEventAction, G4UserEventAction)

PetSensorsEventAction::PetSensorsEventAction() : G4UserEventAction(),
                                                 nevt_(0),
                                                 nupdate_(10),
                                                 min_energy_(0.),
                                                 max_energy_(DBL_MAX),
                                                 min_charge_(0)
{
  msg_ = new G4GenericMessenger(this, "/Actions/PetSensorsEventAction/");

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
  if (min_energy_ >= 0.)
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

    G4bool charge_above_th = false;
    G4double amplitude = 0.;

    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    G4SDManager* sdmgr = G4SDManager::GetSDMpointer();
    G4HCtable* hct = sdmgr->GetHCtable();

    // Get only the PetSensorHitsCollection --> 1
    G4String hcname = hct->GetHCname(1);
    G4String sdname = hct->GetSDname(1);
    int hcid = sdmgr->GetCollectionID(sdname+"/"+hcname);

    if (hcname == ToFSD::GetCollectionUniqueName()){
      G4VHitsCollection* SensHits = hce->GetHC(hcid);
      PetSensorHitsCollection* hits = dynamic_cast<PetSensorHitsCollection*>(SensHits);
      if (!hits) return;
      for (size_t i=0; i<hits->entries(); i++) {
        PetSensorHit* hit = dynamic_cast<PetSensorHit*>(hits->GetHit(i));
        if (!hit) continue;

        amplitude += hit->GetDetPhotons();
      }

      if (amplitude>min_charge_){
        charge_above_th = true;
      }
    }

    PetaloPersistencyManager *pm = dynamic_cast<PetaloPersistencyManager *>(G4VPersistencyManager::GetPersistencyManager());

    if (!event->IsAborted() && edep > 0)
    {
      pm->InteractingEvent(true);
    }
    else
    {
      pm->InteractingEvent(false);
    }
    if (!event->IsAborted() && charge_above_th && edep > min_energy_ && edep < max_energy_)
    {
      pm->StoreCurrentEvent(true);
    }
    else
    {
      pm->StoreCurrentEvent(false);
    }
  }
}
