#include "HQTTtResonancesTools/NLargeJetVHresSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>
#include <iostream>

using std::vector;

namespace top {

NLargeJetVHresSelector::NLargeJetVHresSelector(const std::string& params) :
        SignValueSelector("NLARGEJETVHRES_N", params) {
    checkValueIsInteger();
}

bool NLargeJetVHresSelector::apply(const top::Event& event) const {
    //do stuff with large Jets
    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {

        // pt and eta should already have been applied in object definition
        // but re-apply just in case it has been lowered for CR studies
        int good = 0;
        if (largeJet->pt() > 250000 &&
                std::fabs(largeJet->eta()) < 2.0) {
            ++nGoodJets;
            good = 1;
        }

        largeJet->auxdecor<int>("good") = good;
    }

    return checkInt(nGoodJets, (int) value());
}

}

