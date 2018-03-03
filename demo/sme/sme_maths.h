#ifndef _SME_MATHS_H_
#define _SME_MATHS_H_

#include "genesis.h"

#define smeMOD(a, b)  ((a) % (b) + (b)) % (b)

extern const fix32 fPI;
extern const fix32 fPI2;
extern const fix32 fPIo2;
extern const fix32 fPI34;
extern const fix32 fPI74;
extern const fix32 fPIo4;
extern const fix32 fSQRT2;

fix32 cos(fix32 a);
fix32 sin(fix32 a);
fix32 clamp_angle(fix32 a);     // [-Pi , Pi]
fix32 clamp_angle2(fix32 a);     // [0 , 2Pi[


#endif
