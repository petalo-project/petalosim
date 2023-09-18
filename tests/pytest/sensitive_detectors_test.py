import pandas as pd
import numpy  as np
import os


def test_sensor_ids(petalosim_params):
     """
     Check that sensors are correctly numbered.
     """
     filename, nsipms, n_boards, sipms_per_board, board_ordering = petalosim_params

     sns_response = pd.read_hdf(filename, 'MC/sns_response')
     sipm_ids     = sns_response.sensor_id.unique()

     assert 1 < len(sipm_ids) <= nsipms

     # No sensor position is repeated
     sns_positions = pd.read_hdf(filename, 'MC/sns_positions')
     assert len(sns_positions) == len(sns_positions.sensor_id.unique())

     if n_boards != 0:
          # SiPM-board numbering is correct
          assert (sipm_ids // board_ordering).min() >= 1
          assert (sipm_ids // board_ordering).max() <= n_boards

          # SiPM number inside boards is correct
          assert (sipm_ids % board_ordering).min() >= 0
          assert (sipm_ids % board_ordering).max() <=  sipms_per_board


def test_sensor_ids_pet_box(petalosim_pet_box_params):
     """
     Check that sensors are correctly numbered for the geometry,
     the charge of the event is above a threshold and that the true
     information is stored if charge is detected by the sensors.
     """
     filename, _, _, nsipms, sipms_per_tile, init_sns_id1, init_sns_id2, sensor_name, min_charge_evt = petalosim_pet_box_params

     sns_response = pd.read_hdf(filename, 'MC/sns_response')
     sipm_ids     = sns_response.sensor_id.unique()
     sipm_ids1    = sipm_ids[sipm_ids < 100]
     sipm_ids2    = sipm_ids[sipm_ids > 100]

     assert 1 < len(sipm_ids) <= nsipms

     # No sensor position is repeated
     sns_positions = pd.read_hdf(filename, 'MC/sns_positions')
     assert len(sns_positions) == len(sns_positions.sensor_id.unique())
     assert min(sipm_ids) >= init_sns_id1

     if len(sensor_name) == 2:
         assert sns_positions.sensor_name.nunique() == 2
     else:
         assert sns_positions.sensor_name.unique() == sensor_name

     first_id_second_plane = 111
     sns_z_pos_left  = sns_positions[ sns_positions.sensor_id<first_id_second_plane].z.values
     sns_z_pos_right = sns_positions[~sns_positions.sensor_id<first_id_second_plane].z.values
     if len(sns_z_pos_left) > 0:
         assert len(np.unique(sns_z_pos_left)) == 1
         assert     np.unique(sns_z_pos_left)  < 0
         assert len(sns_z_pos_left) <= sipms_per_tile*4

     if len(sns_z_pos_right) > 0:
         assert len(np.unique(sns_z_pos_right)) == 1
         assert     np.unique(sns_z_pos_right)  < 0
         assert len(sns_z_pos_right) <= sipms_per_tile*4

     # Charge of the whole event above a certain threshold
     for evt_charge in sns_response.groupby('event_id').charge.sum():
         assert evt_charge >= min_charge_evt

     # Check that all the stored events have detected charge
     mcparticles = pd.read_hdf(filename, 'MC/particles')
     assert mcparticles.event_id.nunique() == sns_response.event_id.nunique()


def test_sensor_ids_petit_pyrex_blue(output_tmpdir, base_name_pyrex):
     """
     Check that sensors are correctly numbered for the geometry,
     the charge of the event is above a threshold and that the true
     information is stored if charge is detected by the sensors.
     """

     nsipms         = 128
     sipms_per_tile =  16
     init_sns_id1   =  11
     sensor_name    = 'SiPMHmtsuBlue'
     min_charge_evt = 50
     filename = os.path.join(output_tmpdir, base_name_pyrex+'.h5')

     sns_response = pd.read_hdf(filename, 'MC/sns_response')
     sipm_ids     = sns_response.sensor_id.unique()
     sipm_ids1    = sipm_ids[sipm_ids < 100]
     sipm_ids2    = sipm_ids[sipm_ids > 100]

     assert 1 < len(sipm_ids1) <= nsipms
     assert 1 < len(sipm_ids2) <= nsipms

     # No sensor position is repeated
     sns_positions = pd.read_hdf(filename, 'MC/sns_positions')
     assert len(sns_positions) == len(sns_positions.sensor_id.unique())
     assert min(sipm_ids) >= init_sns_id1

     if len(sensor_name) == 2:
         assert sns_positions.sensor_name.nunique() == 2
     else:
         assert sns_positions.sensor_name.unique() == sensor_name

     first_id_second_plane = 111
     sns_z_pos_left  = sns_positions[ sns_positions.sensor_id<first_id_second_plane].z.values
     sns_z_pos_right = sns_positions[~sns_positions.sensor_id<first_id_second_plane].z.values
     if len(sns_z_pos_left) > 0:
         assert len(np.unique(sns_z_pos_left)) == 1
         assert     np.unique(sns_z_pos_left)  < 0
         assert len(sns_z_pos_left) <= sipms_per_tile*4

     if len(sns_z_pos_right) > 0:
         assert len(np.unique(sns_z_pos_right)) == 1
         assert     np.unique(sns_z_pos_right)  < 0
         assert len(sns_z_pos_right) <= sipms_per_tile*4

     # Charge of the whole event above a certain threshold
     for evt_charge in sns_response.groupby('event_id').charge.sum():
         assert evt_charge >= min_charge_evt

     # Check that all the stored events have detected charge
     mcparticles = pd.read_hdf(filename, 'MC/particles')
     assert mcparticles.event_id.nunique() == sns_response.event_id.nunique()
