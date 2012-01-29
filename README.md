# Simple implementation of RC-5 remote codes

C#/.NET library that offers a high-level API to send RC-5 IR codes. The RC-5
control standard is used by Philips TVs, and possibly other vendors as well.
The library translates the codes into a sequence of pulses and spaces that will
be transmitted using an IR transmitter LED.

All information was gleaned from http://en.wikipedia.org/wiki/RC-5

## IR Device Support

Only supports the IguanaWorks IR transceiver (http://iguanaworks.net/). USB was
tested, althoug the serial version should work just as well as this library simply
uses the IguanaWorks provided IR driver and client.

Direct support of serial transmitters (by toggling the DTR line very quickly)
was attempted but abandoned, as it is very hard to get the frequency of
toggling correctly (Windows is not a real-time operating system). The
IguanaWorks module offloads the high frequency oscillation to a microcontroller
where it belongs :).

Nothing in this software is particularly Windows-specific, so with a little
tweaks it may work on Linux as well.

## How to use

It's a simple C# library, with no dependencies, so you should be able to drop
the library into your project without any problems.

The test project requires NuGet to import NUnit and Moq.
