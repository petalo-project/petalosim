// ----------------------------------------------------------------------------
// petalosim | MicroCellGeometryBase.h
//
// This is an abstract base class for encapsulation of SiPM microcells.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef MICROCELL_BASE_H
#define MICROCELL_BASE_H

#include "nexus/GeometryBase.h"
#include <G4ThreeVector.hh>

using namespace nexus;

class MicroCellGeometryBase : public GeometryBase
{
public:
  /// Destructor
  virtual ~MicroCellGeometryBase();

  void SetPDE(G4double eff);
  G4double GetPDE() const;

protected:
  /// Default constructor defined as protected so no instance of
  /// this base class can be created.
  MicroCellGeometryBase();

private:
  G4double sipm_pde_;
};

// Inline definitions //

inline MicroCellGeometryBase::MicroCellGeometryBase() {}
inline MicroCellGeometryBase::~MicroCellGeometryBase() {}

inline void MicroCellGeometryBase::SetPDE(G4double eff) { sipm_pde_ = eff; }
inline G4double MicroCellGeometryBase::GetPDE() const { return sipm_pde_; }

#endif
