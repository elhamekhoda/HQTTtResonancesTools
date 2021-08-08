#include "HQTTtResonancesTools/NLargeJetAnyTopTagSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

/*

This selector doesn't nothing but select event with at least one large-R jet is tagged by any top-tagger in order to skim the ntuple.

*/

namespace top {

    NLargeJetAnyTopTagSelector::NLargeJetAnyTopTagSelector(const std::string& params) :
    SignValueSelector("NLARGEJETANYTOPTAG_N", params) {
        checkValueIsInteger();
}

bool NLargeJetAnyTopTagSelector::apply(const top::Event& event) const {
    //check the large-R jet top-tagging flags
    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {
        int good = 0;
        if (largeJet->isAvailable<char>("isTagged_JSSWTopTaggerDNN_DNNTaggerTopQuarkInclusive80") || largeJet->isAvailable<char>("isTagged_JSSWTopTaggerDNN_DNNTaggerTopQuarkContained80") ){
            if (largeJet->getAttribute<char>("isTagged_JSSWTopTaggerDNN_DNNTaggerTopQuarkInclusive80") || largeJet->getAttribute<char>("isTagged_JSSWTopTaggerDNN_DNNTaggerTopQuarkContained80") ){
                good = 1;
                ++nGoodJets;
            }
        }
        largeJet->auxdecor<int>("topTagged") = good;
    }


    return checkInt(nGoodJets, (int) multiplicity());
}

}
