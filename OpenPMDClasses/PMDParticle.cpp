/** ____________________________________________________________________________

\file PMDParticle.cpp

\brief PMDParticle Class methods

\author Programmer: Mathieu Lobet
\date Creation:   Fri Oct 14 2016

\warning READ BEFORE MODIFY:
\n This file should be modified/maintained only when located in its original repository.
\n Else, this file is a copy and may not be the lastest version.
\n The modifications will not be considered.

 ____________________________________________________________________________ */

#include "PMDParticle.h"

/** ____________________________________________________________________________
 Method: PMDParticle::PMDParticle

\brief constructor

\author Programmer: Mathieu Lobet

\date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
PMDParticle::PMDParticle()
{
	int i;

	// Verbose behavior
	this->verbose=0;
	// If there is momentum datasets
	this->momentumAvailable = 0;
	// Index of the scalar position data sets
	for(i=0;i<3;i++)
	{
		this->positionsId[i]=-1;
	}
	// Number of momentum dimensions
	this->numDimsMomenta = 0;
	// Number of position dimensions
	this->numDimsPositions = 0;
}

/** ____________________________________________________________________________
 Method: PMDParticle::~PMDParticle

\brief destructor

\author Programmer: Mathieu Lobet

\date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
PMDParticle::~PMDParticle()
{

}

/** ____________________________________________________________________________
 Method: PMDParticle::ScanProperties(hid_t particleGroupId)

 \brief This method scans the attributes, the groups and datasets contained 
 		in a given particle group to build all properties of the particle object.

 \author Programmer: Mathieu Lobet

 \date 	Creation: Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDParticle::ScanParticleGroup(hid_t particleGroupId)
{
	// Local variables
	int 			i;
	char			objectName[64];
	ssize_t 		length;
	hsize_t 		numObjects;
	herr_t 			err;
	H5O_info_t 		objectInfo;

	// Get the number of objects in the current particle group 
	err = H5Gget_num_objs(particleGroupId, &numObjects);

	// Iteration over the objects of the group "particles"
	for (i = 0; i < numObjects; i++)
	{
		// Get the particle group name
		length = H5Gget_objname_by_idx(particleGroupId, (hsize_t)i, 
			objectName, (size_t) 64);	
			
		// Get info in order to get the type: group, dataset...
		err = H5Oget_info_by_name(particleGroupId, objectName , &objectInfo, H5P_DEFAULT);

		// Checking of the type
		switch(objectInfo.type)
		{
		// If the object is a group...
		case H5O_TYPE_GROUP:

			// We first treat the groups that we can recognize
			// Analyzing of the charge group
			if (strcmp(objectName,"charge")==0)
			{
				this->ScanCharge(particleGroupId,objectName);
			}
			// Analyzing of the mass group
			else if (strcmp(objectName,"mass")==0)
			{
				this->ScanMass(particleGroupId,objectName);
			}
			// Special treatment for the positions
			else if (strcmp(objectName,"position")==0)
			{
				this->ScanPositions(particleGroupId,objectName);
			}
			// Special treatment for the momenta
			else if (strcmp(objectName,"momentum")==0)
			{
				this->ScanMomenta(particleGroupId,objectName);
			}
			// Position Offset
			else if (strcmp(objectName,"positionOffset")==0)
			{
				// This is not implemented
			}
			// Then, we analyse all the other groups and their datasets.
			// We create the corresponding vector and scalar data
			else
			{
				this->ScanGroup(particleGroupId,objectName);
			}
			break;

		// If the object is a dataset...
		case H5O_TYPE_DATASET:

			// Datasets at the root of the particle group are treated as scalar data
			this->ScanDataSet(particleGroupId,objectName);
			break;

		default:

			cerr << " Non-valid object type while scanning the 'particles' group: " << objectName << ", this object is ignored." << endl;

			break;
		}

	}

}

/** ____________________________________________________________________________
 Method: PMDParticle::ScanCharge

 \brief This method scans the group `charge` contained in the particle groups and get 
 the useful attributes.

 \author Programmer: Mathieu Lobet

 \date 	Creation: Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDParticle::ScanCharge(hid_t particleGroupId, char * objectName)
{

	int 			i;
	int 			numAttr;
	int 			ndims;
	char 			attrName[64];
	hsize_t 		numObjects;
    hid_t    		chargeGroupId;
	hid_t 			attrId;
	hid_t			attrType;
	hid_t			attrSpace;
	herr_t 			err;
	hsize_t 		sdim[64];

	if (verbose) cerr << " Get attributes from: " << objectName << endl;

	// Openning of the group charge
	chargeGroupId = H5Gopen2(particleGroupId, objectName , H5P_DEFAULT);

	// First, get the useful attributes

	// Number of attributes
	numAttr = H5Aget_num_attrs(chargeGroupId);

	// Iteration over the attributes
    for (i = 0; i < numAttr; i++)
    {
    	// Open the attribute using its loop id.
		attrId = H5Aopen_idx(chargeGroupId, (unsigned int)i );

		// Get the attribute name
		H5Aget_name(attrId, 64, attrName );

		//cerr << attrName << endl;

		// The type of the attribute
		attrType  = H5Aget_type(attrId); 
		// Space
	    attrSpace = H5Aget_space(attrId);
	    // Number of dimensions
	    ndims = H5Sget_simple_extent_ndims(attrSpace);
	    //
	    err = H5Sget_simple_extent_dims(attrSpace, sdim, NULL);

		if (strcmp(attrName,"value")==0)
		{
			// Check it is a 32-bit float
			if (H5T_FLOAT == H5Tget_class(attrType)) 
			{
				err = H5Aread(attrId, attrType, &this->charge);
				//cerr << "charge value: "<< this->charge << endl;
			}
			else
			{
				cerr << " Particle charge is not a 32bit-float" << endl;
			}
		}
		else if (strcmp(attrName,"shape")==0)
		{
			// Check if this parameter is an integer
			// Check it is a 32-bit float
			if (H5T_INTEGER== H5Tget_class(attrType)) 
			{
				err = H5Aread(attrId, attrType, &this->numParticles);
			}
			else
			{
				cerr << " Particle shape is not an integer" << endl;
			}
		}
		H5Aclose(attrId);
    }

	// Then, get the number of objects in the current group
	err = H5Gget_num_objs(chargeGroupId, &numObjects);

	// If no object, the charge is the same for all particles of this group
	if (numObjects>0)
	{
		this->charge = 0;
		// Read the dataset of charges
	}

}

/** ____________________________________________________________________________
 Method: PMDParticle::ScanMass

 \brief This method scans the group `charge` contained in the particle groups and get 
 the useful attributes.

 \author Programmer: Mathieu Lobet

 \date Creation:   Fri Oct 14  2016

 Modifications:

 ____________________________________________________________________________ */
void PMDParticle::ScanMass(hid_t particleGroupId, char * objectName)
{
	int 			i;
	int 			numAttr;
	int 			ndims;
	char 			attrName[64];
	hsize_t 		numObjects;
    hid_t    		chargeGroupId;
	hid_t 			attrId;
	hid_t			attrType;
	hid_t			attrSpace;
	herr_t 			err;
	hsize_t 		sdim[64];

	if (verbose) cerr << " Get attributes from: " << objectName << endl;

	// Openning of the group charge
	chargeGroupId = H5Gopen2(particleGroupId, objectName , H5P_DEFAULT);

	// First, get the useful attributes

	// Number of attributes
	numAttr = H5Aget_num_attrs(chargeGroupId);

	// Iteration over the attributes
    for (i = 0; i < numAttr; i++)
    {
    	// Open the attribute using its loop id.
		attrId = H5Aopen_idx(chargeGroupId, (unsigned int)i );

		// Get the attribute name
		H5Aget_name(attrId, 64, attrName );

		//cerr << attrName << endl;

		// The type of the attribute
		attrType  = H5Aget_type(attrId); 
		// Space
	    attrSpace = H5Aget_space(attrId);
	    // Number of dimensions
	    ndims = H5Sget_simple_extent_ndims(attrSpace);
	    //
	    err = H5Sget_simple_extent_dims(attrSpace, sdim, NULL);

		if (strcmp(attrName,"value")==0)
		{
			// Check it is a 32-bit float
			if (H5T_FLOAT == H5Tget_class(attrType)) 
			{
				err = H5Aread(attrId, attrType, &this->mass);
				//cerr << "charge value: "<< this->charge << endl;
			}
			else
			{
				cerr << " Particle mass is not a 32bit-float" << endl;
			}
		}
		H5Aclose(attrId);
    }

	// Then, get the number of objects in the current group
	err = H5Gget_num_objs(chargeGroupId, &numObjects);

	// If no object, the charge is the same for all particles of this group
	if (numObjects>0)
	{
		this->mass = 0;
		// Read the dataset of charges
	}
}

/** ____________________________________________________________________________
 Method: PMDParticle::ScanPositions

 \brief This method scans the group `Position` located in the particle 
 groups for each iteration.

 \details This method reads and store the useful attributes from the 
 group itself and the datasets. A `scalarDataSet` structure object is created for each dataset 
 and is put in the vector of *scalar* data `this->scalarDataSets` (member of the class `Particle`).

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDParticle::ScanPositions(hid_t particleGroupId, char * objectName)
{

	int 			i;
    char 			bufferName[64];
    char 			datasetName[64];	
	ssize_t 		length;
	hsize_t 		numObjects;
    hid_t    		groupId;
    hid_t			dataSetId;
    scalarDataSet 	scalar;	
	herr_t 			err;
	H5O_info_t 		objectInfo;

	if (verbose) cerr << " PMDParticle::SetPositions(" << objectName << ")" << endl;

	strcpy(bufferName,this->name);
	strcat(bufferName,"/position/");

	// Openning of the group Position
	groupId = H5Gopen2(particleGroupId, objectName , H5P_DEFAULT);

	// First, we get the useful attributes from the position group
	SetScalarAttributes(groupId,&scalar);

	// Then, we scan the different datasets contained in the position group:

	// Get the number of objects
	err = H5Gget_num_objs(groupId, &numObjects);

	// Iteration over the objects of the group "particles"
	for (i = 0; i < numObjects; i++)
	{
		// Get the object name
		length = H5Gget_objname_by_idx(groupId, (hsize_t)i, 
			datasetName, (size_t) 64);	
			
		// Get info in order to get the type: group, dataset...
		err = H5Oget_info_by_name(groupId, datasetName , &objectInfo, H5P_DEFAULT);

		// We check that the object is well a dataset, else, this object is ignored
		if (objectInfo.type == H5O_TYPE_DATASET)
		{

			// Open the dataset for the attributes
			dataSetId = H5Dopen2(groupId, datasetName , H5P_DEFAULT);

			// Get attributes from the dataset
			SetScalarAttributes(dataSetId,&scalar);

			// Name
			strcpy(scalar.name,bufferName);
			strcat(scalar.name,datasetName);

			// Path
			strcpy(scalar.path,this->path);
			strcat(scalar.path,"/");
			strcat(scalar.path,objectName);
			strcat(scalar.path,"/");	
			strcat(scalar.path,datasetName);

			H5Dclose(dataSetId);
		}

		// We add this scalar object to the list of scalar datasets
		this->scalarDataSets.push_back(scalar);

		// We store the index of the position datasets in positionsId to find them easily
		if (strcmp(datasetName,"x")==0)
		{
			// index of the dataset x in the list of datsets
			this->positionsId[0] = this->scalarDataSets.size()-1;
			// Add a new dimension
			this->numDimsPositions += 1;
		}
		if (strcmp(datasetName,"y")==0)
		{
			// index of the dataset y in the list of datsets
			this->positionsId[1] = this->scalarDataSets.size()-1;
			// Add a new dimension
			this->numDimsPositions += 1;
		}
		if (strcmp(datasetName,"z")==0)
		{
			// index of the dataset z in the list of datsets
			this->positionsId[2] = this->scalarDataSets.size()-1;
			// Add a new dimension
			this->numDimsPositions += 1;
		}
	}

	H5Gclose(groupId);
}

/** ____________________________________________________________________________
 Method: PMDParticle::ScanMomenta

 \brief This method scans the group `Momentum` located in the particle 
 groups for each iteration.

 \details This method analyzes the group `momentum` for a given particle group.
 The group `Momentum`, when existing, provides 3 datasets: x, y, z in 3D.
 These datasets can be used to build vtk scalar lists, vtk vectors (p) 
 and vtk expressions (|p| and gamma).

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDParticle::ScanMomenta(hid_t particleGroupId, char * objectName)
{

	int 			i;
	int 			vectorDataSetId[3] = {-1,-1,-1};
    char 			bufferName[64];
    char 			datasetName[64];
	ssize_t 		length;
    hid_t    		groupId;
    hid_t    		dataSetId;
    scalarDataSet 	scalar;
    vectorDataSet	vector;
	hsize_t 		numObjects;
	herr_t 			err;
	H5O_info_t 		objectInfo;

	if (verbose) cerr << " PMDParticle::SetMomenta(" << objectName << ")" << endl;

	// Momenta become available
	this->momentumAvailable = 1;

	strcpy(bufferName,this->name);
	strcat(bufferName,"/momentum");

	// Openning of the group Momentum
	groupId = H5Gopen2(particleGroupId, objectName , H5P_DEFAULT);

	// First, we get the useful attributes from the momentum group
	// For the scalar datasets
	SetScalarAttributes(groupId,&scalar);

	// For the vector datasets
	SetVectorAttributes(groupId,&vector);

	// Then, we scan the different datasets contained in the position group:

	// Get the number of objects
	err = H5Gget_num_objs(groupId, &numObjects);

	// Iteration over the objects of the group "particles"
	for (i = 0; i < numObjects; i++)
	{
		// Get the object name
		length = H5Gget_objname_by_idx(groupId, (hsize_t)i, 
			datasetName, (size_t) 64);

		// Get info in order to get the type: group, dataset...
		err = H5Oget_info_by_name(groupId, datasetName , &objectInfo, H5P_DEFAULT);

		// We check that the object is well a dataset, else, this object is ignored
		if (objectInfo.type == H5O_TYPE_DATASET)
		{

			// Open the dataset for the attributes
			dataSetId = H5Dopen2(groupId, datasetName , H5P_DEFAULT);

			// Get attributes from the dataset
			SetScalarAttributes(dataSetId,&scalar);

			// Name
			strcpy(scalar.name,bufferName);
			strcat(scalar.name,"/");
			strcat(scalar.name,datasetName);

			// Path
			strcpy(scalar.path,this->path);
			strcat(scalar.path,"/");
			strcat(scalar.path,objectName);
			strcat(scalar.path,"/");	
			strcat(scalar.path,datasetName);

			// We add this scalar object to the vector of scalar datasets
			this->scalarDataSets.push_back(scalar);

			// We keep the position of the datasets for the vector construction
			if (strcmp(datasetName,"x")==0)
			{
				vector.dataSetId [0] = this->scalarDataSets.size()-1;
				// Add a new dimension
				this->numDimsMomenta += 1;
			}
			else if (strcmp(datasetName,"y")==0)
			{
				vector.dataSetId [1] = this->scalarDataSets.size()-1;
				// Add a new dimension
				this->numDimsMomenta += 1;
			}
			else if (strcmp(datasetName,"z")==0)
			{
				vector.dataSetId [2] = this->scalarDataSets.size()-1;
				// Add a new dimension
				this->numDimsMomenta += 1;
			}
		}
	}

	// Vector Name
	strcpy(vector.name,bufferName);

	// Vector Path
	strcpy(vector.path,this->path);
	strcat(vector.path,"/");
	strcat(vector.path,objectName);
	strcat(vector.path,"/");

	// We add this vector object to the vector of vector datasets
	this->vectorDataSets.push_back(vector);

}

/** ____________________________________________________________________________
 Method: PMDParticle::ScanGroup

\brief This method scans a particle group and add the resulting scalar and vector data 
		respectively to the vectors `scalarDataSets` and `vectorDataSets`.

\author Programmer: Mathieu Lobet
\date Creation:   Fri Oct 21 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDParticle::ScanGroup(hid_t particleGroupId,char * objectName)
{

	int 			i;
	int 			vectorDataSetId[3] = {-1,-1,-1};
    char 			datasetName[128];
	ssize_t 		length;	
    hid_t    		groupId;
    hid_t    		dataSetId;
    scalarDataSet 	scalar;
    vectorDataSet	vector;
	hsize_t 		numObjects;
	herr_t 			err;
	H5O_info_t 		objectInfo;

	if (verbose) cerr << " PMDParticle::ScanGroup(" << objectName << ")" << endl;

	// Openning of the corresponding group
	groupId = H5Gopen2(particleGroupId, objectName , H5P_DEFAULT);

	// First, we get the useful attributes from the group
	// For the scalar datasets
	SetScalarAttributes(groupId,&scalar);

	// For the vector datasets
	SetVectorAttributes(groupId,&vector);

	// Then, we scan the different datasets contained in the position group:

	// Get the number of objects
	err = H5Gget_num_objs(groupId, &numObjects);

	// Iteration over the objects of the group "particles"
	for (i = 0; i < numObjects; i++)
	{
		// Get the object name
		length = H5Gget_objname_by_idx(groupId, (hsize_t)i, 
			datasetName, (size_t) 64);	

		// Get info in order to get the type: group, dataset...
		err = H5Oget_info_by_name(groupId, datasetName , &objectInfo, H5P_DEFAULT);

		// We check that the object is well a dataset, else, this object is ignored
		if (objectInfo.type == H5O_TYPE_DATASET)
		{

			// Open the dataset for the attributes
			dataSetId = H5Dopen2(groupId, datasetName , H5P_DEFAULT);

			// Get attributes from the dataset
			SetScalarAttributes(dataSetId,&scalar);

			// Name
			strcpy(scalar.name,this->name);
			strcat(scalar.name,"/");
			strcat(scalar.name,objectName);
			strcat(scalar.name,"/");
			strcat(scalar.name,datasetName);

			// Path
			strcpy(scalar.path,this->path);
			strcat(scalar.path,"/");
			strcat(scalar.path,objectName);
			strcat(scalar.path,"/");	
			strcat(scalar.path,datasetName);

			// We add this scalar object to the vector of scalar datasets
			this->scalarDataSets.push_back(scalar);

			// We keep the position of the datasets for the vector construction
			// First vector dimension
			if (strcmp(datasetName,"x")==0)
			{
				vector.dataSetId [0] = this->scalarDataSets.size()-1;
			}
			// Second vector dimension
			else if (strcmp(datasetName,"y")==0)
			{
				vector.dataSetId [1] = this->scalarDataSets.size()-1;
			}
			// Third vector dimension
			else if (strcmp(datasetName,"z")==0)
			{
				vector.dataSetId [2] = this->scalarDataSets.size()-1;
			}

		}

	}

	// Vector Name
	strcpy(vector.name,this->name);
	strcat(vector.name,"/");
	strcat(vector.name,objectName);	

	// Vector Path
	strcpy(vector.path,this->path);
	strcat(vector.path,"/");
	strcat(vector.path,objectName);
	strcat(vector.path,"/");

	// We add this vector object to the vector of vector datasets
	this->vectorDataSets.push_back(vector);	

}

/** ____________________________________________________________________________
 Method: PMDParticle::ScanDataSet

\brief This method scans a particle dataset and add the resulting scalar data 
		to the vector scalarDataSets.

\author Programmer: Mathieu Lobet
\date Creation:   Fri Oct 21 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDParticle::ScanDataSet(hid_t particleGroupId,char * objectName)
{

    scalarDataSet 	scalar;
    hid_t    		dataSetId;

	if (verbose) cerr << " PMDParticle::ScanDataSet(" << objectName << ")" << endl;

	// Openning of the dataset
	dataSetId = H5Dopen2(particleGroupId, objectName , H5P_DEFAULT);

	// We get the useful attributes from the dataset
	SetScalarAttributes(dataSetId,&scalar);

	// Name
	strcpy(scalar.name,this->name);
	strcat(scalar.name,"/");
	strcat(scalar.name,objectName);

	// Path
	strcpy(scalar.path,this->path);
	strcat(scalar.path,"/");
	strcat(scalar.path,objectName);

	// We add this scalar object to the vector of scalar datasets
	this->scalarDataSets.push_back(scalar);

}

/** ____________________________________________________________________________
 Method: PMDParticle::SetScalarAttributes

\brief This method scans the attributes of the object of id objectId and return it 
 in the scalarDataSet data structure scalarObject.

\author Programmer: Mathieu Lobet
\date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDParticle::SetScalarAttributes(hid_t objectId, scalarDataSet * scalarObject)
{
	int 			i;
	int 			numAttr;
	char 			attrName[64];
	hid_t 			attrId;
	hid_t 			attrType;
	hid_t 			attrSpace;

	if (verbose) cerr << " PMDParticle::SetVarAttributes" << endl;

	// Number of attributes
	numAttr = H5Aget_num_attrs(objectId);

	// Iteration over the attributes
    for (i = 0; i < numAttr; i++)
    {
    	// Open the attribute using its loop id.
		attrId = H5Aopen_idx(objectId, (unsigned int)i );

		// The type of the attribute
		attrType  = H5Aget_type(attrId); 
		// Space
	    attrSpace = H5Aget_space(attrId);

		// Get the attribute name
		H5Aget_name(attrId, 64, attrName);

		// Read useful attributes
		if (strcmp(attrName,"unitDimension")==0)
		{
			strcpy(scalarObject->unitLabel,"");
			strcat(scalarObject->unitLabel,this->SetUnitDimension(attrName, attrId, attrType, attrSpace));
		}
		else if (strcmp(attrName,"unitSI")==0)		
		{
			scalarObject->unitSI = SetUnitSI(attrName, attrId, attrType, attrSpace);
		}
    }
}

/** ____________________________________________________________________________
 Method: PMDParticle::SetVectorAttributes

 This method scan the attributes of the object of id objectId and return it 
 in the vectorDataSet data structure vectorObject.

 Programmer: Mathieu Lobet
 Creation:   Fri Oct 21 2016

 Modifications:

// ____________________________________________________________________________ */
void PMDParticle::SetVectorAttributes(hid_t objectId, vectorDataSet * vectorObject)
{
	int 			i;
	int 			numAttr;
	char 			attrName[64];
	hid_t 			attrId;
	hid_t 			attrType;
	hid_t 			attrSpace;

	if (verbose) cerr << " PMDParticle::SetVarAttributes" << endl;

	// Number of attributes
	numAttr = H5Aget_num_attrs(objectId);

	// Iteration over the attributes
    for (i = 0; i < numAttr; i++)
    {
    	// Open the attribute using its loop id.
		attrId = H5Aopen_idx(objectId, (unsigned int)i );

		// The type of the attribute
		attrType  = H5Aget_type(attrId); 
		// Space
	    attrSpace = H5Aget_space(attrId);

		// Get the attribute name
		H5Aget_name(attrId, 64, attrName);

		if (strcmp(attrName,"unitDimension")==0)
		{
			strcpy(vectorObject->unitLabel,"");
			strcat(vectorObject->unitLabel,SetUnitDimension(attrName, attrId, attrType, attrSpace));
		}

    }
}

/** ____________________________________________________________________________
 Method: PMDParticle::SetUnitSI

 \brief this method captures the arrtibute unitSI from a group or a dataset.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
double PMDParticle::SetUnitSI(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
	double unitSI = 0;
    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        err = H5Aread(attrId, attrType, &unitSI);

    }
    else
    {
    	cerr << " PMDParticle::GetUnitSI: unitSI not a float" << endl;
    }
    return unitSI;
}

/** ____________________________________________________________________________
 Method: PMDParticle::GetUnitDimension

 \brief This method reads the UnitDimension attributes and returns
 the unitsLabel paramerer.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
char* PMDParticle::SetUnitDimension(char * name, hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
	int 	i;
	char	buffer[8];
	char	units[8];
	char* 	unitLabel;
	int 	firstunits = 0;

	unitLabel = new char[64];
	strcpy(unitLabel,"");

    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        double * tmpArray = (double *)malloc(sizeof(double)*(int)npoints);

        err = H5Aread(attrId, attrType, tmpArray);

        for(i=0;i<7;i++)
        {

	        if (int(tmpArray[i])!=0)
	        {

	        	// If this is the first units, then we don't put a dot, 
	        	// else there is a dot between two units
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

				strcpy(buffer,"");
	        	if (int(tmpArray[i]) == 1)
	        	{
	        		strcat(buffer, units);
	        	}
	        	else
	        	{
	        		strcat(units,"^%d");
	        		sprintf(buffer, units, int(tmpArray[i]));
	        	}
	        	strcat(unitLabel, buffer);
	        }        	
        }       
        free(tmpArray);
    }

    return unitLabel;
}

/** ____________________________________________________________________________
 Method: PMDParticle::GetNumScalarDatasets

 \brief This method return the number of scalar datasets stored in the vector 
 this->scalarDataSets.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
int PMDParticle::GetNumScalarDatasets()
{
	return this->scalarDataSets.size();
}

/** ____________________________________________________________________________
 Method: PMDParticle::GetNumScalarDatasets

 \brief This method return the number of vector datasets stored in the vector 
 this->scalarDataSets.

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
int PMDParticle::GetNumVectorDatasets()
{
	return this->vectorDataSets.size();
}

