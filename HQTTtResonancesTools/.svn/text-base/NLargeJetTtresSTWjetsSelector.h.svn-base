#ifndef NLARGEJETTTRESSTWJETSSELECTOR_H_
#define NLARGEJETTTRESSTWJETSSELECTOR_H_

#include "TopEventSelectionTools/SignValueSelector.h"
#include "BoostedJetTaggers/SubstructureTopTaggerHelpers.h"
namespace top {

/**
 * @brief Does some stuff
 */
class NLargeJetTtresSTWjetsSelector : public SignValueSelector {
public:
    explicit NLargeJetTtresSTWjetsSelector(const std::string& params);

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
