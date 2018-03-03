#include "sme_maths.h"

const fix32 fPI = FIX32(PI);
const fix32 fPI2 = FIX32(2.0f*PI);
const fix32 fPIo2 = FIX32(PI/2.0f);
const fix32 fPI34 = FIX32(PI*3.0f/4.0f);
const fix32 fPI74 = FIX32(PI*7.0f/4.0f);
const fix32 fPIo4 = FIX32(PI/4.0f);
const fix32 fSQRT2 = FIX32(1.41421f);

fix32 cos(fix32 a)
{
    s16 v = fix32ToInt(fix32Div(fix32Mul(a, FIX32(512.0f)), fPI));
    if (v<0) v += 1024;
    return cosFix32(v%1024);
}

fix32 sin(fix32 a)
{
    s16 v = fix32ToInt(fix32Div(fix32Mul(a, FIX32(512.0f)), fPI));
    if (v<0) v += 1024;
    return sinFix32(v%1024);
}

fix32 clamp_angle(fix32 a)
{
    while (a<-fPI) a += fPI2;
    while (a>fPI) a -= fPI2;
    return a;
}

fix32 clamp_angle2(fix32 a)
{
    while (a<0) a += fPI2;
    while (a>=fPI2) a -= fPI2;
    return a;
}

