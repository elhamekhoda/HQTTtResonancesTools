#include "HQTTtResonancesTools/WFilter.h"

#include "xAODTruth/TruthEventContainer.h"

//#include <vector>

// declared in WFilter.h
// defined here:
double Wglobal_cPtMin      = 0.0;     // cQuark
double Wglobal_cEtaMax     = 5.0;     // cQuark
double Wglobal_charmPtMin  = 4000.0;  //cMeson/Baryon
double Wglobal_charmEtaMax = 3.0;     //cMeson/Baryon // 4 Sherpa1, 3 Sherpa2.2

double Wglobal_bPtMin       = 0.0;      // bQuark
double Wglobal_bEtaMax      = 5.0;      // bQuark
double Wglobal_bottomPtMin  = 0.0;  //bMeson/Baryon
double Wglobal_bottomEtaMax = 4.0;     //bMeson/Baryon

double Wglobal_DeltaR_Jet_part= 0.5;



void Wfind_cquark(bool do_truthMatch, std::vector<TLorentzVector> truth_jets, int pdgId,TLorentzVector part_vec, bool &passC, bool& is_C, bool& is_antiC){

  is_C = false;
  is_antiC = false;

  // check for C
  if (pdgId==4 && part_vec.Pt()>Wglobal_cPtMin && fabs(part_vec.Eta())<Wglobal_cEtaMax) {
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
    	  double dR = (truth_jets.at(i)).DeltaR(part_vec);
    	  if (dR<Wglobal_DeltaR_Jet_part)  {
    		  passC=true;
    		  is_C=true;
    	  };
      };
    } else    {
    	passC=true;
    	is_C=true;
    };
  };

  // check for anti-C
  if (pdgId==-4 && part_vec.Pt()>Wglobal_cPtMin && fabs(part_vec.Eta())<Wglobal_cEtaMax) {
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
    	  double dR = (truth_jets.at(i)).DeltaR(part_vec);
    	  if (dR<Wglobal_DeltaR_Jet_part)  {
    		  passC=true;
    		  is_antiC=true;
    	  };
      };
    } else    {
    	passC=true;
    	is_antiC=true;
    };
  };
} // find_cquark

void Wfind_bquark(bool do_truthMatch, std::vector<TLorentzVector> truth_jets, int part_pdgId,TLorentzVector part_vec,  bool &passB, bool &is_B, bool &is_antiB){
  bool debug = false;

  is_B= false;
  is_antiB = false;

  if (part_pdgId==5 && part_vec.Pt()>Wglobal_bPtMin && fabs(part_vec.Eta())<Wglobal_bEtaMax) {
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
	double dR = (truth_jets.at(i)).DeltaR(part_vec);
	if (dR<Wglobal_DeltaR_Jet_part) {
	  passB = true;
	  is_B  = true;
	};
      }
    } else    {
    	passB = true;
    	is_B  = true;
    }
    if (debug) std::cout<<" found B-QUARK"<<std::endl;
  };

  if (part_pdgId==-5 && part_vec.Pt()>Wglobal_bPtMin && fabs(part_vec.Eta())<Wglobal_bEtaMax) {
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
	double dR = (truth_jets.at(i)).DeltaR(part_vec);
	if (dR<Wglobal_DeltaR_Jet_part) {
	  passB = true;
	  is_antiB  = true;
	};
      }
    } else    {
    	passB = true;
    	is_antiB  = true;
    }
    if (debug) std::cout<<" found anti-B-QUARK"<<std::endl;
  };
} // find_bquark



void Wfind_chadron(bool do_truthMatch, std::vector<TLorentzVector> truth_jets,int pdgId,TLorentzVector part_vec, bool &passC, bool& is_C, bool& is_antiC){

  bool debug= false;
  is_C = false;
  is_antiC = false;

  //if (abs(ch_pdgId) == 4334) std::cout<<" OLD Omega,4334 found"<<std::endl;
  //if (abs(ch_pdgId) == 4332) std::cout<<" correct Omega C-filter,4332 found"<<std::endl;

  if (WisDwithWeakDK(fabs(pdgId)) && part_vec.Pt() > Wglobal_charmPtMin && fabs(part_vec.Eta())<Wglobal_charmEtaMax){
    if (do_truthMatch){
      int counter_DeltaR_pass =0;
      for (unsigned int i=0; i<truth_jets.size(); i++) {
    	  double dR = (truth_jets.at(i)).DeltaR(part_vec);
    	  if (dR<Wglobal_DeltaR_Jet_part) {
    		  counter_DeltaR_pass++;
    		  passC = true;
    		  if (debug) {
    			  std::cout<<"  >>> ("<<counter_DeltaR_pass<<") Information about found C-Hadron:"<<std::endl;
    			  std::cout<<"  >>> pdgID: "<<pdgId<<", pt: "<<part_vec.Pt()<<", |Eta| : "<<fabs(part_vec.Eta())<<std::endl;
    			  std::cout<<" DeltaR to truth jet: "<<dR<<"  (not per definition minimum) "<<std::endl;
    			  std::cout<<" -------------------"<<std::endl;
    		  };
    		  if (abs(pdgId) == 4334 && debug) std::cout<<" OLD Omega,4334 made it !"<<std::endl;
    		  if (abs(pdgId) == 4332 && debug) std::cout<<" correct Omega C-filter,4332 made it!"<<std::endl;
    	  };
      };
    } else    passC=true;
    if (do_truthMatch  && debug ) {
    	std::cout<<" truth_jet requirement prevented finding this particle"<<std::endl;
    	if (passC)  std::cout<<" ... passC (still) set to TRUTH ("<<pdgId <<")"<<std::endl;
    	if (!passC) std::cout<<" ... passC set to FALSE ("<<pdgId <<")"<<std::endl;
    };
  };

  // check for Charm
  if (WisDwithWeakDK(pdgId) && part_vec.Pt() > Wglobal_charmPtMin && fabs(part_vec.Eta())<Wglobal_charmEtaMax) {
    bool check_truthJet=false;
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
	double dR_c = (truth_jets.at(i)).DeltaR(part_vec);
	if (dR_c< Wglobal_DeltaR_Jet_part ) check_truthJet=true;
      };
    };
    if ((do_truthMatch && check_truthJet) || !do_truthMatch)   is_C = true;
  };
  // check for Anti-Charm
  if (WisDwithWeakDK((-1)*pdgId) && part_vec.Pt() > Wglobal_charmPtMin && fabs(part_vec.Eta())<Wglobal_charmEtaMax) {
    bool check_truthJet=false;
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
	double dR_c = (truth_jets.at(i)).DeltaR(part_vec);
	if (dR_c< Wglobal_DeltaR_Jet_part ) check_truthJet=true;
      };
    };
    if ((do_truthMatch && check_truthJet) || !do_truthMatch)   is_antiC = true;
  };
}// find_chadron

void Wfind_bhadron(bool do_truthMatch, std::vector<TLorentzVector> truth_jets,int pdgId,TLorentzVector part_vec, bool &passB, bool& is_B, bool& is_antiB){

  bool debug= false;
  //reset values:
  is_B= false;
  is_antiB = false;

  if (WisBwithWeakDK(fabs(pdgId)) && part_vec.Pt() > Wglobal_bottomPtMin && fabs(part_vec.Eta())<Wglobal_bottomEtaMax){
    bool info_this_part=false;
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
	double dR = (truth_jets.at(i)).DeltaR(part_vec);
	if (dR<Wglobal_DeltaR_Jet_part) {
	  passB = true;
	  info_this_part = true;
	};
      }
    } else    passB=true;
    if (do_truthMatch  && !info_this_part && debug ) {
      if (debug) std::cout<<" truth_jet requirement prevented finding this particle"<<std::endl;
      if (passB)  std::cout<<" ... passB (still) set to TRUTH ("<<pdgId <<")"<<std::endl;
      if (!passB) std::cout<<" ... passB set to FALSE ("<<pdgId <<")"<<std::endl;
    };
  };

  // check for Bottom
  if (WisBwithWeakDK(pdgId) && part_vec.Pt() > Wglobal_bottomPtMin && fabs(part_vec.Eta())<Wglobal_bottomEtaMax) {
    bool check_truthJet=false;
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
    	  double dR_b = (truth_jets.at(i)).DeltaR(part_vec);
    	  if (dR_b<Wglobal_DeltaR_Jet_part) check_truthJet=true;
      };
    };
    if ((do_truthMatch && check_truthJet) || !do_truthMatch)   is_B = true;
  };


  // check for Anti-Bottom
  if (WisBwithWeakDK((-1)*pdgId) && part_vec.Pt()> Wglobal_bottomPtMin && fabs(part_vec.Eta())<Wglobal_bottomEtaMax) {
    bool check_truthJet=false;
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
	double dR_anti = (truth_jets.at(i)).DeltaR(part_vec);
	if (dR_anti<Wglobal_DeltaR_Jet_part) check_truthJet=true;
      };
    };
    if ((do_truthMatch && check_truthJet) || !do_truthMatch) is_antiB = true;
  };

}// find_bhadron




void Wfind_chadron_ext(bool do_truthMatch, std::vector<TLorentzVector> truth_jets,int pdgId,TLorentzVector part_vec, bool &passC, bool& is_C, bool& is_antiC){

  bool debug= false;
  //reset values:
  is_C = false;
  is_antiC = false;
  //if (abs(pdgId) == 4334) std::cout<<" OLD Omega,4334 found"<<std::endl;
  //if (abs(ch_pdgId) == 4332) std::cout<<" correct Omega C-filter,4332 found"<<std::endl;

  if (WisDwithWeakDK_ext(fabs(pdgId)) && part_vec.Pt() > Wglobal_charmPtMin && fabs(part_vec.Eta())<Wglobal_charmEtaMax){
    if (do_truthMatch){
      int counter_DeltaR_pass =0;
      for (unsigned int i=0; i<truth_jets.size(); i++) {
    	  double dR = (truth_jets.at(i)).DeltaR(part_vec);
    	  if (dR<Wglobal_DeltaR_Jet_part) {
    		  counter_DeltaR_pass++;
    		  passC = true;
    		  if (debug) {
    			  std::cout<<"  >>> ("<<counter_DeltaR_pass<<") Information about found C-Hadron:"<<std::endl;
    			  std::cout<<"  >>> pdgID: "<<pdgId<<", pt: "<<part_vec.Pt()<<", |Eta| : "<<fabs(part_vec.Eta())<<std::endl;
    			  std::cout<<" DeltaR to truth jet: "<<dR<<"  (not per definition minimum) "<<std::endl;
    			  std::cout<<" -------------------"<<std::endl;
    		  };
    		  if (abs(pdgId) == 4334 && debug) std::cout<<" OLD Omega,4334 made it !"<<std::endl;
    		  if (abs(pdgId) == 4332 && debug) std::cout<<" correct Omega C-filter,4332 made it!"<<std::endl;
    	  };
      };
    } else    passC=true;
    if (do_truthMatch  && debug ) {
    	std::cout<<" truth_jet requirement prevented finding this particle"<<std::endl;
    	if (passC)  std::cout<<" ... passC (still) set to TRUTH ("<<pdgId <<")"<<std::endl;
    	if (!passC) std::cout<<" ... passC set to FALSE ("<<pdgId <<")"<<std::endl;
    };
  };

  // check for Charm
  if (WisDwithWeakDK_ext(pdgId) && part_vec.Pt() > Wglobal_charmPtMin && fabs(part_vec.Eta())<Wglobal_charmEtaMax) {
    bool check_truthJet=false;
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
	double dR_c = (truth_jets.at(i)).DeltaR(part_vec);
	if (dR_c< Wglobal_DeltaR_Jet_part ) check_truthJet=true;
      };
    };
    if ((do_truthMatch && check_truthJet) || !do_truthMatch)   is_C = true;
  };
  // check for Anti-Charm
  if (WisDwithWeakDK_ext((-1)*pdgId) && part_vec.Pt() > Wglobal_charmPtMin && fabs(part_vec.Eta())<Wglobal_charmEtaMax) {
    bool check_truthJet=false;
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
	double dR_c = (truth_jets.at(i)).DeltaR(part_vec);
	if (dR_c< Wglobal_DeltaR_Jet_part ) check_truthJet=true;
      };
    };
    if ((do_truthMatch && check_truthJet) || !do_truthMatch)   is_antiC = true;
  };
}// find_chadron_ext


void Wfind_bhadron_ext(bool do_truthMatch, std::vector<TLorentzVector> truth_jets,int pdgId,TLorentzVector part_vec, bool &passB, bool& is_B, bool& is_antiB){

  bool debug= false;
  //reset values:
  is_B= false;
  is_antiB = false;

  if (WisBwithWeakDK_ext(fabs(pdgId)) && part_vec.Pt() > Wglobal_bottomPtMin && fabs(part_vec.Eta())<Wglobal_bottomEtaMax){
    bool info_this_part=false;
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
	double dR = (truth_jets.at(i)).DeltaR(part_vec);
	if (dR<Wglobal_DeltaR_Jet_part) {
	  passB = true;
	  info_this_part = true;
	};
      }
    } else    passB=true;
    if (do_truthMatch  && !info_this_part && debug ) {
      if (debug) std::cout<<" truth_jet requirement prevented finding this particle"<<std::endl;
      if (passB)  std::cout<<" ... passB (still) set to TRUTH ("<<pdgId <<")"<<std::endl;
      if (!passB) std::cout<<" ... passB set to FALSE ("<<pdgId <<")"<<std::endl;
    };
  };

  // check for Bottom
  if (WisBwithWeakDK_ext(pdgId) && part_vec.Pt()> Wglobal_bottomPtMin && fabs(part_vec.Eta())<Wglobal_bottomEtaMax) {
    bool check_truthJet=false;
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
    	  double dR_b = (truth_jets.at(i)).DeltaR(part_vec);
    	  if (dR_b<Wglobal_DeltaR_Jet_part) check_truthJet=true;
      };
    };
    if ((do_truthMatch && check_truthJet) || !do_truthMatch) is_B = true;
  };

  // check for Anti-Bottom
  if (WisBwithWeakDK_ext((-1)*pdgId) && part_vec.Pt() > Wglobal_bottomPtMin && fabs(part_vec.Eta())<Wglobal_bottomEtaMax) {
    bool check_truthJet=false;
    if (do_truthMatch){
      for (unsigned int i=0; i<truth_jets.size(); i++) {
	double dR_antib = (truth_jets.at(i)).DeltaR(part_vec);
	if (dR_antib<Wglobal_DeltaR_Jet_part) check_truthJet=true;
      };
    };
    if ((do_truthMatch && check_truthJet) || !do_truthMatch)   is_antiB = true;
  };
}// find_bhadron_ext



void Wset_Cfilter(bool passC, int N_C, int N_antiC, bool& is_Conly, bool& is_cc){
  if (passC) {
    if (N_C>0 && N_antiC >0) is_cc=true;
    if ((N_C>0 && N_antiC==0 ) || (N_C==0 && N_antiC>0)) is_Conly= true;
  };
} //set Cfilter

void Wset_Bfilter(bool passB, int N_B, int N_antiB, bool& is_Bonly, bool& is_bb){
  if (passB) {
    if (N_B>0 && N_antiB >0) is_bb=true;
    if ((N_B>0 && N_antiB==0 ) || (N_B==0 && N_antiB>0)) is_Bonly= true;
  };
} //set Bfilter

bool WisDwithWeakDK(const int id) {
    return ( id == 411   || // D+
	     id == 421   || // D0
	     id == 431   || // Ds
	     id == 4122  || // Lambda_C
	     id == 4132  || // Xi_C^0
	     id == 4232  || // Xi_C^+
	     id == 4212  || // Xi_C^0 // Think, this is a bug and should be 4312 with Xi'C⁰ instead of 4212
	     id == 4322  || // Xi'_C+  This is in fact EM not weak
	     id == 4332); // Omega_C

}
bool WisDwithWeakDK_ext(const int id) {
    return ( id ==  411  || // D+
	     id ==  421  || // D0
	     id ==  431  || // Ds
	     id == 4122  || // Lambda_C
	     id == 4132  || // Xi_C^0
	     id == 4232  || // Xi_C^+
	     id == 4212  || // Xi_C^0 // Think, this is a bug and should be 4312 with Xi'C⁰ instead of 4212
	     id == 4322  || // Xi'_C+  This is in fact EM not weak
	     id == 4332  ||  // Omega_C (correct: 4332)

	     // added new charmed Mesons:
	     id == 10411 || // D*+_0
	     id == 10421 || // D*0_0
	     id ==   413 || // D*(2010)+
	     id ==   423 || // D*(2007)0
	     id == 10413 || // D_1(2420)+
	     id == 10423 || // D_1(2420)0
	     id == 20413 || // D_1(H)+
	     id == 20423 || // D_1(H)0
	     id ==   415 || // D*_2(2460)+
	     id ==   425 || // D*_2(2460)0
	     id == 10431 || // D
	     id ==   433 || // D*+_s0
	     id == 10433 || // D_s1(2536)+
	     id == 20433 || // D_s1(H)+
	     id ==   435 || // D*+_s2

	     // added ccbar Mesons:
	     id ==     441  || // eta_c(1S)
	     id ==    -441  || // eta_c(1S)
	     id ==   10441  || // xi_c0(2S)
	     id ==  -10441  || // xi_c0(2S)
	     id ==  100441  || // eta_c(2S9
	     id == -100441  || // eta_c(2S9
	     id ==     443  || // J/Psi
	     id ==    -443  || // J/Psi
	     id ==   10443  || // h_c(1P)
	     id ==  -10443  || // h_c(1P)
	     id ==   20443  || // xi_c1(1P)
	     id ==  -20443  || // xi_c1(1P)
	     id ==  100443  || // Psi(2S)
	     id == -100443  || // Psi(2S)
	     id ==   30443  || // Psi(3770)
	     id ==  -30443  || // Psi(3770)
	     id == 9000443  || // Psi(4040)
	     id ==-9000443  || // Psi(4040)
	     id == 9010443  || // Psi(4160)
	     id ==-9010443  || // Psi(4160)
	     id == 9020443  || // Psi(4415)
	     id ==-9020443  || // Psi(4415)
	     id ==     445  || // xi_c2(1P)
	     id ==    -445  || // xi_c2(1P)

	     // added new charmed Baryons:
	     id ==    4222  || // Sigma++_c
	     id ==    4112  || // Sigma0_c
	     id ==    4224  || // Sigma*++_c
	     id ==    4214  || // Sigma*+_c
	     id ==    4114  || // Sigma*0_c
	     id ==    4312  || // Xi'0_c
	     id ==    4324  || // Xi*+_c
	     id ==    4314  || // Xi*0_c
	     id ==    4334  || // Omega*0_c
	     id ==    4412  || // Xi+_cc
	     id ==    4422  || // Xi++_cc
	     id ==    4414  || // Xi*+_cc
	     id ==    4424  || // Xi*++_cc
	     id ==    4432  || // Omega+_cc
	     id ==    4434  || // Omega*+_cc
	     id ==    4444   // Omega++_ccc
	     );

  }


bool WisBwithWeakDK(const int id) {
  return ( id == 511   || // B+
	   id == 521   || // B0
	   id == 531   || // Bs
	   id == 541   || // Bc
       id == 5122  || // Lambda_B
	   id == 5132  || // Xi_b-
	   id == 5232  || // X_b0
	   id == 5112  || // Sigma_b-
	   id == 5212  || // Sigma_b0
	   id == 5222  || // Sigma_b+
	   id == 5332 );  // Omega_B
}
bool WisBwithWeakDK_ext(const int id) {
  return ( id ==  511   || // B+
	   id ==  521   || // B0
	   id ==  531   || // Bs
	   id ==  541   || // Bc
       id ==  5122  || // Lambda_B
	   id ==  5132  || // Xi_b-
	   id ==  5232  || // X_b0
	   id ==  5112  || // Sigma_b-
	   id ==  5212  || // Sigma_b0
	   id ==  5222  || // Sigma_b+
	   id ==  5332  || // Omega_B

	   // added new bottom Mesons:
	   id == 10511  || // B*0_0
	   id == 10521  || // B*+_0
	   id ==   513  || // B*0
	   id ==   523  || // B*+
	   id == 10513  || // B_1(L)0
	   id == 10523  || // B_1(L)+
	   id == 20513  || // B_1(H)0
	   id == 20523  || // B_1(H)+
	   id ==   515  || // B*0_2
	   id ==   525  || // B*+_2
	   id == 10531  || // B*0_s
	   id ==   533  || // B*0_s
	   id == 10533  || // B_s1(L)0
	   id == 20533  || // B_s1(H)0
	   id ==   535  || // B*0_s2
	   id == 10541  || // B*+_c
	   id ==   543  || // B*+_c
	   id == 10543  || // B_c1(L)+
	   id == 20543  || // B_c1(H)+
	   id ==   545  || // B*+_c2

	   // added bbbar Mesons:
	   id ==    551  || // eta_b(1S)
	   id ==   -551  || // eta_b(1S)
	   id ==  10551  || // xi_b0(1P)
	   id == -10551  || // xi_b0(1P)
	   id == 100551  || // eta_b(2S)
	   id ==-100551  || // eta_b(2S)
	   id == 110551  || // xi_b0(2P)
	   id ==-110551  || // xi_b0(2P)
	   id == 200551  || // eta_b(3S)
	   id ==-200551  || // eta_b(3S)
	   id == 210551  || // xi_b0(3P)
	   id ==-210551  || // xi_b0(3P)
	   id ==    553  || // Ypsilon(1S)
	   id ==   -553  || // Ypsilon(1S)
	   id ==  10553  || // h_b(1P)
	   id == -10553  || // h_b(1P)
	   id ==  20553  || // xi_b1(1P)
	   id == -20553  || // xi_b1(1P)
	   id ==  30553  || // Ypsilon_1(1D)
	   id == -30553  || // Ypsilon_1(1D)
	   id == 100553  || // Ypsilon(2S)
	   id ==-100553  || // Ypsilon(2S)
	   id == 110553  || //h_b(2P)
	   id ==-110553  || //h_b(2P)
	   id == 120553  || // xi_b1(2P)
	   id ==-120553  || // xi_b1(2P)
	   id == 130553  || // Ypsilon(2D)
	   id ==-130553  || // Ypsilon(2D)
	   id == 200553  || // Ypsilon(3S)
	   id ==-200553  || // Ypsilon(3S)
	   id == 210553  || // h_b(3P)
	   id ==-210553  || // h_b(3P)
	   id == 220553  || // xi_b1(3P)
	   id ==-220553  || // xi_b1(3P)
	   id == 300553  || // Ypsilon(4S)
	   id ==-300553  || // Ypsilon(4S)
	   id ==9000553  || // Ypsilon(10860)
	   id ==-9000553 || // Ypsilon(10860)
	   id ==9010553  || // Ypsilon(11020)
	   id ==-9010553 || // Ypsilon(11020)
	   id ==    555  || // xi_b2(1P)
	   id ==   -555  || // xi_b2(1P)
	   id ==  10555  || // eta_b2(1D)
	   id == -10555  || // eta_b2(1D)
	   id ==  20555  || // Ypsilon_2(1D)
	   id == -20555  || // Ypsilon_2(1D)
	   id == 100555  || // xi_b2(2P)
	   id ==-100555  || // xi_b2(2P)
	   id == 110555  || // eta_b2(2D)
	   id ==-110555  || // eta_b2(2D)
	   id == 120555  || // Ypsilon_2(2D)
	   id ==-120555  || // Ypsilon_2(2D)
	   id == 200555  || // xi_b2(3P)
	   id ==-200555  || // xi_b2(3P)
	   id ==    557  || // Ypsilon_3(1D)
	   id ==   -557  || // Ypsilon_3(1D)
	   id == 100557  || // Ypsilon_3(2D)
	   id ==-100557  || // Ypsilon_3(2D)

	   // added new bottom Baryons:
	   id ==   5114 || // Sigma*-_b
	   id ==   5214 || // Sigma*0_b
	   id ==   5224 || // Sigma*+_b
	   id ==   5312 || // Xi'0_b
	   id ==   5322 || // Xi'0_b
	   id ==   5314 || // Xi*-_b
	   id ==   5324 || // Xi*0_b
	   id ==   5334 || // Omega-_b
	   id ==   5142 || // Xi0_bc
	   id ==   5242 || // Xi0_bc
	   id ==   5412 || // Xi'0_bc
	   id ==   5422 || // Xi'+_bc
	   id ==   5414 || // Xi*0_bc
	   id ==   5424 || // Xi*+_bc
	   id ==   5342 || // Omega0_bc
	   id ==   5432 || // Omega'0_bc
	   id ==   5434 || // Omega*0_bc
	   id ==   5442 || // Omega+_bcc
	   id ==   5444 || // Omega+_bcc
	   id ==   5512 || // Xi-_bb
	   id ==   5522 || // Xi0_bb
	   id ==   5514 || // Xi*-_bb
	   id ==   5524 || // Xi*0_bb
	   id ==   5532 || // Omega-_bb
	   id ==   5534 || // Omega*-_bb
	   id ==   5542 || // Omega0_bbc
	   id ==   5544 || // Omega*0_bbc
	   id ==   5554  // Omega-_bbb
);
  }
