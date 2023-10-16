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
def params_full_body(output_tmpdir, base_name_full_body):
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
def params_nest(output_tmpdir, base_name_nest):
    n_sipm          = 102304
    n_boards        = 0
    sipms_per_board = 0
    board_ordering  = 0
    return os.path.join(output_tmpdir, base_name_nest+'.h5'), n_sipm, n_boards, sipms_per_board, board_ordering


@pytest.fixture(scope = 'session')
def base_name_ring_tiles():
    return 'PET_ring_tiles_sd_test'

@pytest.fixture(scope = 'session')
def params_ring_tiles(output_tmpdir, base_name_ring_tiles):
    n_sipm          = 3840
    n_boards        = 120
    sipms_per_board = 32
    board_ordering  = 1000
    return os.path.join(output_tmpdir, base_name_ring_tiles+'.h5'), n_sipm, n_boards, sipms_per_board, board_ordering


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
def base_name_sat_Hama():
    return 'PETit_sat_Hama_test'

@pytest.fixture(scope = 'session')
def file_name_sat_Hama(output_tmpdir, base_name_sat_Hama):
    return os.path.join(output_tmpdir, base_name_sat_Hama+'.h5')

@pytest.fixture(scope = 'session')
def petit_params_sat_Hama(output_tmpdir, base_name_sat_Hama):
    geom_type  = 'PETit'
    n_pixels   = 788736
    n_sipms    = 128
    separation_pxls  = 1000000
    separation_sipms = 100
    return os.path.join(output_tmpdir, base_name_sat_Hama+'.h5'), base_name_sat_Hama, geom_type, n_pixels, n_sipms, separation_pxls, separation_sipms


@pytest.fixture(scope = 'session')
def base_name_sat_FBK():
    return 'PETit_sat_FBK_test'

@pytest.fixture(scope = 'session')
def file_name_sat_FBK(output_tmpdir, base_name_sat_FBK):
    return os.path.join(output_tmpdir, base_name_sat_FBK+'.h5')

@pytest.fixture(scope = 'session')
def petit_params_sat_FBK(output_tmpdir, base_name_sat_FBK):
    geom_type  = 'PETitFBK'
    n_pixels   = 4221440
    n_sipms    = 512
    separation_pxls = 5000000
    separation_sipms = 500
    return os.path.join(output_tmpdir, base_name_sat_FBK+'.h5'), base_name_sat_FBK, geom_type, n_pixels, n_sipms, separation_pxls, separation_sipms


@pytest.fixture(scope = 'session')
def base_name_petit_HamamatsuVUV():
    return 'PETit_HamamatsuVUV_sd_test'

@pytest.fixture(scope = 'session')
def base_name_petit_HamamatsuBlue():
    return 'PETit_HamamatsuBlue_sd_test'

@pytest.fixture(scope = 'session')
def base_name_petit_FBK():
    return 'PETit_FBK_sd_test'

@pytest.fixture(scope = 'session')
def base_name_petit_mix_Hama_FBK():
    return 'PETit_mix_Hama_FBK_sd_test'


@pytest.fixture(scope = 'session')
def petit_params_HamamatsuVUV(output_tmpdir):
    base_name      = 'PETit_HamamatsuVUV_sd_test'
    geom_type      = 'PETit'
    n_sipm         = 128
    sipms_per_tile =  16
    init_sns_id1   =  11
    init_sns_id2   = 111
    sensor_name    = 'SiPMHmtsuVUV'
    min_charge_evt = 50
    return os.path.join(output_tmpdir, base_name+'.h5'), base_name, geom_type, n_sipm, sipms_per_tile, init_sns_id1, init_sns_id2, sensor_name, min_charge_evt


@pytest.fixture(scope = 'session')
def petit_params_FBK(output_tmpdir):
    base_name      = 'PETit_FBK_sd_test'
    geom_type      = 'PETitFBK'
    n_sipm         = 512
    sipms_per_tile =  64
    init_sns_id1   =  11
    init_sns_id2   = 111
    sensor_name    = 'SiPMFBKVUV'
    min_charge_evt = 50
    return os.path.join(output_tmpdir, base_name+'.h5'), base_name, geom_type, n_sipm, sipms_per_tile, init_sns_id1, init_sns_id2, sensor_name, min_charge_evt

@pytest.fixture(scope = 'session')
def petit_params_mix_Hama_FBK(output_tmpdir):
    base_name      = 'PETit_mix_Hama_FBK_sd_test'
    geom_type      = 'PETitPyrexMix'
    n_sipm         = 320
    sipms_per_tile = 64
    init_sns_id1   = 11
    init_sns_id2   = 111
    sensor_name    = 'SiPMFBKVUV', 'SiPMHmtsuVUV'
    min_charge_evt = 50
    return os.path.join(output_tmpdir, base_name+'.h5'), base_name, geom_type, n_sipm, sipms_per_tile, init_sns_id1, init_sns_id2, sensor_name, min_charge_evt


@pytest.fixture(scope="module",
                params=["base_name_full_body", "base_name_nest",
                        "base_name_ring_tiles",
                        "base_name_petit_HamamatsuVUV",
                        "base_name_petit_FBK",
                        "base_name_petit_mix_Hama_FBK", "base_name_pyrex"],
                ids=["full_body", "nest", "ring_tiles", "petit_HamamatsuVUV",
                     "petit_FBK", "petit_mix_Ham_FBK", "petit_pyrex"])
def petalosim_files(request, output_tmpdir):
    return os.path.join(output_tmpdir, request.getfixturevalue(request.param)+'.h5')


@pytest.fixture(scope="module",
                params=["params_full_body", "params_nest",
                        "params_ring_tiles"],
                ids=["full_body", "nest", "ring_tiles"])
def general_params(request):
    return request.getfixturevalue(request.param)


@pytest.fixture(scope="module",
                params=["petit_params_HamamatsuVUV",
                        "petit_params_FBK",
                        "petit_params_mix_Hama_FBK"],
                ids=["petit_HamamatsuVUV", "petit_FBK",
                     'petit_mix_Ham_FBK'])
def petit_params(request):
    return request.getfixturevalue(request.param)


@pytest.fixture(scope="module",
                params=["petit_params_sat_Hama", "petit_params_sat_FBK"],
                ids=["petit_sat_HamamatsuVUV", "petit_sat_FBK"])
def petit_sat_params(request):
    return request.getfixturevalue(request.param)


