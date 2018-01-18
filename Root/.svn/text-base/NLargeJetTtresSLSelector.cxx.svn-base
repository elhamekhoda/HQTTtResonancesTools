#include "HQTTtResonancesTools/NLargeJetTtresSLSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

namespace top {

NLargeJetTtresSLSelector::NLargeJetTtresSLSelector(const std::string& params) :
        SignValueSelector("NLARGEJETTTRESSL_N", params, true) {
    checkValueIsInteger();
}

bool NLargeJetTtresSLSelector::apply(const top::Event& event) const {
    //Get the lepton - use ptr to avoid copy
    const xAOD::IParticle* lep = nullptr;
    if (event.m_electrons.size() == 1)
        lep = event.m_electrons.front();
    else if (event.m_muons.size() == 1)
        lep = event.m_muons.front();
    else
        return false;

    //Get the highest pT narrow jet "near" the lepton
    //This is given by the previous selection
    const xAOD::Jet* selJet = nullptr;
    for (const auto* const jet : event.m_jets) {
        if (jet->auxdata<char>("closeToLepton") == 1) {
            selJet = jet;
            break;
        }
    }

    //protect against having no jets - just in case
    if (!selJet)
        return false;

    //do stuff with large Jets
    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {
        double d12 = 0;
        largeJet->getAttribute("Split12", d12);

        // pt and eta should already have been applied in object definition
        // but re-apply just in case it has been lowered for CR studies
        int good = 0;
        if (largeJet->pt() > value() &&
                std::fabs(largeJet->eta()) < 2.0 &&
                largeJet->m() > 100e3 &&
                d12 > 40e3 &&
                std::fabs(top::deltaPhi(*largeJet, *lep)) > 2.3 &&
                top::deltaR(*largeJet, *selJet) > 1.5) {

            ++nGoodJets;
            good = 1;
        }

        largeJet->auxdecor<char>("topTagged") = good;
    }

    return checkInt(nGoodJets, (int) multiplicity());
}

}
