
#ifndef PMDITERATION_H
#define PMDITERATION_H

#include "PMDField.h"
#include "PMDParticle.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include <hdf5.h>
//#include <visit-hdf5.h>

//#include "H5Cpp.h"

using namespace std;

/** ____________________________________________________________________________
 Class: PMDIteration

 \brief This class enables to manage the different iterations of an OpenPMD file.

 \details When an OpenPMD file is read, a PMDIteration is created for each iteration.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
class PMDIteration
{
	public:
            	PMDIteration();
            	~PMDIteration();

        void    ScanFields(hid_t fileId);
        void    ScanParticles(hid_t fileId);
        void	PrintInfo();

        int 	verbose;

        // Vector of field objects from the datasets
    	vector <PMDField> fields;

        // Vector of particle objects
        vector <PMDParticle> particles;

        // Iteration attributes
        /// Iteration name
        char  	name[20];
        /// Iteration time step
        float  	dt;
        /// Iteration corresponding time
        float 	time;
        /// factor to convert the time in SI units
        float 	timeUnitSI;

    protected:

};

#endif
