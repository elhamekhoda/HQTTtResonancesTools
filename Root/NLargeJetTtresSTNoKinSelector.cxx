#include "HQTTtResonancesTools/NLargeJetTtresSTNoKinSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

/*

A Large R Jet Class which passes the smooth pre-rec tagger requirements


*/

namespace top {

NLargeJetTtresSTNoKinSelector::NLargeJetTtresSTNoKinSelector(const std::string& params) :
        SignValueSelector("NLARGEJETTTRESSTNOKIN_N", params, true) {
    checkValueIsInteger();
//  STT = STTHelpers::configSubstTagger("TightSmoothTopTag", "SmoothCut_50");
    STL = STTHelpers::configSubstTagger("LooseSmoothTopTag", "SmoothCut_80");

}

bool NLargeJetTtresSTNoKinSelector::apply(const top::Event& event) const {
    //Get the lepton - use ptr to avoid copy
    const xAOD::IParticle* lep = nullptr;
    if (event.m_electrons.size() == 1)
        lep = event.m_electrons.front();
    else if (event.m_muons.size() == 1)
        lep = event.m_muons.front();
    else
        return false;

    //do stuff with large Jets
    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {
        // pt and eta should already have been applied in object definition
        // but re-apply just in case it has been lowered for CR studies
        int good = 0;
        if (largeJet->pt() > value() &&
                std::fabs(largeJet->eta()) < 2.0 &&
                STL->isTagged(*largeJet) == true &&
                std::fabs(top::deltaPhi(*largeJet, *lep)) > 2.3) {

            ++nGoodJets;
            good = 1;
        }

        largeJet->auxdecor<int>("topTagged") = good;
    }

    return checkInt(nGoodJets, (int) multiplicity());
}

}
