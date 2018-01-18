#!/usr/bin/env python
import ROOT
import glob

torun = ["bejets/cutflow", "bmujets/cutflow"]
outdir = '/afs/cern.ch/user/d/dferreir/work/eos/atlas/user/d/dferreir/topana/01122015v1/'

def getRun(dirname):
    onlydir = dirname.split('/')[-1]
    run_number = onlydir.split('.')[2]
    return run_number

def getPeriod(dirname):
    run_number = getRun(dirname)
    run = int(run_number)
    period = '0'
    if run < 276073:
        return 'beforeD' # before D ... don't care
    elif run <= 276954:
        return 'D'
    elif run <= 279928:
        return 'E'
    elif run <= 280422:
        return 'F'
    elif run <= 281075:
        return 'G'
    elif run <= 281411:
        return 'H'
    elif run <= 282482:
        return 'I'
    elif run <= 284484:
        return 'J'
    return 'afterJ'

dirs = glob.glob(outdir+"/*")
dirsToRun = []
for d in dirs:
    if not 'physics_Main' in d:
        continue # ignore MC
    #it is data
    dirsToRun.append(d)

cf = {}
for d in dirsToRun:
    period = getPeriod(d)
    cf[period] = {}
    for item in torun:
        cf[period][item] = []

for d in dirsToRun:
    files = glob.glob(d+"/*")
    period = getPeriod(d)
    for f in files:
        root_file = ROOT.TFile(f)
        for item in torun:
            root_hist = root_file.Get(item)
            if len(cf[period][item]) == 0:
                cf[period][item] = [0]*(root_hist.GetNbinsX()-1)
            for abin in range(1, root_hist.GetNbinsX()):
                cf[period][item][abin-1] = cf[period][item][abin-1] + root_hist.GetBinContent(abin)
for period in sorted(cf.keys()):
    for item in torun:
        print "Printing full cutflow for period ", period, ", selection ", item
        s = "L "
        for abin in range(0, len(cf[period][item])):
            s = s + str(cf[period][item][abin]) + ","
        print s

        
        for abin in range(0, len(cf[period][item])):
            print "{:5}  {:20}".format("C"+str(abin+1), cf[period][item][abin])

