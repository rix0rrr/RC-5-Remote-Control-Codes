// RC5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
namespace po = boost::program_options;
using namespace std;

#include <fstream>

#include "RC5Decoder.h"
#include "RC5Encoder.h"
#include "SerialPort.h"

#define TOGGLE_STATE_FILE ".rc5toggle"

class LinePrinter : public IWordConsumer
{
public:
    virtual void WordReceived(RC5Word &word)
    {
		cout << word << endl;
		cout.flush();
    }
};

class FancyPrinter : public IWordConsumer
{
private:
	RC5Word last;
public:
	FancyPrinter() : last(-1, -1) { }

    virtual void WordReceived(RC5Word &word)
    {
		if (word != last)
			cout << endl << word;
		else
			cout << ".";

		cout.flush();
    }
};

/**
 * Store the toggle bit of the RC5 encoder in a file
 */
void store_rc5_toggle_bit(RC5Encoder& encoder)
{
	ofstream file(TOGGLE_STATE_FILE);
	file << encoder.ToggleBit() ? '1' : '0';
	SetFileAttributes(TOGGLE_STATE_FILE, FILE_ATTRIBUTE_HIDDEN);
}

/**
 * Read the stored RC5 toggle bit
 */
bool next_rc5_toggle_bit()
{
	ifstream file(TOGGLE_STATE_FILE);
	char bit;
	file >> bit;
	return bit == '1';
}

/**
 * Read IR from the given port
 */
void read_ir(string port_name, bool fancy) 
{
	auto_ptr<IWordConsumer> printer(fancy ? (IWordConsumer*) new FancyPrinter() : new LinePrinter());
	RC5Decoder decoder(*printer);
	SerialPort port((char*) port_name.c_str(), true, decoder);

	port.Open();
	port.ReadLoop();

	// We never get here because ReadLoop() hangs indefinitely
	port.Close();
}

/**
 * Write IR to the given port
 */
void write_ir(string port_name, vector<string> codes) 
{
	NullDecoder decoder;
	SerialPort port((char*) port_name.c_str(), true, decoder);
	RC5Encoder encoder(port, next_rc5_toggle_bit());

	port.Open();

	for(vector<string>::iterator it = codes.begin(); it != codes.end(); ++it) {
		RC5Word word(*it);
		cout << word << endl;

		encoder.Transmit(word);

		store_rc5_toggle_bit(encoder);
	}

	port.Close();
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		// Command-line handling
		po::options_description opts("Options");
		po::options_description pos_opts("Names for positional options (hidden)");
		po::positional_options_description args;

		opts.add_options()
			("help,h",                                             "display help information")
			("port,p", po::value<string>()->default_value("COM1"), "serial port to use")
			("repeat,r",                                           "print each repeat on its own line (rx)")
			;
		pos_opts.add_options()
			("command", po::value<string>()->default_value(""))
			("args",    po::value<vector<string>>());

		args.add("command", 1);
		args.add("args",    -1);

		po::variables_map vars;
		po::store(po::command_line_parser(argc, argv).options(pos_opts.add(opts)).positional(args).run(), vars);
		po::notify(vars);

		string command = vars["command"].as<string>();

		if (vars.count("help") || (command != "rx" && command != "tx"))
		{
			cout
				<< "Usage:" << endl
				<< "  rc5.exe [options] rx" << endl
				<< "  rc5.exe [options] tx CODE [CODE [...]]" << endl
				<< endl
				<< opts << endl
				<< "Rx mode will print the received codes to stdout until Ctrl-C is hit." << endl
				<< endl
				<< "For transmission, specify a CODE in the form '0:123' where the first" << endl
				<< "number is the system address and the second number is the command number" << endl
				<< "(both in decimal). Note that rc5.exe will remember the state of the toggle" << endl
				<< "bit between invocations using a file in the current directory." << endl
				<< endl
				<< "(rc5.exe assumes a serial DCD device for rx and a DTR device for tx)" << endl
				;

			return 0;
		}

		string port = vars["port"].as<string>();

		if (command == "rx") read_ir(port, vars.count("repeat") == 0);
		if (command == "tx") write_ir(port, vars["args"].as<vector<string>>());

		return 0;
	} 
	catch (exception& e)
	{
		cerr << "ERROR: " << e.what() << "\n";
		return 1;
	}
}

