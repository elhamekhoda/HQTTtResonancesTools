#ifndef ANGULARCUTSSL_H_
#define ANGULARCUTSSL_H_

#include "AsgTools/AsgTool.h"
#include "PATCore/TAccept.h"
#include "TopEvent/EventTools.h"
#include "BoostedJetTaggers/JSSTaggerBase.h"

namespace top {

class AngularCutsSL : public JSSTaggerBase {
    ASG_TOOL_CLASS0(AngularCutsSL)
public:
    //Default - so root can load based on a name
    AngularCutsSL(const std::string& name);
    StatusCode initialize();
    StatusCode finalize();
    virtual Root::TAccept tag(const xAOD::Jet& largeJet) const;
    mutable const xAOD::IParticle* m_lep;
    mutable const xAOD::Jet* m_selJet;
};

}

#endif