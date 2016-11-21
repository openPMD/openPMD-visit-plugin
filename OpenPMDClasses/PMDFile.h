/** ____________________________________________________________________________

\file PMDFile.h

\brief Definition of the class PMDFile

\author Programmer: Mathieu Lobet
\date Creation:   Fri Oct 14 2016

\warning READ BEFORE MODIFY:
\n This file should be modified/maintained only when located in its original repository.
\n Else, this file is a copy and may not be the lastest version.
\n The modifications will not be considered.

 ____________________________________________________________________________ */

#include "PMDIteration.h"
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
 Class: PMDFile

 \brief The class PMDFile enables to read the structure of an OpenPMD file.

 \details
 This class constitutes the main one to read OpenPMD files.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
class PMDFile
{
	public:
            	PMDFile();
            	~PMDFile();

        /// Activate the verbose
        int                     verbose;
        /// File path
        char                    filePath[128];
        /// OpenPMD version
        char                    version[8];
        /// Id of the file after opening
        hid_t                   fileId;
        /// vector of PMDIteration objects that contains the OpenPMD iterations
        vector <PMDIteration>   iterations;

        void                    OpenFile(char * path);
        void                    ScanFileAttributes();
        void                    ScanIterations();
        void                    ScanFields();
        void                    ScanParticles();
        void                    Print();
        void                    SetVerbose(int value);
        int                     GetNumberIterations();
        int                     ReadScalarDataSet(void * array,int numValues,void * factor,H5T_class_t dataSetClass,char * path);
        int                     ReadFieldScalarBlock(void * array,void * factor,H5T_class_t dataSetClass, fieldBlockStruct * fieldBlock);
        void                    CloseFile();
        
    protected:


    private:

};