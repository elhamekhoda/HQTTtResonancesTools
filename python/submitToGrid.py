#!/usr/bin/env python
import getpass
import TopExamples.grid
import HQTTtResonancesTools.Data_EXOT4_rel21
import HQTTtResonancesTools.Data_EXOT7_rel21
import HQTTtResonancesTools.MC16a_EXOT4
import HQTTtResonancesTools.MC16a_EXOT7

# Configuration
user = getpass.getuser()
config = TopExamples.grid.Config()
config.CMake         = True
config.code          = 'top-xaod'
config.settingsFile  = 'ttres-ljets-cuts-mc16-25ns.txt'
config.gridUsername  = user
config.excludedSites = ''
config.noSubmit      = False
config.mergeType     = 'None' #'None', 'Default' or 'xAOD'
config.memory        = '1000'
config.maxNFilesPerJob  = '5'

if user=='meehan':
	config.suffix        = '13022018v1'
	config.destSE        = 'DESY-HH_LOCALGROUPDISK'
if user=='scalvet':
	config.suffix        = 'HVTcomb.Oct2018.v4.1'
	config.destSE        = ''


# Input Samples
names   = []
#names  += ['MC16_13TeV_25ns_FS_EXOT4_ttbar_nonallhad']
#names  += ['MC16_13TeV_25ns_FS_EXOT4_singletop']
#names  += ['MC16_13TeV_25ns_FS_EXOT4_ttbarV']
#names  += ['MC16_13TeV_25ns_FS_EXOT4_VV']
#names  += ['MC16_13TeV_25ns_FS_EXOT4_dijets']
#names  += ['MC16_13TeV_25ns_FS_EXOT4_Wjets221']
#names  += ['MC16_13TeV_25ns_FS_EXOT4_Zjets221']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime500']
#names  += ['MC16_13TeV_25ns_EXOT4_HVTcombSamples']
samples = TopExamples.grid.Samples(names)

# Submit Jobs
TopExamples.grid.submit(config, samples)

