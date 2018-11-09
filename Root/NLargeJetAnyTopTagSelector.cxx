#include "HQTTtResonancesTools/NLargeJetAnyTopTagSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

/*

This selector doesn't nothing but select event with at least one large-R jet is tagged by any top-tagger in order to skim the ntuple.
**It must be put after all the other TopTagSelectors!**

*/

namespace top {

    NLargeJetAnyTopTagSelector::NLargeJetAnyTopTagSelector(const std::string& params) :
    SignValueSelector("NLARGEJETANYTOPTAG_N", params) {
        checkValueIsInteger();
}

bool NLargeJetAnyTopTagSelector::apply(const top::Event& event) const {    
    //do stuff with large Jets
    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {

        if (largeJet->isAvailable<int>("topTagged")){
            if (largeJet->auxdecor<int>("topTagged")){
                ++nGoodJets;
            }
        }
    }
    
    return checkInt(nGoodJets, (int) multiplicity());
}

}
