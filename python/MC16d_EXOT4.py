import TopExamples.grid


#ttbar background: inclusive
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ttbar_nonallhad").datasets = [
'mc16_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_EXOT4.e6337_s3126_r10201_p4396',
]

#ttbar systematics samples: inclusive
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ttbar_nonallhad_systematics").datasets = [
'mc16_13TeV.410464.aMcAtNloPy8EvtGen_MEN30NLO_A14N23LO_ttbar_noShWe_SingleLep.deriv.DAOD_EXOT4.e6762_a875_r10201_p4396',
'mc16_13TeV.410465.aMcAtNloPy8EvtGen_MEN30NLO_A14N23LO_ttbar_noShWe_dil.deriv.DAOD_EXOT4.e6762_a875_r10201_p4396',
'mc16_13TeV.410557.PowhegHerwig7EvtGen_H7UE_tt_hdamp258p75_704_SingleLep.deriv.DAOD_EXOT4.e6366_a875_r10201_p4396',
'mc16_13TeV.410558.PowhegHerwig7EvtGen_H7UE_tt_hdamp258p75_704_dil.deriv.DAOD_EXOT4.e6366_a875_r10201_p4396',
'mc16_13TeV.411233.PowhegHerwig7EvtGen_tt_hdamp258p75_713_SingleLep.deriv.DAOD_EXOT4.e7580_a875_r10201_p4149',
'mc16_13TeV.411234.PowhegHerwig7EvtGen_tt_hdamp258p75_713_dil.deriv.DAOD_EXOT4.e7580_a875_r10201_p4149',
'mc16_13TeV.410480.PhPy8EG_A14_ttbar_hdamp517p5_SingleLep.deriv.DAOD_EXOT4.e6454_a875_r10201_p4149',
'mc16_13TeV.410482.PhPy8EG_A14_ttbar_hdamp517p5_dil.deriv.DAOD_EXOT4.e6454_a875_r10201_p4149',
'mc16_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_EXOT4.e6337_a875_r10201_p4149',
'mc16_13TeV.411288.PhPy8EG_A14_ttbar_hdamp258p75_MECoff_nonallhad.deriv.DAOD_TOPQ1.e7948_a875_r10201_p4346'
]

#ttbar systematics samples: inclusive AFII
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ttbar_nonallhad_af2").datasets = [
'mc16_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_EXOT4.e6337_a875_r10201_p4149'
]

# Had: Herwig7.0
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ttbar_nonallhad_had").datasets = [
'mc16_13TeV.410557.PowhegHerwig7EvtGen_H7UE_tt_hdamp258p75_704_SingleLep.deriv.DAOD_EXOT4.e6366_a875_r10201_p4396',
'mc16_13TeV.410558.PowhegHerwig7EvtGen_H7UE_tt_hdamp258p75_704_dil.deriv.DAOD_EXOT4.e6366_a875_r10201_p4396',
]

# Had: Herwig7.1
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ttbar_nonallhad_had_v2").datasets = [
'mc16_13TeV.411233.PowhegHerwig7EvtGen_tt_hdamp258p75_713_SingleLep.deriv.DAOD_EXOT4.e7580_a875_r10201_p4149',
'mc16_13TeV.411234.PowhegHerwig7EvtGen_tt_hdamp258p75_713_dil.deriv.DAOD_EXOT4.e7580_a875_r10201_p4149',
]

# hdamp
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ttbar_nonallhad_hdamp").datasets = [
'mc16_13TeV.410480.PhPy8EG_A14_ttbar_hdamp517p5_SingleLep.deriv.DAOD_EXOT4.e6454_a875_r10201_p4149',
'mc16_13TeV.410482.PhPy8EG_A14_ttbar_hdamp517p5_dil.deriv.DAOD_EXOT4.e6454_a875_r10201_p4149',
]

# HS
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ttbar_nonallhad_HS").datasets = [
'mc16_13TeV.410464.aMcAtNloPy8EvtGen_MEN30NLO_A14N23LO_ttbar_noShWe_SingleLep.deriv.DAOD_EXOT4.e6762_a875_r10201_p4396',
'mc16_13TeV.410465.aMcAtNloPy8EvtGen_MEN30NLO_A14N23LO_ttbar_noShWe_dil.deriv.DAOD_EXOT4.e6762_a875_r10201_p4396',
]

# MECoff
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ttbar_nonallhad_MECoff").datasets = [
'mc16_13TeV.411288.PhPy8EG_A14_ttbar_hdamp258p75_MECoff_nonallhad.deriv.DAOD_TOPQ1.e7948_a875_r10201_p4346'
]

#ttbar backgrounds: all-had
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ttbar_allhad").datasets = [
'mc16_13TeV.410471.PhPy8EG_A14_ttbar_hdamp258p75_allhad.deriv.DAOD_EXOT4.e6337_s3126_r10201_p4149',
]

#ttbar backgrounds: mtt sliced samples
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ttbar_nonallhad_mttsliced").datasets = [
'mc16_13TeV.410633.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad_1100_1300.deriv.DAOD_EXOT4.e6602_s3126_r10201_p4149',
'mc16_13TeV.410634.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad_1300_1500.deriv.DAOD_EXOT4.e6602_s3126_r10201_p4149',
'mc16_13TeV.410635.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad_1500_1700.deriv.DAOD_EXOT4.e6685_s3126_r10201_p4149',
'mc16_13TeV.410636.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad_1700_2000.deriv.DAOD_EXOT4.e6685_s3126_r10201_p4149',
'mc16_13TeV.410637.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad_2000_14000.deriv.DAOD_EXOT4.e6685_s3126_r10201_p4149'
]

#ttbar all-had backgrounds: mtt sliced samples
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ttbar_allhad_mttsliced").datasets = [
'mc16_13TeV.410284.PhPy8EG_A14_ttbar_hdamp258p75_allhad_mtt_1100_1300.deriv.DAOD_EXOT4.e6603_s3126_r10201_p4149',
'mc16_13TeV.410285.PhPy8EG_A14_ttbar_hdamp258p75_allhad_mtt_1300_1500.deriv.DAOD_EXOT4.e6686_s3126_r10201_p4149',
'mc16_13TeV.410286.PhPy8EG_A14_ttbar_hdamp258p75_allhad_mtt_1500_1700.deriv.DAOD_EXOT4.e6686_s3126_r10201_p4149',
'mc16_13TeV.410287.PhPy8EG_A14_ttbar_hdamp258p75_allhad_mtt_1700_2000.deriv.DAOD_EXOT4.e6686_s3126_r10201_p4149',
'mc16_13TeV.410288.PhPy8EG_A14_ttbar_hdamp258p75_allhad_mtt_2000_14000.deriv.DAOD_EXOT4.e6686_s3126_r10201_p4149'
]

#Single top samples
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_singletop").datasets = [
'mc16_13TeV.410644.PowhegPythia8EvtGen_A14_singletop_schan_lept_top.deriv.DAOD_EXOT4.e6527_s3126_r10201_p4149',
'mc16_13TeV.410645.PowhegPythia8EvtGen_A14_singletop_schan_lept_antitop.deriv.DAOD_EXOT4.e6527_s3126_r10201_p4149',
'mc16_13TeV.410646.PowhegPythia8EvtGen_A14_Wt_DR_inclusive_top.deriv.DAOD_EXOT4.e6552_s3126_r10201_p4149',
'mc16_13TeV.410647.PowhegPythia8EvtGen_A14_Wt_DR_inclusive_antitop.deriv.DAOD_EXOT4.e6552_s3126_r10201_p4149',
'mc16_13TeV.410658.PhPy8EG_A14_tchan_BW50_lept_top.deriv.DAOD_EXOT4.e6671_s3126_r10201_p4149',
'mc16_13TeV.410659.PhPy8EG_A14_tchan_BW50_lept_antitop.deriv.DAOD_EXOT4.e6671_s3126_r10201_p4149'
]

#Single top systematic samples
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_singletop_syst").datasets = [
'mc16_13TeV.410654.PowhegPythia8EvtGen_A14_Wt_DS_inclusive_top.deriv.DAOD_TOPQ1.e6552_s3126_r10201_p4514',
'mc16_13TeV.410655.PowhegPythia8EvtGen_A14_Wt_DS_inclusive_antitop.deriv.DAOD_TOPQ1.e6552_s3126_r10201_p4514',
'mc16_13TeV.412002.aMcAtNloPythia8EvtGen_HThalfscale_tW_inclusive.deriv.DAOD_TOPQ1.e6817_a875_r10201_p4514',
'mc16_13TeV.600727.PhH7EG_H7UE_716_Wt_DR_inclusive_top.deriv.DAOD_TOPQ1.e8272_a875_r10201_p4514',
'mc16_13TeV.600728.PhH7EG_H7UE_716_Wt_DR_inclusive_antitop.deriv.DAOD_TOPQ1.e8272_a875_r10201_p4514',
]


#ttbar+V samples
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ttbarV").datasets = [
'mc16_13TeV.410155.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttW.deriv.DAOD_EXOT4.e5070_s3126_r10201_p4149',
'mc16_13TeV.410156.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttZnunu.deriv.DAOD_EXOT4.e5070_s3126_r10201_p4149',
'mc16_13TeV.410157.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttZqq.deriv.DAOD_EXOT4.e5070_s3126_r10201_p4149',
]

#Di-boson
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_VV").datasets = [
'mc16_13TeV.363355.Sherpa_221_NNPDF30NNLO_ZqqZvv.deriv.DAOD_EXOT4.e5525_s3126_r10201_p4149',
'mc16_13TeV.363356.Sherpa_221_NNPDF30NNLO_ZqqZll.deriv.DAOD_EXOT4.e5525_s3126_r10201_p4149',
'mc16_13TeV.363357.Sherpa_221_NNPDF30NNLO_WqqZvv.deriv.DAOD_EXOT4.e5525_s3126_r10201_p4149',
'mc16_13TeV.363358.Sherpa_221_NNPDF30NNLO_WqqZll.deriv.DAOD_EXOT4.e5525_s3126_r10201_p4149',
'mc16_13TeV.363359.Sherpa_221_NNPDF30NNLO_WpqqWmlv.deriv.DAOD_EXOT4.e5583_s3126_r10201_p4149',
'mc16_13TeV.363360.Sherpa_221_NNPDF30NNLO_WplvWmqq.deriv.DAOD_EXOT4.e5983_s3126_r10201_p4149',
'mc16_13TeV.363489.Sherpa_221_NNPDF30NNLO_WlvZqq.deriv.DAOD_EXOT4.e5525_s3126_r10201_p4149',
]

#Di-jets
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_dijets").datasets = [
'mc16_13TeV.361020.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ0W.deriv.DAOD_EXOT4.e3569_s3126_r10201_p4149',
'mc16_13TeV.361021.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ1W.deriv.DAOD_EXOT4.e3569_s3126_r10201_p4149',
'mc16_13TeV.361022.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ2W.deriv.DAOD_EXOT4.e3668_s3126_r10201_p4149',
'mc16_13TeV.361023.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ3W.deriv.DAOD_EXOT4.e3668_s3126_r10201_p4149',
'mc16_13TeV.361024.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ4W.deriv.DAOD_EXOT4.e3668_s3126_r10201_p4149',
'mc16_13TeV.361025.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ5W.deriv.DAOD_EXOT4.e3668_s3126_r10201_p4149',
'mc16_13TeV.361026.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ6W.deriv.DAOD_EXOT4.e3569_s3126_r10201_p4149',
'mc16_13TeV.361027.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ7W.deriv.DAOD_EXOT4.e3668_s3126_r10201_p4149',
'mc16_13TeV.361028.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ8W.deriv.DAOD_EXOT4.e3569_s3126_r10201_p4149',
'mc16_13TeV.361029.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ9W.deriv.DAOD_EXOT4.e3569_s3126_r10201_p4149',
'mc16_13TeV.361030.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ10W.deriv.DAOD_EXOT4.e3569_s3126_r10201_p4149',
'mc16_13TeV.361031.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ11W.deriv.DAOD_EXOT4.e3569_s3126_r10201_p4149',
'mc16_13TeV.361032.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ12W.deriv.DAOD_EXOT4.e3668_s3126_r10201_p4149',
]

#W+jets new samples: Sherpa 2.2.11
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Wjets2211").datasets = [
'mc16_13TeV.700338.Sh_2211_Wenu_maxHTpTV2_BFilter.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700339.Sh_2211_Wenu_maxHTpTV2_CFilterBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700340.Sh_2211_Wenu_maxHTpTV2_CVetoBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700341.Sh_2211_Wmunu_maxHTpTV2_BFilter.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700342.Sh_2211_Wmunu_maxHTpTV2_CFilterBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700343.Sh_2211_Wmunu_maxHTpTV2_CVetoBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700344.Sh_2211_Wtaunu_L_maxHTpTV2_BFilter.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700345.Sh_2211_Wtaunu_L_maxHTpTV2_CFilterBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700346.Sh_2211_Wtaunu_L_maxHTpTV2_CVetoBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700347.Sh_2211_Wtaunu_H_maxHTpTV2_BFilter.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700348.Sh_2211_Wtaunu_H_maxHTpTV2_CFilterBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700349.Sh_2211_Wtaunu_H_maxHTpTV2_CVetoBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149'
]

#W+jets samples
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Wjets221").datasets = [
'mc16_13TeV.364156.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV0_70_CVetoBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364157.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV0_70_CFilterBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364158.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV0_70_BFilter.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364159.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV70_140_CVetoBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364160.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV70_140_CFilterBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364161.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV70_140_BFilter.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364162.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV140_280_CVetoBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364163.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV140_280_CFilterBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364164.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV140_280_BFilter.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364165.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV280_500_CVetoBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364166.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV280_500_CFilterBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364167.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV280_500_BFilter.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364168.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV500_1000.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364169.Sherpa_221_NNPDF30NNLO_Wmunu_MAXHTPTV1000_E_CMS.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364170.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV0_70_CVetoBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364171.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV0_70_CFilterBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364172.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV0_70_BFilter.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364173.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV70_140_CVetoBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364174.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV70_140_CFilterBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364175.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV70_140_BFilter.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364176.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV140_280_CVetoBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364177.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV140_280_CFilterBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364178.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV140_280_BFilter.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364179.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV280_500_CVetoBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364180.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV280_500_CFilterBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364181.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV280_500_BFilter.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364182.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV500_1000.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364183.Sherpa_221_NNPDF30NNLO_Wenu_MAXHTPTV1000_E_CMS.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364184.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV0_70_CVetoBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364185.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV0_70_CFilterBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364186.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV0_70_BFilter.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364187.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV70_140_CVetoBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364188.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV70_140_CFilterBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364189.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV70_140_BFilter.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364190.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV140_280_CVetoBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364191.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV140_280_CFilterBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364192.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV140_280_BFilter.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364193.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV280_500_CVetoBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364194.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV280_500_CFilterBVeto.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364195.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV280_500_BFilter.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364196.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV500_1000.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149',
'mc16_13TeV.364197.Sherpa_221_NNPDF30NNLO_Wtaunu_MAXHTPTV1000_E_CMS.deriv.DAOD_EXOT4.e5340_s3126_r10201_p4149'
]

#Z+jets new samples: Sherpa 2.2.11
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zjets2211").datasets = [
'mc16_13TeV.700320.Sh_2211_Zee_maxHTpTV2_BFilter.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',             
'mc16_13TeV.700321.Sh_2211_Zee_maxHTpTV2_CFilterBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',        
'mc16_13TeV.700322.Sh_2211_Zee_maxHTpTV2_CVetoBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',          
'mc16_13TeV.700323.Sh_2211_Zmumu_maxHTpTV2_BFilter.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',           
'mc16_13TeV.700324.Sh_2211_Zmumu_maxHTpTV2_CFilterBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',      
'mc16_13TeV.700325.Sh_2211_Zmumu_maxHTpTV2_CVetoBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',        
'mc16_13TeV.700326.Sh_2211_Ztautau_LL_maxHTpTV2_BFilter.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',      
'mc16_13TeV.700327.Sh_2211_Ztautau_LL_maxHTpTV2_CFilterBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149', 
'mc16_13TeV.700328.Sh_2211_Ztautau_LL_maxHTpTV2_CVetoBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',   
'mc16_13TeV.700329.Sh_2211_Ztautau_LH_maxHTpTV2_BFilter.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',      
'mc16_13TeV.700330.Sh_2211_Ztautau_LH_maxHTpTV2_CFilterBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149', 
'mc16_13TeV.700331.Sh_2211_Ztautau_LH_maxHTpTV2_CVetoBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',   
'mc16_13TeV.700332.Sh_2211_Ztautau_HH_maxHTpTV2_BFilter.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',      
'mc16_13TeV.700333.Sh_2211_Ztautau_HH_maxHTpTV2_CFilterBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149', 
'mc16_13TeV.700334.Sh_2211_Ztautau_HH_maxHTpTV2_CVetoBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149'
'mc16_13TeV.700320.Sh_2211_Zee_maxHTpTV2_BFilter.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700321.Sh_2211_Zee_maxHTpTV2_CFilterBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700322.Sh_2211_Zee_maxHTpTV2_CVetoBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700323.Sh_2211_Zmumu_maxHTpTV2_BFilter.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700324.Sh_2211_Zmumu_maxHTpTV2_CFilterBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149',
'mc16_13TeV.700325.Sh_2211_Zmumu_maxHTpTV2_CVetoBVeto.deriv.DAOD_EXOT4.e8351_s3126_r10201_p4149'
]

#Z+jets samples
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zjets221").datasets = [
'mc16_13TeV.364100.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV0_70_CVetoBVeto.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364101.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV0_70_CFilterBVeto.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364102.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV0_70_BFilter.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364103.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV70_140_CVetoBVeto.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364104.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV70_140_CFilterBVeto.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364105.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV70_140_BFilter.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364106.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV140_280_CVetoBVeto.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364107.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV140_280_CFilterBVeto.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364108.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV140_280_BFilter.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364109.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV280_500_CVetoBVeto.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364110.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV280_500_CFilterBVeto.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364111.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV280_500_BFilter.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364112.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV500_1000.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364113.Sherpa_221_NNPDF30NNLO_Zmumu_MAXHTPTV1000_E_CMS.deriv.DAOD_EXOT4.e5271_s3126_r10201_p4149',
'mc16_13TeV.364114.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV0_70_CVetoBVeto.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364115.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV0_70_CFilterBVeto.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364116.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV0_70_BFilter.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364117.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV70_140_CVetoBVeto.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364118.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV70_140_CFilterBVeto.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364119.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV70_140_BFilter.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364120.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_CVetoBVeto.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364121.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_CFilterBVeto.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364122.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV140_280_BFilter.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364123.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_CVetoBVeto.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364124.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_CFilterBVeto.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364125.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV280_500_BFilter.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364126.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV500_1000.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364127.Sherpa_221_NNPDF30NNLO_Zee_MAXHTPTV1000_E_CMS.deriv.DAOD_EXOT4.e5299_s3126_r10201_p4149',
'mc16_13TeV.364128.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_CVetoBVeto.deriv.DAOD_EXOT4.e5307_s3126_r10201_p4149',
'mc16_13TeV.364129.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_CFilterBVeto.deriv.DAOD_EXOT4.e5307_s3126_r10201_p4149',
'mc16_13TeV.364130.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV0_70_BFilter.deriv.DAOD_EXOT4.e5307_s3126_r10201_p4149',
'mc16_13TeV.364131.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV70_140_CVetoBVeto.deriv.DAOD_EXOT4.e5307_s3126_r10201_p4149',
'mc16_13TeV.364132.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV70_140_CFilterBVeto.deriv.DAOD_EXOT4.e5307_s3126_r10201_p4149',
'mc16_13TeV.364133.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV70_140_BFilter.deriv.DAOD_EXOT4.e5307_s3126_r10201_p4149',
'mc16_13TeV.364134.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV140_280_CVetoBVeto.deriv.DAOD_EXOT4.e5307_s3126_r10201_p4149',
'mc16_13TeV.364135.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV140_280_CFilterBVeto.deriv.DAOD_EXOT4.e5307_s3126_r10201_p4149',
'mc16_13TeV.364136.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV140_280_BFilter.deriv.DAOD_EXOT4.e5307_s3126_r10201_p4149',
'mc16_13TeV.364137.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV280_500_CVetoBVeto.deriv.DAOD_EXOT4.e5307_s3126_r10201_p4149',
'mc16_13TeV.364138.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV280_500_CFilterBVeto.deriv.DAOD_EXOT4.e5313_s3126_r10201_p4149',
'mc16_13TeV.364139.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV280_500_BFilter.deriv.DAOD_EXOT4.e5313_s3126_r10201_p4149',
'mc16_13TeV.364140.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV500_1000.deriv.DAOD_EXOT4.e5307_s3126_r10201_p4149',
'mc16_13TeV.364141.Sherpa_221_NNPDF30NNLO_Ztautau_MAXHTPTV1000_E_CMS.deriv.DAOD_EXOT4.e5307_s3126_r10201_p4149'
]

#Z' signal samples
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime400").datasets = [
'mc16_13TeV.301322.Pythia8EvtGen_A14NNPDF23LO_zprime400_tt.deriv.DAOD_EXOT4.e4061_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime500").datasets = [
'mc16_13TeV.301323.Pythia8EvtGen_A14NNPDF23LO_zprime500_tt.deriv.DAOD_EXOT4.e4061_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime750").datasets = [
'mc16_13TeV.301324.Pythia8EvtGen_A14NNPDF23LO_zprime750_tt.deriv.DAOD_EXOT4.e4061_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime1000").datasets = [
'mc16_13TeV.301325.Pythia8EvtGen_A14NNPDF23LO_zprime1000_tt.deriv.DAOD_EXOT4.e4061_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime1250").datasets = [
'mc16_13TeV.301326.Pythia8EvtGen_A14NNPDF23LO_zprime1250_tt.deriv.DAOD_EXOT4.e4061_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime1500").datasets = [
'mc16_13TeV.301327.Pythia8EvtGen_A14NNPDF23LO_zprime1500_tt.deriv.DAOD_EXOT4.e4061_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime1750").datasets = [
'mc16_13TeV.301328.Pythia8EvtGen_A14NNPDF23LO_zprime1750_tt.deriv.DAOD_EXOT4.e4061_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime2000").datasets = [
'mc16_13TeV.301329.Pythia8EvtGen_A14NNPDF23LO_zprime2000_tt.deriv.DAOD_EXOT4.e4061_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime2250").datasets = [
'mc16_13TeV.301330.Pythia8EvtGen_A14NNPDF23LO_zprime2250_tt.deriv.DAOD_EXOT4.e4061_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime2500").datasets = [
'mc16_13TeV.301331.Pythia8EvtGen_A14NNPDF23LO_zprime2500_tt.deriv.DAOD_EXOT4.e4061_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime2750").datasets = [
'mc16_13TeV.301332.Pythia8EvtGen_A14NNPDF23LO_zprime2750_tt.deriv.DAOD_EXOT4.e4061_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime3000").datasets = [
'mc16_13TeV.301333.Pythia8EvtGen_A14NNPDF23LO_zprime3000_tt.deriv.DAOD_EXOT4.e3723_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime4000").datasets = [
'mc16_13TeV.301334.Pythia8EvtGen_A14NNPDF23LO_zprime4000_tt.deriv.DAOD_EXOT4.e3723_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime5000").datasets = [
'mc16_13TeV.301335.Pythia8EvtGen_A14NNPDF23LO_zprime5000_tt.deriv.DAOD_EXOT4.e3723_s3126_r10201_p4149',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_Zprime6000").datasets = [
'mc16_13TeV.311258.Pythia8EvtGen_A14NNPDF23LO_zprime6000_tt_mtt3000.deriv.DAOD_EXOT4.e7217_s3126_r10201_p3731',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll").datasets = []
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime400'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime500'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime750'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime1000'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime1250'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime1500'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime1750'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime2000'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime2250'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime2500'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime2750'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime3000'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime4000'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime5000'].datasets
TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_ZprimeAll'].datasets += TopExamples.grid.availableDatasets['MC16d_13TeV_25ns_FS_EXOT4_Zprime6000'].datasets

#Z' signal samples: Madgraph
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprimeALL").datasets = [
'mc16_13TeV.508853.MGPy8EG_A14NN23LO_Zprime400_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508854.MGPy8EG_A14NN23LO_Zprime500_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508855.MGPy8EG_A14NN23LO_Zprime750_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508856.MGPy8EG_A14NN23LO_Zprime1000_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508857.MGPy8EG_A14NN23LO_Zprime1250_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508858.MGPy8EG_A14NN23LO_Zprime1500_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508859.MGPy8EG_A14NN23LO_Zprime1750_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508860.MGPy8EG_A14NN23LO_Zprime2000_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508861.MGPy8EG_A14NN23LO_Zprime2500_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508862.MGPy8EG_A14NN23LO_Zprime3000_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508863.MGPy8EG_A14NN23LO_Zprime3500_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508864.MGPy8EG_A14NN23LO_Zprime4000_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508865.MGPy8EG_A14NN23LO_Zprime4500_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508866.MGPy8EG_A14NN23LO_Zprime5000_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
'mc16_13TeV.508867.MGPy8EG_A14NN23LO_Zprime6000_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]

TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime400").datasets = [
'mc16_13TeV.508853.MGPy8EG_A14NN23LO_Zprime400_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime500").datasets = [
'mc16_13TeV.508854.MGPy8EG_A14NN23LO_Zprime500_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime750").datasets = [
'mc16_13TeV.508855.MGPy8EG_A14NN23LO_Zprime750_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime1000").datasets = [
'mc16_13TeV.508856.MGPy8EG_A14NN23LO_Zprime1000_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime1250").datasets = [
'mc16_13TeV.508857.MGPy8EG_A14NN23LO_Zprime1250_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime1500").datasets = [
'mc16_13TeV.508858.MGPy8EG_A14NN23LO_Zprime1500_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime1750").datasets = [
'mc16_13TeV.508859.MGPy8EG_A14NN23LO_Zprime1750_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime2000").datasets = [
'mc16_13TeV.508860.MGPy8EG_A14NN23LO_Zprime2000_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime2500").datasets = [
'mc16_13TeV.508861.MGPy8EG_A14NN23LO_Zprime2500_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime3000").datasets = [
'mc16_13TeV.508862.MGPy8EG_A14NN23LO_Zprime3000_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime3500").datasets = [
'mc16_13TeV.508863.MGPy8EG_A14NN23LO_Zprime3500_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime4000").datasets = [
'mc16_13TeV.508864.MGPy8EG_A14NN23LO_Zprime4000_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime4500").datasets = [
'mc16_13TeV.508865.MGPy8EG_A14NN23LO_Zprime4500_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime5000").datasets = [
'mc16_13TeV.508866.MGPy8EG_A14NN23LO_Zprime5000_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGZprime6000").datasets = [
'mc16_13TeV.508867.MGPy8EG_A14NN23LO_Zprime6000_Width3_tt_nallh.deriv.DAOD_EXOT4.e8357_s3126_r10201_p4840',
]

#kk Gluon signal samples
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGkkg500").datasets = [
'mc16_13TeV.510073.MGPy8EG_A14NN23LO_KKG500_Width30_GlEqGr_tt_nallh.deriv.DAOD_EXOT4.e8418_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGkkg1000").datasets = [
'mc16_13TeV.510074.MGPy8EG_A14NN23LO_KKG1000_Width30_GlEqGr_tt_nallh.deriv.DAOD_EXOT4.e8418_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGkkg1500").datasets = [
'mc16_13TeV.510075.MGPy8EG_A14NN23LO_KKG1500_Width30_GlEqGr_tt_nallh.deriv.DAOD_EXOT4.e8418_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGkkg2000").datasets = [
'mc16_13TeV.510076.MGPy8EG_A14NN23LO_KKG2000_Width30_GlEqGr_tt_nallh.deriv.DAOD_EXOT4.e8418_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGkkg2500").datasets = [
'mc16_13TeV.510077.MGPy8EG_A14NN23LO_KKG2500_Width30_GlEqGr_tt_nallh.deriv.DAOD_EXOT4.e8418_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGkkg3000").datasets = [
'mc16_13TeV.510078.MGPy8EG_A14NN23LO_KKG3000_Width30_GlEqGr_tt_nallh.deriv.DAOD_EXOT4.e8418_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGkkg3500").datasets = [
'mc16_13TeV.510079.MGPy8EG_A14NN23LO_KKG3500_Width30_GlEqGr_tt_nallh.deriv.DAOD_EXOT4.e8418_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGkkg4000").datasets = [
'mc16_13TeV.510080.MGPy8EG_A14NN23LO_KKG4000_Width30_GlEqGr_tt_nallh.deriv.DAOD_EXOT4.e8418_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGkkg4500").datasets = [
'mc16_13TeV.510081.MGPy8EG_A14NN23LO_KKG4500_Width30_GlEqGr_tt_nallh.deriv.DAOD_EXOT4.e8418_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_MGkkg5000").datasets = [
'mc16_13TeV.510082.MGPy8EG_A14NN23LO_KKG5000_Width30_GlEqGr_tt_nallh.deriv.DAOD_EXOT4.e8418_s3126_r10201_p4905',
]

#kk Graviton signal samples
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_KKGrav400").datasets = [
'mc16_13TeV.305012.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_tt_c10_m0400.deriv.DAOD_EXOT4.e4859_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_KKGrav500").datasets = [
'mc16_13TeV.305013.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_tt_c10_m0500.deriv.DAOD_EXOT4.e4859_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_KKGrav750").datasets = [
'mc16_13TeV.305014.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_tt_c10_m0750.deriv.DAOD_EXOT4.e4859_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_KKGrav1000").datasets = [
'mc16_13TeV.305015.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_tt_c10_m1000.deriv.DAOD_EXOT4.e4859_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_KKGrav2000").datasets = [
'mc16_13TeV.305016.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_tt_c10_m2000.deriv.DAOD_EXOT4.e4859_s3126_r10201_p4905',
]
TopExamples.grid.Add("MC16d_13TeV_25ns_FS_EXOT4_KKGrav3000").datasets = [
'mc16_13TeV.305017.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_tt_c10_m3000.deriv.DAOD_EXOT4.e4859_s3126_r10201_p4905',
]


TopExamples.grid.Add("MC16d_EXOT4_combination").datasets = [
'mc16_13TeV.302286.MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvll_m3000.deriv.DAOD_EXOT4.e4069_a875_r10201_p3652',
'mc16_13TeV.302136.MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VzWW_lvqq_m3000.deriv.DAOD_EXOT4.e4069_s3126_r10201_p3652',
'mc16_13TeV.302211.MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_lvqq_m3000.deriv.DAOD_EXOT4.e4148_s3126_r10201_p3652',
'mc16_13TeV.302236.MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_llqq_m3000.deriv.DAOD_EXOT4.e4069_s3126_r10201_p3652',
'mc16_13TeV.302261.MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWZ_vvqq_m3000.deriv.DAOD_EXOT4.e4069_s3126_r10201_p3652',
'mc16_13TeV.302361.MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VcWH_lvqq_m3000.deriv.DAOD_EXOT4.e4069_s3126_r10201_p3652',
'mc16_13TeV.302411.MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VzZH_llqq_m3000.deriv.DAOD_EXOT4.e4069_s3126_r10201_p3652',
'mc16_13TeV.302436.MadGraphPythia8EvtGen_A14NNPDF23LO_HVT_Agv1_VzZH_vvqq_m3000.deriv.DAOD_EXOT4.e4069_s3126_r10201_p3652',
'mc16_13TeV.308488.Pythia8EvtGen_A14NNPDF23LO_Wprime1250_Zprime1000.deriv.DAOD_EXOT4.e5914_s3126_r10201_p3652'
]
