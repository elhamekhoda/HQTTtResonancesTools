#include "HQTTtResonancesTools/NLargeJetTtresMassOnlySelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

namespace top {

NLargeJetTtresMassOnlySelector::NLargeJetTtresMassOnlySelector(const std::string& params) :
        SignValueSelector("NLARGEJETTTRESMASSONLY_N", params, true) {
    checkValueIsInteger();

}

bool NLargeJetTtresMassOnlySelector::apply(const top::Event& event) const {
    //Get the lepton - use ptr to avoid copy
    //I don't see why it is needed to get the lepton @Yu-Heng
    if (event.m_electrons.size() + event.m_muons.size() != 1)
        return false;

    //Get the highest pT narrow jet "near" the lepton
    //This is given by the previous selection
    const xAOD::Jet* selJet = nullptr;
    for (const auto* const jet : event.m_jets) {
        if (jet->auxdata<int>("closeToLepton") == 1) {
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
       
        // pt and eta should already have been applied in object definition
        // but re-apply just in case it has been lowered for CR studies
        int good = 0;
        if (largeJet->pt() > value() &&
            std::fabs(largeJet->eta()) < 2.0 && largeJet->m() > 70e3) {

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
