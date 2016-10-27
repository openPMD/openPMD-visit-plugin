/** ____________________________________________________________________________

\file PMDField.cpp

\brief Methods of the PMDField class

\author Programmer: Mathieu Lobet
\date Creation: Fri Oct 14 2016

\warning READ BEFORE MODIFY:
\n This file should be modified/maintained only when located in its original repository.
\n Else, this file is a copy and may not be the lastest version.
\n The modifications will not be considered.

 ____________________________________________________________________________ */

#include "PMDField.h"

/** ____________________________________________________________________________
 Method: PMDField::PMDField

 \brief Constructor

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
PMDField::PMDField()
{
	verbose=0;
	unitSI=1;
	gridUnitSI=1;
	strcpy(this->name,"");
	strcpy(this->datasetPath,"");
	strcpy(this->groupPath,"");
	strcpy(this->unitsLabel,"");
	strcpy(this->axisLabels,"");
	strcpy(this->dataOrder,"");
}

/** ____________________________________________________________________________
 Method: PMDField::~PMDField

 \brief Destructor

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
PMDField::~PMDField()
{

}

/** ____________________________________________________________________________
 Method: PMDField::ScanAttributes

 \brief This method scans from a group or a dataset the attributes that the current 
 PMDField object needs.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDField::ScanAttributes(hid_t objectId)
{

	int 	numAttr;
	int 	i;
	hid_t 	attrId;
	hid_t	attrType;
	hid_t	attrSpace;
	int 	ndims;
	hsize_t sdim[64];
	herr_t 	err;
	char 	name[64];

	// Number of attributes
	numAttr = H5Aget_num_attrs(objectId);
	if (verbose) cout << "    Number of attributes: " << numAttr << endl;

	// This solution with H5Aiterate2 does not work because GetAttributeInfo needs to  
	// be static and therefore field attribute can not be modified
	// err = H5Aiterate2(objectId, H5_INDEX_NAME, H5_ITER_INC, NULL ,GetAttributeInfo, NULL);
	// But I am not an expert in C++, Mathieu

	// iteration over the attributes
    for (i = 0; i < numAttr; i++)
    {
    	// Open the attribute using its loop id.
		attrId = H5Aopen_idx(objectId, (unsigned int)i );

		// Get the name
		H5Aget_name(attrId, 64, name );

		// The type of the attribute
		attrType  = H5Aget_type(attrId); 
		// Space
	    attrSpace = H5Aget_space(attrId);
	    // Number of dimensions
	    ndims = H5Sget_simple_extent_ndims(attrSpace);
	    //
	    err = H5Sget_simple_extent_dims(attrSpace, sdim, NULL);

		if (strcmp(name,"gridSpacing")==0)
		{
			SetGridSpacing(name, attrId, attrType, attrSpace);
		}
		else if (strcmp(name,"gridGlobalOffset")==0)
		{
			SetGridGlobalOffset(name, attrId, attrType, attrSpace);
		}
		else if (strcmp(name,"position")==0)
		{
			SetGridPosition(name, attrId, attrType, attrSpace);			
		}
		else if (strcmp(name,"axisLabels")==0)
		{
			SetAxisLabels(name, attrId, attrType, attrSpace);
		}
		else if (strcmp(name,"geometry")==0)
		{
			SetGeometry(name, attrId, attrType, attrSpace);
		}
		else if (strcmp(name,"unitSI")==0)
		{
			SetUnitSI(name, attrId, attrType, attrSpace);			
		}
		else if (strcmp(name,"gridUnitSI")==0)
		{
			SetGridUnitSI(name, attrId, attrType, attrSpace);				
		}
		else if (strcmp(name,"timeOffset")==0)
		{
			// To be implemented
		}		
		else if (strcmp(name,"unitDimension")==0)
		{
			SetUnitDimension(name, attrId, attrType, attrSpace);
		}
		else if (strcmp(name,"fieldBoundary")==0)
		{
			SetFieldBoundary(name, attrId, attrType, attrSpace);
		}		
		else if (strcmp(name,"dataOrder")==0)
		{
			SetDataOrder(name, attrId, attrType, attrSpace);
		}
		else if (strcmp(name,"fieldSolver")==0)
		{
			// To be implemented
		}
		else if (strcmp(name,"chargeCorrection")==0)
		{
			// To be implemented
		}
		else if (strcmp(name,"currentSmoothing")==0)
		{
			// To be implemented
		}
		H5Aclose(attrId);
	}
}

/** ____________________________________________________________________________
 Method: PMDField::SetGridDimensions

 \brief This method opens the given dataset to determine the number of 
 points/nodes each direction for the field grid.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDField::SetGridDimensions(hid_t datasetId)
{
    // Data space
    hid_t dataset_space = H5Dget_space(datasetId);
    // Data type
    hid_t dataset_type  = H5Dget_type(datasetId);
    // Storage size
    hsize_t dataset_storage_size = H5Dget_storage_size(datasetId);
    // Dimension from the data space
    hsize_t dims[3] ;
    hsize_t maxdims[3] ;
    ndims =  H5Sget_simple_extent_dims(dataset_space, dims, maxdims );

    nbNodes[0] = dims[0];
    nbNodes[1] = dims[1];
    nbNodes[2] = dims[2];
}

/** ____________________________________________________________________________
 Method: PMDField::SetGridSpacing

 \brief This method reads from the dataset attributes GridSpacing the grid
 spacing (dx, dy, dz) of the field grid.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDField::SetGridSpacing(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        double * tmpArray = (double *)malloc(sizeof(double)*(int)npoints);

        err = H5Aread(attrId, attrType, tmpArray);

        gridSpacing[0] = tmpArray[0];
        gridSpacing[1] = tmpArray[1];		        
        gridSpacing[2] = tmpArray[2];

        free(tmpArray);
    }
}

/** ____________________________________________________________________________
 Method: PMDField::SetGridGlobalOffset

 \brief This method reads from the dataset attributes GridGlobalOffset 
 the relative offset of the field grid (usually relative to the rho grid).

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDField::SetGridGlobalOffset(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        double * tmpArray = (double *)malloc(sizeof(double)*(int)npoints);

        err = H5Aread(attrId, attrType, tmpArray);

        gridGlobalOffset[0] = tmpArray[0];
        gridGlobalOffset[1] = tmpArray[1];		        
        gridGlobalOffset[2] = tmpArray[2];

        free(tmpArray);
    }
}

/** ____________________________________________________________________________
 Method: PMDField::SetGridPosition

 \brief This method reads from the dataset attributes GridPosition 
 the origin of the grid (xmin, ymin, zmin).

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDField::SetGridPosition(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        double * tmpArray = (double *)malloc(sizeof(double)*(int)npoints);

        err = H5Aread(attrId, attrType, tmpArray);

        gridPosition[0] = tmpArray[0];
        gridPosition[1] = tmpArray[1];		        
        gridPosition[2] = tmpArray[2];

        free(tmpArray);
    }
}

/** ____________________________________________________________________________
 Method: PMDField::SetUnitSI

 \brief this method captures the arrtibute unitSI from a group or a dataset.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDField::SetUnitSI(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        err = H5Aread(attrId, attrType, &unitSI);

    }
}

/** ____________________________________________________________________________
 Method: PMDField::SetGridUnitSI

 \brief This method reads from attribute GridUnitSI the factor to get the grid units in SI.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDField::SetGridUnitSI(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        err = H5Aread(attrId, attrType, &gridUnitSI);

    }
}

/** ____________________________________________________________________________
 Method: PMDField::SetGeometry

 \brief This method reads from attribute geometry the type of grid.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDField::SetGeometry(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_STRING == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);
        char tmpchar[64];

        err = H5Aread(attrId, attrType, &tmpchar);

        if (strstr(tmpchar,"cartesian")>0)
        {
        	strcpy(geometry,"cartesian");
        }

    }
}

/** ____________________________________________________________________________
 Method: PMDField::SetAxisLabels

 \brief This method reads from attribute axisLabel the labels for each axis.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDField::SetAxisLabels(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_STRING == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        err = H5Aread(attrId, attrType, this->axisLabels);
    }
}

/** ____________________________________________________________________________
 Method: PMDField::GetUnitDimension

 \brief This method reads the UnitDimension attributes and generates
 the unitsLabel paramerer.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDField::SetUnitDimension(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
	int 	i;
	char 	buffer[64];
	char	units[8];
	int 	firstunits = 0;

    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        double * tmpArray = (double *)malloc(sizeof(double)*(int)npoints);

        err = H5Aread(attrId, attrType, tmpArray);

        strcpy(unitsLabel,"");

        for(i=0;i<7;i++)
        {

	        if (int(tmpArray[i])!=0)
	        {

	        	if (firstunits==0)
	        	{
	        		strcpy(units,"");
	        		firstunits=1;
	        	}
	        	else
	        	{
	        		strcpy(units,".");
	        	}

	        	// List of SI units
	        	switch(i)
	        	{
	        	// Distance, meter
	        	case 0:
	        		strcat(units,"m");
	        		break;
	        	// ass, kg
	        	case 1:
	        		strcat(units,"kg");
	        		break;
	        	// time, second
	        	case 2:
	        		strcat(units,"s");
	        		break;
	        	// Electric Current, Ampere
	            case 3:
	        		strcat(units,"A");
	        		break;
	        	// Temperature, Kelvin
	            case 4:
	        		strcat(units,"K");
	        		break;
				//amount of substance, mole
	            case 5:
	        		strcat(units,"mol");
	        		break;
				//luminous intensity, candela
	            case 6:
	        		strcat(units,"candela");
	        		break;
	        	}
				//amount of substance, mole
				//luminous intensity, candela

	        	if (int(tmpArray[i]) == 1)
	        	{
	        		sprintf(buffer, units, int(tmpArray[i]));
	        	}
	        	else
	        	{
	        		strcat(units,"^%d");
	        		sprintf(buffer, units, int(tmpArray[i]));
	        	}
	        	// Creation of the unitsLabel
	        	strcat(unitsLabel,buffer);
	        }        	
        }       
        free(tmpArray);
    }
}

/** ____________________________________________________________________________
 Method: PMDField::SetFieldBoundary

 \brief This method reads the field boundaries

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDField::SetFieldBoundary(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_STRING == H5Tget_class(attrType)) {

    	// Number of elements
        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        //cout << " Number of elements: " << npoints << endl;

        err = H5Aread(attrId, attrType, this->fieldBoundary);

    }
}

/** ____________________________________________________________________________
 Method: PMDField::SetFieldBoundary

 \brief This method reads the field boundaries

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDField::SetDataOrder(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_STRING == H5Tget_class(attrType)) {

    	// Number of elements
        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        //cout << " Number of elements: " << npoints << endl;

        err = H5Aread(attrId, attrType, this->dataOrder);

    }
}

/** ____________________________________________________________________________
 Method: PMDField::GetNumValues

 \brief This method returns the total number of nodes/points that compose a grid.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
int PMDField::GetNumValues()
{
	return this->nbNodes[0]*this->nbNodes[1]*this->nbNodes[2];
}
