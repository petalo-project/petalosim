// ----------------------------------------------------------------------------
// nexus | PetaloPhysics.h
//
// This class registers any new physics process defined in nexus.
//
// The NEXT Collaboration
// ----------------------------------------------------------------------------

#ifndef PETALO_PHYSICS_H
#define PETALO_PHYSICS_H

#include <G4VPhysicsConstructor.hh>

class G4GenericMessenger;


namespace nexus {

  class WavelengthShifting;

  class PetaloPhysics: public G4VPhysicsConstructor
  {
  public:
    /// Constructor
    PetaloPhysics();
    /// Destructor
    ~PetaloPhysics();

    /// Construct all required particles (Geant4 mandatory method)
    virtual void ConstructParticle();
    /// Construct all required physics processes (Geant4 mandatory method)
    virtual void ConstructProcess();

  private:
    G4bool risetime_; ///< Rise time for LYSO

    G4bool noCompt_; ///< Switch off Compton scattering

    G4GenericMessenger* msg_;
    WavelengthShifting* wls_;
  };

} // end namespace nexus

#endif
