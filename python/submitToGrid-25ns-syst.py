#!/usr/bin/env python
import getpass
import TopExamples.grid
import HQTTtResonancesTools.DC15MC13TeV_25ns_EXOT4
import HQTTtResonancesTools.DC15Data13TeV_25ns_EXOT4

config = TopExamples.grid.Config()
config.code          = 'top-xaod'
config.settingsFile  = 'ttres-ljets-cuts-mc15-25ns-syst.txt'

config.gridUsername  = 'dferreir'
config.suffix        = '22112015TT'
config.excludedSites = ''
config.noSubmit      = False
config.mergeType     = 'Default' #'None', 'Default' or 'xAOD'


if getpass.getuser()=="scalvet": 
	config.gridUsername  = 'scalvet'
	config.suffix        = 'v1.2std'
	config.suffix        = 'v1.2qcd'
	config.mergeType     = 'None' #'None', 'Default' or 'xAOD'

names   = []
# ttbar modelling syst.
names += ['MC15_13TeV_25ns_FS_EXOT4_ttbarPowhegHerwigAF2']
names += ['MC15_13TeV_25ns_FS_EXOT4_ttbarMCAtNLOHerwigAF2']
names += ['MC15_13TeV_25ns_FS_EXOT4_ttbarPowhegPythiaAF2']
names += ['MC15_13TeV_25ns_FS_EXOT4_ttbarRadLo']
names += ['MC15_13TeV_25ns_FS_EXOT4_ttbarRadHi']
names += ['MC15_13TeV_25ns_FS_EXOT4_ttbarSherpaAF2']
samples = TopExamples.grid.Samples(names)
TopExamples.grid.submit(config, samples)

