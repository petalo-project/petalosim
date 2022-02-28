// ----------------------------------------------------------------------------
// petalosim | Back2backGammas.h
//
// This generator simulates two gammas, product of a positron annihilation.
// The non-collinearity of the momenta and the Doppler shift of the energy
// are taken into account. The first gamma is generated with random direction,
// by default. However, it is possible to specify a limited solid angle.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef BACK2BACK_H
#define BACK2BACK_H

#include <G4VPrimaryGenerator.hh>

class G4Event;
class G4GenericMessenger;

namespace nexus {

  class GeometryBase;

  class Back2backGammas: public G4VPrimaryGenerator
  {
  public:
    //Constructor
    Back2backGammas();
    //Destructor
    ~Back2backGammas();

    void GeneratePrimaryVertex(G4Event* evt);

  private:

    G4GenericMessenger* msg_;
    const GeometryBase* geom_;

    G4String region_;

    G4double costheta_min_;
    G4double costheta_max_;
    G4double phi_min_;
    G4double phi_max_;
  };

}// end namespace nexus
#endif
