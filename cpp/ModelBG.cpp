/***************************************************************
 * Name:      ModelBG.cpp
 * Purpose:   Code for Model BG Mid Tier Class
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#include "ModelBG.h"

namespace openvpa {

    ModelBG::ModelBG() : Model::Model() {
        modeling_type = MODELING_TYPE_BG;
    }

    bool ModelBG::isEmpty() {
        return (bool) bg.empty();
    }

    bool ModelBG::reset() {
        bg = Mat();
        updated = false;
        return true;
    }
}
