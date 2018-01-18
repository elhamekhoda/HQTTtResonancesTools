#ifndef MUONTTRESFAKES_H_
#define MUONTTRESFAKES_H_

#include "TopObjectSelectionTools/MuonSelectionBase.h"
#include "TopObjectSelectionTools/MuonMC15.h"
#include "TopObjectSelectionTools/IsolationTools.h"

#include "MuonSelectorTools/MuonSelectionTool.h"
#include <memory>

namespace top {

/**
 * @brief Select muons based on some early MC15 suggestions and extra tt resonances cuts.
 */
class MuonTtresFakes : public top::MuonMC15 {
public:
    /**
     * @brief Construct the tool to select good muons.
     *
     * @param ptcut The minimum pT cut for good muons.
     * @param etamax The minimum |eta| cut for good muons (applied by the MuonSelectorTool).
     * @param isolation The isolation the user wants to apply.  Don't want any
     * isolation to be applied?  Then leave this as a nullptr.
     */
    MuonTtresFakes(double ptcut, IsolationBase *isolation);

    /**
     * @brief Implements the logic to select good muons.
     *
     * @param mu The muon that we want to check.
     * @return True if the muon is good, false otherwise.
     */
    bool passSelection(const xAOD::Muon& mu) const override;
 
    /**
     * @brief The loose selection needed by some background estimates.
     
     * @param mu
     * @return
     */  
    bool passSelectionLoose(const xAOD::Muon& mu) const override;        
    /*
    * No cuts on d0sig and z0*sin(theta)
    */

};

}

#endif
