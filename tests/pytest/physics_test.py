import pytest
import os

import pandas as pd

def test_opt_photons_are_produced_by_nest(file_name_nest):
    """Check that optical photons come from the NEST process, called 'S1'"""

    particles   = pd.read_hdf(file_name_nest, 'MC/particles')
    opt_photons = particles[particles.particle_name == 'opticalphoton']

    creator_processes = opt_photons.creator_proc.unique()
    
    assert creator_processes == ['S1']
    
