// ----------------------------------------------------------------------------
///  \file   
///  \brief  
///
///  \author   <miquel.nebot@ific.uv.es>,
///  \date     14 Oct 2013
///  \version  $Id$
///
///  Copyright (c) 2013 NEXT Collaboration
// ----------------------------------------------------------------------------

#ifndef __NEXTNEW_ICS__
#define __NEXTNEW_ICS__

#include "BaseGeometry.h"
#include "CylinderPointSampler.h"

#include <G4Navigator.hh>

class G4GenericMessenger;

namespace nexus {

  class NextNewIcs: public BaseGeometry
  {
  public:
    /// Constructor
    NextNewIcs(const G4double lat_nozzle_z_pos,
	       const G4double up_nozzle_z_pos );

    /// Destructor
    ~NextNewIcs();

    /// Generate a vertex within a given region of the geometry
    G4ThreeVector GenerateVertex(const G4String& region) const;

    /// Builder
    void Construct();


  private:
    // Dimensions
    G4double _body_out_diam, _body_length, _body_thickness;
    G4double _tracking_tread_diam, _tracking_tread_length;
   
    // Dimensions coming from outside  
    G4double  _lat_nozzle_in_diam, _lat_nozzle_x_pos, _lat_nozzle_z_pos;
    G4double  _up_nozzle_in_diam, _up_nozzle_y_pos, _up_nozzle_z_pos;

    // Visibility of the shielding
    G4bool _visibility;

    // Vertex generators
    CylinderPointSampler* _body_gen;
    CylinderPointSampler* _tread_gen;
    G4double _body_perc;

    // Geometry Navigator
    G4Navigator* _geom_navigator;

    // Messenger for the definition of control commands
    G4GenericMessenger* _msg; 

  };

} // end namespace nexus

#endif