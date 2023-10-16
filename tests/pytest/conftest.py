import pytest
import os


@pytest.fixture(scope = 'session')
def PETALODIR():
    return os.environ['PETALODIR']


@pytest.fixture(scope = 'session')
def config_tmpdir(tmpdir_factory):
    return tmpdir_factory.mktemp('configure_tests')


@pytest.fixture(scope = 'session')
def output_tmpdir(tmpdir_factory):
    return tmpdir_factory.mktemp('output_files')


@pytest.fixture(scope = 'session')
def base_name_full_body():
    return 'PET_full_body_sd_test'

@pytest.fixture(scope = 'session')
def petalosim_params_full_body(output_tmpdir, base_name_full_body):
    n_sipm          = 102304
    n_boards        = 0
    sipms_per_board = 0
    board_ordering  = 0
    return os.path.join(output_tmpdir, base_name_full_body+'.h5'), n_sipm, n_boards, sipms_per_board, board_ordering


@pytest.fixture(scope = 'session')
def base_name_nest():
    return 'PET_nest_test'

@pytest.fixture(scope = 'session')
def file_name_nest(output_tmpdir, base_name_nest):
    return os.path.join(output_tmpdir, base_name_nest+'.h5')

@pytest.fixture(scope = 'session')
def petalosim_params_nest(output_tmpdir, base_name_nest):
    n_sipm          = 102304
    n_boards        = 0
    sipms_per_board = 0
    board_ordering  = 0
    return os.path.join(output_tmpdir, base_name_nest+'.h5'), n_sipm, n_boards, sipms_per_board, board_ordering


@pytest.fixture(scope = 'session')
def base_name_phantom():
    return 'PET_phantom_test'

@pytest.fixture(scope = 'session')
def file_name_phantom(output_tmpdir, base_name_phantom):
    return os.path.join(output_tmpdir, base_name_phantom+'.h5')

@pytest.fixture(scope = 'session')
def base_name_pyrex():
    return 'PETit_pyrex_test'

@pytest.fixture(scope = 'session')
def file_name_pyrex(output_tmpdir, base_name_pyrex):
    return os.path.join(output_tmpdir, base_name_pyrex+'.h5')

@pytest.fixture(scope = 'session')
def petalosim_params_pyrex_HamamatsuBlue(output_tmpdir, base_name_pyrex):
    n_sipm         = 128
    sipms_per_tile =  16
    init_sns_id1   =  11
    init_sns_id2   = 111
    sensor_name    = 'SiPMHmtsuBlue'
    min_charge_evt = 50
    return os.path.join(output_tmpdir, base_name_pyrex+'.h5'),  n_sipm, sipms_per_tile, init_sns_id1, init_sns_id2, sensor_name, min_charge_evt


@pytest.fixture(scope = 'session')
def base_name_ring_tiles():
    return 'PET_ring_tiles_sd_test'

@pytest.fixture(scope = 'session')
def petalosim_params_ring_tiles(output_tmpdir, base_name_ring_tiles):
    n_sipm          = 3840
    n_boards        = 120
    sipms_per_board = 32
    board_ordering  = 1000
    return os.path.join(output_tmpdir, base_name_ring_tiles+'.h5'), n_sipm, n_boards, sipms_per_board, board_ordering


@pytest.fixture(scope = 'session')
def base_name_pet_box_HamamatsuVUV():
    return 'PET_box_HamamatsuVUV_sd_test'

@pytest.fixture(scope = 'session')
def base_name_pet_box_HamamatsuBlue():
    return 'PET_box_HamamatsuBlue_sd_test'

@pytest.fixture(scope = 'session')
def base_name_pet_box_FBK():
    return 'PET_box_FBK_sd_test'

@pytest.fixture(scope = 'session')
def base_name_pet_box_mix_Ham_FBK():
    return 'PET_box_mix_Ham_FBK_sd_test'


@pytest.fixture(scope = 'session')
def petalosim_params_pet_box_HamamatsuVUV(output_tmpdir):
    base_name      = 'PET_box_HamamatsuVUV_sd_test'
    geom_type      = 'PETit'
    n_sipm         = 128
    sipms_per_tile =  16
    init_sns_id1   =  11
    init_sns_id2   = 111
    sensor_name    = 'SiPMHmtsuVUV'
    min_charge_evt = 50
    return os.path.join(output_tmpdir, base_name+'.h5'), base_name, geom_type, n_sipm, sipms_per_tile, init_sns_id1, init_sns_id2, sensor_name, min_charge_evt


@pytest.fixture(scope = 'session')
def petalosim_params_pet_box_FBK(output_tmpdir):
    base_name      = 'PET_box_FBK_sd_test'
    geom_type      = 'PETitFBK'
    n_sipm         = 512
    sipms_per_tile =  64
    init_sns_id1   =  11
    init_sns_id2   = 111
    sensor_name    = 'SiPMFBKVUV'
    min_charge_evt = 50
    return os.path.join(output_tmpdir, base_name+'.h5'), base_name, geom_type, n_sipm, sipms_per_tile, init_sns_id1, init_sns_id2, sensor_name, min_charge_evt

@pytest.fixture(scope = 'session')
def petalosim_params_pet_box_mix_Ham_FBK(output_tmpdir):
    base_name      = 'PET_box_mix_Ham_FBK_sd_test'
    geom_type      = 'PETitPyrexMix'
    n_sipm         = 320
    sipms_per_tile = 64
    init_sns_id1   = 11
    init_sns_id2   = 111
    sensor_name    = 'SiPMFBKVUV', 'SiPMHmtsuVUV'
    min_charge_evt = 50
    return os.path.join(output_tmpdir, base_name+'.h5'), base_name, geom_type, n_sipm, sipms_per_tile, init_sns_id1, init_sns_id2, sensor_name, min_charge_evt


@pytest.fixture(scope="module",
                params=["base_name_full_body", "base_name_nest", "base_name_ring_tiles",
                        "base_name_pet_box_HamamatsuVUV",
                        "base_name_pet_box_FBK", "base_name_pet_box_mix_Ham_FBK", "base_name_pyrex"],
                ids=["full_body", "nest", "ring_tiles", "pet_box_HamamatsuVUV", "pet_box_FBK", "pet_box_mix_Ham_FBK", "petit_pyrex"])
def petalosim_files(request, output_tmpdir):
    return os.path.join(output_tmpdir, request.getfixturevalue(request.param)+'.h5')


@pytest.fixture(scope="module",
                params=["petalosim_params_full_body", "petalosim_params_nest",
                        "petalosim_params_ring_tiles"],
                ids=["full_body", "nest", "ring_tiles"])
def petalosim_params(request):
    return request.getfixturevalue(request.param)


@pytest.fixture(scope="module",
                params=["petalosim_params_pet_box_HamamatsuVUV",
                        "petalosim_params_pet_box_FBK", "petalosim_params_pet_box_mix_Ham_FBK"],
                ids=["pet_box_HamamatsuVUV", "pet_box_FBK", 'pet_box_mix_Ham_FBK'])
def petalosim_pet_box_params(request):
    return request.getfixturevalue(request.param)
