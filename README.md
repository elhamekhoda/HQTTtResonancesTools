# HQTTtResonancesTools

This Tool is developed to fulfill the purpose of the exotics ttbar resonances search in lepton+jets and all-hadronic channel.

## Current Version:

**AanalysisTop, 21.2.65** 

### Latest release based updates
* Do not use AnalysisTop release 21.2.30 - 21.2.33. There are different bugs there.
* Due to a typo in the default config option in AnalysisTop, large-R jet JES/JMS config has been defined in our config text file like `LargeRJESJMSConfig CombMass`
* A new selection class for top-tagged large-jets is added in the tool: **NLargeJetTtresSmoothedTTMassTau32Selector**. Top tagging is done using SmoothTopTagger with 80% WP using mass and tau32 variable.

## General Updates

* This framework can be used for **_all-hadronic_** analysis as well now. A simplefied config file has been added.
* All the Top-tagging tools are initialized in ```TtresEventSaverFlatNtuple```
* Available top-taggers are used and a corresponding good flag is saved in the output Ntuple (Example: ```ljet_good_sub80```)
* The  NLargeJetTtresSTSelector class only calls ```SubstructureTopTagger```  with 80% WP
    * The "good" flag corresponds to this WP if one uses ```LARGEJETTTRESST_N 300000 >= 1``` in the config file
* The _NLargeJetTtresSubstructureTopTag80Selector_ class also calles the```SubstructureTopTagger```  with 80% WP
    * The "good" flag corresponds to this setup if one uses `LARGEJETTTRESSUBTOPTAG80_N 300000 >= 1` in the config file
* The _NLargeJetTtresSubstructureTopTag50Selector_ class also calles the```SubstructureTopTagger```  with 80% WP
    * The "good" flag corresponds to this setup if one uses `LARGEJETTTRESSUBTOPTAG50_N 300000 >= 1` in the config file
* The  _NLargeJetTtresSmoothedTTMassTau32Selector_ class only calls ```SmoothedTopTagger```  with 80% WP using mass and tau32
    * The "good" flag corresponds to this WP if one uses ```LARGEJETTTRESSTTMT_N 300000 >= 1``` in the config file
* Don't use both large-R jet selection class in one config file. Remember the name of the "good" flag is the same for both the classes at this point. New updates will be coming very soon.
* Differnt b-tagging discriminants for track jets are stored in the output Ntuple
* Based on the recent jet uncertainty recommendation ```JetUncertainties_NPModel 21NP``` has been changed to ```JetUncertainties_NPModel GlobalReduction```. (23 Nps)
More info: [here](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/JetUncertaintiesRel21Moriond2018SmallR)


## Useful pages

* Latest update of [Analysis Top](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/AnalysisTop21)
* [b-tagging](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarksRelease21) recommendations
* [Boosted jet tagging](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BoostedJetTaggingRecommendation2017#SmoothTopTaggerSection) recommendations
