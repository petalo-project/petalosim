// ----------------------------------------------------------------------------
// petalosim | PetaloPersistencyManager.cc
//
// This class writes all the relevant information of the simulation
// to an ouput file.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetaloPersistencyManager.h"
#include "HDF5Writer.h"
#include "ToFSD.h"
#include "ChargeSD.h"
#include "PetSaveAllSteppingAction.h"

#include "nexus/Trajectory.h"
#include "nexus/TrajectoryMap.h"
#include "nexus/IonizationSD.h"
#include "nexus/NexusApp.h"
#include "nexus/DetectorConstruction.h"
#include "nexus/FactoryBase.h"

#include <G4GenericMessenger.hh>
#include <G4Event.hh>
#include <G4TrajectoryContainer.hh>
#include <G4Trajectory.hh>
#include <G4SDManager.hh>
#include <G4HCtable.hh>
#include <G4RunManager.hh>
#include <G4Run.hh>
#include <G4OpticalPhoton.hh>

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <CLHEP/Units/SystemOfUnits.h>

using namespace nexus;
using namespace CLHEP;

REGISTER_CLASS(PetaloPersistencyManager, PersistencyManagerBase)

PetaloPersistencyManager::PetaloPersistencyManager():
  PersistencyManagerBase(), msg_(0), output_file_("petalo_out"),
  store_evt_(true), store_steps_(false),
  interacting_evt_(false), save_int_e_numb_(false),
  efield_(0), event_type_("other"),
  saved_evts_(0), interacting_evts_(0), nevt_(0), start_id_(0), first_evt_(true),
  thr_charge_(0), tof_time_(50.*nanosecond), sns_only_(false),
  save_tot_charge_(true), sipm_cells_(false), h5writer_(0)
{
  msg_ = new G4GenericMessenger(this, "/petalosim/persistency/");
  msg_->DeclareProperty("output_file", output_file_, "Path of output file.");
  msg_->DeclareProperty("eventType", event_type_,
                        "Type of event: bb0nu, bb2nu or background.");
  msg_->DeclareProperty("start_id", start_id_,
                        "Starting event ID for this job.");
  msg_->DeclareProperty("thr_charge", thr_charge_, "Threshold for the charge saved in file.");
  msg_->DeclareProperty("sns_only", sns_only_, "If true, no true information is saved.");
  msg_->DeclareProperty("save_tot_charge", save_tot_charge_, "If true, total charge is saved.");
  msg_->DeclareProperty("sipm_cells", sipm_cells_,
                        "True if each individual cell of SiPMs is simulated.");

  G4GenericMessenger::Command& time_cmd =
    msg_->DeclareProperty("tof_time", tof_time_, "Time saved in tof table per sensor");
  time_cmd.SetUnitCategory("Time");
  time_cmd.SetParameterName("tof_time", false);
  time_cmd.SetRange("tof_time>0.");

  init_macro_ = "";
  macros_.clear();
  delayed_macros_.clear();
  secondary_macros_.clear();
}



PetaloPersistencyManager::~PetaloPersistencyManager()
{
  delete msg_;
  delete h5writer_;
}



void PetaloPersistencyManager::OpenFile()
{
  h5writer_ = new HDF5Writer();
  G4String hdf5file = output_file_ + ".h5";
  h5writer_->Open(hdf5file, store_steps_);
  return;
}



void PetaloPersistencyManager::CloseFile()
{
  h5writer_->Close();
  return;
}



G4bool PetaloPersistencyManager::Store(const G4Event* event)
{
  if (interacting_evt_) {
    interacting_evts_++;
  }

  if (!store_evt_) {
    TrajectoryMap::Clear();
    if (store_steps_) {
      PetSaveAllSteppingAction* sa = (PetSaveAllSteppingAction*)
        G4RunManager::GetRunManager()->GetUserSteppingAction();
      sa->Reset();
    }
    return false;
  }

  saved_evts_++;


  if (first_evt_) {
    first_evt_ = false;
    nevt_ = start_id_;
  }

  if (store_steps_)
    StoreSteps();

  if (sns_only_ == false) {
    StoreTrajectories(event->GetTrajectoryContainer());
  }

  StoreHits(event->GetHCofThisEvent());

  nevt_++;

  TrajectoryMap::Clear();
  StoreCurrentEvent(true);

  return true;
}



void PetaloPersistencyManager::StoreTrajectories(G4TrajectoryContainer* tc)
{

  // If the pointer is null, no trajectories were stored in this event
  if (!tc) return;

  // Loop through the trajectories stored in the container
  for (size_t i=0; i<tc->entries(); ++i) {
    Trajectory* trj = dynamic_cast<Trajectory*>((*tc)[i]);
    if (!trj) continue;

    G4bool save_opt_phot = false;
    std::ifstream init_read(init_macro_, std::ifstream::in);

    while (init_read.good()) {
      std::string key, value;
      std::getline(init_read, key, ' ');
      std::getline(init_read, value);
      if ((key == "/nexus/RegisterTrackingAction") && (value == "OpticalTrackingAction")) {
        save_opt_phot = true;
        break;
      }
    }

    if ((trj->GetParticleDefinition() == G4OpticalPhoton::Definition()) &&
        (save_opt_phot == false)) {
      continue;
    }

    G4int trackid = trj->GetTrackID();

    G4double length = trj->GetTrackLength();

    G4ThreeVector ini_xyz = trj->GetInitialPosition();
    G4double ini_t = trj->GetInitialTime();
    G4ThreeVector final_xyz = trj->GetFinalPosition();
    G4double final_t = trj->GetFinalTime();

    G4String ini_volume = trj->GetInitialVolume();
    G4String final_volume = trj->GetFinalVolume();

    G4double mass = trj->GetParticleDefinition()->GetPDGMass();
    G4ThreeVector ini_mom = trj->GetInitialMomentum();
    G4double energy = sqrt(ini_mom.mag2() + mass*mass);
    G4ThreeVector final_mom = trj->GetFinalMomentum();

    float kin_energy = energy - mass;
    char primary = 0;
    G4int mother_id = 0;
    if (!trj->GetParentID()) {
      primary = 1;
    } else {
      mother_id = trj->GetParentID();
    }

    h5writer_->WriteParticleInfo(nevt_, trackid, trj->GetParticleName().c_str(),
				 primary, mother_id,
				 (float)ini_xyz.x(), (float)ini_xyz.y(),
                                 (float)ini_xyz.z(), (float)ini_t,
				 (float)final_xyz.x(), (float)final_xyz.y(),
                                 (float)final_xyz.z(), (float)final_t,
				 ini_volume.c_str(), final_volume.c_str(),
				 (float)ini_mom.x(), (float)ini_mom.y(),
                                 (float)ini_mom.z(), (float)final_mom.x(),
                                 (float)final_mom.y(), (float)final_mom.z(),
				 kin_energy, length,
                                 trj->GetCreatorProcess().c_str(),
				 trj->GetFinalProcess().c_str());

  }
}



void PetaloPersistencyManager::StoreHits(G4HCofThisEvent* hce)
{
  if (!hce) return;

  G4SDManager* sdmgr = G4SDManager::GetSDMpointer();
  G4HCtable* hct = sdmgr->GetHCtable();

  // Loop through the hits collections
  for (int i=0; i<hct->entries(); i++) {

    // Collection are identified univocally (in principle) using
    // their id number, and this can be obtained using the collection
    // and sensitive detector names.
    G4String hcname = hct->GetHCname(i);
    G4String sdname = hct->GetSDname(i);
    int hcid = sdmgr->GetCollectionID(sdname+"/"+hcname);
    // Fetch collection using the id number
    G4VHitsCollection* hits = hce->GetHC(hcid);

    if (hcname == IonizationSD::GetCollectionUniqueName()) {
      if (sns_only_ == false) {
	StoreIonizationHits(hits);
      }
    }
    else if (hcname == ToFSD::GetCollectionUniqueName())
      StoreSensorHits(hits);
    else if (hcname == ChargeSD::GetCollectionUniqueName())
      StoreChargeHits(hits);
    else {
      G4String msg =
        "Collection of hits '" + sdname + "/" + hcname
        + "' is of an unknown type and will not be stored.";
      G4Exception("StoreHits()", "[PetaloPersistencyManager]", JustWarning, msg);
    }
  }
}



void PetaloPersistencyManager::StoreIonizationHits(G4VHitsCollection* hc)
 {
   IonizationHitsCollection* hits =
     dynamic_cast<IonizationHitsCollection*>(hc);
   if (!hits) return;

   std::string sdname = hits->GetSDname();

   for (size_t i=0; i<hits->entries(); i++) {

     IonizationHit* hit = dynamic_cast<IonizationHit*>(hits->GetHit(i));
     if (!hit) continue;

     G4int trackid = hit->GetTrackID();
     G4ThreeVector hit_pos = hit->GetPosition();

     h5writer_->WriteHitInfo(nevt_, trackid,
			     hit_pos[0], hit_pos[1], hit_pos[2],
			     hit->GetTime(), hit->GetEnergyDeposit(),
			     sdname.c_str());
   }

 }



void PetaloPersistencyManager::StoreSensorHits(G4VHitsCollection* hc)
{
  PetSensorHitsCollection* hits = dynamic_cast<PetSensorHitsCollection*>(hc);
  if (!hits) return;

  for (size_t i=0; i<hits->entries(); i++) {

    PetSensorHit* hit = dynamic_cast<PetSensorHit*>(hits->GetHit(i));
    if (!hit) continue;

    G4int s_id   = hit->GetSnsID();
    G4int charge = hit->GetDetPhotons();

    if (charge > thr_charge_){
      std::string sdname = hits->GetSDname();
      G4ThreeVector xyz = hit->GetPosition();
      if (save_tot_charge_ == true) {
        h5writer_->WriteSensorDataInfo(nevt_, (unsigned int)s_id, (unsigned int)charge);
      }
      std::vector<G4int>::iterator pos_it =
        std::find(sns_posvec_.begin(), sns_posvec_.end(), s_id);
      if (pos_it == sns_posvec_.end()) {
        h5writer_->WriteSensorPosInfo((unsigned int)s_id, sdname.c_str(),
                                      (float)xyz.x(), (float)xyz.y(), (float)xyz.z());
        sns_posvec_.push_back(s_id);
      }
      // Save also individual photons
      const std::map<G4double, G4int>& phot = hit->GetPhotonMap();
      std::map<G4double, G4int>::const_iterator it;
      for (it = phot.begin(); it != phot.end(); ++it) {
        if (sipm_cells_) {
          h5writer_->WriteSensorTofInfo(nevt_, (unsigned int)s_id, (float)it->first,
                                        (unsigned int)it->second);
        } else {
          if (it->first <= tof_time_){
            h5writer_->WriteSensorTofInfo(nevt_, (unsigned int)s_id, (float)it->first,
                                          (unsigned int)it->second);
          }
          else {
            break;
          }
        }
      }
    }
  }
}




void PetaloPersistencyManager::StoreChargeHits(G4VHitsCollection* hc)
{
  ChargeHitsCollection* hits = dynamic_cast<ChargeHitsCollection*>(hc);
  if (!hits) return;

  std::vector<G4int > sensor_ids;
  for (size_t i=0; i<hits->entries(); i++) {

    ChargeHit* hit = dynamic_cast<ChargeHit*>(hits->GetHit(i));
    if (!hit) continue;

    wire_bin_size_ = hit->GetBinSize();
    const std::map<G4double, G4int>& wvfm = hit->GetChargeWaveform();

    std::map<G4double, G4int>::const_iterator it;
    for (it = wvfm.begin(); it != wvfm.end(); ++it) {
      unsigned int time_bin = (unsigned int)((*it).first/wire_bin_size_+0.5);
      unsigned int charge   = (unsigned int)((*it).second+0.5);
      h5writer_->WriteChargeDataInfo(nevt_, (unsigned int)hit->GetSensorID(), time_bin, charge);
    }


    std::vector<G4int>::iterator pos_it =
      std::find(charge_posvec_.begin(), charge_posvec_.end(), hit->GetSensorID());
    if (pos_it == charge_posvec_.end()) {
      std::string sdname = hits->GetSDname();
      G4ThreeVector xyz  = hit->GetPosition();
      h5writer_->WriteSensorPosInfo((unsigned int)hit->GetSensorID(), sdname.c_str(),
                                    (float)xyz.x(), (float)xyz.y(), (float)xyz.z());
      charge_posvec_.push_back(hit->GetSensorID());
    }
  }
}

void PetaloPersistencyManager::StoreSteps()
{
  PetSaveAllSteppingAction* sa = (PetSaveAllSteppingAction*)
    G4RunManager::GetRunManager()->GetUserSteppingAction();

  StepContainer<G4String> initial_volumes = sa->get_initial_volumes();
  StepContainer<G4String>   final_volumes = sa->get_final_volumes  ();
  StepContainer<G4String>      proc_names = sa->get_proc_names     ();

  StepContainer<G4ThreeVector> initial_poss = sa->get_initial_poss();
  StepContainer<G4ThreeVector>   final_poss = sa->get_final_poss  ();

  for (auto it = initial_volumes.begin(); it != initial_volumes.end(); ++it) {
    std::pair<G4int, G4String> key           = it->first;
    G4int                      track_id      = key.first;
    G4String                   particle_name = key.second;

    for (size_t step_id=0; step_id < it->second.size(); ++step_id) {
      h5writer_->WriteStep(nevt_, track_id, particle_name, step_id,
                           initial_volumes[key][step_id],
                           final_volumes[key][step_id],
                           proc_names[key][step_id],
                           initial_poss   [key][step_id].x(),
                           initial_poss   [key][step_id].y(),
                           initial_poss   [key][step_id].z(),
                           final_poss   [key][step_id].x(),
                           final_poss   [key][step_id].y(),
                           final_poss   [key][step_id].z());
    }
  }
  sa->Reset();
}

G4bool PetaloPersistencyManager::Store(const G4Run*)
{

  // Store the number of events to be processed
  NexusApp* app = (NexusApp*) G4RunManager::GetRunManager();
  G4int num_events = app->GetNumberOfEventsToBeProcessed();

  G4String key = "num_events";
  h5writer_->WriteRunInfo(key, std::to_string(num_events).c_str());
  key = "saved_events";
  h5writer_->WriteRunInfo(key, std::to_string(saved_evts_).c_str());

  if (save_int_e_numb_) {
    key = "interacting_events";
    h5writer_->WriteRunInfo(key,  std::to_string(interacting_evts_).c_str());
   }
  key = "wire_bin_size";
  h5writer_->WriteRunInfo(key, (std::to_string(wire_bin_size_/nanosecond)+" ns").c_str());
  key = "electric_field";
  h5writer_->WriteRunInfo(key, (std::to_string(efield_)+" V/cm").c_str());

  SaveConfigurationInfo(init_macro_);
  for (unsigned long i=0; i<macros_.size(); i++) {
    SaveConfigurationInfo(macros_[i]);
  }
  for (unsigned long i=0; i<delayed_macros_.size(); i++) {
    SaveConfigurationInfo(delayed_macros_[i]);
  }
  for (unsigned long i=0; i<secondary_macros_.size(); i++) {
    SaveConfigurationInfo(secondary_macros_[i]);
  }

  return true;
}

void PetaloPersistencyManager::SaveConfigurationInfo(G4String file_name)
{
  std::ifstream history(file_name, std::ifstream::in);
  while (history.good()) {

    G4String line;
    std::getline(history, line);
    if (line[0] == '#')
      continue;

    std::stringstream ss(line);
    G4String key, value;
    std::getline(ss, key, ' ');
    std::getline(ss, value);

    if (key != "") {
      auto found_other_macro = key.find("/control/execute");
      if (found_other_macro == std::string::npos) {
        if (key[0] == '\n') {
          key.erase(0, 1);
        }
	h5writer_->WriteRunInfo(key.c_str(), value.c_str());
      }

      if (found_other_macro != std::string::npos)
        secondary_macros_.push_back(value);
    }

  }

  history.close();
}
