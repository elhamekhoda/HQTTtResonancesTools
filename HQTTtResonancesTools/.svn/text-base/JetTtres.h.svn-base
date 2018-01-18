#ifndef JETTTRES_H_
#define JETTTRES_H_

//#include "TopObjectSelectionTools/JetSelectionBase.h"
#include "TopObjectSelectionTools/JetMC15.h"

#include <string>

namespace top {

/**
 * @brief A class that performs object cuts on Jets (of any size!).  At the
 * moment it includes a cut on pT, eta and JVT.
 */

class JetTtres : public JetMC15 {
public:
    /**
     * @brief A tool to select some jets for MC15 based on the pT and eta cuts.
     *
     * @param ptcut The minimum pT to accept good jets.
     * @param etamax The maximum eta cut.
     * @param jvtmin The minimum cut on the jet vertex fraction
     */
    JetTtres(double ptcut, double etamax, double jvtmin, const std::string &ghostCollectionName);

    ///Does nothing.
    ~JetTtres();

    /**
     * @brief Implements the logic to select jets based on pT and eta.
     *
     * It cuts on pt, eta, jvt.
     *
     * @param jet The jet to check.
     * @return True if the jet passes the cuts, false otherwise.
     */
    virtual bool passSelection(const xAOD::Jet& jet) override final;


private:

    ///The name of the ghost collection
    std::string m_ghostCollectionName;

};

}

#endif
