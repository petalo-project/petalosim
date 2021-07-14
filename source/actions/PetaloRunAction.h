// ----------------------------------------------------------------------------
// petalosim | PetaloRunAction.h
//
// This is the default run action of the PETALO simulations.
// A message at the beginning and at the end of the simulation is printed.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PETALO_RUN_ACTION_H
#define PETALO_RUN_ACTION_H

#include <G4UserRunAction.hh>

class PetaloRunAction : public G4UserRunAction
{
public:
  /// Constructor
  PetaloRunAction();
  /// Destructor
  ~PetaloRunAction();

  virtual void BeginOfRunAction(const G4Run *);
  virtual void EndOfRunAction(const G4Run *);
};

#endif
