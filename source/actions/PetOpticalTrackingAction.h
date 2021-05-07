// ----------------------------------------------------------------------------
// petalosim | PetOpticalTrackingAction.h
//
// This class saves the trajectories of optical photons, in addition to the
// particles saved by the default tracking action. Its purpose is to store
// optical photon information in the output file.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef P_OPTICAL_TRACKING_ACTION_H
#define P_OPTICAL_TRACKING_ACTION_H

#include <G4UserTrackingAction.hh>

class G4Track;

// Optical-checking user tracking action

class PetOpticalTrackingAction : public G4UserTrackingAction
{
public:
  /// Constructor
  PetOpticalTrackingAction();
  /// Destructor
  virtual ~PetOpticalTrackingAction();

  virtual void PreUserTrackingAction(const G4Track *);
  virtual void PostUserTrackingAction(const G4Track *);
};

#endif
