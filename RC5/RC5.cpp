// RC5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "RC5Decoder.h"
#include "SerialPortReader.h"

class WordPrinter : public IWordConsumer
{
public:
    virtual void WordReceived(RC5Word& word)
    {
        printf("%d %d (%d)\n", word.System(), word.Command(), word.Repetition());
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    WordPrinter printer;
    RC5Decoder decoder(printer);
    SerialPortReader reader(TEXT("COM1"), true, decoder);

    reader.Open();
    reader.Run();
}

