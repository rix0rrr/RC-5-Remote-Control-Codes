#pragma once
class RC5Word
{
private:
    int  repetition;
    int  system;
    int  command;
public:
    RC5Word(int system, int command, int repetition=0) : system(system), command(command), repetition(repetition) { }
    ~RC5Word() { }

    int System()     { return system; }
    int Command()    { return command; }
    int Repetition() { return repetition; }
};

