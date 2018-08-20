#ifndef NLARGEJETTTRESSMOOTHEDTTMASST32SELECTOR_H_
#define NLARGEJETTTRESSMOOTHEDTTMASST32SELECTOR_H_


#include "TopEventSelectionTools/SignValueSelector.h"
#include "JetAnalysisInterfaces/IJetSelectorTool.h"
#include "BoostedJetTaggers/SmoothedTopTagger.h"


namespace top {

/**
 * @brief Does some stuff
 */
class NLargeJetTtresSmoothedTTMassTau32Selector : public SignValueSelector {
public:
    explicit NLargeJetTtresSmoothedTTMassTau32Selector(const std::string& params);

    /**
     * @brief Write a description ;)
     *
     * @param event The event to perform the operations on.
     * @return True if the event passes the selection, false otherwise.
     */
     bool apply(const top::Event& event) const override;
   
     //SmoothTopTagger Variables (w 80% efficiency)
     std::unique_ptr<SmoothedTopTagger> m_smoothedTopTaggerMT80;//mass+tau32

      //Other smooth top taggers
/*
     std::unique_ptr<SmoothedTopTagger> m_smoothedTopTaggerMT50;//mass+tau32
     std::unique_ptr<SmoothedTopTagger> m_smoothedTopTaggerTS80;//tau32+Split23
     std::unique_ptr<SmoothedTopTagger> m_smoothedTopTaggerTS50;//tau32+Split23
     std::unique_ptr<SmoothedTopTagger> m_smoothedTopTaggerQT80;//Qw+tau32
     std::unique_ptr<SmoothedTopTagger> m_smoothedTopTaggerQT50;//Qw+tau32
*/

     };

}

#endif

