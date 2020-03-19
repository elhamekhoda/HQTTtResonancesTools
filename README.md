# HQTTtResonancesTools

This Tool is developed to fulfill the purpose of the exotics ttbar resonances search in lepton+jets and all-hadronic channel.

## Current Version:

**AanalysisTop, 21.2.113** 

### Latest release based updates
* Do not use AnalysisTop release < 21.2.90. The derivations are updated now.
* Note that `AnalysisTop` and `AnalysisBase` is merged abover AnalysisTop 21.2.100. So you have to set `AnalysisBase` environment now.
* Recommendation is to use release >= `AnalysisBase 21.2.113`
* You need CentOS 7 operating system to run the latest AnalysisTop/AnalysisBase releases
* Top-tagging issue: #5
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
* The  _NLargeJetTtresFHDNNTopTagSelector_ class calls ```ShapeBasedDNNTopTagger```  with several different WPs
    * If one uses in the config file
        1.  ```LJET_DNNTOPTAG_INCLUSIVE80_N 350000 >= 1```, the "good" flags corresponds to 80% WP using Rel21 inclusive top definition. 
        2.  ```LJET_DNNTOPTAG_INCLUSIVE850_N 350000 >= 1```, the "good" flags corresponds to 50% WP using Rel21 inclusive top definition. 
        3.  ```LJET_DNNTOPTAG_CONTAINED80_N 350000 >= 1```, the "good" flags corresponds to 80% WP using Rel21 fully-contained top definition. 
        4.  ```LJET_DNNTOPTAG_CONTAINED50_N 350000 >= 1```, the "good" flags corresponds to 50% WP using Rel21 fully-contained top definition. 
        5.  ```LJET_DNNTOPTAG_TTRES0L1B_N 350000 >= 1```, the "good" flags corresponds to full had. sensitivity-based WP in 1bSR using Rel20.7 fully-contained top definition. 
        6.  ```LJET_DNNTOPTAG_TTRES0L2B_N 350000 >= 1```, the "good" flags corresponds to full had. sensitivity-based WP in 2bSR using Rel20.7 fully-contained top definition. 
        7.  ```LJET_DNNTOPTAG_TTRES1L**EFF_N 350000 >= 1```, the "good" flags corresponds to l+jets fixed efficiency WP from 50%~95% (step size 5%) using Rel20.7 fully-contained top definition derived by @kkrowpma. 
    *   To store the __DNNisTagged__ flag or __DNNScore__, one can also add, for example, 
        ```bash
        ExtraTopTaggingWP DNNTOPTAG_INCLUSIVE80 DNNTOPTAG_INCLUSIVE50 DNNTOPTAG_CONTAINED50 DNNTOPTAG_CONTAINED50 DNNTOPTAG_TTRES1L50EFF DNNTOPTAG_TTRES1L95EFF
        ```

* Don't use both large-R jet selection class in one config file. Remember the name of the "good" flag is the same for both the classes at this point. New updates will be coming very soon.
* Differnt b-tagging discriminants for track jets are stored in the output Ntuple
* Based on the recent jet uncertainty recommendation ```JetUncertainties_NPModel 21NP``` has been changed to ```JetUncertainties_NPModel GlobalReduction```. (23 Nps)
More info: [here](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/JetUncertaintiesRel21Moriond2018SmallR)


## Useful pages

* Latest update of [Analysis Top](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/AnalysisTop21)
* [b-tagging](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarksRelease21) recommendations
* [Boosted jet tagging](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BoostedJetTaggingRecommendation2017#SmoothTopTaggerSection) recommendations
