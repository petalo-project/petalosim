// ----------------------------------------------------------------------------
// petalosim | PetMaterialsList.cc
//
// Definition of materials of common use.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetMaterialsList.h"

#include <G4Material.hh>
#include <G4Element.hh>
#include <G4NistManager.hh>


namespace petmaterials {

G4Material* CarbonFiber()
{
  G4String name = "CarbonFiber";

  G4Material* mat = G4Material::GetMaterial(name, false);

  if (mat == 0) {
    G4NistManager* nist = G4NistManager::Instance();

    G4Element* C = nist->FindOrBuildElement("C");

    mat = new G4Material(name, 1.6*g/cm3, 1);
    mat->AddElement(C, 1);
  }

  return mat;
}



G4Material* LYSO()
{
  G4String name = "LYSO";

  G4Material* mat = G4Material::GetMaterial(name, false);

  if (mat == 0) {
    G4NistManager* nist = G4NistManager::Instance();

    G4Element* Si = nist->FindOrBuildElement("Si");
    G4Element* O = nist->FindOrBuildElement("O");
    G4Element* Y = nist->FindOrBuildElement("Y");
    G4Element* Lu = nist->FindOrBuildElement("Lu");

    // Saint Gobain LYSO cristals
    mat = new G4Material(name, 7.1*g/cm3, 4, kStateSolid);
    mat->AddElement(Si, 0.064);
    mat->AddElement(O, 0.182);
    mat->AddElement(Y, 0.040);
    mat->AddElement(Lu, 0.714);
  }

  return mat;
}


G4Material* FR4()
{
  // Material widely used for printed circuit boards

  G4String name = "FR4";
  G4Material* mat = G4Material::GetMaterial(name, false);

  if (mat == 0) {

    G4NistManager* nist = G4NistManager::Instance();
    G4double density = 1.850*g/cm3;

    mat = new G4Material(name, density, 3, kStateSolid);

    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
    G4Element* O = nist->FindOrBuildElement("O");

    mat->AddElement(H, 12);
    mat->AddElement(C, 18);
    mat->AddElement(O,  3);
  }

  return mat;
}
}