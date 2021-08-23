//
// Created by hp on 8/21/2021.
//

#include <cstdint>
#include "TheMostSillyEffect.h"


 int16_t * TheMostSillyEffect::sillySoundEffect(const int16_t *sourceData, int32_t numSamples) {

     auto * newData = new int16_t[numSamples] { 0 };
     for (int i = 0; i < numSamples; ++i) {
         newData[i] = sourceData[i] * 3;
     }
     return newData;
}
