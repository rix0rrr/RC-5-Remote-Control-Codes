#include "stdafx.h"

#include "RC5Word.h"

RC5Word::RC5Word(string commasep)
{
	vector<string> parts;
	boost::split(parts, commasep, boost::is_any_of(",:;|."));
	if (parts.size() < 2) throw runtime_error(string("Expecting two parts in ") + commasep);

	system     = boost::lexical_cast<int>(parts[0]);
	command    = boost::lexical_cast<int>(parts[1]);
	repetition = 0;
}

ostream& operator<<(ostream& out, const RC5Word& word) {
	out << word.System() << ":" << word.Command();
	return out;
}