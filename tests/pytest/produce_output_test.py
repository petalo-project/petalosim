import pytest

import os
import subprocess


@pytest.mark.order(1)
def test_create_petalo_output_file_full_body(config_tmpdir, output_tmpdir, PETALODIR, base_name_full_body):

     init_text = f"""
/PhysicsList/RegisterPhysics G4EmStandardPhysics_option4
/PhysicsList/RegisterPhysics G4DecayPhysics
/PhysicsList/RegisterPhysics G4RadioactiveDecayPhysics
/PhysicsList/RegisterPhysics G4OpticalPhysics
/PhysicsList/RegisterPhysics PetaloPhysics
/PhysicsList/RegisterPhysics G4StepLimiterPhysics

### GEOMETRY
/nexus/RegisterGeometry FullRingInfinity

### GENERATOR
/nexus/RegisterGenerator Back2backGammas

### ACTIONS
/nexus/RegisterRunAction DefaultRunAction
/nexus/RegisterEventAction PetaloEventAction
/nexus/RegisterTrackingAction PetaloTrackingAction

/nexus/RegisterPersistencyManager PetaloPersistencyManager

/nexus/RegisterMacro {config_tmpdir}/{base_name_full_body}.config.mac
"""
     init_path = os.path.join(config_tmpdir, base_name_full_body+'.init.mac')
     init_file = open(init_path,'w')
     init_file.write(init_text)
     init_file.close()

     config_text = f"""
/run/verbose 1
/event/verbose 0
/tracking/verbose 0

/process/em/verbose 0

/Geometry/FullRingInfinity/depth 3. cm
/Geometry/FullRingInfinity/sipm_pitch 7. mm
/Geometry/FullRingInfinity/inner_radius 380. mm
/Geometry/FullRingInfinity/sipm_rows 278
/Geometry/FullRingInfinity/instrumented_faces 1
/Geometry/FullRingInfinity/specific_vertex 0. 0. 0. cm

/Geometry/SiPMpet/efficiency 0.2
/Geometry/SiPMpet/visibility true
/Geometry/SiPMpet/size 6. mm

/Generator/Back2back/region AD_HOC

/process/optical/processActivation Cerenkov false

/petalosim/persistency/output_file {output_tmpdir}/{base_name_full_body}
/nexus/random_seed 16062020

"""

     config_path = os.path.join(config_tmpdir, base_name_full_body+'.config.mac')
     config_file = open(config_path,'w')
     config_file.write(config_text)
     config_file.close()

     my_env    = os.environ
     petalo_exe = PETALODIR + '/bin/petalo'
     command   = [petalo_exe, '-b', '-n', '20', init_path]
     p         = subprocess.run(command, check=True, env=my_env)


@pytest.mark.order(2)
def test_create_petalo_output_file_ring_tiles(config_tmpdir, output_tmpdir, PETALODIR, base_name_ring_tiles):

     init_text = f"""
/PhysicsList/RegisterPhysics G4EmStandardPhysics_option4
/PhysicsList/RegisterPhysics G4DecayPhysics
/PhysicsList/RegisterPhysics G4RadioactiveDecayPhysics
/PhysicsList/RegisterPhysics G4OpticalPhysics
/PhysicsList/RegisterPhysics PetaloPhysics
/PhysicsList/RegisterPhysics G4StepLimiterPhysics

### GEOMETRY
/nexus/RegisterGeometry FullRingTiles

### GENERATOR
/nexus/RegisterGenerator Back2backGammas

### ACTIONS
/nexus/RegisterRunAction DefaultRunAction
/nexus/RegisterEventAction PetaloEventAction
/nexus/RegisterTrackingAction PetaloTrackingAction

/nexus/RegisterPersistencyManager PetaloPersistencyManager

/nexus/RegisterMacro {config_tmpdir}/{base_name_ring_tiles}.config.mac
"""
     init_path = os.path.join(config_tmpdir, base_name_ring_tiles+'.init.mac')
     init_file = open(init_path,'w')
     init_file.write(init_text)
     init_file.close()

     config_text = f"""
/run/verbose 1
/event/verbose 0
/tracking/verbose 0

/process/em/verbose 0

/Geometry/FullRingTiles/depth 3. cm
/Geometry/FullRingTiles/inner_radius 165. mm
/Geometry/FullRingTiles/tile_rows 2
/Geometry/FullRingTiles/instrumented_faces 1

/Geometry/SiPMpet/efficiency 0.2
/Geometry/SiPMpet/visibility true
/Geometry/SiPMpet/size 3. mm

/Generator/Back2back/region CENTER

/petalosim/persistency/output_file {output_tmpdir}/{base_name_ring_tiles}
/nexus/random_seed 16062020

"""

     config_path = os.path.join(config_tmpdir, base_name_ring_tiles+'.config.mac')
     config_file = open(config_path,'w')
     config_file.write(config_text)
     config_file.close()

     my_env    = os.environ
     petalo_exe = PETALODIR + '/bin/petalo'
     command   = [petalo_exe, '-b', '-n', '20', init_path]
     p         = subprocess.run(command, check=True, env=my_env)


@pytest.mark.order(3)
def test_create_petalo_output_file_petit_all_tiles(config_tmpdir, output_tmpdir, PETALODIR, petit_params):

     _, base_name, geom_type, _, _, _, _, _, min_charge_evt = petit_params

     init_text = f"""
/PhysicsList/RegisterPhysics G4EmStandardPhysics_option4
/PhysicsList/RegisterPhysics G4DecayPhysics
/PhysicsList/RegisterPhysics G4RadioactiveDecayPhysics
/PhysicsList/RegisterPhysics G4OpticalPhysics
/PhysicsList/RegisterPhysics PetaloPhysics
/PhysicsList/RegisterPhysics G4StepLimiterPhysics

### GEOMETRY
/nexus/RegisterGeometry {geom_type}

### GENERATOR
/nexus/RegisterGenerator IonGenerator
#/nexus/RegisterGenerator SingleParticleGenerator

### ACTIONS
/nexus/RegisterRunAction DefaultRunAction
/nexus/RegisterEventAction PetSensorsEventAction
/nexus/RegisterTrackingAction PetaloTrackingAction

/nexus/RegisterPersistencyManager PetaloPersistencyManager

/nexus/RegisterMacro {config_tmpdir}/{base_name}.config.mac
"""
     init_path = os.path.join(config_tmpdir, base_name+'.init.mac')
     init_file = open(init_path,'w')
     init_file.write(init_text)
     init_file.close()

     config_text = f"""
/run/verbose 1
/event/verbose 0
/tracking/verbose 0

/process/em/verbose 0

/Generator/IonGenerator/region SOURCE
/Generator/IonGenerator/atomic_number 11
/Generator/IonGenerator/mass_number 22

/Actions/PetSensorsEventAction/min_charge {min_charge_evt}

/petalosim/persistency/output_file {output_tmpdir}/{base_name}

/nexus/random_seed 23102022
"""

     config_path = os.path.join(config_tmpdir, base_name+'.config.mac')
     config_file = open(config_path,'w')
     config_file.write(config_text)
     config_file.close()

     my_env    = os.environ
     petalo_exe = PETALODIR + '/bin/petalo'
     command   = [petalo_exe, '-b', '-n', '50', init_path]
     p         = subprocess.run(command, check=True, env=my_env)


@pytest.mark.order(4)
def test_create_petalo_output_file_petit_pyrex(config_tmpdir, output_tmpdir, PETALODIR, base_name_pyrex):

     init_text = f"""
/PhysicsList/RegisterPhysics G4EmStandardPhysics_option4
/PhysicsList/RegisterPhysics G4DecayPhysics
/PhysicsList/RegisterPhysics G4RadioactiveDecayPhysics
/PhysicsList/RegisterPhysics G4OpticalPhysics
/PhysicsList/RegisterPhysics PetaloPhysics
/PhysicsList/RegisterPhysics G4StepLimiterPhysics

### GEOMETRY
/nexus/RegisterGeometry PETitPyrex

### GENERATOR
/nexus/RegisterGenerator IonGenerator
#/nexus/RegisterGenerator SingleParticleGenerator

### ACTIONS
/nexus/RegisterRunAction DefaultRunAction
/nexus/RegisterEventAction PetSensorsEventAction
/nexus/RegisterTrackingAction PetaloTrackingAction

/nexus/RegisterPersistencyManager PetaloPersistencyManager

/nexus/RegisterMacro {config_tmpdir}/{base_name_pyrex}.config.mac
"""
     init_path = os.path.join(config_tmpdir, base_name_pyrex+'.init.mac')
     init_file = open(init_path,'w')
     init_file.write(init_text)
     init_file.close()

     config_text = f"""
/run/verbose 1
/event/verbose 0
/tracking/verbose 0

/process/em/verbose 0

/Geometry/PETitPyrex/blue_tiles true

/Generator/IonGenerator/region SOURCE
/Generator/IonGenerator/atomic_number 11
/Generator/IonGenerator/mass_number 22

/Actions/PetSensorsEventAction/min_charge 50

/petalosim/persistency/output_file {output_tmpdir}/{base_name_pyrex}

/nexus/random_seed 23102022
"""

     config_path = os.path.join(config_tmpdir, base_name_pyrex+'.config.mac')
     config_file = open(config_path,'w')
     config_file.write(config_text)
     config_file.close()

     my_env    = os.environ
     petalo_exe = PETALODIR + '/bin/petalo'
     command   = [petalo_exe, '-b', '-n', '20', init_path]
     p         = subprocess.run(command, check=True, env=my_env)


@pytest.mark.order(5)
def test_create_petalo_output_file_petit_saturation(config_tmpdir, output_tmpdir, PETALODIR, petit_sat_params):

     _, base_name, geom_type, _, _, _, _ = petit_sat_params

     init_text = f"""
/PhysicsList/RegisterPhysics G4EmStandardPhysics_option4
/PhysicsList/RegisterPhysics G4DecayPhysics
/PhysicsList/RegisterPhysics G4RadioactiveDecayPhysics
/PhysicsList/RegisterPhysics G4OpticalPhysics
/PhysicsList/RegisterPhysics PetaloPhysics
/PhysicsList/RegisterPhysics G4StepLimiterPhysics

### GEOMETRY
/nexus/RegisterGeometry {geom_type}

### GENERATOR
/nexus/RegisterGenerator IonGenerator
#/nexus/RegisterGenerator SingleParticleGenerator

### ACTIONS
/nexus/RegisterRunAction DefaultRunAction
/nexus/RegisterEventAction PetSensorsEventAction
/nexus/RegisterTrackingAction PetaloTrackingAction

/nexus/RegisterPersistencyManager PetaloPersistencyManager

/nexus/RegisterMacro {config_tmpdir}/{base_name}.config.mac
"""
     init_path = os.path.join(config_tmpdir, base_name+'.init.mac')
     init_file = open(init_path,'w')
     init_file.write(init_text)
     init_file.close()

     config_text = f"""
/run/verbose 1
/event/verbose 0
/tracking/verbose 0

/process/em/verbose 0

/Geometry/{geom_type}/sipm_cells true

/Generator/IonGenerator/region SOURCE
/Generator/IonGenerator/atomic_number 11
/Generator/IonGenerator/mass_number 22

/petalosim/persistency/sipm_cells true
/petalosim/persistency/save_tot_charge false

/petalosim/persistency/output_file {output_tmpdir}/{base_name}

/nexus/random_seed 23102022
"""

     config_path = os.path.join(config_tmpdir, base_name+'.config.mac')
     config_file = open(config_path,'w')
     config_file.write(config_text)
     config_file.close()

     my_env    = os.environ
     petalo_exe = PETALODIR + '/bin/petalo'
     command   = [petalo_exe, '-b', '-n', '20', init_path]
     p         = subprocess.run(command, check=True, env=my_env)


@pytest.mark.order(6)
def test_create_petalo_output_file_nest(config_tmpdir, output_tmpdir, PETALODIR, base_name_nest):

     init_text = f"""
/PhysicsList/RegisterPhysics G4EmStandardPhysics_option4
/PhysicsList/RegisterPhysics G4DecayPhysics
/PhysicsList/RegisterPhysics G4RadioactiveDecayPhysics
/PhysicsList/RegisterPhysics G4OpticalPhysics
/PhysicsList/RegisterPhysics PetaloPhysics
/PhysicsList/RegisterPhysics G4StepLimiterPhysics

### GEOMETRY
/nexus/RegisterGeometry FullRingInfinity

### GENERATOR
/nexus/RegisterGenerator Back2backGammas

### ACTIONS
/nexus/RegisterRunAction DefaultRunAction
/nexus/RegisterEventAction PetaloEventAction
/nexus/RegisterTrackingAction OpticalTrackingAction
/nexus/RegisterStackingAction PetNESTStackingAction

/nexus/RegisterPersistencyManager PetaloPersistencyManager

/nexus/RegisterMacro {config_tmpdir}/{base_name_nest}.config.mac
"""
     init_path = os.path.join(config_tmpdir, base_name_nest+'.init.mac')
     init_file = open(init_path,'w')
     init_file.write(init_text)
     init_file.close()

     config_text = f"""
/run/verbose 1
/event/verbose 0
/tracking/verbose 0

/process/em/verbose 0

/Geometry/FullRingInfinity/depth 3. cm
/Geometry/FullRingInfinity/sipm_pitch 7. mm
/Geometry/FullRingInfinity/inner_radius 380. mm
/Geometry/FullRingInfinity/sipm_rows 278
/Geometry/FullRingInfinity/instrumented_faces 1
/Geometry/FullRingInfinity/specific_vertex 0. 0. 0. cm

/Geometry/SiPMpet/efficiency 0.2
/Geometry/SiPMpet/visibility true
/Geometry/SiPMpet/size 6. mm

/Generator/Back2back/region AD_HOC

/process/optical/processActivation Scintillation false
/process/optical/processActivation Cerenkov false

/PhysicsList/Petalo/nest true

/petalosim/persistency/output_file {output_tmpdir}/{base_name_nest}
/nexus/random_seed 16062020

"""

     config_path = os.path.join(config_tmpdir, base_name_nest+'.config.mac')
     config_file = open(config_path,'w')
     config_file.write(config_text)
     config_file.close()

     my_env    = os.environ
     petalo_exe = PETALODIR + '/bin/petalo'
     command   = [petalo_exe, '-b', '-n', '1', init_path]
     p         = subprocess.run(command, check=True, env=my_env)


@pytest.mark.order(6)
def test_create_petalo_output_file_phantom(config_tmpdir, output_tmpdir, PETALODIR, base_name_phantom):

     init_text = f"""
/PhysicsList/RegisterPhysics G4EmStandardPhysics_option4
/PhysicsList/RegisterPhysics G4DecayPhysics
/PhysicsList/RegisterPhysics G4RadioactiveDecayPhysics
/PhysicsList/RegisterPhysics PetaloPhysics
/PhysicsList/RegisterPhysics G4StepLimiterPhysics

### GEOMETRY
/nexus/RegisterGeometry FullRingInfinity

### GENERATOR
/nexus/RegisterGenerator Back2backGammas

### ACTIONS
/nexus/RegisterRunAction DefaultRunAction
/nexus/RegisterEventAction PetaloEventAction
/nexus/RegisterTrackingAction DefaultTrackingAction

/nexus/RegisterPersistencyManager PetaloPersistencyManager

/nexus/RegisterMacro {config_tmpdir}/{base_name_phantom}.config.mac
"""
     init_path = os.path.join(config_tmpdir, base_name_phantom+'.init.mac')
     init_file = open(init_path,'w')
     init_file.write(init_text)
     init_file.close()

     config_text = f"""
/run/verbose 1
/event/verbose 0
/tracking/verbose 0

/process/em/verbose 0

/Geometry/FullRingInfinity/depth 3. cm
/Geometry/FullRingInfinity/sipm_pitch 7. mm
/Geometry/FullRingInfinity/inner_radius 380. mm
/Geometry/FullRingInfinity/sipm_rows 278
/Geometry/FullRingInfinity/instrumented_faces 1
/Geometry/FullRingInfinity/phantom true

/Geometry/SiPMpet/efficiency 0.2
/Geometry/SiPMpet/visibility true
/Geometry/SiPMpet/size 6. mm

/Generator/Back2back/region JPHANTOM

/petalosim/persistency/output_file {output_tmpdir}/{base_name_phantom}
/nexus/random_seed 16062020

"""

     config_path = os.path.join(config_tmpdir, base_name_phantom+'.config.mac')
     config_file = open(config_path,'w')
     config_file.write(config_text)
     config_file.close()

     my_env    = os.environ
     petalo_exe = PETALODIR + '/bin/petalo'
     command   = [petalo_exe, '-b', '-n', '10000', init_path]
     p         = subprocess.run(command, check=True, env=my_env)
