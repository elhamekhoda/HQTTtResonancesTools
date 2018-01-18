#!/usr/bin/env python
import getpass
import TopExamples.grid
import HQTTtResonancesTools.DC15MC13TeV_25ns_mc15c_EXOT4
import HQTTtResonancesTools.DC15Data13TeV_25ns_207_EXOT4

config = TopExamples.grid.Config()
config.code          = 'top-xaod'
config.settingsFile  = 'ttres-ljets-cuts-mc15c-25ns.txt'
#config.settingsFile  = 'ttres-ljets-cuts-mc15c-25ns-pdf.txt'
#config.settingsFile  = 'ttres-ljets-cuts-mc15c-25ns-syst.txt'
#config.settingsFile  = 'ttres-ljets-cuts-mc15c-fakes-WjetsCR+SR-25ns-e.txt'
#config.settingsFile  = 'ttres-ljets-cuts-mc15c-fakes-WjetsCR+SR-25ns-mu.txt'

config.gridUsername  = 'dferreir'
config.suffix        = '13112016v2'
#config.suffix        = '13112016PDFv1'
#config.suffix        = '13112016Systv1'
#config.suffix        = '13112016QCDev1'
#config.suffix        = '13112016QCDmuv1'
config.excludedSites = 'ANALY_SCINET'
config.noSubmit      = False
config.mergeType     = 'None' #'None', 'Default' or 'xAOD'
config.memory        = '1000'
config.destSE        = 'DESY-HH_LOCALGROUPDISK'
#config.maxNFilesPerJob  = '2'
config.maxNFilesPerJob  = '1'

names   = []
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_ttbarPowhegPythia']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_ttbarPowhegPythia_mttsliced']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_singletop']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Wjets222']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zjets222', 'MC15c_13TeV_25ns_FS_EXOT4_VV']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime400']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime500']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime750']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime1000']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime1250']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime1500']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime1750']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime2000']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime2250']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime2500']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime2750']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime3000']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime4000']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Zprime5000']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Gtt400']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Gtt500']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Gtt750']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Gtt1000']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Gtt2000']
#names  += ['MC15c_13TeV_25ns_FS_EXOT4_Gtt3000']

#names += ['MC15c_13TeV_25ns_FS_EXOT4_ttbarLO']
#names += ['MC15c_13TeV_25ns_FS_EXOT4_ttbarV']

#names += ['mccrashed']
#names += ['datacrashed']

#names += ['Data15_13TeV_25ns_207_EXOT4']
#names += ['Data16_13TeV_25ns_33257ipb_EXOT4']
# systematics
#names += ['MC15c_13TeV_25ns_FS_EXOT4_ttbaraMcAtNlo_PDF']
#names += ['MC15c_13TeV_25ns_FS_EXOT4_ttbarPowhegHerwig']
#names += ['MC15c_13TeV_25ns_FS_EXOT4_ttbarMCAtNLOHerwig']
#names += ['MC15c_13TeV_25ns_FS_EXOT4_ttbarRadLo']
#names += ['MC15c_13TeV_25ns_FS_EXOT4_ttbarRadHi']

samples = TopExamples.grid.Samples(names)
TopExamples.grid.submit(config, samples)

