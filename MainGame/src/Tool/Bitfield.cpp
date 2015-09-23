// Copyright(c) 2015, Rodolfo Lam
// All rights reserved.

#include "stdafx.h"
#include "Bitfield.h"

int Bitfield::set(int field, int value)
{
    field |= value;
    return field;
}

/*
True = The flag is in the field
False = The flag is NOT in the field
*/
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