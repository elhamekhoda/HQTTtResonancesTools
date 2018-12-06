#!/usr/bin/env python
import os

'''
setupAtlas
lsetup pyami
./getStatusOfSignalSamples.py
'''


rtag = {}
rtag['a'] = 'r9364'
#rtag['c'] = 'r9781'
rtag['d'] = 'r10201'
rtag['e'] = 'r10724'

ptag = 'p3565'

zprime=[]
zprime+=['mc16_13TeV.301322.Pythia8EvtGen_A14NNPDF23LO_zprime400_tt.deriv.DAOD_EXOT4.e4061_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301323.Pythia8EvtGen_A14NNPDF23LO_zprime500_tt.deriv.DAOD_EXOT4.e4061_s3126_rtag_ptag']
zprime+=['mc16_13TeV.305786.Pythia8EvtGen_A14NNPDF23LO_zprime600_tt.deriv.DAOD_EXOT4.e5261_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301324.Pythia8EvtGen_A14NNPDF23LO_zprime750_tt.deriv.DAOD_EXOT4.e4061_s3126_rtag_ptag']
zprime+=['mc16_13TeV.305787.Pythia8EvtGen_A14NNPDF23LO_zprime850_tt.deriv.DAOD_EXOT4.e5261_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301325.Pythia8EvtGen_A14NNPDF23LO_zprime1000_tt.deriv.DAOD_EXOT4.e4061_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301326.Pythia8EvtGen_A14NNPDF23LO_zprime1250_tt.deriv.DAOD_EXOT4.e4061_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301327.Pythia8EvtGen_A14NNPDF23LO_zprime1500_tt.deriv.DAOD_EXOT4.e4061_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301328.Pythia8EvtGen_A14NNPDF23LO_zprime1750_tt.deriv.DAOD_EXOT4.e4061_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301329.Pythia8EvtGen_A14NNPDF23LO_zprime2000_tt.deriv.DAOD_EXOT4.e4061_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301330.Pythia8EvtGen_A14NNPDF23LO_zprime2250_tt.deriv.DAOD_EXOT4.e4061_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301331.Pythia8EvtGen_A14NNPDF23LO_zprime2500_tt.deriv.DAOD_EXOT4.e4061_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301332.Pythia8EvtGen_A14NNPDF23LO_zprime2750_tt.deriv.DAOD_EXOT4.e4061_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301333.Pythia8EvtGen_A14NNPDF23LO_zprime3000_tt.deriv.DAOD_EXOT4.e3723_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301334.Pythia8EvtGen_A14NNPDF23LO_zprime4000_tt.deriv.DAOD_EXOT4.e3723_s3126_rtag_ptag']
zprime+=['mc16_13TeV.301335.Pythia8EvtGen_A14NNPDF23LO_zprime5000_tt.deriv.DAOD_EXOT4.e3723_s3126_rtag_ptag']

KKgluon=[]

samples = []
samples += zprime


for sample in samples:
	for period in ('a', 'd', 'e'):
		SAMPLE_EXOT4 = sample.replace('rtag', rtag[period]).replace('ptag', ptag)#.replace('.deriv.DAOD_EXOT4.', '.recon.AOD.')
		SAMPLE_AOD = sample.replace('rtag', rtag[period]).replace('_ptag', '').replace('.deriv.DAOD_EXOT4.', '.recon.AOD.')
		com_EXOT4 = 'ami show dataset info '+SAMPLE_EXOT4
		com_AOD   = 'ami show dataset info '+SAMPLE_AOD
		
		#is there DAOD ?
		lines= os.popen(com_EXOT4).readlines()
		missingEXOT4=0
		SAMPLE = SAMPLE_EXOT4
		if len(lines)==1: # No -> then try AOD
			lines= os.popen(com_AOD).readlines()
			SAMPLE = SAMPLE_AOD
			missingEXOT4=1
			
		totalEvents	= -1
		completion	= -1
		subcampaign	= ''
		
		for line in lines:
			l=line.split()
			if l[0]=='totalEvents':	totalEvents=l[-1]
			if l[0]=='completion':	completion=l[-1]
			if l[0]=='subcampaign':	
				subcampaign=l[-1]
				if subcampaign!='MC16'+period:
					print "error! ",subcampaign,"!=",'MC16'+rtag[period]
		
		print SAMPLE,
		if missingEXOT4: print '\033[91m No EXOT4 \tMC16'+period+'\033[0m',
		else: 		 print '\tMC16'+period,
		print totalEvents,completion,"%"
		
