// ----------------------------------------------------------------------------
// petalosim | DoubleParticle.h
//
// This generator is based on the SingleParticle generator, but simulates
// two particles instead of one. The region passed to the GenerateVertex()
// method must provide two positions.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef DOUBLE_PARTICLE_H
#define DOUBLE_PARTICLE_H

#include <G4VPrimaryGenerator.hh>

class G4GenericMessenger;
class G4Event;
class G4ParticleDefinition;


namespace nexus { class GeometryBase; }

class DoubleParticle: public G4VPrimaryGenerator
{
public:
  /// Constructor
  DoubleParticle();
  /// Destructor
  ~DoubleParticle();
  
  /// This method is invoked at the beginning of the event. It sets
  /// a primary vertex (that is, a particle in a given position and time)
  /// in the event.
  void GeneratePrimaryVertex(G4Event*);
  
private:
  
  void SetParticleDefinition(G4String);
  
  /// Generate a random kinetic energy with flat probability in
  //  the interval [energy_min, max_energy].
  G4double RandomEnergy() const;
  
private:
  G4GenericMessenger* msg_;
  
  G4ParticleDefinition* particle_definition_;
  
  G4double energy_min_; ///< Minimum kinetic energy
  G4double max_energy_; ///< Maximum kinetic energy
  
  const nexus::GeometryBase* geom_; ///< Pointer to the detector geometry
  
  G4String region_;
  
  
};
#endif
