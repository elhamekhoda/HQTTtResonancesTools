#ifndef ELECTRONTTRESFAKES_H_
#define ELECTRONTTRESFAKES_H_

#include "TopObjectSelectionTools/ElectronLikelihoodMC15.h"
#include "TopObjectSelectionTools/ElectronCutBasedMC15.h"
#include "TopObjectSelectionTools/IsolationTools.h"
#include "TopObjectSelectionTools/ElectronSelectionBase.h"

#include <memory>

namespace top {

/**
 * @brief Electron selection based on the cut-based thingy.
 */

class ElectronTtresFakes : public top::ElectronLikelihoodMC15 {
public:
    /**
     * @brief Class to help select cut-based good electrons.
     *
     * @param ptcut The minimum pT cut to apply to the electrons.
     * @param vetoCrack Do you want to veto 1.37 < |cluster_eta| < 1.52?
     * @param quality The definition for good electrons, e.g. "Tight"
     * @param qualityLoose The loose defeinition, for fake-lepton estimates with
     * the matrix method?
     * @param isolation nullptr for un-isolated, or a new "isolation object" to
     * apply isolation cuts
     */
    //ElectronTtresFakes(bool isPrimaryxAOD, double ptcut, bool vetoCrack, const std::string& quality, const std::string& qualityLoose, IsolationBase* isolation);
    ElectronTtresFakes(double ptcut, bool vetoCrack, const std::string& quality, const std::string& qualityLoose, StandardIsolation* isolation);
    /**
     * @brief The cuts to select good electrons for your analysis should be
     * implemented in here.
     *
     * @param el The electron to cut on (all electrons in the event are passed
     * to the tool)
     * @return True if this is a good electron, false otherwise.
     */
    bool passSelection(const xAOD::Electron& el) const override;

    /**
     * @brief The loose selection needed by some background estimates.
     * Removing the z0 and d0sig cuts on the loose selection
     * @param el
     * @return
     */
    bool passSelectionLoose(const xAOD::Electron& el) const override;
    bool passSelectionNoIsolation(const xAOD::Electron& el,const std::string& operatingPoint) const;


};

}

#endif
