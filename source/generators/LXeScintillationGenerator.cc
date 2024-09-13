// ----------------------------------------------------------------------------
// petalosim | LXeScintillationGenerator.cc
//
// This class is the primary generator of a number of optical photons with
// energy following the scintillation spectrum of LXe,
// where the vertex is produced.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "LXeScintillationGenerator.h"
#include "PetMaterialProperties.h"

#include "nexus/DetectorConstruction.h"
#include "nexus/GeometryBase.h"
#include "nexus/MaterialProperties.h"
#include "nexus/FactoryBase.h"

#include <G4GenericMessenger.hh>
#include <G4ParticleDefinition.hh>
#include <G4OpticalPhoton.hh>
#include <G4RunManager.hh>
#include <G4ParticleTable.hh>
#include <G4PrimaryVertex.hh>
#include <G4Event.hh>
#include <G4RandomDirection.hh>
#include <G4MaterialPropertiesTable.hh>
#include <Randomize.hh>

using namespace nexus;
using namespace CLHEP;

REGISTER_CLASS(LXeScintillationGenerator, G4VPrimaryGenerator)


LXeScintillationGenerator::LXeScintillationGenerator() :
  G4VPrimaryGenerator(), msg_(0), geom_(0), nphotons_(100000)
{
  msg_ = new G4GenericMessenger(this, "/Generator/LXeScintGenerator/",
    "Control commands of LXe scintillation generator.");

  msg_->DeclareProperty("region", region_,
    "Region of the geometry where the vertex is generated.");

  msg_->DeclareProperty("nphotons", nphotons_, "Number of photons");

  DetectorConstruction* detconst =
    (DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
  geom_ = detconst->GetGeometry();

}

LXeScintillationGenerator::~LXeScintillationGenerator()
{
  delete msg_;
}


void LXeScintillationGenerator::GeneratePrimaryVertex(G4Event* event)
{
  G4ParticleDefinition* particle_definition = G4OpticalPhoton::Definition();

  // Generate an initial position for the particle using the geometry
  G4ThreeVector position = geom_->GenerateVertex(region_);
  // Particle generated at start-of-event
  G4double time = 0.;

  // Energy is sampled from integral (like it is
  // done in G4Scintillation)
  G4MaterialPropertiesTable* mpt = materialprops::LXe();
  // Using fast or slow component here is irrelevant, since we're not using
  // time and they're the same in energy.
  G4MaterialPropertyVector* spectrum =
    mpt->GetProperty("SCINTILLATIONCOMPONENT1");
  G4PhysicsOrderedFreeVector* spectrum_integral =
      new G4PhysicsOrderedFreeVector();
  ComputeCumulativeDistribution(*spectrum, *spectrum_integral);
  G4double sc_max = spectrum_integral->GetMaxValue();

  // Create a new vertex
  G4PrimaryVertex* vertex = new G4PrimaryVertex(position, time);

  for ( G4int i = 0; i<nphotons_; i++)
    {
      // Generate random direction by default
      G4ThreeVector momentum_direction_ = G4RandomDirection();
      // Determine photon energy
      G4double sc_value = G4UniformRand()*sc_max;
      G4double pmod = spectrum_integral->GetEnergy(sc_value);

      G4double px = pmod * momentum_direction_.x();
      G4double py = pmod * momentum_direction_.y();
      G4double pz = pmod * momentum_direction_.z();

      // Create the new primary particle and set it some properties
      G4PrimaryParticle* particle =
        new G4PrimaryParticle(particle_definition, px, py, pz);

      G4ThreeVector polarization = G4RandomDirection();
      particle->SetPolarization(polarization);

      // Add particle to the vertex and this to the event
      vertex->SetPrimary(particle);
    }
  event->AddPrimaryVertex(vertex);
}


void LXeScintillationGenerator::ComputeCumulativeDistribution(
  const G4PhysicsOrderedFreeVector& pdf, G4PhysicsOrderedFreeVector& cdf)
{
  G4double sum = 0.;
  cdf.InsertValues(pdf.Energy(0), sum);

  for (unsigned int i=1; i<pdf.GetVectorLength(); ++i) {
    G4double area =
      0.5 * (pdf.Energy(i) - pdf.Energy(i-1)) * (pdf[i] + pdf[i-1]);
    sum = sum + area;
    cdf.InsertValues(pdf.Energy(i), sum);
  }
}
