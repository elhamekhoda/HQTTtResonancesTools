#include "HQTTtResonancesTools/NLargeJetTtresAHSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

namespace top {

NLargeJetTtresAHSelector::NLargeJetTtresAHSelector(const std::string& params) :
        SignValueSelector("NLARGEJETTTRESAH_N", params) {
    checkValueIsInteger();
}

bool NLargeJetTtresAHSelector::apply(const top::Event& event) const {
    //do stuff with large Jets
    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {
        double d12 = 0;
        largeJet->getAttribute("Split12", d12);

        // pt and eta should already have been applied in object definition
        // but re-apply just in case it has been lowered for CR studies
        int good = 0;
        if (largeJet->pt() > 300e3 &&
                std::fabs(largeJet->eta()) < 2.0 &&
                largeJet->m() > 100e3 &&
                d12 > 40e3) {

            ++nGoodJets;
            good = 1;
        }

        largeJet->auxdecor<char>("topTagged") = good;
    }

    return checkInt(nGoodJets, (int) value());
}

}
