#include "HQTTtResonancesTools/NLargeJetTtresSubstructureTopTag50Selector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

/*

A Large R Jet Class which passes the smooth pre-rec tagger requirements
It uses an old tagger class baed on not-fully contained smoothed toptagging. This was used in rel20.7 analysis
Here we intialize the tool with 50% WP based on mass and tau32


*/

namespace top {
  
  NLargeJetTtresSubstructureTopTag50Selector::NLargeJetTtresSubstructureTopTag50Selector(const std::string& params) :
    SignValueSelector("NLARGEJETTTRESSUBTOPTAG50_N", params, true) {
    checkValueIsInteger();
    //STL = STTHelpers::configSubstTagger("TightSmoothTopTag", "SmoothCut_50");
    STL = STTHelpers::configSubstTagger("LooseSmoothTopTag", "SmoothCut_50");
    
  }
  
  bool NLargeJetTtresSubstructureTopTag50Selector::apply(const top::Event& event) const {    
    //do stuff with large Jets
    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {
      
      // pt and eta should already have been applied in object definition
      // but re-apply just in case it has been lowered for CR studies
      int good = 0;
      if (largeJet->pt() > value() &&
        std::fabs(largeJet->eta()) < 2.0 &&
        STL->isTagged(*largeJet) == true ){

          ++nGoodJets;
          good = 1;
      }
      
      largeJet->auxdecor<int>("topTagged") = good;
    }
    
    return checkInt(nGoodJets, (int) multiplicity());
  }
  
}