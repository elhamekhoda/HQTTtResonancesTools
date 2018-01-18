#ifndef TTBARPARTONHISTORYSAVER_H_ 
#define TTBARPARTONHISTORYSAVER_H_ 

#include "TopAnalysis/EventSaverFlatNtuple.h"
#include "TLorentzVector.h"

namespace top {

class TtbarPartonHistorySaver : public top::EventSaverFlatNtuple {
public:
    
    //Default - so root can load based on a name        
    TtbarPartonHistorySaver();

    //Default - so we can clean up
    ~TtbarPartonHistorySaver();

    //Run once at the start of the job
    //using top::EventSaverBase::initialise;
    void initialize(std::shared_ptr<top::TopConfig> config, TFile* file, const std::vector<std::string>& extraBranches);
    virtual StatusCode initialize(){return StatusCode::SUCCESS;} 

    //Run over the particles in the truth container
    bool topWb(const top::Event& event, int start, TLorentzVector& t_p4, TLorentzVector& W_p4, TLorentzVector& b_p4, TLorentzVector& Wdecay1_p4, int& Wdecay1_pdgId, TLorentzVector& Wdecay2_p4, int& Wdecay2_pdgId) const;

    // SaveEvent	
    void saveEvent(const top::Event& event);

private:
    //Extra variable to write out
    
    float MC_ttbar_beforeFSR_m;
    float MC_ttbar_beforeFSR_pt;
    float MC_ttbar_beforeFSR_eta;
    float MC_ttbar_beforeFSR_phi;

    float MC_ttbar_afterFSR_m;
    float MC_ttbar_afterFSR_pt;
    float MC_ttbar_afterFSR_eta;
    float MC_ttbar_afterFSR_phi;

    float MC_t_beforeFSR_m;
    float MC_t_beforeFSR_pt;
    float MC_t_beforeFSR_eta;
    float MC_t_beforeFSR_phi;

    float MC_tbar_beforeFSR_m;
    float MC_tbar_beforeFSR_pt;
    float MC_tbar_beforeFSR_eta;
    float MC_tbar_beforeFSR_phi;

    float MC_W_from_t_m;
    float MC_W_from_t_pt;
    float MC_W_from_t_eta;
    float MC_W_from_t_phi;

    float MC_W_from_tbar_m;
    float MC_W_from_tbar_pt;
    float MC_W_from_tbar_eta;
    float MC_W_from_tbar_phi;
    
    float MC_b_from_t_m;
    float MC_b_from_t_pt;
    float MC_b_from_t_eta;
    float MC_b_from_t_phi;

    float MC_b_from_tbar_m;
    float MC_b_from_tbar_pt;
    float MC_b_from_tbar_eta;
    float MC_b_from_tbar_phi;

    float MC_Wdecay1_from_t_m;
    float MC_Wdecay1_from_t_pt;
    float MC_Wdecay1_from_t_eta;
    float MC_Wdecay1_from_t_phi;
    int MC_Wdecay1_from_t_pdgId;

    float MC_Wdecay1_from_tbar_m;
    float MC_Wdecay1_from_tbar_pt;
    float MC_Wdecay1_from_tbar_eta;
    float MC_Wdecay1_from_tbar_phi;
    int   MC_Wdecay1_from_tbar_pdgId;

    float MC_Wdecay2_from_t_m;
    float MC_Wdecay2_from_t_pt;
    float MC_Wdecay2_from_t_eta;
    float MC_Wdecay2_from_t_phi;
    int   MC_Wdecay2_from_t_pdgId;

    float MC_Wdecay2_from_tbar_m;
    float MC_Wdecay2_from_tbar_pt;
    float MC_Wdecay2_from_tbar_eta;
    float MC_Wdecay2_from_tbar_phi; 
    int MC_Wdecay2_from_tbar_pdgId;

    void IniVar();

    ClassDef(top::TtbarPartonHistorySaver, 0);
};

} // namespace top

#endif
