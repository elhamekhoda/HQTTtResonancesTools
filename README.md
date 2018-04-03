# HQTTtResonancesTools

This Tool is developed to fulfill the purpose of the exotics ttbar resonances search in lepton+jets and all-hadronic channel.

## Current Version:

**AanalysisTop, 21.2.24** 
### Latest release based updates
* Due to a typo in the default config option in AnalysisTop, large-R jet JES/JMS config has been defined in our config text file like `LargeRJESJMSConfig CombMass`
* Issue with b-tagging SF for track jets. Here is the [JIRA link](https://its.cern.ch/jira/browse/AFT-329)

## General Updates

* All the Top-tagging tools are initialized in ```TtresEventSaverFlatNtuple```
* Available top-taggers are used and a corresponding good flag is saved in the output Ntuple
* The  NLargeJetTtresSLSelector class only calls ```SubstructureTopTagger```  with 80% WP
    * The "good" flag corresponds to this WP if one uses ```LARGEJETTTRESST_N 300000 >= 1``` in the config file
* Differnt b-tagging discriminants for track jets are stored in the output Ntuple
* Based on the recent jet uncertainty recommendation `JetUncertainties_NPModel 21NP` has been changed to `JetUncertainties_NPModel GlobalReduction`. (23 Nps)
More info: [here](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/JetUncertaintiesRel21Moriond2018SmallR)


## Useful pages

* Latest update of [Analysis Top](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/AnalysisTop21)
* [b-tagging](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarksRelease21) recommendations
* [Boosted jet tagging](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BoostedJetTaggingRecommendation2017#SmoothTopTaggerSection) recommendations
