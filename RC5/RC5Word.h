#pragma once

#include <boost/algorithm/string.hpp>
using namespace std;

class RC5Word
{
private:
    int  repetition;
    int  system;
    int  command;
public:
    RC5Word(int system, int command, int repetition=0) : system(system), command(command), repetition(repetition) { }
	RC5Word(string commasep);

    ~RC5Word() { }

    int System()     const { return system; }
    int Command()    const { return command; }
    int Repetition() const { return repetition; }

	bool RC5Word::operator==(const RC5Word& that) {
		return system == that.system && command == that.command;
	}

	bool RC5Word::operator!=(const RC5Word& that) {
		return !(*this == that);
	}
};

ostream& operator<<(ostream& out, const RC5Word& word);
