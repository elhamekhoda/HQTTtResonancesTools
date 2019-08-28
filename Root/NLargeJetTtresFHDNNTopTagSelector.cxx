#include "HQTTtResonancesTools/NLargeJetTtresFHDNNTopTagSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>
#include <stdexcept>

/*

A Large R Jet Class which passes the smooth pre-rec tagger requirements
It uses an old tagger class baed on not-fully contained smoothed toptagging. This was used in rel20.7 analysis
Here we intialize the tool with 80% WP based on mass and tau32


*/

namespace top {

NLargeJetTtresFHDNNTopTagSelector::NLargeJetTtresFHDNNTopTagSelector(const std::string& params, const std::string& region, std::shared_ptr<top::TopConfig> config) :
    SignValueSelector("NLARGEJET_DNN" + region + "_N", params, true),
    m_dec_isTagged("isTagged"),
    m_dec_isTaggedAny("topTagged")
     {
    checkValueIsInteger();
    m_region = region;
    m_dec_isTagged = SG::AuxElement::Decorator<int>(("DNNTaggerTopQuark" + m_region).c_str());
    m_TopTaggerDNN = std::unique_ptr<JSSWTopTaggerDNN>( new JSSWTopTaggerDNN("JSSWTopTaggerDNN" + region) );

    if (region == "ContainedTtres0L0B") {
        top::check(m_TopTaggerDNN->setProperty( "ConfigFile",   "JSSWTopTaggerDNN/JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC15c_20170824_BOOSTSetup_Ttres0l0b.dat"), "Failed to set property for ConfigFile");
    } else if (region == "ContainedTtres0L1B") {
        top::check(m_TopTaggerDNN->setProperty( "ConfigFile",   "JSSWTopTaggerDNN/JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC15c_20170824_BOOSTSetup_Ttres0l1b.dat"), "Failed to set property for ConfigFile");
    } else if (region == "ContainedTtres0L2B") {
        top::check(m_TopTaggerDNN->setProperty( "ConfigFile",   "JSSWTopTaggerDNN/JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC15c_20170824_BOOSTSetup_Ttres0l2b.dat"), "Failed to set property for ConfigFile");
    } else if (region.find("ContainedTtres1L") == 0) {
        size_t pos = region.find("Eff");
        std::string eff = region.substr(pos - 2, 2);
        top::check(m_TopTaggerDNN->setProperty( "ConfigFile",   "JSSWTopTaggerDNN/JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC15c_20170824_BOOSTSetup_Ttres1l" + eff + "Eff.dat"), "Failed to set property for ConfigFile");
    } else if (region == "Contained80") {
        top::check(m_TopTaggerDNN->setProperty( "CalibArea",    "JSSWTopTaggerDNN/Rel21/"), "Failed to set property for CalibArea" );
        top::check(m_TopTaggerDNN->setProperty( "ConfigFile",   "JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC16d_20190827_80Eff.dat"), "Failed to set property for ConfigFile");
    } else if (region == "Contained50") {
        top::check(m_TopTaggerDNN->setProperty( "CalibArea",    "JSSWTopTaggerDNN/Rel21/"), "Failed to set property for CalibArea" );
        top::check(m_TopTaggerDNN->setProperty( "ConfigFile",   "JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC16d_20190405_50Eff.dat"), "Failed to set property for ConfigFile");
    } else if (region == "Inclusive80") {
        top::check(m_TopTaggerDNN->setProperty( "CalibArea",    "JSSWTopTaggerDNN/Rel21/"), "Failed to set property for CalibArea" );
        top::check(m_TopTaggerDNN->setProperty( "ConfigFile",   "JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkInclusive_MC16d_20190405_80Eff.dat"), "Failed to set property for ConfigFile");
    } else if (region == "Inclusive50") {
        top::check(m_TopTaggerDNN->setProperty( "CalibArea",    "JSSWTopTaggerDNN/Rel21/"), "Failed to set property for CalibArea" );
        top::check(m_TopTaggerDNN->setProperty( "ConfigFile",   "JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkInclusive_MC16d_20190405_50Eff.dat"), "Failed to set property for ConfigFile");
    } else {
        std::stringstream errMsg;
        errMsg << "TopTagging WP: " << "\"" << "JSSWTopTaggerDNN" + region << "\"" << " is not available!";
        throw std::invalid_argument(errMsg.str());
    }
    std::cout << "DSID:" << (int)config->getDSID() << std::endl;
    top::check(m_TopTaggerDNN->setProperty( "DSID",   config != nullptr ? (int)config->getDSID() : -1), "Failed to set DSID for ConfigFile");
    top::check(m_TopTaggerDNN->setProperty( "IsMC",   config != nullptr ? config->isMC() : true),       "Failed to set IsMC for ConfigFile");
    // top::check(m_TopTaggerDNN->setProperty( "JetPtMin",   value()), "Failed to set property for ConfigFile");
    // top::check(m_TopTaggerDNN->setProperty( "OutputLevel",   MSG::VERBOSE), "Failed to set property for ConfigFile");

    top::check(m_TopTaggerDNN->initialize(), "Initializing failed");
}

bool NLargeJetTtresFHDNNTopTagSelector::apply(const top::Event& event) const {
    //do stuff with large Jets
    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {

        // pt and eta should already have been applied in object definition
        // but re-apply just in case it has been lowered for CR studies
        int good = 0;
        if (m_TopTaggerDNN->tag(*largeJet)) {
            ++nGoodJets;
            good = 1;
        }

        m_dec_isTagged(*largeJet) = good;
        if (!m_dec_isTaggedAny.isAvailable(*largeJet)) {
            m_dec_isTaggedAny(*largeJet) = good;
        } else if (good) {
            m_dec_isTaggedAny(*largeJet) = 1;
        }
    }

    return checkInt(nGoodJets, (int) multiplicity());
}

}
