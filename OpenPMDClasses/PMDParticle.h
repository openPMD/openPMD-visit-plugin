/** ____________________________________________________________________________

\file PMDParticle.h

\brief PMDParticle Class definition

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

/// Structure to store particle block properties
struct particleBlockStruct
{
    /// Block dimension
    int     numParticles;
    /// minimum node index
    int     minParticle;
    /// maximum node index
    int     maxParticle;
    /// Path to the dataSet
    char    dataSetPath[128];
};

/** ____________________________________________________________________________
 Class: PMDParticle

 \brief
 This class represents the Particle groups in the OpenPMD files.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
class PMDParticle
{
	public:

        /// Structure to store scalar vtk metadata
        struct scalarDataSet
        {
            /// Scalar data name
            char    name[64]; 
            /// Scalar data path in the OpenPMD file        
            char    path[128];
            /// Units computed from the OpenPMD file
            char    unitLabel[64];
            /// Factor for SI units
            double  unitSI;
            /// Weighting Power
            double  weigthingPower;
            /// Time offset read from OpenPMD files            
            double  timeOffset;
            /// Macro-weighted read from OpenPMD file
            int     macroWeighted;
        };

        /// Structure to store vector vtk metadata
        struct vectorDataSet
        {
            /// Vector data name
            char    name[64];
            /// Vector data path in the OpenPMD file      
            char    path[128];
            /// Units computed from the OpenPMD file
            char    unitLabel[64];
            /// Corresponding scalarDataSet objects in scalarDataSets for each vector component
            int     dataSetId[3];  
            /// Time offset read from OpenPMD files
            double  timeOffset;
            /// Weighting Power
            double  weigthingPower;     
        };

        // Constructor and destructor
            	   PMDParticle();
            	   ~PMDParticle();

        // Particle methods
        void        ScanParticleGroup(hid_t particleGroupId);
        void        ScanCharge(hid_t particleGroupId, char * objectName);
        void        ScanMass(hid_t particleGroupId, char * objectName);
        void        ScanPositions(hid_t particleGroupId, char * objectName);
        void        ScanMomenta(hid_t particleGroupId, char * objectName);
        void        ScanDataSet(hid_t particleGroupId,char * objectName);
        void        ScanGroup(hid_t particleGroupId,char * objectName);

        int         GetNumScalarDatasets();
        int         GetNumVectorDatasets();

        // Particle attributes
        /// Activate outputs
        int         verbose;
        /// Particle group name
        char        name[64];  
        /// Particle path     
        char        path[64];
        /// Number of particles
        long        numParticles;
        /// Particle mass (when constant for all particles)
        double      charge;  
        /// Particle mass (when constant for all particles)     
        double      mass;
        ///         position dimension
        int         numDimsPositions;
        ///         momentum dimension
        int         numDimsMomenta;
        /// This flag is true when the momentum is available
        bool        momentumAvailable;
        /// ScalarDataSet index in scalarDataSets for the x positions
        int         positionsId[3];

        /// Contain all the scalar for this particle group
        vector <scalarDataSet> scalarDataSets;
        /// Contain all the vectors for this particle group 
        vector <vectorDataSet> vectorDataSets;

    protected:


    private:

        void        SetScalarAttributes(hid_t objectId,scalarDataSet * scalar);
        void        SetVectorAttributes(hid_t objectId, vectorDataSet * vectorObject);
        char*       SetUnitDimension(char* name, hid_t attrId, hid_t attrType, hid_t attrSpace);
        double      SetUnitSI(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace);        

};