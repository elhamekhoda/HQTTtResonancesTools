#include "HQTTtResonancesTools/TtbarPartonHistorySaver.h"

#include "TopEvent/Event.h"
#include "TopEventSelectionTools/TreeManager.h"

#include "xAODTruth/TruthParticle.h"

namespace top {

//Always initialise primitive types in the constructor
//________________________________________________________________________________________________________________
TtbarPartonHistorySaver::TtbarPartonHistorySaver() {IniVar();}

//________________________________________________________________________________________________________________
TtbarPartonHistorySaver::~TtbarPartonHistorySaver() {}
//________________________________________________________________________________________________________________
void TtbarPartonHistorySaver::initialize(std::shared_ptr<top::TopConfig> config, TFile* file, const std::vector<std::string>& extraBranches) {
//________________________________________________________________________________________________________________

    EventSaverFlatNtuple::initialize(config, file, extraBranches);

    for (auto systematicTree : treeManagers()){

	systematicTree->makeOutputVariable(MC_ttbar_beforeFSR_m,     	"MC_ttbar_beforeFSR_m");
	systematicTree->makeOutputVariable(MC_ttbar_beforeFSR_pt,     	"MC_ttbar_beforeFSR_pt");
	systematicTree->makeOutputVariable(MC_ttbar_beforeFSR_eta,     	"MC_ttbar_beforeFSR_eta");
	systematicTree->makeOutputVariable(MC_ttbar_beforeFSR_phi,     	"MC_ttbar_beforeFSR_phi");

	systematicTree->makeOutputVariable(MC_ttbar_afterFSR_m,     	"MC_ttbar_afterFSR_m");
	systematicTree->makeOutputVariable(MC_ttbar_afterFSR_pt,     	"MC_ttbar_afterFSR_pt");
	systematicTree->makeOutputVariable(MC_ttbar_afterFSR_eta,     	"MC_ttbar_afterFSR_eta");
	systematicTree->makeOutputVariable(MC_ttbar_afterFSR_phi,     	"MC_ttbar_afterFSR_phi");

	systematicTree->makeOutputVariable(MC_t_beforeFSR_m,     	"MC_t_beforeFSR_m");
	systematicTree->makeOutputVariable(MC_t_beforeFSR_pt,     	"MC_t_beforeFSR_pt");
	systematicTree->makeOutputVariable(MC_t_beforeFSR_eta,     	"MC_t_beforeFSR_eta");
	systematicTree->makeOutputVariable(MC_t_beforeFSR_phi,     	"MC_t_beforeFSR_phi");

	systematicTree->makeOutputVariable(MC_tbar_beforeFSR_m,     	"MC_tbar_beforeFSR_m");
	systematicTree->makeOutputVariable(MC_tbar_beforeFSR_pt,     	"MC_tbar_beforeFSR_pt");
	systematicTree->makeOutputVariable(MC_tbar_beforeFSR_eta,     	"MC_tbar_beforeFSR_eta");
	systematicTree->makeOutputVariable(MC_tbar_beforeFSR_phi,     	"MC_tbar_beforeFSR_phi");

	systematicTree->makeOutputVariable(MC_W_from_t_m,     		"MC_W_from_t_m");
	systematicTree->makeOutputVariable(MC_W_from_t_pt,     		"MC_W_from_t_pt");
	systematicTree->makeOutputVariable(MC_W_from_t_eta,     	"MC_W_from_t_eta");
	systematicTree->makeOutputVariable(MC_W_from_t_phi,     	"MC_W_from_t_phi");

	systematicTree->makeOutputVariable(MC_W_from_tbar_m,     	"MC_W_from_tbar_m");
	systematicTree->makeOutputVariable(MC_W_from_tbar_pt,     	"MC_W_from_tbar_pt");
	systematicTree->makeOutputVariable(MC_W_from_tbar_eta,     	"MC_W_from_tbar_eta");
	systematicTree->makeOutputVariable(MC_W_from_tbar_phi,     	"MC_W_from_tbar_phi");

	systematicTree->makeOutputVariable(MC_b_from_t_m,     		"MC_b_from_t_m");
	systematicTree->makeOutputVariable(MC_b_from_t_pt,     		"MC_b_from_t_pt");
	systematicTree->makeOutputVariable(MC_b_from_t_eta,     	"MC_b_from_t_eta");
	systematicTree->makeOutputVariable(MC_b_from_t_phi,     	"MC_b_from_t_phi");

	systematicTree->makeOutputVariable(MC_b_from_tbar_m,     	"MC_b_from_tbar_m");
	systematicTree->makeOutputVariable(MC_b_from_tbar_pt,     	"MC_b_from_tbar_pt");
	systematicTree->makeOutputVariable(MC_b_from_tbar_eta,     	"MC_b_from_tbar_eta");
	systematicTree->makeOutputVariable(MC_b_from_tbar_phi,     	"MC_b_from_tbar_phi");

	systematicTree->makeOutputVariable(MC_Wdecay1_from_t_m,     	"MC_Wdecay1_from_t_m");
	systematicTree->makeOutputVariable(MC_Wdecay1_from_t_pt,     	"MC_Wdecay1_from_t_pt");
	systematicTree->makeOutputVariable(MC_Wdecay1_from_t_eta,     	"MC_Wdecay1_from_t_eta");
	systematicTree->makeOutputVariable(MC_Wdecay1_from_t_phi,     	"MC_Wdecay1_from_t_phi");
	systematicTree->makeOutputVariable(MC_Wdecay1_from_t_pdgId,    	"MC_Wdecay1_from_t_pdgId");

	systematicTree->makeOutputVariable(MC_Wdecay1_from_tbar_m,     	"MC_Wdecay1_from_tbar_m");
	systematicTree->makeOutputVariable(MC_Wdecay1_from_tbar_pt,    	"MC_Wdecay1_from_tbar_pt");
	systematicTree->makeOutputVariable(MC_Wdecay1_from_tbar_eta,   	"MC_Wdecay1_from_tbar_eta");
	systematicTree->makeOutputVariable(MC_Wdecay1_from_tbar_phi,   	"MC_Wdecay1_from_tbar_phi");
	systematicTree->makeOutputVariable(MC_Wdecay1_from_tbar_pdgId, 	"MC_Wdecay1_from_tbar_pdgId");

	systematicTree->makeOutputVariable(MC_Wdecay2_from_t_m,     	"MC_Wdecay2_from_t_m");
	systematicTree->makeOutputVariable(MC_Wdecay2_from_t_pt,     	"MC_Wdecay2_from_t_pt");
	systematicTree->makeOutputVariable(MC_Wdecay2_from_t_eta,     	"MC_Wdecay2_from_t_eta");
	systematicTree->makeOutputVariable(MC_Wdecay2_from_t_phi,     	"MC_Wdecay2_from_t_phi");
	systematicTree->makeOutputVariable(MC_Wdecay2_from_t_pdgId,    	"MC_Wdecay2_from_t_pdgId");

	systematicTree->makeOutputVariable(MC_Wdecay2_from_tbar_m,     	"MC_Wdecay2_from_tbar_m");
	systematicTree->makeOutputVariable(MC_Wdecay2_from_tbar_pt,    	"MC_Wdecay2_from_tbar_pt");
	systematicTree->makeOutputVariable(MC_Wdecay2_from_tbar_eta,   	"MC_Wdecay2_from_tbar_eta");
	systematicTree->makeOutputVariable(MC_Wdecay2_from_tbar_phi,   	"MC_Wdecay2_from_tbar_phi");
	systematicTree->makeOutputVariable(MC_Wdecay2_from_tbar_pdgId, 	"MC_Wdecay2_from_tbar_pdgId");

  }
}

//Initialize variables
void TtbarPartonHistorySaver::IniVar(){

	MC_ttbar_beforeFSR_m = -1; 
	MC_ttbar_beforeFSR_pt = -1; 
	MC_ttbar_beforeFSR_eta = -1000; 
	MC_ttbar_beforeFSR_phi = -1000; 

	MC_ttbar_afterFSR_m = -1; 
	MC_ttbar_afterFSR_pt = -1; 
	MC_ttbar_afterFSR_eta = -1000; 
	MC_ttbar_afterFSR_phi = -1000; 

	MC_t_beforeFSR_m = -1; 
	MC_t_beforeFSR_pt = -1; 
	MC_t_beforeFSR_eta = -1000;
	MC_t_beforeFSR_phi = -1000; 

	MC_tbar_beforeFSR_m = -1; 
	MC_tbar_beforeFSR_pt = -1;
	MC_tbar_beforeFSR_eta = -1000;
	MC_tbar_beforeFSR_phi = -1000; 

	MC_W_from_t_m = -1; 
	MC_W_from_t_pt = -1; 
	MC_W_from_t_eta = -1000; 
	MC_W_from_t_phi = -1000; 

	MC_W_from_tbar_m = -1; 
	MC_W_from_tbar_pt = -1; 
	MC_W_from_tbar_eta = -1000; 
	MC_W_from_tbar_phi = -1000; 

	MC_b_from_t_m = -1; 
	MC_b_from_t_pt = -1; 
	MC_b_from_t_eta = -1000; 
	MC_b_from_t_phi = -1000; 

	MC_b_from_tbar_m = -1;
	MC_b_from_tbar_pt = -1; 
	MC_b_from_tbar_eta = -1000; 
	MC_b_from_tbar_phi = -1000; 

	MC_Wdecay1_from_t_m = -1;
	MC_Wdecay1_from_t_pt = -1;
	MC_Wdecay1_from_t_eta = -1000; 
	MC_Wdecay1_from_t_phi = -1000; 
	MC_Wdecay1_from_t_pdgId = 0; 

	MC_Wdecay1_from_tbar_m = -1;
	MC_Wdecay1_from_tbar_pt = -1; 
	MC_Wdecay1_from_tbar_eta = -1000;
	MC_Wdecay1_from_tbar_phi = -1000;
	MC_Wdecay1_from_tbar_pdgId = 0; 

	MC_Wdecay2_from_t_m = -1; 
	MC_Wdecay2_from_t_pt = -1; 
	MC_Wdecay2_from_t_eta = -1000; 
	MC_Wdecay2_from_t_phi = -1000; 
	MC_Wdecay2_from_t_pdgId = 0; 

	MC_Wdecay2_from_tbar_m = -1; 
	MC_Wdecay2_from_tbar_pt = -1; 
	MC_Wdecay2_from_tbar_eta = -1000;
	MC_Wdecay2_from_tbar_phi = -1000; 
	MC_Wdecay2_from_tbar_pdgId = 0;

}

//Internal Functions
const xAOD::TruthParticle* findAfterFSR(const xAOD::TruthParticle* particle, bool verbose) {
	bool isAfterFSR(false);
        const int particle_ID = particle->pdgId();
	int forLoop  = 0;
	while(!isAfterFSR){
		forLoop  = 0;
		for (size_t j=0; j< particle->nChildren(); j++ ) {
			const xAOD::TruthParticle* tmp_children = particle->child(j);			
			if (tmp_children->pdgId()==particle_ID){
				if (verbose)	std::cout << "\t Searching last particle after FSR: " << *tmp_children << "\n" << std::endl;
				particle = particle->child(j);
				forLoop++;
				break;
			}//if
		}//for
		if (forLoop == 0){
			isAfterFSR = true;
			if (verbose)	std::cout << "\t Particle after FSR: " << *particle << "\n" << std::endl;
		}//if
	}//while
        return particle;
}

bool findTopBeforeFSR(const xAOD::TruthParticle* particle, bool verbose) {
	bool skipit(false);			
	for (size_t i=0; i< particle->nParents(); i++ ) {
		const xAOD::TruthParticle* parent = particle->parent(i);
		if (parent->isTop()){
			if (verbose)	std::cout << "skip - parent is a top: " << *particle << "\n" << std::endl;
			skipit=true; break;
		}
	}
	return skipit;
}



//________________________________________________________________________________________________________________
bool TtbarPartonHistorySaver::topWb(const top::Event& event, int start, TLorentzVector& t_p4, TLorentzVector& W_p4, TLorentzVector& b_p4, TLorentzVector& Wdecay1_p4, int& Wdecay1_pdgId, TLorentzVector& Wdecay2_p4, int& Wdecay2_pdgId) const {
//________________________________________________________________________________________________________________ 
	// if verbose
	bool verbose(false);
		
	bool hasT 		= false;
	bool hasW		= false;
	bool hasB		= false;
	bool hasWdecayProd1	= false;
	bool hasWdecayProd2	= false;
	bool hasWdecay		= false;

	bool good_top_event 	= false;

	for (const xAOD::TruthParticle* particle : *event.m_truth) {
		if (particle->pdgId() == start) {
			
			// kepping only top after FSR
			if (findTopBeforeFSR(particle, verbose)) continue;
			
			// top before FSR
			t_p4 = particle->p4();
			hasT = true;		
			
			// demanding the last tops after FSR
			particle = top::findAfterFSR(particle, verbose);
			
			for (size_t k=0; k < particle->nChildren(); k++) {
				const xAOD::TruthParticle* topChildren = particle->child(k);					

				if (fabs(topChildren->pdgId()) == 24){	
					// demanding the last W after FSR 
					topChildren = top::findAfterFSR(topChildren, verbose);
					
					// W boson after FSR
					W_p4 = topChildren->p4(); 
					hasW = true;

					for (size_t q = 0; q < topChildren->nChildren(); ++q) {
						const xAOD::TruthParticle* WChildren = topChildren->child(q);
						if (fabs(WChildren->pdgId())<17){
							if (WChildren->pdgId()>0){
								Wdecay1_p4 = WChildren->p4();
								Wdecay1_pdgId = WChildren->pdgId();
								hasWdecayProd1 = true;
							}else{
								Wdecay2_p4 = WChildren->p4();
								Wdecay2_pdgId = WChildren->pdgId();
								hasWdecayProd2 = true;
						 	}//else
						}//if						
					}//for
					if (hasWdecayProd1 && hasWdecayProd2)	hasWdecay = true;

				} else if (fabs(topChildren->pdgId()) == 5) {					
					b_p4 = topChildren->p4();
					hasB = true;
				} //else if		
	
			} //for (size_t k=0; k < particle->nChildren(); k++)
		} //if(particle->pdgId() == start)
	} //for(const xAOD::TruthParticle* particle : *event.m_truth)

	if (hasT && hasW && hasB && hasWdecay)	good_top_event = true;

	return good_top_event;	
}

//________________________________________________________________________________________________________________ 
void TtbarPartonHistorySaver::saveEvent(const top::Event& event) {
//________________________________________________________________________________________________________________ 
	
	// Initialize variables
	IniVar();
	
	TLorentzVector t;
	TLorentzVector Wp;
	TLorentzVector b;
	TLorentzVector WpDecay1;	
	TLorentzVector WpDecay2;
	int WpDecay1_pdgId;
	int WpDecay2_pdgId;
	bool event_top = topWb(event, 6, t, Wp, b, WpDecay1, WpDecay1_pdgId, WpDecay2, WpDecay2_pdgId);
	
	TLorentzVector tbar;
	TLorentzVector Wm;
	TLorentzVector bbar;
	TLorentzVector WmDecay1;	
	TLorentzVector WmDecay2;
	int WmDecay1_pdgId;
	int WmDecay2_pdgId;
	bool event_topbar = topWb(event, -6, tbar, Wm, bbar, WmDecay1, WmDecay1_pdgId, WmDecay2, WmDecay2_pdgId);

	if (event_top && event_topbar){
		TLorentzVector temp = t+tbar;
		MC_ttbar_beforeFSR_m	= temp.M();
		MC_ttbar_beforeFSR_pt	= temp.Pt();
		MC_ttbar_beforeFSR_eta	= temp.Eta();
		MC_ttbar_beforeFSR_phi	= temp.Phi();
		
		temp = WmDecay1+WmDecay2+b + WpDecay1+WpDecay2+bbar;
		MC_ttbar_afterFSR_m	= temp.M();
		MC_ttbar_afterFSR_pt	= temp.Pt();
		MC_ttbar_afterFSR_eta	= temp .Eta();
		MC_ttbar_afterFSR_phi	= temp .Phi();
	}	
	if (event_top){
		MC_t_beforeFSR_m	= t.M();
		MC_t_beforeFSR_pt	= t.Pt();
		MC_t_beforeFSR_eta	= t.Eta();
		MC_t_beforeFSR_phi	= t.Phi();

		MC_W_from_t_m		= Wp.M();
		MC_W_from_t_pt		= Wp.Pt();
 		MC_W_from_t_eta		= Wp.Eta();
		MC_W_from_t_phi		= Wp.Phi();
    
		MC_b_from_t_m		= b.M();
		MC_b_from_t_pt		= b.Pt();
		MC_b_from_t_eta		= b.Eta();
		MC_b_from_t_phi		= b.Phi();

		MC_Wdecay1_from_t_m	= WpDecay1.M();
		MC_Wdecay1_from_t_pt	= WpDecay1.Pt();
		MC_Wdecay1_from_t_eta	= WpDecay1.Eta();
		MC_Wdecay1_from_t_phi	= WpDecay1.Phi();
		MC_Wdecay1_from_t_pdgId	= WpDecay1_pdgId;

		MC_Wdecay2_from_t_m	= WpDecay2.M();
		MC_Wdecay2_from_t_pt	= WpDecay2.Pt();
		MC_Wdecay2_from_t_eta	= WpDecay2.Eta();
		MC_Wdecay2_from_t_phi	= WpDecay2.Phi();
		MC_Wdecay2_from_t_pdgId	= WpDecay2_pdgId;
	}	
	if (event_topbar){

		MC_tbar_beforeFSR_m	= tbar.M();
		MC_tbar_beforeFSR_pt	= tbar.Pt();
		MC_tbar_beforeFSR_eta	= tbar.Eta();
		MC_tbar_beforeFSR_phi	= tbar.Phi();

		MC_W_from_tbar_m	= Wm.M();
		MC_W_from_tbar_pt	= Wm.Pt();
		MC_W_from_tbar_eta	= Wm.Eta();
		MC_W_from_tbar_phi	= Wm.Phi();

		MC_b_from_tbar_m	= bbar.M();
		MC_b_from_tbar_pt	= bbar.Pt();
		MC_b_from_tbar_eta	= bbar.Eta();
		MC_b_from_tbar_phi	= bbar.Phi();

		MC_Wdecay1_from_tbar_m		= WmDecay1.M();
		MC_Wdecay1_from_tbar_pt		= WmDecay1.Pt();
		MC_Wdecay1_from_tbar_eta	= WmDecay1.Eta();
		MC_Wdecay1_from_tbar_phi	= WmDecay1.Phi();
		MC_Wdecay1_from_tbar_pdgId	= WmDecay1_pdgId;

		MC_Wdecay2_from_tbar_m		= WmDecay2.M();
		MC_Wdecay2_from_tbar_pt		= WmDecay2.Pt();
		MC_Wdecay2_from_tbar_eta	= WmDecay2.Eta();
		MC_Wdecay2_from_tbar_phi	= WmDecay2.Phi(); 
		MC_Wdecay2_from_tbar_pdgId	= WmDecay2_pdgId;				
	}
	//Saving event
	EventSaverFlatNtuple::saveEvent(event);	
	
}

} // namespace top
