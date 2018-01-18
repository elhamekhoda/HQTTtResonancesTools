#!/usr/bin/env python
import ROOT
import glob

torun = ["bejets", "bmujets"]
outdir = '/afs/cern.ch/user/d/dferreir/work/eos/atlas/user/d/dferreir/topana/01122015v1/'
cfbin = 15

#suf = 'tt'
#firstrun = 410000-2
#lastrun = 410000+2
###cf = 'cutflow'
###cf = 'cutflow_mc_pu'
#cf = 'cutflow_scale_factors'

suf = 'dt'
firstrun = 276262
lastrun = 284484
cf = 'cutflow'

def getRun(dirname):
    onlydir = dirname.split('/')[-1]
    run_number = onlydir.split('.')[2]
    return run_number

dirs = glob.glob(outdir+"/*")
dirsToRun = []
for d in dirs:
    if suf == 'tt':
        if not '410000' in d:
            continue
    else:
        if not 'physics_Main' in d:
            continue # ignore MC
    #it is data
    dirsToRun.append(d)

bins = lastrun-firstrun+1
bins = int(bins/1000.0)
h = {}
for ch in torun:
    h[ch] = ROOT.TH1F(ch, " ; Run number ; Yield", bins, firstrun-0.5, lastrun+0.5)
    h[ch].Sumw2()

htest = ROOT.TH1F("htest", "", 1, -1, 2)
for d in dirsToRun:
    files = glob.glob(d+"/*")
    run = float(getRun(d))

    print "In directory ", d

    for f in files:
        root_file = ROOT.TFile(f)
        for channel in torun:
            root_hist = root_file.Get(channel+"/"+cf)
            n = root_hist.GetBinContent(cfbin)
            h[channel].Fill(run, n)
            #for i in range(0, int(n)):
            #    h[channel].Fill(run)

    #c = ROOT.TChain("nominal")
    #for f in files:
    #    c.Add(f)
    #for channel in torun:
    #    c.Draw("1>>htest", channel)
    #    for i in range(0, int(htest.Integral())):
    #        h[channel].Fill(run)

hratio = h["bmujets"].Clone("bratio")
hratio.Divide(h["bmujets"], h["bejets"], 1, 1, "")

canv = ROOT.TCanvas("c", "", 800, 600)
count = 0
col = [ROOT.kBlue, ROOT.kRed, ROOT.kGreen, ROOT.kMagenta]
maximum = 0
mrat = hratio.GetBinContent(hratio.GetMaximumBin())
for ch in torun:
  if h[ch].GetBinContent(h[ch].GetMaximumBin()) > maximum:
    maximum = h[ch].GetBinContent(h[ch].GetMaximumBin())

for ch in torun:
    h[ch].SetLineWidth(2)
    h[ch].SetLineColor(col[count])
    h[ch].SetStats(0)
    h[ch].GetYaxis().SetRangeUser(0, 1.2*maximum)
    if count == 0:
        h[ch].Draw("e1")
    else:
        h[ch].Draw("e1 same")
    count = count + 1
canv.SaveAs("yieldByRun_cf"+str(cfbin)+suf+".eps")

hratio.SetLineWidth(2)
hratio.SetLineColor(col[count])
hratio.GetYaxis().SetRangeUser(0, 1.2*mrat)
hratio.GetYaxis().SetTitle("muon channel yield / electron channel yield")
hratio.SetStats(0)
hratio.Draw("e1")

canv.SaveAs("yieldRatioByRun_cf"+str(cfbin)+suf+".eps")

