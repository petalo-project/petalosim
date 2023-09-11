// ----------------------------------------------------------------------------
// petalosim | PETitBox.cc
//
// Basic geometry common to the different configurations of PETit.
// It includes the aluminum box, the xenon and the source tube.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PETIT_BOX_H
#define PETIT_BOX_H

#include "nexus/GeometryBase.h"

class G4Material;

namespace nexus
{
class SpherePointSampler;
class IonizationSD;
}

using namespace nexus;

class PETitBox : public GeometryBase
{

 public:
  // Constructor
  PETitBox();
  //Destructor
  ~PETitBox();


  void Construct();
  void SetXeMaterial(G4Material* mat);
  void SetIoniSD(IonizationSD* ionisd);
  void SetMaxStepSize(G4double step_size);
  void SetLXePressure(G4double pressure);
  void SetVisibility(G4bool vis);

  G4LogicalVolume* GetActiveVolume() const;
  G4double GetHatZSize() const;
  G4double GetBoxSize() const;
  G4double GetBoxThickness() const;

private:
  G4bool visibility_;
  G4ThreeVector source_pos_;
  G4double box_size_, box_thickness_;

  G4double ih_x_size_, ih_y_size_, ih_z_size_;
  G4double ih_thick_wall_, ih_thick_roof_;
  G4double source_tube_thick_wall_, source_tube_int_radius_;
  G4double dist_source_roof_, source_tube_thick_roof_;

  G4double  max_step_size_, pressure_;

  SpherePointSampler* source_gen_;

  G4Material* LXe_mat_;
  IonizationSD* ionisd_;

  G4LogicalVolume* active_logic_;
};

inline void PETitBox::SetXeMaterial(G4Material* mat) {LXe_mat_ = mat;}
inline void PETitBox::SetIoniSD(IonizationSD* ionisd) {ionisd_ = ionisd;}
inline void PETitBox::SetMaxStepSize(G4double step_size) {max_step_size_ = step_size;}
inline void PETitBox::SetLXePressure(G4double pressure) {pressure_ = pressure;}
inline void PETitBox::SetVisibility(G4bool vis) {visibility_ = vis;}

inline G4LogicalVolume* PETitBox::GetActiveVolume() const {return active_logic_;}
inline G4double PETitBox::GetHatZSize() const {return ih_z_size_;}
inline G4double PETitBox::GetBoxSize() const {return box_size_;}
inline G4double PETitBox::GetBoxThickness() const {return box_thickness_;}


#endif
