#include "stdafx.h"
#include "Bitfield.h"

int Bitfield::set(int field, int value)
{
    field |= value;
    return field;
}

bool Bitfield::test(int field, int value)
{
    if ((field & value) != 0)
    {
        return true;
    } else
    {
        return false;
    }
}

void Bitfield::clear(int field, int value)
{
    field &= ~value;
}