import TopExamples.grid

#ttbar backgrounds: inclusive
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_ttbar_allhad").datasets = [
'mc16_13TeV.410471.PhPy8EG_A14_ttbar_hdamp258p75_allhad.deriv.DAOD_EXOT7.e6337_s3126_r9364_p3729',
]

#ttbar backgrounds: mtt sliced
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_ttbar_allhad_mttsliced").datasets = [
'mc16_13TeV.410284.PhPy8EG_A14_ttbar_hdamp258p75_allhad_mtt_1100_1300.deriv.DAOD_EXOT7.e6603_s3126_r9364_p3729',
'mc16_13TeV.410285.PhPy8EG_A14_ttbar_hdamp258p75_allhad_mtt_1300_1500.deriv.DAOD_EXOT7.e6686_s3126_r9364_p3729',
'mc16_13TeV.410286.PhPy8EG_A14_ttbar_hdamp258p75_allhad_mtt_1500_1700.deriv.DAOD_EXOT7.e6686_s3126_r9364_p3729',
'mc16_13TeV.410287.PhPy8EG_A14_ttbar_hdamp258p75_allhad_mtt_1700_2000.deriv.DAOD_EXOT7.e6686_s3126_r9364_p3729',
'mc16_13TeV.410288.PhPy8EG_A14_ttbar_hdamp258p75_allhad_mtt_2000_14000.deriv.DAOD_EXOT7.e6686_s3126_r9364_p3729',
]

#ttbar backgrounds: nonallhad inclusive
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_ttbar_nonallhad").datasets = [
'mc16_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_EXOT7.e6337_s3126_r9364_p3729',
]

#ttbar backgrounds: nonallhad mtt sliced samples
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_ttbar_nonallhad_mttsliced").datasets = [
'mc16_13TeV.410633.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad_1100_1300.deriv.DAOD_EXOT7.e6602_s3126_r9364_p3729',
'mc16_13TeV.410634.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad_1300_1500.deriv.DAOD_EXOT7.e6602_s3126_r9364_p3729',
'mc16_13TeV.410635.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad_1500_1700.deriv.DAOD_EXOT7.e6685_s3126_r9364_p3729',
'mc16_13TeV.410636.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad_1700_2000.deriv.DAOD_EXOT7.e6685_s3126_r9364_p3729',
'mc16_13TeV.410637.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad_2000_14000.deriv.DAOD_EXOT7.e6685_s3126_r9364_p3729',
]

#Single top samples
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_singletop").datasets = [

]

#ttbar+V samples
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_ttbarV").datasets = [

]

#Di-boson
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_VV").datasets = [

]

#Di-jets
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_dijets").datasets = [
# 'mc16_13TeV.361020.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ0W.deriv.DAOD_EXOT7.e3569_s3126_r9364_p3729',
# 'mc16_13TeV.361021.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ1W.deriv.DAOD_EXOT7.e3569_s3126_r9364_p3729',
# 'mc16_13TeV.361022.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ2W.deriv.DAOD_EXOT7.e3668_s3126_r9364_p3729',
'mc16_13TeV.361023.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ3W.deriv.DAOD_EXOT7.e3668_s3126_r9364_p3729',
'mc16_13TeV.361024.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ4W.deriv.DAOD_EXOT7.e3668_s3126_r9364_p3729',
'mc16_13TeV.361025.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ5W.deriv.DAOD_EXOT7.e3668_s3126_r9364_p3729',
'mc16_13TeV.361026.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ6W.deriv.DAOD_EXOT7.e3569_s3126_r9364_p3729',
'mc16_13TeV.361027.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ7W.deriv.DAOD_EXOT7.e3668_s3126_r9364_p3729',
'mc16_13TeV.361028.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ8W.deriv.DAOD_EXOT7.e3569_s3126_r9364_p3729',
'mc16_13TeV.361029.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ9W.deriv.DAOD_EXOT7.e3569_s3126_r9364_p3729',
'mc16_13TeV.361030.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ10W.deriv.DAOD_EXOT7.e3569_s3126_r9364_p3729',
'mc16_13TeV.361031.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ11W.deriv.DAOD_EXOT7.e3569_s3126_r9364_p3729',
'mc16_13TeV.361032.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ12W.deriv.DAOD_EXOT7.e3668_s3126_r9364_p3729',
]


#W+jets samples
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Wjets221").datasets = [

]


#Z+jets samples
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zjets221").datasets = [

]


#Z' signal samples
TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime400").datasets = [
'mc16_13TeV.301322.Pythia8EvtGen_A14NNPDF23LO_zprime400_tt.deriv.DAOD_EXOT7.e4061_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime500").datasets = [
'mc16_13TeV.301323.Pythia8EvtGen_A14NNPDF23LO_zprime500_tt.deriv.DAOD_EXOT7.e4061_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime750").datasets = [
'mc16_13TeV.301324.Pythia8EvtGen_A14NNPDF23LO_zprime750_tt.deriv.DAOD_EXOT7.e4061_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime1000").datasets = [
'mc16_13TeV.301325.Pythia8EvtGen_A14NNPDF23LO_zprime1000_tt.deriv.DAOD_EXOT7.e4061_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime1250").datasets = [
'mc16_13TeV.301326.Pythia8EvtGen_A14NNPDF23LO_zprime1250_tt.deriv.DAOD_EXOT7.e4061_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime1500").datasets = [
'mc16_13TeV.301327.Pythia8EvtGen_A14NNPDF23LO_zprime1500_tt.deriv.DAOD_EXOT7.e4061_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime1750").datasets = [
'mc16_13TeV.301328.Pythia8EvtGen_A14NNPDF23LO_zprime1750_tt.deriv.DAOD_EXOT7.e4061_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime2000").datasets = [
'mc16_13TeV.301329.Pythia8EvtGen_A14NNPDF23LO_zprime2000_tt.deriv.DAOD_EXOT7.e4061_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime2250").datasets = [
'mc16_13TeV.301330.Pythia8EvtGen_A14NNPDF23LO_zprime2250_tt.deriv.DAOD_EXOT7.e4061_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime2500").datasets = [
'mc16_13TeV.301331.Pythia8EvtGen_A14NNPDF23LO_zprime2500_tt.deriv.DAOD_EXOT7.e4061_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime2750").datasets = [
'mc16_13TeV.301332.Pythia8EvtGen_A14NNPDF23LO_zprime2750_tt.deriv.DAOD_EXOT7.e4061_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime3000").datasets = [
'mc16_13TeV.301333.Pythia8EvtGen_A14NNPDF23LO_zprime3000_tt.deriv.DAOD_EXOT7.e3723_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime4000").datasets = [
'mc16_13TeV.301334.Pythia8EvtGen_A14NNPDF23LO_zprime4000_tt.deriv.DAOD_EXOT7.e3723_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime5000").datasets = [
'mc16_13TeV.301335.Pythia8EvtGen_A14NNPDF23LO_zprime5000_tt.deriv.DAOD_EXOT7.e3723_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_FS_EXOT7_Zprime6000").datasets = [
'mc16_13TeV.311258.Pythia8EvtGen_A14NNPDF23LO_zprime6000_tt_mtt3000.deriv.DAOD_EXOT7.e7217_s3126_r9364_p3729', #skimmed for now
]

TopExamples.grid.Add("MC16a_13TeV_25ns_EXOT7_HVT1000").datasets = [
'mc16_13TeV.310487.MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_Vztt_allj_m1000.deriv.DAOD_EXOT7.e6916_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_EXOT7_HVT4000").datasets = [
'mc16_13TeV.310488.MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_Vztt_allj_m4000.deriv.DAOD_EXOT7.e6916_s3126_r9364_p3731',
]

TopExamples.grid.Add("MC16a_13TeV_25ns_EXOT7_HVT8000").datasets = [
'mc16_13TeV.310489.MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_Vztt_allj_m8000.deriv.DAOD_EXOT7.e6916_s3126_r9364_p3731',
]