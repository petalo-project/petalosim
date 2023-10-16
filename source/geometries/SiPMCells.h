// ----------------------------------------------------------------------------
// petalosim | SiPMCell.h
//
// Generic SiPM geometry with individual cells.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef SIPM_CELL_H
#define SIPM_CELL_H

#include "nexus/GeometryBase.h"

using namespace nexus;

class MicroCellGeometryBase;

class SiPMCells : public GeometryBase
{
public:
  /// Constructor
  SiPMCells();
  /// Destructor
  ~SiPMCells();

  /// Invoke this method to build the volumes of the geometry
  void Construct();

  void SetNumbOfMicroCells(G4int npxl);
  void SetPitch(G4double pitch);
  void SetMicroCell(G4String pxl_name);
  void SetPDE(G4double pde);
  G4double GetPDE() const;

  void SetDim(G4ThreeVector dim);
  G4ThreeVector GetDim() const;
  
private:
  G4int n_microcells_;
  G4double pitch_;
  G4String pxl_name_;
  MicroCellGeometryBase* pxl_;
  G4ThreeVector dim_;
  G4double sipm_pde_;
};

inline void SiPMCells::SetNumbOfMicroCells(G4int npxl) { n_microcells_ = npxl;}
inline void SiPMCells::SetPitch(G4double pitch) { pitch_ = pitch;}
inline void SiPMCells::SetMicroCell(G4String pxl) { pxl_name_ = pxl;}
inline void SiPMCells::SetPDE(G4double pde) { sipm_pde_ = pde;}
inline G4double SiPMCells::GetPDE() const { return sipm_pde_;}
inline void SiPMCells::SetDim(G4ThreeVector dim) {dim_ = dim;}
inline G4ThreeVector SiPMCells::GetDim() const {return dim_;}

#endif
