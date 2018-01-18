#ifndef NLARGEJETTTRESST80NOLEPTONSELECTOR_H_
#define NLARGEJETTTRESST80NOLEPTONSELECTOR_H_

#include "TopEventSelectionTools/SignValueSelector.h"
#include "BoostedJetTaggers/SubstructureTopTaggerHelpers.h"
namespace top {

/**
 * @brief Does some stuff
 */
class NLargeJetTtresST80NoLeptonSelector : public SignValueSelector {
public:
    explicit NLargeJetTtresST80NoLeptonSelector(const std::string& params);

    /**
     * @brief Write a description ;)
     *
     * @param event The event to perform the operations on.
     * @return True if the event passes the selection, false otherwise.
     */
     bool apply(const top::Event& event) const override;

     SubstructureTopTagger *STL;
};

}

#endif
