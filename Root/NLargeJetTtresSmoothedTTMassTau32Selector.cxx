#include "HQTTtResonancesTools/NLargeJetTtresSmoothedTTMassTau32Selector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

/*

A Large R Jet Class which passes the smooth pre-rec tagger requirements. 
This class will tag a large-R jets with the SmoothTopTagger using the mass and tau32 variables with 80% efficiency.


*/
  
namespace top {

NLargeJetTtresSmoothedTTMassTau32Selector::NLargeJetTtresSmoothedTTMassTau32Selector(const std::string& params) :
        SignValueSelector("NLARGEJETTTRESSTTMT_N", params, true) {
    checkValueIsInteger();
    m_smoothedTopTaggerMT80 = nullptr;
    m_smoothedTopTaggerMT80 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerMT80" ) );
    top::check(m_smoothedTopTaggerMT80->setProperty( "ConfigFile", "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_MassTau32FixedSignalEfficiency80_MC15c_20161209.dat"),"Failed to set property for ConfigFile");
    top::check(m_smoothedTopTaggerMT80->initialize(),"Initializing failed");

  }

//============================================== Other Smooth TopTagger definitions ===================================================//
/*
  m_smoothedTopTaggerMT50 = nullptr;
  m_smoothedTopTaggerTS80 = nullptr;m_smoothedTopTaggerTS50 = nullptr;
  m_smoothedTopTaggerQT80 = nullptr;m_smoothedTopTaggerQT50 = nullptr;
 
  m_smoothedTopTaggerMT50 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerMT50" ) );
  m_smoothedTopTaggerTS80 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerTS80" ) );
  m_smoothedTopTaggerTS50 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerTS50" ) );
  m_smoothedTopTaggerQT80 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerQT80" ) );
  m_smoothedTopTaggerQT50 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerQT50" ) );
   
  top::check(m_smoothedTopTaggerMT50->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_MassTau32FixedSignalEfficiency50_MC15c_20161209.dat"),"Failed to set property for ConfigFile");
  top::check(m_smoothedTopTaggerTS80->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_Tau32Split23FixedSignalEfficiency80_MC15c_20161209.dat"),"Failed to set property for ConfigFile");
  top::check(m_smoothedTopTaggerTS50->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_Tau32Split23FixedSignalEfficiency50_MC15c_20161209.dat"),"Failed to set property for ConfigFile");
  top::check(m_smoothedTopTaggerQT80->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_QwTau32FixedSignalEfficiency80_MC15c_20161209.dat"),"Failed to set property for ConfigFile");
  top::check(m_smoothedTopTaggerQT50->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_QwTau32FixedSignalEfficiency50_MC15c_20161209.dat"),"Failed to set property for ConfigFile");
  
  top::check(m_smoothedTopTaggerMT50->initialize(),"Initializing failed");
  top::check(m_smoothedTopTaggerTS80->initialize(),"Initializing failed");
  top::check(m_smoothedTopTaggerTS50->initialize(),"Initializing failed");
  top::check(m_smoothedTopTaggerQT80->initialize(),"Initializing failed");
  top::check(m_smoothedTopTaggerQT50->initialize(),"Initializing failed");

*/
//=========================================================================================================================================//

bool NLargeJetTtresSmoothedTTMassTau32Selector::apply(const top::Event& event) const {

  int nGoodJets = 0;
  for (const auto* const largeJet : event.m_largeJets) {

    // pt and eta should already have been applied in object definition
    // but re-apply just in case it has been lowered for CR studies

    int good = 0;
    //good_smooth_mt50=0; good_smooth_ts80=0,good_smooth_ts50=0,good_smooth_qt80=0,good_smooth_qt50=0;

    if (largeJet->pt() > value() && std::fabs(largeJet->eta()) < 2.0) {

      if(m_smoothedTopTaggerMT80->tag(*largeJet)){
        good = 1;
        ++nGoodJets;
      }
    }
/*    
      if(m_smoothedTopTaggerMT80->tag(*largeJet)) good_smooth_mt80 = 1; 
      if(m_smoothedTopTaggerMT50->tag(*largeJet)) good_smooth_mt50 = 1; 
      if(m_smoothedTopTaggerTS80->tag(*largeJet)) good_smooth_ts80 = 1; 
      if(m_smoothedTopTaggerTS50->tag(*largeJet)) good_smooth_ts50 = 1; 
      if(m_smoothedTopTaggerQT80->tag(*largeJet)) good_smooth_qt80 = 1; 
      if(m_smoothedTopTaggerQT50->tag(*largeJet)) good_smooth_qt50 = 1; 

*/

      //Decorate the jet pointer with the "toptagged" label
        if (!largeJet->isAvailable<int>("topTagged")) {
            largeJet->auxdecor<int>("topTagged") = good;
        } else if (good) {
            largeJet->auxdecor<int>("topTagged") = 1;
        }
    }

    return checkInt(nGoodJets, (int) multiplicity());
  }

}

