#include "HQTTtResonancesTools/JetTtres.h"
#include "TopConfiguration/TopConfig.h"
#include <vector>
#include <cmath>
#include <string>

namespace top {

JetTtres::JetTtres(const double ptcut, const double etamax, const bool doJVTCut, const std::string fwdJetSel, const double ghostjetPtmin, const std::string &ghostCollectionName)
  : JetMC15(ptcut, etamax, doJVTCut, fwdJetSel),
    m_ghostjetPtmin(ghostjetPtmin),
    m_ghostCollectionName(ghostCollectionName) {
}

JetTtres::~JetTtres() {}

bool JetTtres::passSelection(const xAOD::Jet& jet) {
    jet.auxdecor<char>("good") = 0;
    if( !JetMC15::passSelection(jet) ) {
       return false;
    }
    
    jet.auxdecor<char>("good") = 1;
    jet.auxdecor<char>("closeToLepton") = 0;

    // TODO
    // some of this only works for MC.

    int nGhostJets = 0;
    int nTrueBGhostJets = 0;
    int nTrueBBGhostJets = 0;
    int nTrueCGhostJets = 0;
    int nTrueCCGhostJets = 0;
    int nTrueLGhostJets = 0;

    // set nonsense values if no ghost collection.
    if (m_ghostCollectionName == "") {
        nGhostJets = -1;
        nTrueBGhostJets = -1;
        nTrueBBGhostJets = -1;
        nTrueCGhostJets = -1;
        nTrueCCGhostJets = -1;
        nTrueLGhostJets = -1;
    } else {
        try {
          const std::vector<ElementLink<xAOD::IParticleContainer> >& ghostJets =
              jet.getAttribute<std::vector<ElementLink<xAOD::IParticleContainer> > >(m_ghostCollectionName);

          for (const auto& link : ghostJets) {
            const xAOD::Jet *ghostjet = dynamic_cast<const xAOD::Jet*>(*link);

            // make sure the ghost jets will be taggable
            if (ghostjet->pt() < m_ghostjetPtmin)
                continue;

            nGhostJets++;

            // labeling preference: BB, B, CC, C, L
            int nBs = 0;
            if (ghostjet->isAvailable<int>("GhostBHadronsFinalCount"))
              nBs = ghostjet->getAttribute<int>("GhostBHadronsFinalCount");
            int nCs = 0;
            if (ghostjet->isAvailable<int>("GhostCHadronsFinalCount"))
              nCs = ghostjet->getAttribute<int>("GhostCHadronsFinalCount");
            if (nBs > 1)
                nTrueBBGhostJets++;
            else if (nBs == 1)
                nTrueBGhostJets++;
            else if (nCs > 1)
                nTrueCCGhostJets++;
            else if (nCs == 1)
                nTrueCGhostJets++;
            else
                nTrueLGhostJets++;
          }
        } catch (...) {
        }

    }

    jet.auxdecor<int>("nGhostJets") = nGhostJets;
    jet.auxdecor<int>("nTrueBGhostJets") = nTrueBGhostJets;
    jet.auxdecor<int>("nTrueBBGhostJets") = nTrueBBGhostJets;
    jet.auxdecor<int>("nTrueCGhostJets") = nTrueCGhostJets;
    jet.auxdecor<int>("nTrueCCGhostJets") = nTrueCCGhostJets;
    jet.auxdecor<int>("nTrueLGhostJets") = nTrueLGhostJets;


    return true;
}


}
