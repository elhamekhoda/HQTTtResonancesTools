#include "HQTTtResonancesTools/NLargeJetTtresFakesSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

namespace top {

NLargeJetTtresFakesSelector::NLargeJetTtresFakesSelector(const std::string& params) :
        SignValueSelector("NLARGEJETTTRESFAKES_N", params, true) {
    checkValueIsInteger();
}

bool NLargeJetTtresFakesSelector::apply(const top::Event& event) const {
 
    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {
        //double d12 = 0;
        //largeJet->getAttribute("Split12", d12);

        // pt and eta should already have been applied in object definition
        // but re-apply just in case it has been lowered for CR studies
        int good = 0;
        if (largeJet->pt() > value() &&
            std::fabs(largeJet->eta()) < 2.0 &&
            largeJet->m() < 70e3 ) {

            ++nGoodJets;
            good = 1;
        }

        if (!largeJet->isAvailable<int>("topTagged")) {
            largeJet->auxdecor<int>("topTagged") = good;
        } else if (good) {
            largeJet->auxdecor<int>("topTagged") = 1;
        }
    }

    return checkInt(nGoodJets, (int) multiplicity());
}

}
