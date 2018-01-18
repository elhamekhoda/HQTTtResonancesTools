#!/usr/bin/env python
import getpass
import TopExamples.grid
import HQTTtResonancesTools.DC15Data13TeV_25ns_207_EXOT4
import HQTTtResonancesTools.DC15MC13TeV_25ns_mc15c_EXOT4

config = TopExamples.grid.Config()
config.code          = 'top-xaod'

channel = 'e'
channel = 'mu'
doWCRSR=0
if doWCRSR==0:
	if channel == 'e':
		config.settingsFile  = 'ttres-ljets-cuts-mc15c-fakes-25ns-e.txt'
	elif channel == 'mu':
		config.settingsFile  = 'ttres-ljets-cuts-mc15c-fakes-25ns-mu.txt'
else: 
	if channel == 'e':
		config.settingsFile  = 'ttres-ljets-cuts-mc15c-fakes-WjetsCR+SR-25ns-e.txt'

	elif channel == 'mu':
		config.settingsFile  = 'ttres-ljets-cuts-mc15c-fakes-WjetsCR+SR-25ns-mu.txt'

config.gridUsername  = 'dferreir'
config.suffix        = '09022016v1'
config.excludedSites = ''
config.noSubmit      = False
config.mergeType     = 'None' #'None', 'Default' or 'xAOD'
config.memory        = '1000'

if getpass.getuser()=="scalvet": 
	config.gridUsername  = 'scalvet'
	if doWCRSR==0:
		if channel == 'e':  config.suffix       = 'AT2429.v1.0qcde'
		if channel == 'mu': config.suffix       = 'AT2429.v1.0qcdm'
	if doWCRSR==1:
		if channel == 'e':  config.suffix       = 'AT2429.v1.0SRe'
		if channel == 'mu': config.suffix       = 'AT2429.v1.0SRm'

names   = []
#names += ['MC15c_13TeV_25ns_FS_EXOT4_ttbarPowhegPythia']
if doWCRSR==0:
	
	names += ['MC15c_13TeV_25ns_FS_EXOT4_singletop']
	names += ['MC15c_13TeV_25ns_FS_EXOT4_Wjets222']
	names += ['MC15c_13TeV_25ns_FS_EXOT4_Zjets222']
	names += ['MC15c_13TeV_25ns_FS_EXOT4_VV']
	
	names += ['Data16_13TeV_25ns_33257ipb_EXOT4']
	names += ['Data15_13TeV_25ns_207_EXOT4']
	
samples = TopExamples.grid.Samples(names)
TopExamples.grid.submit(config, samples)

