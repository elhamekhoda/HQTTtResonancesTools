'''

If you're tired of the memory leakage problem of AnalysisTop and often have trouble due to this when submitting jobs to grid.
This is for you! I can't guarantee that it works well for everyone so if you have problem or question just come to us.

'''
#!/usr/bin/env python
import getpass
import os
import TopExamples.grid
import HQTTtResonancesTools.cluster

# import HQTTtResonancesTools.Data_EXOT4_rel21
# import HQTTtResonancesTools.Data_EXOT7_rel21
import HQTTtResonancesTools.MC16a_EXOT7
# import HQTTtResonancesTools.MC16d_EXOT7

# Configuration
user = os.getenv('CERN_USER')
config = TopExamples.grid.Config()
config.CMake           = True
config.code            = 'top-xaod'
config.settingsFile  = 'ttres-allhad-cuts-mc16-25ns.txt'
config.gridUsername  = user
config.excludedSites = ''
config.noSubmit        = True
config.mergeType     = 'None' #'None', 'Default' or 'xAOD'
config.memory          = '2000'
config.forceSite = 'DESY-HH,AUTO'
config.maxNFilesPerJob  = '5'
config.reuseTarBall    =  True
#config.checkPRW = True
config.suffix = 'SL.20181103.v2'
config.destSE          = 'DESY-HH_LOCALGROUPDISK'

if user=='meehan':
    config.suffix          = '20181029_Data1718v0'
    config.destSE          = 'DESY-HH_LOCALGROUPDISK'
if user=='scalvet':
    config.suffix          = 'HVTcomb.Oct2018.v4.1'
    config.destSE          = ''

# Input Samples
names   = []
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime400']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime500']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime750']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime1000']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime1250']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime1500']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime1750']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime2000']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime2250']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime2500']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime2750']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime3000']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime4000']
names  += ['MC16_13TeV_25ns_FS_EXOT4_Zprime5000']

samples = TopExamples.grid.Samples(names)

# TopExamples.grid.submit(config, samples)
HQTTtResonancesTools.cluster.submit_to_cluster(config, samples, './output/')
