// ----------------------------------------------------------------------------
// nexus | PetSaveAllEventAction.h
//
// This class is based on DefaultEventAction and modified to store
// all the events, no matter how much energy is deposited.
//
// The NEXT Collaboration
// ----------------------------------------------------------------------------

#ifndef P_SAVEALL_EVENT_ACTION_H
#define P_SAVEALL_EVENT_ACTION_H

#include <G4UserEventAction.hh>
#include <globals.hh>

class G4Event;
class G4GenericMessenger;

namespace nexus {

  class PetSaveAllEventAction: public G4UserEventAction
  {
  public:
    /// Constructor
    PetSaveAllEventAction();
    /// Destructor
    ~PetSaveAllEventAction();

    /// Hook at the beginning of the event loop
    void BeginOfEventAction(const G4Event*);
    /// Hook at the end of the event loop
    void EndOfEventAction(const G4Event*);

  private:

    G4int nevt_, nupdate_;

  };

} // namespace nexus

#endif
