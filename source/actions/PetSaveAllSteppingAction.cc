// ----------------------------------------------------------------------------
// petalosim | PetSaveAllSteppingAction.cc
//
// This class sets the store_steps flag of the persistency manager
// so all steps are saved.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetSaveAllSteppingAction.h"
#include "PetaloPersistencyManager.h"

#include "nexus/FactoryBase.h"

#include <G4Step.hh>
#include <G4VPersistencyManager.hh>
#include <G4ProcessManager.hh>
#include <G4ParticleTable.hh>

REGISTER_CLASS(PetSaveAllSteppingAction, G4UserSteppingAction)

PetSaveAllSteppingAction::PetSaveAllSteppingAction():
G4UserSteppingAction(),
msg_(0),
selected_volumes_()
{
  msg_ = new G4GenericMessenger(this, "/Actions/PetSaveAllSteppingAction/");

  msg_->DeclareMethod("select_particle",
                      &PetSaveAllSteppingAction::AddSelectedParticle,
                      "add a new particle to select");

  msg_->DeclareMethod("select_volume",
                      &PetSaveAllSteppingAction::AddSelectedVolume,
                      "add a new volume to select");

  PetaloPersistencyManager* pm = dynamic_cast<PetaloPersistencyManager*>
        (G4VPersistencyManager::GetPersistencyManager());

  pm->StoreSteps(true);

}



PetSaveAllSteppingAction::~PetSaveAllSteppingAction()
{
}



void PetSaveAllSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4ParticleDefinition* pdef          = step->GetTrack()->GetDefinition();
  G4int                 track_id      = step->GetTrack()->GetTrackID();
  G4String              particle_name = pdef->GetParticleName();

  if (!KeepParticle(pdef)) return;

  G4StepPoint* pre  = step->GetPreStepPoint();
  G4StepPoint* post = step->GetPostStepPoint();

  G4ThreeVector initial_pos = pre ->GetPosition();
  G4ThreeVector   final_pos = post->GetPosition();

  G4String initial_volume = pre ->GetTouchableHandle()->GetVolume()->GetName();
  G4String   final_volume = post->GetTouchableHandle()->GetVolume()->GetName();
  G4String      proc_name = post->GetProcessDefinedStep()->GetProcessName();

  if (!KeepVolume(initial_volume, final_volume))
    return;

  std::pair<G4int, G4String> key = std::make_pair(track_id, particle_name);

  initial_volumes_[key].push_back(initial_volume);
    final_volumes_[key].push_back(  final_volume);
       proc_names_[key].push_back(     proc_name);

  initial_poss_   [key].push_back(initial_pos);
    final_poss_   [key].push_back(  final_pos);
}


void PetSaveAllSteppingAction::AddSelectedParticle(G4String particle_name)
{
  G4ParticleDefinition* pdef =
    G4ParticleTable::GetParticleTable()->FindParticle(particle_name);
  if (!pdef) {
    G4String msg =
      "No particle description was found for particle " + particle_name;
    G4Exception("[PetSaveAllSteppingAction]", "AddSelectedParticle()",
                FatalException, msg);
  }
  selected_particles_.push_back(pdef);
}


void PetSaveAllSteppingAction::AddSelectedVolume(G4String volume_name)
{
  selected_volumes_.push_back(volume_name);
}


G4bool PetSaveAllSteppingAction::KeepParticle(G4ParticleDefinition* pdef)
{
  if (!selected_particles_.size()) return true;

  auto it =
    std::find(selected_particles_.begin(), selected_particles_.end(), pdef);
  return it != selected_particles_.end();
}


G4bool PetSaveAllSteppingAction::KeepVolume(G4String& initial_volume,
                                            G4String& final_volume)
{
  if (!selected_volumes_.size()) return true;

  for (auto volume=selected_volumes_.begin(); volume != selected_volumes_.end(); volume++)
  {
    if (G4StrUtil::contains(initial_volume, *volume)) return true;
    if (G4StrUtil::contains(final_volume, *volume)) return true;
  }

  return false;
}



void PetSaveAllSteppingAction::Reset()
{
  initial_volumes_.clear();
    final_volumes_.clear();
       proc_names_.clear();

  initial_poss_   .clear();
    final_poss_   .clear();
}
