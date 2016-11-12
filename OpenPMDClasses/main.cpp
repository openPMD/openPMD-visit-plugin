#include "PMDFile.h"

#include <iostream>
#include <string>
#include <stdlib.h>

#include <stdio.h>


//#include "H5Cpp.h"
//#include <hdf5.h>
//#include <visit-hdf5.h>


int main(void)
{

	// Path toward an OpenPMD file
	char path[128];
	strcpy(path, "../../../OpenPMD/openPMD-example-datasets-draft/example-thetaMode/hdf5/data00000500.h5");

	// Create the OpenPMD Container
	PMDFile openPMDFile;

	// Opening of the file
	openPMDFile.OpenFile(path);

	// Scan the file attributes
	openPMDFile.ScanFileAttributes();

	// Scan the iterations
	openPMDFile.ScanIterations();

	// for each iteration, scan the fields
	openPMDFile.ScanFields();

	// For each iteration, scan the particle groups
	openPMDFile.ScanParticles();

	// Read dataset
	// float * array = new float[1016];
	// char dataSetPath[128] = "/data/1000/particles/beam/position/x";
	// float factor = 1.;
	// openPMDFile.ReadScalarDataSet(array,1016,&factor,H5T_FLOAT,dataSetPath);
	// for (int i=0;i<1016;i++)
	// {
	// 	cout << array[i] << endl;	
	// }

	// Print the content
	openPMDFile.Print();

	// Close the file
	openPMDFile.CloseFile();


    return 0;
}





