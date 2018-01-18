// $Id: OverlapRemovalIndicesTtres.cxx 639409 2015-01-14 12:56:37Z simon $
#include "HQTTtResonancesTools/OverlapRemovalIndicesTtres.h"
#include "TopEvent/EventTools.h"

#include <list>

namespace top {

  OverlapRemovalIndicesTtres::OverlapRemovalIndicesTtres() {
  }

void OverlapRemovalIndicesTtres::overlapremoval(const xAOD::PhotonContainer*   photon ,
                                		const xAOD::ElectronContainer* el  ,
                                		const xAOD::MuonContainer*     mu  ,
                                		const xAOD::TauJetContainer*   tau ,
		                                const xAOD::JetContainer*      jet ,
                		                const xAOD::JetContainer*      ljet,
						std::vector<unsigned int>& goodPhotons,
                		                std::vector<unsigned int>& goodElectrons,
                		                std::vector<unsigned int>& goodMuons,
                		                std::vector<unsigned int>& goodTaus,
                		                std::vector<unsigned int>& goodJets,
                		                std::vector<unsigned int>& goodLargeRJets,
                		                const bool isLooseEvent )                                      
  {
      // Work internally with std::list
      // What passed the pre-overlap removal selection?
      std::vector<unsigned int> IN_el,IN_mu,IN_jet,IN_ljet,IN_tau,IN_photon;
      std::list<unsigned int> l_el,l_mu,l_jet,l_ljet,l_tau,l_photon;
      unsigned int index_el(0),index_mu(0),index_jet(0),index_ljet(0),index_tau(0),index_photon(0);
     
      std::string passTopCuts("");
      if (isLooseEvent) {
        passTopCuts = "passPreORSelectionLoose";
      }
      if (!isLooseEvent) {
        passTopCuts = "passPreORSelection";
      }     

      if (el) {
          for( auto x : *el ){
              if( x->auxdataConst< char >(passTopCuts) == 1 ){
                  IN_el.push_back(index_el);
                  l_el.push_back(index_el);
              }
              ++index_el;
          }
      }

      if (mu) {
          for( auto x : *mu ){
              if( x->auxdataConst< char >(passTopCuts) == 1 ){
                  IN_mu.push_back(index_mu);
                  l_mu.push_back(index_mu);
              }
              ++index_mu;
          }
      }

      if (jet) {
          for( auto x : *jet ){
              if( x->auxdataConst< char >(passTopCuts) == 1 ){
                  IN_jet.push_back(index_jet);
                  l_jet.push_back(index_jet);
              }
              ++index_jet;
          }
      }

      if (ljet) {
          for( auto x : *ljet ){
              if( x->auxdataConst< char >(passTopCuts) == 1 ){
                  IN_ljet.push_back(index_ljet);
                  l_ljet.push_back(index_ljet);
              }
              ++index_ljet;
          }
      }

      if (tau) {
	for( auto x : *tau ){
	  if( x->auxdataConst< char >(passTopCuts) == 1 ){
	    IN_tau.push_back(index_tau);
	    l_tau.push_back(index_tau);
	  }
	  ++index_tau;
	}
      }

      if (photon) {
	for( auto x : *photon ){
	  if( x->auxdataConst< char >(passTopCuts) == 1 ){
	    IN_photon.push_back(index_photon);
	    l_photon.push_back(index_photon);
	  }
	  ++index_photon;
	}
      }

      // Jets and Muons - remove muon with dR < 0.4
      for( auto j : IN_jet ){
          for( auto m : IN_mu ){
              if( top::deltaR( *(jet->at(j) ) , *(mu->at(m)) ) < 0.04 + 10e3/((mu->at(m))->pt()) ){
                  l_mu.remove( m );
              }
          }
      }

      // Save what's left of the std::lists into the OUT vectors;
	
      goodElectrons.clear();
      goodMuons.clear();
      goodJets.clear();
      goodLargeRJets.clear();
      goodTaus.clear();
      goodPhotons.clear();      

      for( auto i : l_el  )  {goodElectrons.push_back(i); }
      for( auto i : l_mu  )  {goodMuons.push_back(i); }
      for( auto i : l_jet )  {goodJets.push_back(i);}
      for( auto i : l_ljet)  {goodLargeRJets.push_back(i);}
      for( auto i : l_tau )  {goodTaus.push_back(i);}
      for( auto i : l_photon){goodPhotons.push_back(i);}
  }

  void OverlapRemovalIndicesTtres::print(std::ostream& os) const {
      os << "fill in\n";
  }
}
