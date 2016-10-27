/** ____________________________________________________________________________

\file PMDField.h

\brief PMDField class description

\author Programmer: Mathieu Lobet
\date Creation:   Fri Oct 14 2016

\warning READ BEFORE MODIFY:
\n This file should be modified/maintained only when located in its original repository.
\n Else, this file is a copy and may not be the lastest version.
\n The modifications will not be considered.

 ____________________________________________________________________________ */

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
 Class: PMDField

 \brief This class represents the field datasets in the OpenPMD file.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
class PMDField
{
	public:
            	PMDField();
            	~PMDField();
        void    ScanAttributes(hid_t object_id);
        void    SetGridDimensions(hid_t dataset_id);
        int     GetNumValues();

        /// Activate verbose
        int     verbose; 

        // Field attributes
        /// name of the field
        char    name[64];    
        /// path to the dataset in the hdf5 file
        char    datasetPath[128];  
        /// path to the group containing this dataset   
        char    groupPath[128];   
        /// number of dimensions    
        int  	ndims;       
        /// number of nodes in each direction        
        int     nbNodes[3];     
        /// Grid spacing in each direction (max 3)      
        double  gridSpacing[3];      
        /// Origin of the grid 
        double  gridGlobalOffset[3];  
        /// Shift of the grid
        double  gridPosition[3];  
        /// Units for the axis    
        double  unitSI;       
        /// Units for the grid        
        double  gridUnitSI;    
        /// Geometry of the grid     
        char    geometry[64];    
        /// Labels     
        char    axisLabels[16];
        /// Units    
        char    unitsLabel[64];  
        /// Field boundary conditions
        char    fieldBoundary[64];
        /// Data order (C or Fortran)
        char    dataOrder[8];

    protected:

    private:
        void    SetGridSpacing(char * name, hid_t attr_id, hid_t attr_type, hid_t attr_space);
        void    SetGridGlobalOffset(char * name, hid_t attr_id, hid_t attr_type, hid_t attr_space);
        void    SetGridPosition(char * name, hid_t attr_id, hid_t attr_type, hid_t attr_space);
        void    SetUnitSI(char * name, hid_t attr_id, hid_t attr_type, hid_t attr_space);
        void    SetGridUnitSI(char * name, hid_t attr_id, hid_t attr_type, hid_t attr_space);
        void    SetGeometry(char * name, hid_t attr_id, hid_t attr_type, hid_t attr_space);
        void    SetAxisLabels(char * name, hid_t attr_id, hid_t attr_type, hid_t attr_space);
        void    SetUnitDimension(char * name, hid_t attr_id, hid_t attr_type, hid_t attr_space);
        void    SetFieldBoundary(char * name, hid_t attr_id, hid_t attr_type, hid_t attr_space);
        void    SetDataOrder(char * name, hid_t attr_id, hid_t attr_type, hid_t attr_space);
};