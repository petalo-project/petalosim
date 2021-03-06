// ----------------------------------------------------------------------------
// petalosim | PetAnalysisTrackingAction.h
//
// This class produces histograms of the wavelengths and the time of production
// of the optical photons.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef P_ANALYSIS_TRACKING_ACTION_H
#define P_ANALYSIS_TRACKING_ACTION_H

#include "nexus/GeometryBase.h"
#include <G4UserTrackingAction.hh>

class G4Track;
class G4GenericMessenger;
class TH1F;
class TFile;

using namespace nexus;

// General-purpose user tracking action

class PetAnalysisTrackingAction : public G4UserTrackingAction
{
public:
  /// Constructor
  PetAnalysisTrackingAction();
  /// Destructor
  virtual ~PetAnalysisTrackingAction();

  virtual void PreUserTrackingAction(const G4Track *);
  virtual void PostUserTrackingAction(const G4Track *);

private:
  G4GenericMessenger *msg_;
  G4String file_name_;
  G4int file_no_;

  //TH1F* hScintEnergy_;
  //TH1F* hCherEnergy_;

  TH1F *hCherLambda_;
  TH1F *hScintLambda_;
  TH1F *hScintTime;

  TFile *OptPhotons_;
};

#endif
