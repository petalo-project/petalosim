// ----------------------------------------------------------------------------
// petalosim | PetaloTrackingAction.h
//
// This class is the default tracking action of the PETALO simulation.
// It stores in memory the trajectories of all particles, except for
// optical photons, with the relevant tracking information that will be
// saved to the output file.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PETALO_TRACKING_ACTION_H
#define PETALO_TRACKING_ACTION_H

#include <G4UserTrackingAction.hh>

class G4Track;

// General-purpose user tracking action

class PetaloTrackingAction : public G4UserTrackingAction
{
public:
  /// Constructor
  PetaloTrackingAction();
  /// Destructor
  virtual ~PetaloTrackingAction();

  virtual void PreUserTrackingAction(const G4Track *);
  virtual void PostUserTrackingAction(const G4Track *);
};

#endif
