import pytest
import os

import pandas as pd

def test_opt_photons_and_ioni_elec_are_produced_by_nest(file_name_nest):
    """Check that optical photons come from the NEST process, called 'S1'"""

    particles = pd.read_hdf(file_name_nest, 'MC/particles')
    selection = (particles.particle_name == 'opticalphoton') | (particles.particle_name == 'thermalelectron') 
    opt_photons_and_elec = particles[selection]

    creator_processes = opt_photons_and_elec.creator_proc.unique()
    
    assert creator_processes == ['S1']
    
