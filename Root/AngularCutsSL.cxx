#include "HQTTtResonancesTools/AngularCutsSL.h"

namespace top {

AngularCutsSL::AngularCutsSL(const std::string &name) :
JSSTaggerBase(name){
}

StatusCode AngularCutsSL::initialize(){
    ATH_MSG_INFO( "Initializing AngularCutsSL tool" );
    m_accept.addCut( "deltaPhiHigh"    , "True if deltaPhi(largeJet,lep) > 1.0."  );
    m_accept.addCut( "deltaRHigh"    , "True if the deltaR(largeJet,leading jet-close-to-lep) > 1.0."  );
    return StatusCode::SUCCESS;
}

Root::TAccept& AngularCutsSL::tag(const xAOD::Jet& largeJet) const {
    m_accept.clear();
    if ( std::fabs(top::deltaPhi(largeJet, *m_lep)) > 1.0 ) {   //changed to 1.0 from 2.3
        m_accept.setCutResult("deltaPhiHigh", true);
    }
    if ( top::deltaR(largeJet, *m_selJet) > 1.0 ) { //Changed to 1.0 from 1.5
        m_accept.setCutResult("deltaRHigh", true);
    }
    if (m_accept) {
         largeJet.auxdecor<int>("angular_cuts") = 1;
    }
    else {
        largeJet.auxdecor<int>("angular_cuts") = 0;
    }

    return m_accept;
}

StatusCode AngularCutsSL::finalize(){
  /* Delete or clear anything */
  return StatusCode::SUCCESS;
}


}