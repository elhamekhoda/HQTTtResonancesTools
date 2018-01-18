#include "HQTTtResonancesTools/NLargeJetTtresLeptonicSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

namespace top {

NLargeJetTtresLeptonicSelector::NLargeJetTtresLeptonicSelector(const std::string& params) :
        SignValueSelector("NLARGEJETTTRESLEPTONIC_N", params) {
    checkValueIsInteger();
}

bool NLargeJetTtresLeptonicSelector::apply(const top::Event& event) const {
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
        if (largeJet->pt() > 200e3 &&
                std::fabs(largeJet->eta()) < 2.0 &&
                largeJet->m() > 50e3 &&
                top::deltaR(*largeJet, *lep) < 1.0) {

            ++nGoodJets;
            good = 1;
        }

        largeJet->auxdecor<char>("topTagged") = good;
    }

    return checkInt(nGoodJets, (int) value());
}

}
