#!/usr/bin/env python
import ROOT
import glob

torun = ["bejets/cutflow", "bmujets/cutflow"]
outdir = '/afs/cern.ch/user/d/dferreir/work/eos/atlas/user/d/dferreir/topana/11112015v2/'

def getRun(dirname):
    onlydir = dirname.split('/')[-1]
    run_number = onlydir.split('.')[2]
    return run_number

dirs = glob.glob(outdir+"/*")
dirsToRun = []
for d in dirs:
    if not 'physics_Main' in d:
        continue # ignore MC
    #it is data
    dirsToRun.append(d)

cf = {}
for d in dirsToRun:
    run = getRun(d)
    cf[run] = {}
    for item in torun:
        cf[run][item] = []

for d in dirsToRun:
    files = glob.glob(d+"/*")
    run = getRun(d)
    for f in files:
        root_file = ROOT.TFile(f)
        for item in torun:
            root_hist = root_file.Get(item)
            if len(cf[run][item]) == 0:
                cf[run][item] = [0]*(root_hist.GetNbinsX()-1)
            for abin in range(1, root_hist.GetNbinsX()):
                cf[run][item][abin-1] = cf[run][item][abin-1] + root_hist.GetBinContent(abin)


for run in sorted(cf.keys()):
    for item in torun:
        print "Printing full cutflow for run ", run, ", selection ", item
        for abin in range(0, len(cf[run][item])):
            print "{:5}  {:20}".format("C"+str(abin+1), cf[run][item][abin])

