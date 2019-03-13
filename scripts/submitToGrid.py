#!/usr/bin/env python
import os
import datetime
import getpass
import TopExamples.grid

### Pre-defined Sample List ###
import HQTTtResonancesTools.Data_EXOT4_rel21
import HQTTtResonancesTools.Data_EXOT7_rel21
import HQTTtResonancesTools.MC16a_EXOT4
import HQTTtResonancesTools.MC16d_EXOT4
import HQTTtResonancesTools.MC16e_EXOT4
import HQTTtResonancesTools.MC16a_EXOT7

### Configuration ###
config = TopExamples.grid.Config()
config.CMake           = True
config.code            = 'top-xaod'
config.settingsFile    = 'build/data/ttres-ljets-cuts-mc16-25ns.txt' # use this if l+jets analysis
#config.settingsFile    = 'build/data/ttres-allhad-cuts-mc16-25ns.txt' # use this instead if allhad. analysis
config.gridUsername    = os.getenv('CERN_USER', getpass.getuser())
config.suffix          = format(datetime.date.today(),'%Y%m%dv0')
# config.forceSite       = 'DESY-HH,AUTO'
# config.excludedSites   = 'ANALY_SCINET'
# config.noSubmit        = False
config.mergeType       = 'None' #'None', 'Default' or 'xAOD' # Highly recommend not to merge for efficiency issue coming later when running off-line analysis
# config.memory          = '1600'
# config.destSE          = 'DESY-HH_SCRATCHDISK' # consider using groupdisk if you have access to any (e.g. 'DESY-HH_LOCALGROUPDISK')
# config.maxNFilesPerJob = '5' # Recommend to set it high if data (~50) and low if MC (~1) for efficiency
# config.checkPRW        = True
# config.otherOptions    = '--useNewCode' # All other options that go into `prun` commends.
# config.customTDPFile   = None # Don't forget to change this if you're running with a custom TDP File. `PathResolver` expression _IS_ supported. The default is the official one, "dev/AnalysisTop/TopDataPreparation/XSection-MC15-13TeV.data".

if config.gridUsername=='scalvet':
    config.suffix        = 'HVTcomb.Dec2018.AT57.v1.0'
    config.destSE        = ''

### Input Samples ###

names   = []
if False: # an example
    subcampaign = 'MC16a'
    deriv = 'EXOT4' # use this if l+jets analysis
    # deriv = 'EXOT7' # use this instead if allhad. analysis
    names  += ['{}_13TeV_25ns_FS_{}_Zprime400'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime500'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime750'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime1000'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime1250'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime1500'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime1750'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime2000'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime2250'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime2500'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime2750'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime3000'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime4000'.format(subcampaign, deriv)]
    names  += ['{}_13TeV_25ns_FS_{}_Zprime5000'.format(subcampaign, deriv)]
## If the wanted input samples is not in any of the lists, do it by yourself. An example:
# TopExamples.grid.Add('MC16a_13TeV_25ns_EXOT7_HVT4000').datasets = [
# 'mc16_13TeV.310488.MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_Vztt_allj_m4000.deriv.DAOD_EXOT7.e6916_s3126_r9364_p3731',
# ]
# names  += ['MC16a_13TeV_25ns_EXOT7_HVT4000']

#names  += ['MC16a_13TeV_25ns_FS_{}_ttbar_nonallhad'.format(deriv)]
#names  += ['MC16a_13TeV_25ns_FS_{}_singletop'.format(deriv)]
#names  += ['MC16a_13TeV_25ns_FS_{}_ttbarV'.format(deriv)]
#names  += ['MC16a_13TeV_25ns_FS_{}_VV'.format(deriv)]
#names  += ['MC16a_13TeV_25ns_FS_{}_dijets'.format(deriv)]
#names  += ['MC16a_13TeV_25ns_FS_{}_Wjets221'.format(deriv)]
#names  += ['MC16a_13TeV_25ns_FS_{}_Zjets221'.format(deriv)]
names  += ['MC16a_{}_combination'.format(deriv)]
#names  += ['MC16d_{}_combination'.format(deriv)]
#names  += ['MC16e_{}_combination'.format(deriv)]

samples = TopExamples.grid.Samples(names)

### Submit Jobs ###
TopExamples.grid.submit(config, samples)