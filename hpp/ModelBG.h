/***************************************************************
 * Name:      ModelBG.h
 * Purpose:   Code for Model BG Mid Tier Class Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMODELBG_H
#define OPENVPAMODELBG_H

#include "Model.h"

using namespace std;

namespace openvpa {

    class ModelBG: virtual public Model {

        protected:
            Mat bg;

        public:
            ModelBG();
            virtual ~ModelBG() {};
            bool isEmpty();
            bool reset();
            int type() {return MODELING_TYPE_BG;};
    };
}

#endif
