CSE331 Project 2 --- Cach Simulator
-----------------------------------
[![Build Status](https://magnum.travis-ci.com/izzycecil/cse331-project2.svg?token=D8UDof62FHzPoKtHhJjk&branch=master)](https://magnum.travis-ci.com/izzycecil/cse331-project2)

This is an extremely simple cache simulator for NMT's CSE331 (Computer
  Architecture) class. Given a config file and a trace file, this simulator
  will generate information about the runtime of a process. More will be
  filled in this section after assignment has been turend in, for outside
  use.


Building
---------
In order to make the project, simply run `make`. If you would like to run unit
tests, simply run `make test`. In both cases, binaries will be deployed into 
the `bin` directory.

File Structure
--------------
```
include -> header files
src     -> implementation files
lib     -> googletest
bin     -> executables
test    -> unit tests
confs   -> config files
traces  -> trace files
```
