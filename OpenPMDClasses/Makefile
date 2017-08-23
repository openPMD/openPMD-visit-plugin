# C++ Compiler
#CXX=g++
#CXX=h5c++
CXX=h5pcc -DTEST=1 -O2
# Compiler arguments
CARGS= -DTEST=1 -O2
# Libraries
# LDFLAGS=-I/Applications/VisIt.app/Contents/Resources/2.10.3/darwin-x86_64/include/hdf5/include/
# LDFLAGS=-I/usr/local/hdf5/include/include/
# sources
SRC=PMDField.o PMDParticle.o PMDIteration.o PMDFile.o main.o
# Appllication name
APPNAME=test_OpenPMD

%.o:%.cpp
	$(CXX) $(CARGS) -c -o $@ $< $(LDFLAGS)

all: clean build

build: $(SRC)
	$(CXX) $(CARGS) -o $(APPNAME) $(SRC) $(LDFLAGS)

clean:
	rm -rf *.o
	rm -f test_OpenPMD
