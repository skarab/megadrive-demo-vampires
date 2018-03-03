#include "sme_time.h"

fix32 sme_DeltaTime = 0;
fix32 sme_Last = 0;

void smeTIME_Update()
{
    const fix32 current = getTick()<<7;
    const fix32 delta = current-sme_Last;
    sme_Last = current;
    sme_DeltaTime = delta;
}

