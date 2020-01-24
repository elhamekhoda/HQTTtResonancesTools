#!/usr/bin/env python
import ROOT
import glob

#torun = ["bejets/cutflow", "bmujets/cutflow"]
#torun = ["bejets/cutflow_mc_pu", "bmujets/cutflow_mc_pu"]
#torun = ["bejets/cutflow_scale_factors", "bmujets/cutflow_scale_factors"]
#torun = ["bejets/cutflow_btag_scale_factors", "bmujets/cutflow_btag_scale_factors"]

torun = ["rejets_2017/cutflow", ]


yields={}
Nevents = {}

outdir = '/eos/user/b/bngair/Badr/wl/'
#outdir = '/AtlasDisk/group/Zprime/grid_v2.0/std/'
lumi = 44.3e3

def CutDef(cut):
    if cut=="C1": return "Initial count"
    if cut=="C2": return "GRL"
    if cut=="C3": return "Calorimeter quality"
    if cut=="C4": return "Primary vertex quality"
    if cut=="C5": return "RUN_NUMBER $\geq$ 325713 "
    if cut=="C6": return " 340453 $\geq$ RUN_NUMBER  "
    if cut=="C7": return " Trigger  "
    if cut=="C8": return "$\geq$1 lepton"
    if cut=="C9": return "1 lepton"
    if cut=="C10": return "veto other flavor"
    if cut=="C11": return "trigger matching"
    if cut=="C12": return "bad jet veto"
    if cut=="C13": return "JET_N 25000 $\geq$ 1"
    if cut=="C14": return "JET_N 25000 $\geq$ 2"
    if cut=="C15": return "JET_N 25000 $\geq$ 3"
    if cut=="C16": return "JET_N 25000 $\geq$ 4"
    if cut=="C17": return "veto bad muon"
    return ""

def cutOff(value):
	v=`value`.split('.')
	return v[0]+"."+v[1][:3]

def printAll(samples, addSummary = True):
    nSamples = len(samples)
    for item in torun:
        print "\\begin{table}[!ht]"
        sStyle = "|c"
        N = nSamples
        if addSummary:
            N = N + 2
        for i in range(0, N):
            sStyle = sStyle + "|c"
        sStyle = sStyle + "|"
        print "\\begin{tabular}{",sStyle,"}"
        print "\\hline"
        sNames = "Cut ID "
        for i in range(0, nSamples):
            sNames = sNames + " & " + samples[i] + " "
        if addSummary:
            sNames = sNames + " & " + "$\\sum$ bkg. "
            sNames = sNames + " & " + "$\\frac{\\textrm{data}}{\\sum \\textrm{bkg.}}$ "
        sNames = sNames + " \\\\"
        print sNames
        print "\\hline"
        for abin in range(0, len(yields[samples[0]+item])):
            sRow = "{:24}".format(CutDef("C"+str(abin+1)))
            sumBkg = 0
            theData = 0
            for s in samples:
                sRow = sRow + " & "
                sRow = sRow +  "{:8}".format(int(yields[s+item][abin]))
                if not "data" in s.lower():
                    sumBkg = sumBkg + yields[s+item][abin]
                else:
                    theData = theData + yields[s+item][abin]
            
            if addSummary:
                sRow = sRow + " & "
                sRow = sRow +  "{:8}".format(int(sumBkg))
                sRow = sRow + " & "
                if sumBkg != 0:
                    sRow = sRow +  "{:4}".format(cutOff(theData/sumBkg))
                else:
                    sRow = sRow +  "{:4}".format("-")
            sRow = sRow + "\\\\"
            print sRow
        print "\\hline"
        print "\\end{tabular}"
        print "\\caption{}"
        print "\\end{table}"


def printAllEff(samples):
    nSamples = len(samples)
    for item in torun:
        print "\\begin{table}[!ht]"
        sStyle = "|c"
        for i in range(0, nSamples+2):
            sStyle = sStyle + "|c"
        sStyle = sStyle + "|"
        print "\\begin{tabular}{",sStyle,"}"
        print "\\hline"
        sNames = "Cut ID "
        for i in range(0, nSamples):
            sNames = sNames + " & " + samples[i] + " "
        sNames = sNames + " \\\\"
        print sNames
        print "\\hline"
        for abin in range(1, len(yields[samples[0]+item])):
            sRow = "{:24}".format(CutDef("C"+str(abin+1)))
            sumBkg = 0
            theData = 0
            for s in samples:
                sRow = sRow + " & "
                if yields[s+item][abin]/yields[s+item][abin-1] > 0.01:
                    sRow = sRow +  "{:5}".format(`yields[s+item][abin]/yields[s+item][abin-1]`[:4])
                else:
                    sRow = sRow +  "{:5}".format("$<0.01$")
            
            sRow = sRow + "\\\\"
            print sRow
        print "\\hline"
        print "\\end{tabular}"
        print "\\caption{}"
        print "\\end{table}"

def getYieldsForThisSample(sampleName, DIDs, Xsections):
    for item in torun:
        if yields.has_key(sampleName+item)==False:
            yields[sampleName+item]=[]

    for did in DIDs:
        dirsToRun = []
        dirs = glob.glob(outdir+"/*"+`did`+"*")
        for d in dirs:
            dirsToRun.append(d)

        for d in dirsToRun:
            files = glob.glob(d+"/*")
            for f in files:
                root_file = ROOT.TFile(f)
                sw = root_file.Get("sumWeights")
                for entry in range(0,sw.GetEntries()):
                    sw.GetEntry(entry)
                    if not sw.dsid in Nevents:
                        Nevents[sw.dsid] = 0
                    if "mc_pu" in torun[0]:
                        Nevents[sw.dsid] = Nevents[sw.dsid] + sw.totalEventsWeighted
                    else:
                        Nevents[sw.dsid] = Nevents[sw.dsid] + sw.totalEvents

        for d in dirsToRun:
            files = glob.glob(d+"/*")
            for f in files:
                root_file = ROOT.TFile(f)
                for item in torun:
                    root_hist = root_file.Get(item)
                    if len(yields[sampleName+item]) == 0:
                        yields[sampleName+item] = [0]*(root_hist.GetNbinsX()-1)
                    for abin in range(1, root_hist.GetNbinsX()):
                        if Xsections == None:
                            yields[sampleName+item][abin-1] = yields[sampleName+item][abin-1] + root_hist.GetBinContent(abin)
                        else:
                            yields[sampleName+item][abin-1] = yields[sampleName+item][abin-1] + root_hist.GetBinContent(abin)*lumi*Xsections[did]/Nevents[did]

    for item in torun:
        print "\\begin{table}[!ht]"
        print "\\begin{tabular}{|c|c|}"
        print "\\hline"
        print "\\multicolumn{2}{c}{", sampleName, ", ", item.split('/')[0], "} \\\\"
        print "\\hline"
        print " Cut ID & Yield \\\\"
        print "\\hline"
        for abin in range(0, len(yields[sampleName+item])):
            print "{:5} & {:20} \\\\".format("C"+str(abin+1), yields[sampleName+item][abin])
        print "\\hline"
        print "\\end{tabular}"
        print "\\end{table}"

def getDataYields(sampleName):	
    for item in torun:
        if yields.has_key(sampleName+item)==False:
            yields[sampleName+item]=[]

    dirsToRun = []
    dirs = glob.glob(outdir+"/*physics_Main*")
    for d in dirs:
        if not 'physics_Main' in d:
            continue
        dirsToRun.append(d)

    for d in dirsToRun:
        files = glob.glob(d+"/*")
        for f in files:
            root_file = ROOT.TFile(f)
            for item in torun:
                root_hist = root_file.Get(item)
                if len(yields[sampleName+item]) == 0:
                    yields[sampleName+item] = [0]*(root_hist.GetNbinsX()-1)
                for abin in range(1, root_hist.GetNbinsX()):
                    yields[sampleName+item][abin-1] = yields[sampleName+item][abin-1] + root_hist.GetBinContent(abin)

    for item in torun:
        print "\\begin{table}[!ht]"
        print "\\begin{tabular}{|c|c|}"
        print "\\hline"
        print "\\multicolumn{2}{c}{", sampleName, ", ", item.split('/')[0], "} \\"
        print "\\hline"
        print " Cut ID & Yield \\\\"
        print "\\hline"
        for abin in range(0, len(yields[sampleName+item])):
            print "{:5}  {:20}".format("C"+str(abin+1), yields[sampleName+item][abin])
        print "\\hline"
        print "\\end{tabular}"
        print "\\end{table}"

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

#getDataYields("data")

all=[]
torun = ["rejets_2017/cutflow", ]
getYieldsForThisSample("W+jets", range(364156,364197+1), Xsections)
getYieldsForThisSample("$t\\bar{t}$", [410470,], Xsections)
getYieldsForThisSample("single-t", range(410644,410659), Xsections)
getYieldsForThisSample("Z+jets", range(364100,364141+1), Xsections)
getYieldsForThisSample("VV", range(410155,410157+1), Xsections)

torun = ["rmujets_2017/cutflow", ]
getYieldsForThisSample("W+jets", range(364156,364197+1), Xsections)
getYieldsForThisSample("$t\\bar{t}$", [410470,], Xsections)
getYieldsForThisSample("single-t", range(410644,410659), Xsections)
getYieldsForThisSample("Z+jets", range(364100,364141+1), Xsections)
getYieldsForThisSample("VV", range(410155,410157+1), Xsections)

torun = ["rejets_2017/cutflow", ]
getYieldsForThisSample("W+jets", range(364156,364197+1), Xsections)
getYieldsForThisSample("$t\\bar{t}$", [410470,], Xsections)
getYieldsForThisSample("single-t", range(410644,410659), Xsections)
getYieldsForThisSample("Z+jets", range(364100,364141+1), Xsections)
getYieldsForThisSample("VV", range(410155,410157+1), Xsections)


torun = ["rmujets_2017/cutflow", ]
getYieldsForThisSample("W+jets", range(364156,364197+1), Xsections)
getYieldsForThisSample("$t\\bar{t}$", [410470,], Xsections)
getYieldsForThisSample("single-t", range(410644,410659), Xsections)
getYieldsForThisSample("Z+jets", range(364100,364141+1), Xsections)
getYieldsForThisSample("VV", range(410155,410157+1), Xsections)


torun = ["bejets_2017/cutflow", ]
all=["W+jets","$t\\bar{t}$","single-t","Z+jets","VV",  ]
printAll(all , False)
printAllEff(all)

torun = ["bmujets_2017/cutflow", ]
all=["W+jets","$t\\bar{t}$","single-t","Z+jets","VV",   ]
printAll(all, False)
printAllEff(all)

torun = ["rejets_2017/cutflow", ]
all=["W+jets","$t\\bar{t}$","single-t","Z+jets","VV",   ]
printAll(all , False)
printAllEff(all)

torun = ["rmujets_2017/cutflow", ]
all=["W+jets","$t\\bar{t}$","single-t","Z+jets","VV",   ]
printAll(all, False)
printAllEff(all)


