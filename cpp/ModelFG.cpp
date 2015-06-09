/***************************************************************
 * Name:      ModelFG.cpp
 * Purpose:   Code for Model FG Mid Tier Class
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "ModelFG.h"

namespace openvpa {

    ModelFG::ModelFG() : Model::Model() {
        modeling_type = MODELING_TYPE_FG;
    }

    bool ModelFG::isEmpty() {
        return (bool) fg.empty();
    }

    bool ModelFG::reset() {
        fg = Mat();
        updated = false;
        return true;
    }
}
