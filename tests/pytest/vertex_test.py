import pytest
import os

import pandas as pd

def test_vertices_are_generated_in_phantom(file_name_phantom):
    """Check that the vertices are produced only in the phantom"""

    particles = pd.read_hdf(file_name_phantom, 'MC/particles')
    primaries = particles[particles.primary == 1]
    volumes   = primaries.initial_volume

    correct_volumes = ['WATER_BCKG', 'ROD0', 'ROD1', 'ROD2', 'ROD3', 'ROD4',
                       'ROD5', 'SPHERE0', 'SPHERE1', 'SPHERE2', 'SPHERE3',
                       'SPHERE4', 'SPHERE5']

    assert volumes.all() in correct_volumes
