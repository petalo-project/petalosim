// ----------------------------------------------------------------------------
// nexus | PetAnalysisEventAction.h
//
// This class is based on DefaultEventAction and modified to produce
// a histogram of the number of scintillation photons event by event.
//
// The NEXT Collaboration
// ----------------------------------------------------------------------------

#ifndef P_ANALYSIS_EVENT_ACTION_H
#define P_ANALYSIS_EVENT_ACTION_H

#include <G4UserEventAction.hh>
#include <globals.hh>

class G4Event;
class G4GenericMessenger;
class TH1F;
class TFile;

namespace nexus {

  /// This class is a general-purpose event run action.

  class PetAnalysisEventAction: public G4UserEventAction
  {
  public:
    /// Constructor
    PetAnalysisEventAction();
    /// Destructor
    ~PetAnalysisEventAction();

    /// Hook at the beginning of the event loop
    void BeginOfEventAction(const G4Event*);
    /// Hook at the end of the event loop
    void EndOfEventAction(const G4Event*);

  private:
    G4GenericMessenger* msg_;
    G4int nevt_, nupdate_;
    G4double energy_threshold_;
    G4double energy_max_;
    G4String file_name_;
    G4int file_no_;

    TH1F* hNPhotons;

    TFile* Histo;
  };

} // namespace nexus

#endif
