#include "HQTTtResonancesTools/NJetCloseLeptonSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include <vector>
#include <cmath>

namespace top {

NJetCloseLeptonSelector::NJetCloseLeptonSelector(const std::string& params) :
            SignValueSelector("NJETCLOSELEPTON_N", params) {
    checkValueIsInteger();
}

bool NJetCloseLeptonSelector::apply(const top::Event& event) const {
    //Get the lepton - use ptr to avoid copy
    const xAOD::IParticle* lep(nullptr);
    if (event.m_electrons.size() == 1)
        lep = event.m_electrons.front();
    else if (event.m_muons.size() == 1)
        lep = event.m_muons.front();
    else
        return false;

    //Get the highest pT narrow jet "near" the lepton
    const xAOD::Jet* selJet(nullptr);
    double highestpt(0.);
    int nJets(0);
    for (const auto* const jet : event.m_jets) {
        if (top::deltaR(*lep, *jet) < 1.5 && jet->pt() > highestpt) {
            nJets++;
            selJet = jet;
            highestpt = jet->pt();
        }
    }

    if (selJet)
        selJet->auxdecor<char>("closeToLepton") = 1;

    return checkInt(nJets, (int) value());
}

}
