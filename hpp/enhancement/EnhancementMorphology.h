/***************************************************************
 * Name:      EnhancementMorphology.h
 * Purpose:   Code for Enhancement Class Extension Header
 * Author:    Bogdan Alecu
 * Created:   2011-09-16
 * Copyright: Bogdan Alecu
 * License:   GNU GPL
 **************************************************************/

#ifndef OPENVPAENHANCEMENTMORPHOLOGY_H
#define OPENVPAENHANCEMENTMORPHOLOGY_H

#include "Enhancement.h"

using namespace std;

namespace openvpa {

    class EnhancementMorphology: virtual public Enhancement {

        public:

            EnhancementMorphology();
            ~EnhancementMorphology();
            bool reset();
            Mat& get(Mat& frm);
            int mode() {return ENHANCEMENT_MODE_MORPHOLOGY;};
    };
}

#endif
