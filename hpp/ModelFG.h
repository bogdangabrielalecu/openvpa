/***************************************************************
 * Name:      ModelFG.h
 * Purpose:   Code for Model FG Mid Tier Class Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-15
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAMODELFG_H
#define OPENVPAMODELFG_H

#include "Model.h"

using namespace std;

namespace openvpa {

    class ModelFG: virtual public Model {

        protected:
            Mat fg;

        public:
            ModelFG();
            virtual ~ModelFG() {};
            bool isEmpty();
            bool reset();
            int type() {return MODELING_TYPE_FG;};
    };
}

#endif
