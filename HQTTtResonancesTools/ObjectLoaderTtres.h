#ifndef OBJECTLOADERTTRES_H_
#define OBJECTLOADERTTRES_H_

#include "TopAnalysis/ObjectLoaderBase.h"

namespace top {

/**
 * @brief TTres use a slightly different object selection, so they configure
 * it here.  You can load this at run time instead by setting the
 * ObjectSelectionName in the configuration file to top::ObjectLoaderTtres.
 */
class ObjectLoaderTtres : public ObjectLoaderBase {
public:
    /**
     * @brief Setup the configuration.
     *
     * @param topConfig Settings from the config file at run time.
     *
     * @return Fully configured object selection.
     */
    top::TopObjectSelection* init(std::shared_ptr<top::TopConfig> topConfig);

    static bool setTOPQ1;

    ClassDef(top::ObjectLoaderTtres, 0)
};

}

#endif
