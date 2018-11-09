#include "HQTTtResonancesTools/NLargeJetTtresFHDNNTopTagSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

/*

A Large R Jet Class which passes the smooth pre-rec tagger requirements
It uses an old tagger class baed on not-fully contained smoothed toptagging. This was used in rel20.7 analysis
Here we intialize the tool with 80% WP based on mass and tau32


*/

namespace top {
  
  NLargeJetTtresFHDNNTopTagSelector::NLargeJetTtresFHDNNTopTagSelector(const std::string& params, const std::string& region) :
    SignValueSelector("NLARGEJETTTRES" + region + "DNN_N", params, true) {
    m_region = region;
    checkValueIsInteger();
    //STL = STTHelpers::configSubstTagger("TightSmoothTopTag", "SmoothCut_80");
    TopTaggerDNN = new JSSWTopTaggerDNN("JSSWTopTaggerTtres" + region + "DNN");
    if (region == "0L0B") {
        top::check(TopTaggerDNN->setProperty( "ConfigFile",   "JSSWTopTaggerDNN/JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC15c_20170824_BOOSTSetup_Ttres0l0b.dat"), "Failed to set property for ConfigFile");
    } else if (region == "0L1B") {
        top::check(TopTaggerDNN->setProperty( "ConfigFile",   "JSSWTopTaggerDNN/JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC15c_20170824_BOOSTSetup_Ttres0l1b.dat"), "Failed to set property for ConfigFile");
    } else if (region == "0L2B") {
        top::check(TopTaggerDNN->setProperty( "ConfigFile",   "JSSWTopTaggerDNN/JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC15c_20170824_BOOSTSetup_Ttres0l2b.dat"), "Failed to set property for ConfigFile");
    }
    
    top::check(TopTaggerDNN->setProperty( "JetPtMin",   value()), "Failed to set property for ConfigFile");
    // top::check(TopTaggerDNN->setProperty( "OutputLevel",   MSG::VERBOSE), "Failed to set property for ConfigFile");
    top::check(TopTaggerDNN->initialize(), "Initializing failed");
  }
  
  bool NLargeJetTtresFHDNNTopTagSelector::apply(const top::Event& event) const {    
    //do stuff with large Jets
    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {
      
      // pt and eta should already have been applied in object definition
      // but re-apply just in case it has been lowered for CR studies
      int good = 0;
      if (TopTaggerDNN->tag(*largeJet)){
          ++nGoodJets;
          good = 1;
      }
      
      largeJet->auxdecor<int>("topTagged") = good;
      largeJet->auxdecor<int>("DNNTaggerTopQuarkTtres" + m_region) = good;
    }
    
    return checkInt(nGoodJets, (int) multiplicity());
  }
  
}
