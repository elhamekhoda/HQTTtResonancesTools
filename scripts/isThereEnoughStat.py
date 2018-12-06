#!/usr/bin/env python
from ROOT import *
import os

rtag = {}
rtag['a'] = 'r9364'
rtag['c'] = 'r9781'
#rtag['d'] = 'r10201'
rtag['e'] = 'r10724'

inDir='/AtlasDisk/user/scalvet/Zprime/ExotComb/Tuples/AnalysisTop-21.2.53_chi2cut/'


def ComputeYields(fout, ch, var, category, massName, mass):
	selection = ''
	if category.find("re")!=-1:    selection = '(rejets_2015  ||rejets_2016  ||rejets_2017 )'
	elif category.find("rmu")!=-1: selection = '(rmujets_2015 ||rmujets_2016 ||rmujets_2017)'
	elif category.find("be")!=-1:  selection = '(bejets_2015  ||bejets_2016  ||bejets_2017 )'
	elif category.find("bmu")!=-1: selection = '(bmujets_2015 ||bmujets_2016 ||bmujets_2017)'
	
	if category.find("1")!=-1: selection+='&& Btagcat==1'
	if category.find("2")!=-1: selection+='&& Btagcat==2'
	if category.find("3")!=-1: selection+='&& Btagcat==3'
	ch.Draw(var+'>>h_'+category+'_'+massName, selection)
	h = gDirectory.Get('h_'+category+'_'+massName)
	
	xmin = mass*0.7
	xmax = mass*1.3
	
	l1 = TLine(xmin, 0, xmin, h.GetMaximum()); l1.Draw()
	l2 = TLine(xmax, 0, xmax, h.GetMaximum()); l2.Draw()
	
	print massName,category,h.Integral(h.FindBin(xmin), h.FindBin(xmax))
	
	fout.cd()
	gPad.Write(category+'_'+massName)
	
def Doit(fout, ID, massName, mass, period):
	ch = TChain('nominal')
	files = inDir+'*'+ID+'*'+rtag[period]+'*/*root*'
	print files
	for f in os.popen('ls '+files).readlines():
		ch.Add(f[:-1])
	
	ComputeYields(fout, ch, 'MA_ttbar_m/1000', "re1", massName, mass)
	ComputeYields(fout, ch, 'MA_ttbar_m/1000', "re2", massName, mass)
	ComputeYields(fout, ch, 'MA_ttbar_m/1000', "re3", massName, mass)
	ComputeYields(fout, ch, 'MA_ttbar_m/1000', "rmu1", massName, mass)
	ComputeYields(fout, ch, 'MA_ttbar_m/1000', "rmu2", massName, mass)
	ComputeYields(fout, ch, 'MA_ttbar_m/1000', "rmu3", massName, mass)
	ComputeYields(fout, ch, 'MA_ttbarFJ_m/1000', "be1", massName, mass)
	ComputeYields(fout, ch, 'MA_ttbarFJ_m/1000', "be2", massName, mass)
	ComputeYields(fout, ch, 'MA_ttbarFJ_m/1000', "be3", massName, mass)
	ComputeYields(fout, ch, 'MA_ttbarFJ_m/1000', "bmu1", massName, mass)
	ComputeYields(fout, ch, 'MA_ttbarFJ_m/1000', "bmu2", massName, mass)
	ComputeYields(fout, ch, 'MA_ttbarFJ_m/1000', "bmu3", massName, mass)
	
	
	
	


zprime=[]
zprime+=[('301322.', 'zprime400',400)]
zprime+=[('301323.', 'zprime500',500)]
zprime+=[('301324.', 'zprime750',750)]
zprime+=[('301325.', 'zprime1000',1000)]
zprime+=[('301326.', 'zprime1250',1250)]
zprime+=[('301327.', 'zprime1500',1500)]
zprime+=[('301328.', 'zprime1750',1750)]
zprime+=[('301329.', 'zprime2000',2000)]
zprime+=[('301330.', 'zprime2250',2250)]
zprime+=[('301331.', 'zprime2500',2500)]
zprime+=[('301332.', 'zprime2750',2750)]
zprime+=[('301333.', 'zprime3000',3000)]
zprime+=[('301334.', 'zprime4000',4000)]
zprime+=[('301335.', 'zprime5000',5000)]

fout = TFile("f_out.root", "RECREATE")

for (ID,massName, mass) in zprime:
	Doit(fout, ID, massName, mass, 'a')


fout.Close()
