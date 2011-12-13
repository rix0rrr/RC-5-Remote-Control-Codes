# Simple implementation of RC-5 remote codes

This remote control standard is used by Philips TVs, and possibly other vendors
as well.

All information was gleaned from http://en.wikipedia.org/wiki/RC-5

## IR Device Support

Currently only supports serial DCD receivers and DTR transmitters (the ones you
solder together yourself). 

Note: serial receivers probably only work on desktop machines and not using USB
adapters.

Uses Windows-specific APIs (serial, timing) so only works on Windows.

## How to use

For using this repository, it helps if you're a programmer because the code
does only what I need, user interface-wise, and I haven't given a lot of
thought to how other people might integrate it.

Currently, the code compiles to a command-line tool that you can call for
either receiving (and displaying the result on standard out) or reading
from standard in and transmitting that code.
