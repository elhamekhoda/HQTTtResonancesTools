#include "HQTTtResonancesTools/AngularCutsSL.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include "xAODJet/JetAttributes.h"

#include <vector>
#include <cmath>

/*

A Large R Jet Class which passes the smooth pre-rec tagger requirements


*/

namespace top {

AngularCutsSL::AngularCutsSL(const std::string& params) :
        SignValueSelector("ANGULARCUTSSL", params) {
    checkValueIsInteger();

}

bool AngularCutsSL::apply(const top::Event& event) const {
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
        
	if (largeJet->auxdecor<char>("topTagged") != 1 ) continue;
       
        if ( std::fabs(top::deltaPhi(*largeJet, *lep)) > 2.3 &&
             top::deltaR(*largeJet, *selJet) > 1.5) {

            ++nGoodJets;
        }
	else  largeJet->auxdecor<char>("topTagged") = 0;

    }

    return checkInt(nGoodJets, (int) value());
}

}
