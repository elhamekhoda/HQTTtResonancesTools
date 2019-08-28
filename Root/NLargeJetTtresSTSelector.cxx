#include "HQTTtResonancesTools/NLargeJetTtresSTSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

/*

A Large R Jet Class which passes the smooth pre-rec tagger requirements


*/

namespace top {

  NLargeJetTtresSTSelector::NLargeJetTtresSTSelector(const std::string& params) :
    SignValueSelector("NLARGEJETTTRESST_N", params, true) {
    checkValueIsInteger();
    //STT = STTHelpers::configSubstTagger("TightSmoothTopTag", "SmoothCut_50");
    STL80 = STTHelpers::configSubstTagger("LooseSmoothTopTag", "SmoothCut_80");
    //STL50 = STTHelpers::configSubstTagger("LooseSmoothTopTag", "SmoothCut_50");

/*
  //rel21 definition
  m_smoothedTopTaggerMT80 = nullptr;m_smoothedTopTaggerMT50 = nullptr;
  m_smoothedTopTaggerTS80 = nullptr;m_smoothedTopTaggerTS50 = nullptr;
  m_smoothedTopTaggerQT80 = nullptr;m_smoothedTopTaggerQT50 = nullptr;
  m_bdtTopTagger80 = nullptr;m_dnnTopTagger80 = nullptr;
  m_smoothedTopTaggerMT80 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerMT80" ) );
  m_smoothedTopTaggerMT50 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerMT50" ) );
  m_smoothedTopTaggerTS80 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerTS80" ) );
  m_smoothedTopTaggerTS50 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerTS50" ) );
  m_smoothedTopTaggerQT80 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerQT80" ) );
  m_smoothedTopTaggerQT50 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerQT50" ) );
  m_bdtTopTagger80 = std::unique_ptr<JSSWTopTaggerBDT>( new JSSWTopTaggerBDT( "JSSWTopTaggerBDT80" ) );
  m_dnnTopTagger80 = std::unique_ptr<JSSWTopTaggerDNN>( new JSSWTopTaggerDNN( "JSSWTopTaggerDNN80" ) );
  m_topoTopTagger = std::unique_ptr<TopoclusterTopTagger>( new TopoclusterTopTagger( "TopoclusterTopTagger" ) );
  top::check(m_smoothedTopTaggerMT80->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_MassTau32FixedSignalEfficiency80_MC15c_20161209.dat"),"Failed to set property for ConfigFile");
  top::check(m_smoothedTopTaggerMT50->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_MassTau32FixedSignalEfficiency50_MC15c_20161209.dat"),"Failed to set property for ConfigFile");
  top::check(m_smoothedTopTaggerTS80->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_Tau32Split23FixedSignalEfficiency80_MC15c_20161209.dat"),"Failed to set property for ConfigFile");
  top::check(m_smoothedTopTaggerTS50->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_Tau32Split23FixedSignalEfficiency50_MC15c_20161209.dat"),"Failed to set property for ConfigFile");
  top::check(m_smoothedTopTaggerQT80->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_QwTau32FixedSignalEfficiency80_MC15c_20161209.dat"),"Failed to set property for ConfigFile");
  top::check(m_smoothedTopTaggerQT50->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_QwTau32FixedSignalEfficiency50_MC15c_20161209.dat"),"Failed to set property for ConfigFile");
  top::check(m_bdtTopTagger80->setProperty( "ConfigFile",   "JSSWTopTaggerBDT/JSSBDTTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC15c_20170824_BOOSTSetup80Eff.dat"),"Failed to set property for ConfigFile");
  top::check(m_dnnTopTagger80->setProperty( "ConfigFile",   "JSSWTopTaggerDNN/JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC15c_20170824_BOOSTSetup80Eff.dat"),"Failed to set property for ConfigFile");
  top::check(m_topoTopTagger->setProperty( "ConfigFile",   "TopoclusterTopTagger/TopoclusterTopTagger_AntiKt10LCTopoTrimmed_TopQuark_MC15c_20170511_NOTFORANALYSIS.dat"),"Failed to set property for ConfigFile");
  top::check(m_smoothedTopTaggerMT80->initialize(),"Initializing failed");
  top::check(m_smoothedTopTaggerMT50->initialize(),"Initializing failed");
  top::check(m_smoothedTopTaggerTS80->initialize(),"Initializing failed");
  top::check(m_smoothedTopTaggerTS50->initialize(),"Initializing failed");
  top::check(m_smoothedTopTaggerQT80->initialize(),"Initializing failed");
  top::check(m_smoothedTopTaggerQT50->initialize(),"Initializing failed");
  top::check(m_bdtTopTagger80->initialize(),"Initializing failed");
  top::check(m_dnnTopTagger80->initialize(),"Initializing failed");
  top::check(m_topoTopTagger->initialize(),"Initializing failed");

*/

  }

bool NLargeJetTtresSTSelector::apply(const top::Event& event) const {

    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {
//    double d12 = 0;
//    largeJet->getAttribute("Split12", d12);
       
      // pt and eta should already have been applied in object definition
      // but re-apply just in case it has been lowered for CR studies
      int good = 0;
      //int good_sub_80=0; good_sub_50=0,good_smooth_mt80=0,good_smooth_mt50=0;
      //int good_smooth_ts80=0,good_smooth_ts50=0,good_smooth_qt80=0,good_smooth_qt50=0;
      //int good_bdt_80=0,good_dnn_80=0,good_topo=0;

      //if (largeJet->pt() > value() &&
      //    std::fabs(largeJet->eta()) < 2.0 &&
      //    STL->isTagged(*largeJet) == true ) {

      if (largeJet->pt() > value() && std::fabs(largeJet->eta()) < 2.0) {
        if (STL80->isTagged(*largeJet) == true ) {
          good = 1;
          ++nGoodJets;
        }
      }

/*
      if (STL50->isTagged(*largeJet) == true ) good_sub_50 = 1;
      if(m_smoothedTopTaggerMT80->tag(*largeJet)) good_smooth_mt80 = 1; 
      if(m_smoothedTopTaggerMT50->tag(*largeJet)) good_smooth_mt50 = 1; 
      if(m_smoothedTopTaggerTS80->tag(*largeJet)) good_smooth_ts80 = 1; 
      if(m_smoothedTopTaggerTS50->tag(*largeJet)) good_smooth_ts50 = 1; 
      if(m_smoothedTopTaggerQT80->tag(*largeJet)) good_smooth_qt80 = 1; 
      if(m_smoothedTopTaggerQT50->tag(*largeJet)) good_smooth_qt50 = 1; 
      if(m_bdtTopTagger80->tag(*largeJet)) good_bdt_80 = 1; 
      if(m_dnnTopTagger80->tag(*largeJet)) good_dnn_80 = 1; 
      if(m_topoTopTagger->tag(*largeJet)) good_topo = 1; 

*/

      //Decorate the jet pointer
        if (!largeJet->isAvailable<int>("topTagged")) {
            largeJet->auxdecor<int>("topTagged") = good;
        } else if (good) {
            largeJet->auxdecor<int>("topTagged") = 1;
        }

/*    largeJet->auxdecor<int>("topTaggedSub80") = good_sub_80;
      largeJet->auxdecor<int>("topTaggedSub50") = good_sub_50;
      largeJet->auxdecor<int>("topTaggedSmoothMT80") = good_smooth_mt80;
      largeJet->auxdecor<int>("topTaggedSmoothMT50") = good_smooth_mt50;
      largeJet->auxdecor<int>("topTaggedSmoothTS80") = good_smooth_ts80;
      largeJet->auxdecor<int>("topTaggedSmoothTS50") = good_smooth_ts50;
      largeJet->auxdecor<int>("topTaggedSmoothQT80") = good_smooth_qt80;
      largeJet->auxdecor<int>("topTaggedSmoothQT50") = good_smooth_qt50;
      largeJet->auxdecor<int>("topTaggedBDT80") = good_bdt_80;
      largeJet->auxdecor<int>("topTaggedDNN80") = good_dnn_80;
      largeJet->auxdecor<int>("topTaggedTopoCluster") = good_topo;
*/
      //std::cout << "smoothed top tag MT80 result=" << m_smoothedTopTaggerMT80->tag(*largeJet) << std::endl; 
      //std::cout << "smoothed top tag TS80 result=" << m_smoothedTopTaggerTS80->tag(*largeJet) << std::endl; 

      //std::cout << "score bdt/dnn=" << bdt_score << "/" << dnn_score << std::endl;
      //float bdt_score = largeJet->auxdecor<float>("BDTScore_Score");
      //float dnn_score = largeJet->auxdecor<float>("DNNScore_Score");
      //float topo_score = largeJet->auxdecor<float>("TopoclusterTopTaggerTopQuark_Score");

      //std::cout << "AUx : " << largeJet->auxdecor<int>("topTaggedDNN80") << std::endl;

    }
    return checkInt(nGoodJets, (int) multiplicity());
  }
}
