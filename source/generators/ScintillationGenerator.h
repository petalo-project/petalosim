// ----------------------------------------------------------------------------
// petalosim | ScintillationGenerator.h
//
// This class is the primary generator of a number of optical photons with
// energy following the scintillation spectrum of the material
// where the vertex is produced.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef SCINTILLATIONGENERATOR_H
#define SCINTILLATIONGENERATOR_H

#include <G4VPrimaryGenerator.hh>
#include <G4Navigator.hh>
#include <G4TransportationManager.hh>

class G4GenericMessenger;
class G4Event;
class G4PhysicsOrderedFreeVector;

namespace nexus {

  class GeometryBase;


  /// Primary generator (concrete class of G4VPrimaryGenerator) for events
  /// consisting of a single particle. The user must specify via configuration
  /// parameters the particle type, a kinetic energy interval (a random
  /// value with random .

  class ScintillationGenerator: public G4VPrimaryGenerator
  {
  public:
    /// Constructor
    ScintillationGenerator();
    /// Destructor
    ~ScintillationGenerator();

    /// This method is invoked at the beginning of the event. It sets
    /// a primary vertex (that is, a particle in a given position and time)
    /// in the event.
    void GeneratePrimaryVertex(G4Event*);

  private:

    void ComputeCumulativeDistribution(const G4PhysicsOrderedFreeVector&,
                                       G4PhysicsOrderedFreeVector&);

    G4GenericMessenger* msg_;
    G4Navigator* geom_navigator_; ///< Geometry Navigator
    const GeometryBase* geom_; ///< Pointer to the detector geometry

    G4String region_;
    G4int    nphotons_;


  };

} // end namespace nexus

#endif // __SCINTILLATIONGENERATOR__
