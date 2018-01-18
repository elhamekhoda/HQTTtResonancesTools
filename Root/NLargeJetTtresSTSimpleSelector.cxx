#include "HQTTtResonancesTools/NLargeJetTtresSTSimpleSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

/*

A Large R Jet Class which passes the smooth pre-rec tagger requirements


*/

namespace top {

NLargeJetTtresSTSimpleSelector::NLargeJetTtresSTSimpleSelector(const std::string& params) :
        SignValueSelector("NLARGEJETTTRESSTSIMPLE_N", params, true) {
    checkValueIsInteger();
//  STT = STTHelpers::configSubstTagger("TightSmoothTopTag", "SmoothCut_50");
    STL = STTHelpers::configSubstTagger("LooseSmoothTopTag", "SmoothCut_80");

}

bool NLargeJetTtresSTSimpleSelector::apply(const top::Event& event) const {
    //do stuff with large Jets
    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {
        // pt and eta should already have been applied in object definition
        // but re-apply just in case it has been lowered for CR studies
        int good = 0;
        if (largeJet->pt() > value() &&
            std::fabs(largeJet->eta()) < 2.0 &&
            STL->isTagged(*largeJet) == true) {

            ++nGoodJets;
            good = 1;
        }

        largeJet->auxdecor<char>("topTagged") = good;
    }

    return checkInt(nGoodJets, (int) multiplicity());
}

}
