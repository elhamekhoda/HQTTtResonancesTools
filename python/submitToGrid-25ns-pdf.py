#!/usr/bin/env python
import getpass
import TopExamples.grid
import HQTTtResonancesTools.DC15MC13TeV_25ns_EXOT4
import HQTTtResonancesTools.DC15Data13TeV_25ns_EXOT4

config = TopExamples.grid.Config()
config.code          = 'top-xaod'
config.settingsFile  = 'ttres-ljets-cuts-mc15-25ns-pdf.txt'

config.gridUsername  = 'dferreir'
config.suffix        = '22112015PDF'
config.excludedSites = ''
config.noSubmit      = False
config.mergeType     = 'None' #'None', 'Default' or 'xAOD'


if getpass.getuser()=="scalvet": 
	config.gridUsername  = 'scalvet'
	config.suffix        = 'v1.2std'
	config.suffix        = 'v1.2qcd'
	config.mergeType     = 'None' #'None', 'Default' or 'xAOD'

names   = []
names += ['MC15_13TeV_25ns_FS_EXOT4_ttbaraMcAtNlo_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime400_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime500_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime750_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime1000_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime1250_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime1500_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime1750_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime2000_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime2250_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime2500_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime2750_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime3000_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime4000_PDF']
#names += ['MC15_13TeV_25ns_FS_EXOT4_Zprime5000_PDF']
samples = TopExamples.grid.Samples(names)
TopExamples.grid.submit(config, samples)

