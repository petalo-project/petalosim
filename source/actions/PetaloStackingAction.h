// ----------------------------------------------------------------------------
// petalosim | PetaloStackingAction.h
//
// This class is an example of how to implement a stacking action, if needed.
// At the moment, it is not used in the PETALO simulation.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PETALO_STACKING_ACTION_H
#define PETALO_STACKING_ACTION_H

#include <G4UserStackingAction.hh>

// General-purpose user stacking action

class PetaloStackingAction : public G4UserStackingAction
{
public:
  /// Constructor
  PetaloStackingAction();
  /// Destructor
  ~PetaloStackingAction();

  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *);
  virtual void NewStage();
  virtual void PrepareNewEvent();
};

#endif
