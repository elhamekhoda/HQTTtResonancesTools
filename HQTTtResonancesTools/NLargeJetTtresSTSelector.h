#ifndef NLARGEJETTTRESSTSELECTOR_H_
#define NLARGEJETTTRESSTSELECTOR_H_

#include "TopEventSelectionTools/SignValueSelector.h"
#include "BoostedJetTaggers/SubstructureTopTaggerHelpers.h"
#include "JetAnalysisInterfaces/IJetSelectorTool.h"
#include "BoostedJetTaggers/SmoothedTopTagger.h"
#include "BoostedJetTaggers/JSSWTopTaggerBDT.h"
#include "BoostedJetTaggers/JSSWTopTaggerDNN.h"
#include "BoostedJetTaggers/TopoclusterTopTagger.h"

namespace top {

/**
 * @brief Does some stuff
 */
class NLargeJetTtresSTSelector : public SignValueSelector {
public:
    explicit NLargeJetTtresSTSelector(const std::string& params);

    /**
     * @brief Write a description ;)
     *
     * @param event The event to perform the operations on.
     * @return True if the event passes the selection, false otherwise.
     */
     bool apply(const top::Event& event) const override;
   
     //SubstructureTopTagger Variables
     //SubstructureTopTagger *STL;
     SubstructureTopTagger *STL80;
     SubstructureTopTagger *STL50;

     //Other taggers
     std::unique_ptr<SmoothedTopTagger> m_smoothedTopTaggerMT80;//mass+tau32
     std::unique_ptr<SmoothedTopTagger> m_smoothedTopTaggerMT50;//mass+tau32
     std::unique_ptr<SmoothedTopTagger> m_smoothedTopTaggerTS80;//tau32+Split23
     std::unique_ptr<SmoothedTopTagger> m_smoothedTopTaggerTS50;//tau32+Split23
     std::unique_ptr<SmoothedTopTagger> m_smoothedTopTaggerQT80;//Qw+tau32
     std::unique_ptr<SmoothedTopTagger> m_smoothedTopTaggerQT50;//Qw+tau32
     std::unique_ptr<JSSWTopTaggerBDT> m_bdtTopTagger80;//BDT top tagger
     std::unique_ptr<JSSWTopTaggerDNN> m_dnnTopTagger80;//DNN top tagger
     std::unique_ptr<TopoclusterTopTagger> m_topoTopTagger;//Topo Cluster top tagger


};

}

#endif
