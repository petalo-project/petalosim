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


G4Material* PetMaterialsList::Steel()
{
  // Composition ranges correspond to stainless steel grade 304L

  G4String name = "Steel";

  G4Material* mat = G4Material::GetMaterial(name, false);

  if (mat == 0) {

    mat = new G4Material(name, 8000*kg/m3, 4);

    G4NistManager* nist = G4NistManager::Instance();

    G4Element* Fe = nist->FindOrBuildElement("Fe");
    mat->AddElement(Fe, 0.66);

    G4Element* Cr = nist->FindOrBuildElement("Cr");
    mat->AddElement(Cr, 0.20);

    G4Element* Mn = nist->FindOrBuildElement("Mn");
    mat->AddElement(Mn, 0.02);

    G4Element* Ni = nist->FindOrBuildElement("Ni");
    mat->AddElement(Ni, 0.12);
  }

  return mat;
}


G4Material* PetMaterialsList::Steel316Ti()
{
  G4String name = "Steel316Ti";

  G4Material* mat = G4Material::GetMaterial(name, false);

  if (!mat) {
    mat = new G4Material(name, 8000*kg/m3, 6);

    G4NistManager* nist = G4NistManager::Instance();

    G4Element* Fe = nist->FindOrBuildElement("Fe");
    mat->AddElement(Fe, 0.636);

    G4Element* Cr = nist->FindOrBuildElement("Cr");
    mat->AddElement(Cr, 0.18);

    G4Element* Ni = nist->FindOrBuildElement("Ni");
    mat->AddElement(Ni, 0.14);

    G4Element* Mo = nist->FindOrBuildElement("Mo");
    mat->AddElement(Mo, 0.03);

    G4Element* Si = nist->FindOrBuildElement("Si");
    mat->AddElement(Si, 0.007);

    G4Element* Ti = nist->FindOrBuildElement("Ti");
    mat->AddElement(Ti, 0.007);
  }

  return mat;
}


G4Material* PetMaterialsList::Epoxy()
{
  G4String name = "Epoxy";

  G4Material* mat = G4Material::GetMaterial(name, false);

  if (mat == 0) {
    G4NistManager* nist = G4NistManager::Instance();

    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
    G4Element* O = nist->FindOrBuildElement("O");

    mat = new G4Material(name, 1.3*g/cm3, 3);
    mat->AddElement(H, 44);
    mat->AddElement(C, 15);
    mat->AddElement(O, 7);
  }

  return mat;
}


G4Material* PetMaterialsList::CarbonFiber()
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


G4Material* PetMaterialsList::Kovar()
{
  G4String name = "Kovar";

  G4Material* mat = G4Material::GetMaterial(name, false);

  if (mat == 0) {
    G4NistManager* nist = G4NistManager::Instance();

    G4Element* Fe = nist->FindOrBuildElement("Fe");
    G4Element* Ni = nist->FindOrBuildElement("Ni");
    G4Element* Co = nist->FindOrBuildElement("Co");

    mat = new G4Material(name, 8.35*g/cm3, 3);
    mat->AddElement(Fe, 54);
    mat->AddElement(Ni, 29);
    mat->AddElement(Co, 17);
  }

  return mat;
}


G4Material* PetMaterialsList::PEEK()
{
  G4String name = "PEEK";

  G4Material* mat = G4Material::GetMaterial(name, false);

  if (mat == 0) {
    G4NistManager* nist = G4NistManager::Instance();

    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
    G4Element* O = nist->FindOrBuildElement("O");

    mat = new G4Material(name, 1.3*g/cm3, 3, kStateSolid);
    mat->AddElement(H, 12);
    mat->AddElement(C, 18);
    mat->AddElement(O,  3);
  }

  return mat;
}


G4Material* PetMaterialsList::FusedSilica()
{
  G4Material* mat =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  return mat;
}


G4Material* PetMaterialsList::PS() // polystyrene
{
  G4String name = "PS";

  // Check whether material exists already in the materials table
  G4Material* mat = G4Material::GetMaterial(name, false);

  if (mat == 0) {
    G4NistManager* nist = G4NistManager::Instance();

    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
    //      G4Element* O = nist->FindOrBuildElement("O");

    //      mat = new G4Material(name, 1320*kg/m3, 3, kStateSolid);
    mat = new G4Material(name, 1.05*g/cm3, 2, kStateSolid);
    mat->AddElement(H, 8);
    mat->AddElement(C, 8);
    //      mat->AddElement(O,  3);
  }

  return mat;
}


G4Material* PetMaterialsList::TPB()
{
  G4String name = "TPB"; // Tetraphenyl butadiene

  G4Material* mat = G4Material::GetMaterial(name, false);

  if (mat == 0) {
    G4NistManager* nist = G4NistManager::Instance();

    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");

    mat = new G4Material(name, 1*g/cm3, 2, kStateSolid);
    mat->AddElement(H, 22);
    mat->AddElement(C, 28);
  }

  return mat;
}


G4Material* PetMaterialsList::Kevlar()
{
  G4String name = "Kevlar"; //

  G4Material* mat = G4Material::GetMaterial(name, false);

  if (mat == 0) {
    G4NistManager* nist = G4NistManager::Instance();

    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
    G4Element* N = nist->FindOrBuildElement("N");
    G4Element* O = nist->FindOrBuildElement("O");

    mat = new G4Material(name, 1.44*g/cm3, 4, kStateSolid);
    mat->AddElement(H, 10);
    mat->AddElement(C, 14);
    mat->AddElement(O, 2);
    mat->AddElement(N, 2);
  }

  return mat;

}


G4Material* PetMaterialsList::HDPE()
{
  G4String name = "HDPE";

  G4Material* mat = G4Material::GetMaterial(name, false);

  if (mat == 0) {
    G4NistManager* nist = G4NistManager::Instance();

    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");

    mat = new G4Material(name, .954*g/cm3, 2, kStateSolid);
    mat->AddElement(H, 4);
    mat->AddElement(C, 2);
  }

  return mat;

}


G4Material* PetMaterialsList::LYSO()
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


G4Material* PetMaterialsList::FR4()
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


G4Material* PetMaterialsList::CopyMaterial(G4Material* original, G4String newname)
{
  G4Material* newmat = G4Material::GetMaterial(newname, false);

  if (newmat == 0) {
    G4double density = original->GetDensity();
    G4State state = original->GetState();
    G4double temperature = original->GetTemperature();
    G4double pressure = original->GetPressure();

    G4int n_elem = original->GetNumberOfElements();
    if (n_elem == 1){
      G4double z = original->GetZ();
      G4double a = original->GetA();
      newmat =
	new G4Material(newname, z, a, density, state, temperature, pressure);
    } else {
      // Gas mixture
      const G4double * fractions = original->GetFractionVector();
      newmat =
	new G4Material(newname, density, n_elem, state, temperature, pressure);
      for (G4int i = 0; i < n_elem; ++i)
	newmat->AddElement(new G4Element(original->GetElement(i)->GetName(),
					 original->GetElement(i)->GetSymbol(),
					 original->GetElement(i)->GetZ(),
					 original->GetElement(i)->GetA()),
			   fractions[i]);
    }
  }

  return newmat;
}


G4Material* PetMaterialsList::FakeDielectric(G4Material* model_mat, G4String name)
{
  return CopyMaterial(model_mat, name);
}
