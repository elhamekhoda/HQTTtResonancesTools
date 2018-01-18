#ifndef ANGULARCUTSSL_H_
#define ANGULARCUTSSL_H_

#include "TopEventSelectionTools/SignValueSelector.h"
namespace top {

/**
 * @brief Does some stuff
 */
class AngularCutsSL : public SignValueSelector {
public:
    explicit AngularCutsSL(const std::string& params);

    /**
     * @brief Write a description ;)
     *
     * @param event The event to perform the operations on.
     * @return True if the event passes the selection, false otherwise.
     */
     bool apply(const top::Event& event) const override;
   

};

}

#endif
