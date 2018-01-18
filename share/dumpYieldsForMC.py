#!/usr/bin/env python
#import ROOT
from ROOT import *
import glob
from array import array

torun = ["bejets", "bmujets"]
#outdir = '/AtlasDisk/group/Zprime/grid_v2.0/std/'
outdir = '/afs/cern.ch/user/d/dferreir/work/eos/atlas/user/d/dferreir/topana/01122015v1'

yields={}
#lumi=3316.68 # latest GRL
lumi=3.20905e3

def getYieldsForThisSample(sampleName, DIDs, Xsections):	
	for item in torun:
		if yields.has_key(sampleName+item)==False:
			yields[sampleName+item]=0
		
	for DID in DIDs:
		dirsToRun = []
		dirs = glob.glob(outdir+"/*"+`DID`+"*")
		for d in dirs:
			if 'physics_Main' in d:
	    		    continue # ignore data
			#it is MC
			dirsToRun.append(d)
		#print dirsToRun
		cutttbar = "1"
		if DID==410000:\
			cutttbar="(MC_ttbar_beforeFSR_m<1100000)"
		
		for d in dirsToRun:
			files = glob.glob(d+"/*")
			ch = TChain("nominal")
			Nevents=0
			for f in files:
				ch.Add(f)
				root_file = TFile(f)
				temp = array('f', [0])
				tree = gDirectory.Get("sumWeights")
				tree.GetEntry(0)
				Nevents += tree.totalEventsWeighted

			for item in torun:
				h=TH1D("h"+sampleName+item, "", 1, -1, 2)
				ch.Draw("1>>h"+sampleName+item, item+"*(weight_mc*weight_pileup*weight_trackjet_bTagSF_70*weight_leptonSF)*"+cutttbar+"*"+`lumi*Xsections[DID]/Nevents`)
				#ch.Draw("1>>h"+sampleName+item, item+"*(weight_mc*weight_pileup)*"+cutttbar+"*"+`lumi*Xsections[DID]/Nevents`)
				yields[sampleName+item]+=h.Integral()
				#print DID, Xsections[DID],"\t",Nevents,"\t --> w =",lumi*Xsections[DID]/Nevents,"\t\t",h.Integral()
	for item in torun:
		print sampleName,item,yields[sampleName+item]

def getDataYields(sampleName):	
	for item in torun:
		if yields.has_key(sampleName+item)==False:
			yields[sampleName+item]=0
		
	dirsToRun = []
	dirs = glob.glob(outdir+"/*physics_Main*")
	for d in dirs:
		if not 'physics_Main' in d:
    		    continue
		dirsToRun.append(d)
	#print dirsToRun
	cutttbar = "1"
	
	for d in dirsToRun:
		files = glob.glob(d+"/*")
		ch = TChain("nominal")
		for f in files:
			ch.Add(f)

		for item in torun:
			h=TH1D("h"+sampleName+item, "", 1, -1, 2)
			ch.Draw("1>>h"+sampleName+item, item)
			yields[sampleName+item]+=h.Integral()
	for item in torun:
		print sampleName,item,yields[sampleName+item]


def ReadTDP(TDPfile):
	f = open(TDPfile, 'r')
	result={}
	for line in f.readlines():
		l=line.split()
		if line[0]=="#":
			continue
		if len(l)<3:
			continue
		#print line[:-1]
		result[int(l[0])] = float(l[1])*float(l[2])
	f.close()
	return result

Xsections = ReadTDP("../../TopDataPreparation/data/XSection-MC15-13TeV-fromSusyGrp.data")

getDataYields("data")

getYieldsForThisSample("ttbar", [410000,], Xsections)
getYieldsForThisSample("ttbarHM", range(301528,301532+1), Xsections)
getYieldsForThisSample("W", range(361300,361320+1)+range(361325,361344+1)+range(361348,361368+1), Xsections)
getYieldsForThisSample("single-top", range(410011,410016+1), Xsections)
getYieldsForThisSample("Z", range(361372,361392+1)+range(361396,361416+1)+range(361420,361440+1), Xsections)
getYieldsForThisSample("dibosons", range(361081,361087+1), Xsections)


