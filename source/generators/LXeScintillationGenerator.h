// ----------------------------------------------------------------------------
// petalosim | LXeScintillationGenerator.h
//
// This class is the primary generator of a number of optical photons with
// energy following the scintillation spectrum of LXe,
// where the vertex is produced.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef LXESCINTILLATIONGENERATOR_H
#define LXESCINTILLATIONGENERATOR_H

#include <G4VPrimaryGenerator.hh>
#include <G4PhysicsOrderedFreeVector.hh>

class G4GenericMessenger;
class G4Event;

namespace nexus { class GeometryBase; }

  class LXeScintillationGenerator: public G4VPrimaryGenerator
  {
  public:
    /// Constructor
    LXeScintillationGenerator();
    /// Destructor
    ~LXeScintillationGenerator();

    /// This method is invoked at the beginning of the event. It sets
    /// a primary vertex (that is, a particle in a given position and time)
    /// in the event.
    void GeneratePrimaryVertex(G4Event*);

  private:

    void ComputeCumulativeDistribution(const G4PhysicsOrderedFreeVector&,
                                       G4PhysicsOrderedFreeVector&);

    G4GenericMessenger* msg_;
    const nexus::GeometryBase* geom_; ///< Pointer to the detector geometry

    G4String region_;
    G4int    nphotons_;

  };

#endif
