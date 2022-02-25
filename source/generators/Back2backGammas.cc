// ----------------------------------------------------------------------------
// petalosim | Back2backGammas.cc
//
// This generator simulates two gammas, product of a positron annihilation.
// The non-collinearity of the momenta and the Doppler shift of the energy
// are taken into account. The first gamma is generated with random direction,
// by default. However, it is possible to specify a limited solid angle.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "Back2backGammas.h"
#include "PetaloUtils.h"

#include "nexus/RandomUtils.h"
#include "nexus/DetectorConstruction.h"
#include "nexus/GeometryBase.h"
#include "nexus/FactoryBase.h"

#include <G4Event.hh>
#include <G4GenericMessenger.hh>
#include <G4RunManager.hh>
#include <G4ParticleTable.hh>
#include <G4RandomDirection.hh>
#include <Randomize.hh>

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace CLHEP;
using namespace nexus;

REGISTER_CLASS(Back2backGammas, G4VPrimaryGenerator)

Back2backGammas::Back2backGammas(): geom_(0), costheta_min_(-1.),
                                    costheta_max_(1.),
                                    phi_min_(0.), phi_max_(2.*pi)
{
  //G4cout << "Limits = " << std::numeric_limits<unsigned int>::max() << G4endl;
  msg_ = new G4GenericMessenger(this, "/Generator/Back2back/",
                                "Control commands of 511-keV back to back gammas generator.");

  msg_->DeclareProperty("region", region_,
                        "Set the region of the geometry where the vertex will be generated.");

  msg_->DeclareProperty("min_costheta", costheta_min_,
                        "Set minimum cosTheta for the direction of the particle.");
  msg_->DeclareProperty("max_costheta", costheta_max_,
                        "Set maximum cosTheta for the direction of the particle.");
  msg_->DeclareProperty("min_phi", phi_min_,
                        "Set minimum phi for the direction of the particle.");
  msg_->DeclareProperty("max_phi", phi_max_,
                        "Set maximum phi for the direction of the particle.");

  DetectorConstruction* detconst =
    (DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
  geom_ = detconst->GetGeometry();
}

Back2backGammas::~Back2backGammas()
{
}

void Back2backGammas::GeneratePrimaryVertex(G4Event* evt)
{
  G4ParticleDefinition* gamma =
    G4ParticleTable::GetParticleTable()->FindParticle("gamma");

  auto dir1 = (costheta_min_ != -1. || costheta_max_ != 1. || phi_min_ != 0. || phi_max_ != 2.*pi) ?
               RandomDirectionInRange(costheta_min_, costheta_max_, phi_min_, phi_max_):
               G4RandomDirection();

  auto [dir2, e1, e2] = CalculateNonCollinearKinematicInBodyTissue(dir1);

  auto p1 = e1 * dir1;
  auto p2 = e2 * dir2;

  G4ThreeVector position = geom_->GenerateVertex(region_);
  G4double time = 0.;
  auto vertex = new G4PrimaryVertex(position, time);

  vertex->SetPrimary(new G4PrimaryParticle(gamma,  p1.x(),  p1.y(),  p1.z()));
  vertex->SetPrimary(new G4PrimaryParticle(gamma, -p2.x(), -p2.y(), -p2.z()));

  evt->AddPrimaryVertex(vertex);
}
