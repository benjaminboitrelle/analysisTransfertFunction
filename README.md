# analysisTransfertFunction

This software is used to read an ASCII file containing pixel's response with respect a defined threshold.
It creates multiple plot:
* Transfert function fitted with a complementary error function
* A histogram of the mean calculated with the fit result of the S-Curve
* A histogram of the sigma calculated with the fit result of the S-Curve
It returns a ROOT file.

## Compliation

To compile the project:

mkdir build && cd build;
cmake ..
make

And then to run the project:
./analysisTransfertFunction

## Information

This project was written, compile and tested with macOS Sierra (version 10.12.6).
It is a C++ based software using ROOT classes.
Needs at least C++11 and the ROOT version used is 6.10.06.
The compiler used is clang-900.0.38.

## TO DO

* Test compilation and running software with Windows and Linus machine
* Read parameters from a config file instead of hard-coded numbers
* Add a scatter plot of pixel's response for different thresholds
