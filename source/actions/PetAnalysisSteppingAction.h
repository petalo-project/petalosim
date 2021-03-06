// ----------------------------------------------------------------------------
// petalosim | PetAnalysisSteppingAction.h
//
// This class allows the user to print the total number of photons detected by
// all kinds of photosensors at the end of the run.
// It produces histograms with photon velocities and wavelengths.
// It also shows examples of information that can be accessed at the stepping
// level, so it is useful for debugging.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef P_ANALYSIS_STEPPING_ACTION_H
#define P_ANALYSIS_STEPPING_ACTION_H

#include <G4UserSteppingAction.hh>
#include <G4GenericMessenger.hh>
#include <globals.hh>

#include <vector>

class G4Step;
class G4GenericMessenger;
class TH1F;
class TH2F;


class PetAnalysisSteppingAction : public G4UserSteppingAction
{
public:
  /// Constructor
  PetAnalysisSteppingAction();
  /// Destructor
  ~PetAnalysisSteppingAction();

  virtual void UserSteppingAction(const G4Step *);

private:
  G4int detected;
  G4int not_det;

  std::vector<double> times;
  std::vector<double> wavelengths;

  TH1F *hVelocity;
  TH2F *hVE;
  TH2F *hTV;
  TH1F *hCherLambdaDet;

  G4GenericMessenger *msg_;
  G4int file_no_;

  typedef std::map<G4String, int> detectorCounts;
  detectorCounts my_counts;
};

#endif
