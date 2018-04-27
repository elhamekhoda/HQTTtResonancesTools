#include "HQTTtResonancesTools/AngularCutsSLSelector.h"

#include "TopEvent/EventTools.h"
#include "TopEvent/Event.h"

#include <vector>
#include <cmath>
/*

A Large R Jet Class which is well-seperated from all the leptonic-top decay products (lepton and jets)

*/

namespace top {

AngularCutsSLSelector::AngularCutsSLSelector(const std::string& params) :
        SignValueSelector("ANGULARCUTSSL", params) {
    checkValueIsInteger();
    m_AngularCutsSL = new AngularCutsSL("AngularCutsSL");
    check(m_AngularCutsSL->initialize(), "Initializing failed");
}

bool AngularCutsSLSelector::apply(const top::Event& event) const {
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
    m_AngularCutsSL->m_lep = lep;
    m_AngularCutsSL->m_selJet = selJet;
    //do stuff with large Jets
    int nGoodJets = 0;
    for (const auto* const largeJet : event.m_largeJets) {
        if ( m_AngularCutsSL->tag(*largeJet) && largeJet->auxdecor<int>("topTagged") == 1 ) {
        // if ( largeJet->auxdecor<int>("topTagged") == 1 ) {
            ++nGoodJets;
        }
        else {
            largeJet->auxdecor<int>("topTagged") = 0; 
            //largeJet->auxdecor<int>("topTaggedSub80") = 0;
            //largeJet->auxdecor<int>("topTaggedSub50") = 0;
            //largeJet->auxdecor<int>("topTaggedSmoothMT80") = 0;
            //largeJet->auxdecor<int>("topTaggedSmoothMT50") = 0;
            //largeJet->auxdecor<int>("topTaggedSmoothTS80") = 0;
            //largeJet->auxdecor<int>("topTaggedSmoothTS50") = 0;
            //largeJet->auxdecor<int>("topTaggedSmoothQT80") = 0;
            //largeJet->auxdecor<int>("topTaggedSmoothQT50") = 0;
            //largeJet->auxdecor<int>("topTaggedBDT80") = 0;
            //largeJet->auxdecor<int>("topTaggedDNN80") = 0;
            //largeJet->auxdecor<int>("topTaggedTopoCluster") = 0;
        }
    }

    return checkInt(nGoodJets, (int) value());
}


}