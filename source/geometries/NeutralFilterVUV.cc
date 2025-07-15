// ----------------------------------------------------------------------------
// petalosim | MicroCellHmtsuVUV.cc
//
// Neutral VUV filter.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "NeutralFilterVUV.h"
#include <G4Box.hh>
#include <G4NistManager.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4SDManager.hh>
#include <G4VisAttributes.hh>

using namespace nexus;
using namespace CLHEP;

NeutralFilterVUV::NeutralFilterVUV() : GeometryBase(),
                                       filter_depth_(2. * mm)
{
}


NeutralFilterVUV::~NeutralFilterVUV()
{
}

void NeutralFilterVUV::Construct()
{
  G4double filter_xy = 50 * mm;

  G4Box* filter_solid =
      new G4Box("FILTER", filter_xy/2., filter_xy/2., filter_depth_/2);

  G4Material* fused_silica =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  G4LogicalVolume* filter_logic =
    new G4LogicalVolume(filter_solid, fused_silica, "FILTER");

  this->SetLogicalVolume(filter_logic);

    // Filter transmitance.
  const G4int entries = 89;

  G4double energies[entries] = {1.37755 * eV, 1.39303 * eV, 1.40886 * eV, 1.42505 * eV,
                                1.44162 * eV, 1.45858 * eV, 1.47595 * eV, 1.49373 * eV,
                                1.51195 * eV, 1.53061 * eV, 1.54975 * eV, 1.56936 * eV,
                                1.58948 * eV, 1.61012 * eV, 1.63131 * eV, 1.65306 * eV,
                                1.67540 * eV, 1.69835 * eV, 1.72194 * eV, 1.74619 * eV,
                                1.77114 * eV, 1.79681 * eV, 1.82323 * eV, 1.85044 * eV,
                                1.87848 * eV, 1.90738 * eV, 1.93718 * eV, 1.96793 * eV,
                                1.99967 * eV, 2.03245 * eV, 2.06633 * eV, 2.10135 * eV,
                                2.13758 * eV, 2.17508 * eV, 2.21392 * eV, 2.25418 * eV,
                                2.29592 * eV, 2.33924 * eV, 2.38423 * eV, 2.43098 * eV,
                                2.47960 * eV, 2.53020 * eV, 2.58291 * eV, 2.63787 * eV,
                                2.69521 * eV, 2.75511 * eV, 2.81772 * eV, 2.88325 * eV,
                                2.95190 * eV, 3.02390 * eV, 3.09950 * eV, 3.17897 * eV,
                                3.26263 * eV, 3.35081 * eV, 3.44388 * eV, 3.54228 * eV,
                                3.64647 * eV, 3.75696 * eV, 3.87437 * eV, 3.99935 * eV,
                                4.13266 * eV, 4.27517 * eV, 4.42785 * eV, 4.59185 * eV,
                                4.76846 * eV, 4.95920 * eV, 5.16583 * eV, 5.39043 * eV,
                                5.63545 * eV, 5.90380 * eV, 6.19900 * eV, 6.27384 * eV,
                                6.37538 * eV, 6.47692 * eV, 6.57846 * eV, 6.68000 * eV,
                                6.78153 * eV, 6.88307 * eV, 6.98461 * eV, 7.08615 * eV,
                                7.18769 * eV, 7.28923 * eV, 7.39076 * eV, 7.49230 * eV,
                                7.59384 * eV, 7.69538 * eV, 7.79692 * eV, 7.89846 * eV,
                                8.      * eV};

  G4double absorption_length_lin[entries] = { 14.06192 * mm, 14.02297 * mm, 13.99387 * mm, 13.93202 * mm,
                                              13.81263 * mm, 13.69203 * mm, 13.56811 * mm, 13.47453 * mm,
                                              13.45241 * mm, 13.37914 * mm, 13.32542 * mm, 13.30358 * mm,
                                              13.25648 * mm, 13.22932 * mm, 13.17035 * mm, 13.10741 * mm,
                                              13.06487 * mm, 13.02949 * mm, 12.95845 * mm, 12.92710 * mm,
                                              12.88499 * mm, 12.80620 * mm, 12.75031 * mm, 12.71820 * mm,
                                              12.68084 * mm, 12.64654 * mm, 12.57481 * mm, 12.53474 * mm,
                                              12.50508 * mm, 12.43781 * mm, 12.39943 * mm, 12.35708 * mm,
                                              12.29325 * mm, 12.24492 * mm, 12.20272 * mm, 12.17086 * mm,
                                              12.11495 * mm, 12.06118 * mm, 12.02942 * mm, 11.95323 * mm,
                                              11.92041 * mm, 11.90945 * mm, 11.81912 * mm, 11.76223 * mm,
                                              11.72925 * mm, 11.64656 * mm, 11.59008 * mm, 11.55716 * mm,
                                              11.45604 * mm, 11.35718 * mm, 11.29203 * mm, 11.16896 * mm,
                                              11.05424 * mm, 10.93651 * mm, 10.76848 * mm, 10.58308 * mm,
                                              10.37474 * mm, 10.13175 * mm,  9.85383 * mm,  9.54595 * mm,
                                               9.21473 * mm,  8.89823 * mm,  8.61788 * mm,  8.33931 * mm,
                                               8.08924 * mm,  7.88285 * mm,  7.67611 * mm,  7.48203 * mm,
                                               7.27135 * mm,  6.98990 * mm,  6.62925 * mm,  6.54281 * mm,
                                               6.42856 * mm,  6.31764 * mm,  6.20990 * mm,  6.10521 * mm,
                                               6.00344 * mm,  5.90447 * mm,  5.80817 * mm,  5.71445 * mm,
                                               5.62320 * mm,  5.53432 * mm,  5.44772 * mm,  5.36332 * mm,
                                               5.28101 * mm,  5.20074 * mm,  5.12242 * mm,  5.04598 * mm,
                                               4.97135 * mm};

//  G4double absorption_length_quad[entries] = {14.06192 * mm, 14.02297 * mm, 13.99387 * mm, 13.93202 * mm,
//                                              13.81263 * mm, 13.69203 * mm, 13.56811 * mm, 13.47453 * mm,
//                                              13.45241 * mm, 13.37914 * mm, 13.32542 * mm, 13.30358 * mm,
//                                              13.25648 * mm, 13.22932 * mm, 13.17035 * mm, 13.10741 * mm,
//                                              13.06487 * mm, 13.02949 * mm, 12.95845 * mm, 12.92710 * mm,
//                                              12.88499 * mm, 12.80620 * mm, 12.75031 * mm, 12.71820 * mm,
//                                              12.68084 * mm, 12.64654 * mm, 12.57481 * mm, 12.53474 * mm,
//                                              12.50508 * mm, 12.43781 * mm, 12.39943 * mm, 12.35708 * mm,
//                                              12.29325 * mm, 12.24492 * mm, 12.20272 * mm, 12.17086 * mm,
//                                              12.11495 * mm, 12.06118 * mm, 12.02942 * mm, 11.95323 * mm,
//                                              11.92041 * mm, 11.90945 * mm, 11.81912 * mm, 11.76223 * mm,
//                                              11.72925 * mm, 11.64656 * mm, 11.59008 * mm, 11.55716 * mm,
//                                              11.45604 * mm, 11.35718 * mm, 11.29203 * mm, 11.16896 * mm,
//                                              11.05424 * mm, 10.93651 * mm, 10.76848 * mm, 10.58308 * mm,
//                                              10.37474 * mm, 10.13175 * mm,  9.85383 * mm,  9.54595 * mm,
//                                               9.21473 * mm,  8.89823 * mm,  8.61788 * mm,  8.33931 * mm,
//                                               8.08924 * mm,  7.88285 * mm,  7.67611 * mm,  7.48203 * mm,
//                                               7.27135 * mm,  6.98990 * mm,  6.62925 * mm,  6.53135 * mm,
//                                               6.39524 * mm,  6.25592 * mm,  6.11400 * mm,  5.97004 * mm,
//                                               5.82460 * mm,  5.67818 * mm,  5.53127 * mm,  5.38430 * mm,
//                                               5.23767 * mm,  5.09174 * mm,  4.94683 * mm,  4.80325 * mm,
//                                               4.66125 * mm,  4.52105 * mm,  4.38285 * mm,  4.24681 * mm,
//                                               4.11308 * mm};

  G4double refraction_index[entries] = {1.45041, 1.45133, 1.45224, 1.45314, 1.45406,
                                        1.45499, 1.45594, 1.45692, 1.45793, 1.45898,
                                        1.46007, 1.46119, 1.46237, 1.46358, 1.46485,
                                        1.46616, 1.46752, 1.46894, 1.47041, 1.47194,
                                        1.47353, 1.47517, 1.47688, 1.47865, 1.48049,
                                        1.48240, 1.48437, 1.48642, 1.48855, 1.49075,
                                        1.49303, 1.49539, 1.49784, 1.50038, 1.50302,
                                        1.50575, 1.50858, 1.51151, 1.51456, 1.51772,
                                        1.52099, 1.52440, 1.52793, 1.53160, 1.53541,
                                        1.53938, 1.54350, 1.54780, 1.55227, 1.55692,
                                        1.56178, 1.56685, 1.57215, 1.57768, 1.58347,
                                        1.58953, 1.59588, 1.60256, 1.60957, 1.61694,
                                        1.62471, 1.63292, 1.64159, 1.65077, 1.66052,
                                        1.67088};

  // Propiedades del filtro como material
  G4MaterialPropertiesTable* silica_mpt = new G4MaterialPropertiesTable();
  silica_mpt->AddProperty("RINDEX", energies, refraction_index, entries);
  silica_mpt->AddProperty("ABSLENGTH", energies, absorption_length_lin, entries);

  fused_silica->SetMaterialPropertiesTable(silica_mpt);


  // Propiedades de la superficie del filtro
  G4OpticalSurface *filter_opsurf =
      new G4OpticalSurface("FILTER_OPSURF", unified, polished, dielectric_dielectric);

  G4double reflectivity[entries] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0. };

  G4double transmittance[entries] = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                                    1., 1., 1., 1., 1., 1., 1., 1., 1.};

  G4MaterialPropertiesTable* filter_mpt = new G4MaterialPropertiesTable();
  filter_mpt->AddProperty("REFLECTIVITY", energies, reflectivity, entries);
  filter_mpt->AddProperty("TRANSMITTANCE", energies, transmittance, entries);
  filter_opsurf->SetMaterialPropertiesTable(filter_mpt);

  new G4LogicalSkinSurface("FILTER_OPSURF", filter_logic, filter_opsurf);


  G4VisAttributes filter_col = G4Colour::Yellow();
  filter_logic->SetVisAttributes(filter_col);

}
