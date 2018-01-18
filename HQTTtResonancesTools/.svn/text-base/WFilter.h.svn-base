#ifndef WFILTER_H_
#define WFILTER_H_

#include <vector>
#include "xAODJet/JetContainer.h"

/**
* @brief Checks if truth particle is a C-meson, B-Meson and decides (with the number of C-/B-Mesons, whether sample is W+c/W+cc/W+b/W+bb
*
* @returns variables: passC, passB, is_C, is_antiC
*/

// global variable used by both filters:
extern double Wglobal_DeltaR_Jet_part;

// C-Filter
void Wfind_cquark( bool do_truthMatch, std::vector<TLorentzVector> truth_jets,int pdgId,TLorentzVector part_vec, bool &passC, bool &is_C, bool &is_antiC);

void Wfind_chadron(bool do_truthMatch, std::vector<TLorentzVector> truth_jets,int pdgId,TLorentzVector part_vec, bool &passC, bool &is_C, bool &is_antiC);

void Wset_Cfilter( bool passC, int N_C, int N_antiC, bool &is_Conly, bool &is_cc);

bool WisDwithWeakDK(const int pID);

// declaration of global variables, used in HQTTtres
extern double Wglobal_cPtMin;         // c-Quark
extern double Wglobal_cEtaMax;        // c-Quark
extern double Wglobal_charmPtMin;     // c-Meson/Baryon
extern double Wglobal_charmEtaMax;    // c-Meson/Baryon

// B-Filter
void Wfind_bquark( bool do_truhtMatch, std::vector<TLorentzVector> truth_jets,int pdgId,TLorentzVector part_vec, bool &passB, bool &is_B, bool &is_antiB);

void Wfind_bhadron(bool do_truthMatch, std::vector<TLorentzVector> truth_jets,int pdgId,TLorentzVector part_vec, bool &passB, bool &is_B, bool &is_antiB);

void Wset_Bfilter(bool passB, int N_B, int N_antiB, bool &is_Bonly, bool &is_bb);

bool WisBwithWeakDK(const int pID);


extern double Wglobal_bPtMin;         	 // b-Quark
extern double Wglobal_bEtaMax;       	 // b-Quark
extern double Wglobal_bottomPtMin;  	 // b-Meson/Baryon
extern double Wglobal_bottomEtaMax;      // b-Meson/Baryon

// extended C-Filter: added pdgID of more c-Mesons/c-Baryons:
void Wfind_chadron_ext(bool do_truthMatch, std::vector<TLorentzVector> truth_jets,int pdgId,TLorentzVector part_vec, bool &passC, bool& is_C, bool& is_antiC);

bool WisDwithWeakDK_ext(const int pID);

// extended B-Filter: added pdgID of more b-Mesons/b-Baryons:
void Wfind_bhadron_ext(bool do_truthMatch, std::vector<TLorentzVector> truth_jets,int pdgId,TLorentzVector part_vec, bool &passB, bool& is_B, bool& is_antiB);

bool WisBwithWeakDK_ext(const int pID);

#endif
