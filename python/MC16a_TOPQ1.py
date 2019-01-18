import TopExamples.grid

#ttbar backgrounds: nonallhad inclusive
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_TOPQ1_ttbar_nonallhad").datasets = [
'mc16_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_TOPQ1.e6337_s3126_r9364_p3731',
]

#ttbar backgrounds: nonallhad mtt sliced samples
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_TOPQ1_ttbar_nonallhad_mttsliced").datasets = [
#Please be informed that we don't have nonallhad mttsliced ttbar TOPQ1 including AntiKtVR30Rmax4Rmin02TrackJets for MC16a
#Use the corresponding MC16a_EXOT7: <"MC16a_13TeV_25ns_FS_EXOT7_ttbar_nonallhad_mttsliced"> instead
]