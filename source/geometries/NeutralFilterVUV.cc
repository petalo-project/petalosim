// ----------------------------------------------------------------------------
// petalosim | MicroCellHmtsuVUV.cc
//
// Neutral VUV filter.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "NeutralFilterVUV.h"

#include "nexus/MaterialsList.h"

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

  G4Material* fused_silica = materials::FusedSilica();

  G4LogicalVolume* filter_logic =
    new G4LogicalVolume(filter_solid, fused_silica, "FILTER");

  this->SetLogicalVolume(filter_logic);

  // Adjusted filter absorption length.
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

  G4double absorption_length_lin[entries] = {26.80192 * mm, 26.72297 * mm, 26.69387 * mm, 26.47202 * mm,
                                             25.99263 * mm, 25.63203 * mm, 25.26811 * mm, 24.97453 * mm,
                                             24.89241 * mm, 24.56914 * mm, 24.43542 * mm, 24.39358 * mm,
                                             24.27648 * mm, 24.16932 * mm, 24.04035 * mm, 23.83741 * mm,
                                             23.68487 * mm, 23.58949 * mm, 23.39445 * mm, 23.30710 * mm,
                                             23.18099 * mm, 22.94620 * mm, 22.80031 * mm, 22.72820 * mm,
                                             22.64084 * mm, 22.54654 * mm, 22.35481 * mm, 22.23474 * mm,
                                             22.14708 * mm, 21.96781 * mm, 21.86943 * mm, 21.72708 * mm,
                                             21.56325 * mm, 21.42492 * mm, 21.30272 * mm, 21.23086 * mm,
                                             21.08495 * mm, 20.92218 * mm, 20.82942 * mm, 20.65323 * mm,
                                             20.58041 * mm, 20.55945 * mm, 20.33912 * mm, 20.22223 * mm,
                                             20.14925 * mm, 19.94656 * mm, 19.86008 * mm, 19.76716 * mm,
                                             19.53604 * mm, 19.31718 * mm, 19.19203 * mm, 18.92896 * mm,
                                             18.62424 * mm, 18.38651 * mm, 18.01848 * mm, 17.58308 * mm,
                                             17.03474 * mm, 16.51175 * mm, 15.87383 * mm, 15.08595 * mm,
                                             14.28473 * mm, 13.61823 * mm, 13.01788 * mm, 12.49931 * mm,
                                             12.02524 * mm, 11.68285 * mm, 11.33611 * mm, 11.03203 * mm,
                                             10.70135 * mm, 10.23990 * mm,  9.6347  * mm, 9.481250 * mm,
                                              9.27307 * mm,  9.06490 * mm,  8.85672 * mm, 8.648553 * mm,
                                              8.44037 * mm,  8.23220 * mm,  8.02403 * mm, 7.815855 * mm,
                                              7.60768 * mm,  7.39950 * mm,  7.19133 * mm, 6.983158 * mm,
                                              6.77498 * mm,  6.56680 * mm,  6.35863 * mm, 6.150461 * mm,
                                              5.94228 * mm};

//  G4double absorption_length_quad[entries] = {26.80192 * mm, 26.72297 * mm, 26.69387 * mm, 26.47202 * mm,
//                                              25.99263 * mm, 25.63203 * mm, 25.26811 * mm, 24.97453 * mm,
//                                              24.89241 * mm, 24.56914 * mm, 24.43542 * mm, 24.39358 * mm,
//                                              24.27648 * mm, 24.16932 * mm, 24.04035 * mm, 23.83741 * mm,
//                                              23.68487 * mm, 23.58949 * mm, 23.39445 * mm, 23.3071  * mm,
//                                              23.18099 * mm, 22.9462  * mm, 22.80031 * mm, 22.7282  * mm,
//                                              22.64084 * mm, 22.54654 * mm, 22.35481 * mm, 22.23474 * mm,
//                                              22.14708 * mm, 21.96781 * mm, 21.86943 * mm, 21.72708 * mm,
//                                              21.56325 * mm, 21.42492 * mm, 21.30272 * mm, 21.23086 * mm,
//                                              21.08495 * mm, 20.92218 * mm, 20.82942 * mm, 20.65323 * mm,
//                                              20.58041 * mm, 20.55945 * mm, 20.33912 * mm, 20.22223 * mm,
//                                              20.14925 * mm, 19.94656 * mm, 19.86008 * mm, 19.76716 * mm,
//                                              19.53604 * mm, 19.31718 * mm, 19.19203 * mm, 18.92896 * mm,
//                                              18.62424 * mm, 18.38651 * mm, 18.01848 * mm, 17.58308 * mm,
//                                              17.03474 * mm, 16.51175 * mm, 15.87383 * mm, 15.08595 * mm,
//                                              14.28473 * mm, 13.61823 * mm, 13.01788 * mm, 12.49931 * mm,
//                                              12.02524 * mm, 11.68285 * mm, 11.33611 * mm, 11.03203 * mm,
//                                              10.70135 * mm, 10.2399  * mm,  9.6347  * mm,  9.46603 * mm,
//                                               9.22739 * mm,  8.97742 * mm,  8.71612 * mm,  8.44350 * mm,
//                                               8.15955 * mm,  7.86428 * mm,  7.55768 * mm,  7.23976 * mm,
//                                               6.91052 * mm,  6.56995 * mm,  6.21805 * mm,  5.85483 * mm,
//                                               5.48028 * mm,  5.09441 * mm,  4.69722 * mm,  4.28869 * mm,
//                                               3.86885 * mm};

  G4double refraction_index[entries] = {1.45020, 1.45035, 1.45049, 1.45064, 1.45079,
                                        1.45095, 1.45110, 1.45126, 1.45143, 1.45159,
                                        1.45177, 1.45194, 1.45212, 1.45230, 1.45249,
                                        1.45268, 1.45288, 1.45309, 1.45330, 1.45352,
                                        1.45374, 1.45397, 1.45421, 1.45446, 1.45472,
                                        1.45498, 1.45526, 1.45555, 1.45585, 1.45616,
                                        1.45649, 1.45683, 1.45718, 1.45756, 1.45795,
                                        1.45836, 1.45879, 1.45925, 1.45973, 1.46024,
                                        1.46077, 1.46134, 1.46195, 1.46259, 1.46328,
                                        1.46401, 1.46480, 1.46564, 1.46654, 1.46751,
                                        1.46856, 1.46970, 1.47093, 1.47227, 1.47374,
                                        1.47534, 1.47710, 1.47904, 1.48119, 1.48357,
                                        1.48624, 1.48923, 1.49261, 1.49645, 1.50084,
                                        1.50589, 1.51178, 1.51869, 1.52690, 1.53681,
                                        1.54895, 1.55227, 1.55692, 1.56178, 1.56685,
                                        1.57215, 1.57768, 1.58347, 1.58953, 1.59588,
                                        1.60256, 1.60957, 1.61694, 1.62471, 1.63292,
                                        1.64159, 1.65077, 1.66052, 1.67088};

  // Add filter properties due to the fused silica material
  G4MaterialPropertiesTable* silica_mpt = new G4MaterialPropertiesTable();
  silica_mpt->AddProperty("RINDEX", energies, refraction_index, entries);
  silica_mpt->AddProperty("ABSLENGTH", energies, absorption_length_lin_new, entries);

  fused_silica->SetMaterialPropertiesTable(silica_mpt);


  G4VisAttributes filter_col = G4Colour::Yellow();
  filter_logic->SetVisAttributes(filter_col);

}
