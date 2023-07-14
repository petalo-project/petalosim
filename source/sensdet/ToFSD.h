// ----------------------------------------------------------------------------
// petalosim | ToFSD.h
//
// This class is the sensitive detector used for PETALO.
// The first time a photoelectron is detected by a particular sensor,
// a PetSensorHit instance is created with two tasks:
// storing the total charge response and the times of the individual photons.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TOF_SD_H
#define TOF_SD_H

#include "PetSensorHit.h"
#include <G4VSensitiveDetector.hh>

class G4Step;
class G4HCofThisEvent;
class G4VTouchable;
class G4TouchableHistory;
class G4OpBoundaryProcess;

class ToFSD : public G4VSensitiveDetector
{
public:
  /// Constructor providing names for the sensitive detector
  /// and the collection of hits
  ToFSD(G4String sdname);
  /// The destructor
  ~ToFSD();

  /// Initialization of the sensitive detector. Invoked at the beginning
  /// of every event. The collection of hits is created here and registered
  /// in the event (so that it can be retrieved thru the G4HCofThisEvent object).
  void Initialize(G4HCofThisEvent *);

  /// Method invoked at the end of every event
  void EndOfEvent(G4HCofThisEvent *);

  /// Set the depth of the sensitive detector in the geometry hierarchy
  void SetDetectorVolumeDepth(G4int);
  /// Return the depth of the sensitive detector in the volume hierarchy
  G4int GetDetectorVolumeDepth() const;
  /// Set the depth of the SD's mother volume in the geometry hierarchy
  void SetMotherVolumeDepth(G4int);
  /// Return the depth of the SD's mother volume in the geometry hierarchy
  G4int GetMotherVolumeDepth() const;
  /// Set the naming order (typically, a multiple of 10) of the SD
  void SetDetectorNamingOrder(G4int);
  /// Return the naming order of the SD
  G4int GetDetectorNamingOrder() const;

  /// Set the depth of the SD's grandmother volume in the geometry hierarchy
  void SetGrandMotherVolumeDepth(G4int);
  /// Return the depth of the SD's grandmother volume in the geometry hierarchy
  G4int GetGrandMotherVolumeDepth() const;

  /// Set the box geometry parameter
  void SetBoxGeom(G4int bg);

  /// Set type of SiPM (with every microcell or not)
  void SetSiPMCells(G4int cell);

  /// Return the unique name of the hits collection created
  /// by this sensitive detector. This will be used by the
  /// persistency manager to select the collection.
  static G4String GetCollectionUniqueName();

private:
  G4bool ProcessHits(G4Step *, G4TouchableHistory *);

  G4int FindID(const G4VTouchable *);

  G4int naming_order_;      ///< Order of the naming scheme
  G4int sensor_depth_;      ///< Depth of the SD in the geometry tree
  G4int mother_depth_;      ///< Depth of the SD's mother in the geometry tree
  G4int grandmother_depth_; ///< Depth of the SD's grandmother in the geometry tree

  G4int box_geom_; ///< Required to change the naming_order_ for the case of the PetBox
  G4int sipm_cells_; ///< 1 if each individual microcell is simulated in SiPMs

  PetSensorHitsCollection *HC_; ///< Pointer to the collection of hits
};

// INLINE METHODS //////////////////////////////////////////////////

inline void ToFSD::SetDetectorVolumeDepth(G4int d) { sensor_depth_ = d; }
inline G4int ToFSD::GetDetectorVolumeDepth() const { return sensor_depth_; }

inline void ToFSD::SetMotherVolumeDepth(G4int d) { mother_depth_ = d; }
inline G4int ToFSD::GetMotherVolumeDepth() const { return mother_depth_; }

inline void ToFSD::SetDetectorNamingOrder(G4int o) { naming_order_ = o; }
inline G4int ToFSD::GetDetectorNamingOrder() const { return naming_order_; }

inline void ToFSD::SetGrandMotherVolumeDepth(G4int d) { grandmother_depth_ = d; }
inline G4int ToFSD::GetGrandMotherVolumeDepth() const { return grandmother_depth_; }

inline void ToFSD::SetBoxGeom(G4int bg) { box_geom_ = bg; }
inline void ToFSD::SetSiPMCells(G4int cell) { sipm_cells_ = cell; }

#endif
