# analysisTransfertFunction

This software is used to read an ASCII file containing pixel's response with respect a defined threshold.
It is based on C++ and is using ROOT classes.

It creates multiple plots:
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

## Input

The input file is a ".txt" file which has the following structure:

 | threshold 0 | response pixel 0 | response pixel 1 | ... | response pixel n |
 | threshold 1 | response pixel 0 | response pixel 1 | ... | response pixel n |
 | ... | ... | ... | ... | ... |
 | threshold n | response pixel 0 | response pixel 1 | ... | response pixel n |

## Information

This project was written, compile and tested with macOS Sierra (version 10.12.6).
Needs at least C++11 and the ROOT version used is 6.10.06.
The compiler used is clang-900.0.38.

## TO DO

* Test compilation and running software with Windows and Linux machines
* Read parameters from a config file instead of hard-coded numbers
* Add a scatter plot of pixel's response for different thresholds
