// ----------------------------------------------------------------------------
// petalosim | TileGeometryBase.h
//
// This is an abstract base class for encapsulation of SiPM board geometries.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TILE_BASE_GEOMETRY_H
#define TILE_BASE_GEOMETRY_H

#include "PetaloUtils.h"
#include "nexus/GeometryBase.h"
#include <G4ThreeVector.hh>


class G4VPhysicalVolume;
using namespace nexus;

class TileGeometryBase : public GeometryBase
{
public:
  /// Destructor
  virtual ~TileGeometryBase();

  void SetBoxConf(petit_conf box_conf);
  petit_conf GetBoxConf() const;

  void SetTileVisibility(G4bool vis);
  G4bool GetTileVisibility() const;

  void SetTileReflectivity(G4double refl);
  G4double GetTileReflectivity() const;

  void SetPDE(G4double eff);
  G4double GetPDE() const;

  void SetMotherPhysicalVolume(G4VPhysicalVolume *mpv);
  G4VPhysicalVolume *GetMotherPhysicalVolume() const;

  G4VPhysicalVolume *mpv_;

protected:
  /// Default constructor defined as protected so no instance of
  /// this base class can be created.
  TileGeometryBase();

private:
  petit_conf box_conf_;
  G4bool tile_vis_;
  G4double tile_refl_;
  G4double sipm_pde_;
};

// Inline definitions ///////////////////////////////////

inline TileGeometryBase::TileGeometryBase() : box_conf_(hama) {}
inline TileGeometryBase::~TileGeometryBase() {}

inline void TileGeometryBase::SetBoxConf(petit_conf box_conf) { box_conf_ = box_conf; }
inline petit_conf TileGeometryBase::GetBoxConf() const { return box_conf_; }

inline void TileGeometryBase::SetTileVisibility(G4bool vis) { tile_vis_ = vis; }
inline G4bool TileGeometryBase::GetTileVisibility() const { return tile_vis_; }

inline void TileGeometryBase::SetTileReflectivity(G4double refl) { tile_refl_ = refl; }
inline G4double TileGeometryBase::GetTileReflectivity() const { return tile_refl_; }

inline void TileGeometryBase::SetPDE(G4double eff) { sipm_pde_ = eff; }
inline G4double TileGeometryBase::GetPDE() const { return sipm_pde_; }

inline void TileGeometryBase::SetMotherPhysicalVolume(G4VPhysicalVolume *mpv) { mpv_ = mpv; }
inline G4VPhysicalVolume *TileGeometryBase::GetMotherPhysicalVolume() const { return mpv_; }

#endif
