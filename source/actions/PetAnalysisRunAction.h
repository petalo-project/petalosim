// ----------------------------------------------------------------------------
// nexus | PetaloRunAction.h
//
// This is the default run action of the PETALO simulations.
// The G4 analysis manager is opened and closed.
// A message at the beginning and at the end of the simulation is also printed.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PET_ANALYSIS_RUN_ACTION_H
#define PET_ANALYSIS_RUN_ACTION_H

#include <G4UserRunAction.hh>

namespace nexus {

  class PetAnalysisRunAction: public G4UserRunAction
  {
  public:
    /// Constructor
    PetAnalysisRunAction();
    /// Destructor
    ~PetAnalysisRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

  };

}

#endif
