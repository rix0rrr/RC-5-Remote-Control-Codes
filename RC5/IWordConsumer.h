#pragma once
#include "RC5Word.h"

class IWordConsumer
{
public:
    virtual ~IWordConsumer() { }
    virtual void WordReceived(RC5Word& word) = 0;
};

