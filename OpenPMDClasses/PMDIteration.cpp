/*****************************************************************************
*
* Copyright (c) 2000 - 2015, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

/** ____________________________________________________________________________

\file PMDiteration.cpp

\brief PMDIteration class methods

\author Programmer: Mathieu Lobet
\date Creation:   Fri Oct 14 2016

\warning READ BEFORE MODIFY:
\n This file should be modified/maintained only when located in its original repository.
\n Else, this file is a copy and may not be the lastest version.
\n The modifications will not be considered.

 _______________________________________________________________________________ */

#include "PMDiteration.h"

vector <PMDIteration> iterations;

/** ____________________________________________________________________________
 Method: PMDIteration::PMDIteration

 \brief Constructor

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
PMDIteration::PMDIteration()
{
	verbose = 0;
}

/** ____________________________________________________________________________
 Method: PMDIteration::~PMDIteration

 \brief Destructor

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
PMDIteration::~PMDIteration()
{

}

/** ____________________________________________________________________________
 Method: PMDIteration::ScanFields

 \brief This method analyzes the group "fields" for the current iteration 
 and gathers all the found datasets in the vector "fields".


 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDIteration::ScanFields(hid_t fileId)
{
    hid_t    		groupId;
    hid_t    		sub_groupId;
    hid_t			dataset_id;
	char 			path[64];
	hsize_t 		nb_objects;
	hsize_t 		nb_sub_objects;
	herr_t 			err;
	int 			i;
	int 			i_sub_object;
	int 			nb_attr;
	int 			iattr;
	char 			tmp_name[64];
	char 			object_name[64];
	char 			subObjectName[64];
	ssize_t 		length;
	H5O_info_t 		object_info;
	PMDField 		field;

	strcpy (path,"/data/");
	strcat (path,name);	
	strcat (path,"/fields");

	if (verbose) cout << " Scanning field datasets in: "<< path << endl;

	// Openning of the group "fields" of the current iteration
	groupId = H5Gopen2(fileId, path , H5P_DEFAULT);


	// Get useful attributes fron the "fields" group
	field.ScanAttributes(groupId);

	/* 	
		Iteration over the objects in the group "fields"

	   	We look for the datasets and the groups.
		The idea is to make a vector containing all field datasets 
		with their properties.
		I would prefer doing it recursively (and in a more beautiful manner) 
		using the function H5Lvisit but the fact that useful attributes can 
		be contained in groups make it more tricky and less readable.
		As a consequence, I prefer doing it by "hand".
		Be prepared for an explicit but ugly code... 
		Mathieu
	*/

	// First we get the number of objects
	err = H5Gget_num_objs(groupId, &nb_objects);
	if (verbose) cout << " Number of objects in the fields group: "<< nb_objects << endl;

	// Iteration over the objects
	for (i = 0; i < nb_objects; i++)
	{
		// When an object is a group, we get useful information
		// from it and then scan the datasets inside.
		// If there are some sub-groups, they are not consider.

		// Get the object name
		length = H5Gget_objname_by_idx(groupId, (hsize_t)i, 
			object_name, (size_t) 64);

		// Get the type: group, dataset...
		// int object_type =  H5Gget_objtype_by_idx(groupId, (size_t)i );
		err = H5Oget_info_by_name(groupId, object_name , &object_info, H5P_DEFAULT);

		// Checking of the type
		switch(object_info.type)
		{
		// If group...
		case H5O_TYPE_GROUP:
			if (verbose) cout << " - Object #" << i <<": " << object_name << " (group)"<< endl;

			// Openning of the group
			sub_groupId = H5Gopen2(groupId, object_name, H5P_DEFAULT);

			// Save the first part of the name
			strcpy(tmp_name,object_name);

			// Get useful attributes from the group
			field.ScanAttributes(sub_groupId);

			// Get the number of datasets 
			err = H5Gget_num_objs(sub_groupId, &nb_sub_objects);

			// Then, we iterate over the datasets in this group
			for (i_sub_object = 0; i_sub_object < nb_sub_objects; i_sub_object++)
			{
				// Get the dataset name
				length = H5Gget_objname_by_idx(sub_groupId, (hsize_t) i_sub_object, 
					subObjectName, (size_t) 64);

				// Save the name
				strcpy(field.name,tmp_name);
				strcat(field.name,"/");
				strcat(field.name,subObjectName);

				// Save the group path
				strcpy(field.groupPath,"/data/");
				strcat(field.groupPath,this->name);
				strcat(field.groupPath,"/fields/");
				strcat(field.groupPath,tmp_name);

				// Save the path
				strcpy(field.datasetPath,field.groupPath);
				strcat(field.datasetPath,"/");
				strcat(field.datasetPath,subObjectName);

				// Openning of the dataset
				dataset_id = H5Dopen2(sub_groupId, subObjectName, H5P_DEFAULT);

				// Get useful attributes from the group
				field.ScanAttributes(dataset_id);

				// Set the grid dimension from the dataset
				field.SetGridDimensions(dataset_id);

				// Insert the field in the list of files
				fields.push_back(field);

				H5Dclose(dataset_id);

			}

			H5Gclose(sub_groupId);

		break;
		// If dataset...
		case H5O_TYPE_DATASET:
			if (verbose) cout << " - Object #" << i <<": " << object_name << " (dataset)"<< endl;

			// Let's get the useful attributes of this dataset, since it is localized in "fields"
			// it owns all its useful attributes.

			// Openning of the dataset
			dataset_id = H5Dopen2(groupId, object_name, H5P_DEFAULT);

			// Save the name
			strcpy(field.name,object_name);

			// Save the group path
			strcpy(field.groupPath,"/data/");
			strcat(field.groupPath,this->name);
			strcat(field.groupPath,"/fields");	
				
			// Save the dataset path
			strcpy(field.datasetPath,field.groupPath);
			strcat(field.datasetPath,"/");	
			strcat(field.datasetPath,object_name);

			// Scan useful attributes
			field.ScanAttributes(dataset_id);

			// Insert the field in the vector of fields
			fields.push_back(field);

			// Close the current dataset
			H5Dclose(dataset_id);

		break;

		default:
		cout << "visitLinks: node '" << name <<
		"' has an unknown type " << object_info.type << std::endl;
		break;
		}

	}
	H5Gclose(groupId);
}

/** ____________________________________________________________________________
 Method: PMDIteration::ScanParticles

 \brief This method analyzes the group "particles" for the current iteration.


 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDIteration::ScanParticles(hid_t fileId)
{

	int 			i;
	char 			path[64];
	char			objectName[64];
    hid_t    		groupId;
    hid_t			particleGroupId;
	hsize_t 		numObjects;
	ssize_t 		length;
	herr_t 			err;
	H5O_info_t 		objectInfo;

	// Creation of the path to the group "particles" having all the species
	strcpy (path,"/data/");
	strcat (path,this->name);	
	strcat (path,"/particles");

	if (verbose) cerr << " Scanning particle groups in: "<< path << endl;

	err = H5Gget_objinfo (fileId, path, 0, NULL);

	if (err!=0)
	{
		cerr << " No group named particles of path: " << path << endl;
	}
	else
	{

		// Openning of the group "particles" of the current iteration
		groupId = H5Gopen2(fileId, path , H5P_DEFAULT);	

		// First we get the number of objects
		err = H5Gget_num_objs(groupId, &numObjects);
		if (verbose) cerr << " Number of objects in the particle group: "<< numObjects << endl;

		// Iteration over the objects of the group "particles"
		for (i = 0; i < numObjects; i++)
		{

			// Create a temporary particle object
			PMDParticle		particle;

			// Get the particle group name
			length = H5Gget_objname_by_idx(groupId, (hsize_t)i, 
				objectName, (size_t) 64);

			// Get info in order to get the type: group, dataset...
			err = H5Oget_info_by_name(groupId, objectName , &objectInfo, H5P_DEFAULT);

			// Check that the object is well a group
			if (H5O_TYPE_GROUP==objectInfo.type)
			{
				// Openning of the particle group
				particleGroupId = H5Gopen2(groupId, objectName, H5P_DEFAULT);

				// Save the name
				strcpy(particle.name,objectName);

				// Save the path
				strcpy(particle.path,path);
				strcat(particle.path,"/");
				strcat(particle.path,objectName);

				// Scan properties via datasets and attributes in this particle group
				particle.ScanParticleGroup(particleGroupId);

				// Insert the particle object in the vector of particles
				this->particles.push_back(particle);

			}
		}
	}
}

/** ____________________________________________________________________________
 Method: PMDIteration::PrintInfo

 \brief This method prints the content of an object PMDIteration.


 \details This method prints the content of an object PMDIteration: 
 - field datasets
 - particle datasets

 \author Programmer: Mathieu Lobet
 \date Creation:   Fri Oct 14 2016

 Modifications:

 ____________________________________________________________________________ */
void PMDIteration::PrintInfo()
{

	int i;

	cout << " Information about iteration " << name << endl;
	cout << " - dt: "<< dt << endl;
    cout << " - time: "<< time << endl;
    cout << " - timeUnitSI: "<< timeUnitSI << endl;
    cout << endl;
	cout << " Number of field datasets: " << fields.size() << endl;
	for (std::vector<PMDField>::iterator field = fields.begin() ; field != fields.end(); ++field)
    {
    	cout << " Field name: " << field->name << endl;
    	cout << " - dataset path: " << field->datasetPath << endl;
    	cout << " - group path: " << field->groupPath << endl;
    	cout << " - dx: " << field->gridSpacing[0]<< " dy: " << field->gridSpacing[1] << " dz: " << field->gridSpacing[2] << endl;
    	cout << " - xmin: " << field->gridGlobalOffset[0]<< " ymin: " << field->gridGlobalOffset[1] << " zmin: " << field->gridGlobalOffset[2] << endl;
    	cout << " - number of dimensions: " << field->ndims << endl;
    	cout << " - nx: " << field->nbNodes[0]<< " ny: " << field->nbNodes[1] << " nz: " << field->nbNodes[2] << endl;
    	cout << " - xshift: " << field->gridPosition[0]<< " yshift: " << field->gridPosition[1] << " zshift: " << field->gridPosition[2] << endl;
    	cout << " - Unit SI: " << field->unitSI<< endl;
    	cout << " - Grid Unit SI: " << field->gridUnitSI<< endl;
    	cout << " - Geometry: " << field->geometry << endl;
    	cout << " - xlabel: " << field->axisLabels[0] << " ylabel: " << field->axisLabels[1] << " zlabel: " << field->axisLabels[2] << endl;
    	cout << " - Units: " << field->unitsLabel << endl;
    	cout << " - Data order: " << field->dataOrder << endl;
    	cout << endl;
    }
    cout << endl;
	cout << " Number of particle groups: " << particles.size() << endl;
	for (std::vector<PMDParticle>::iterator particle = particles.begin() ; particle != particles.end(); ++particle)
    {
    	cout << endl;
    	cout << " Particle name: " << particle->name << endl;
    	cout << " - charge: " << particle->charge << endl;
    	cout << " - mass: " << particle->mass << endl;
    	cout << " - number of particles: " << particle->numParticles << endl;
    	cout << " - Position datasets: " << particle->positionsId[0] << " " << particle->positionsId[1] << " "<< particle->positionsId[2]  << endl; 
    	cout << " Number of scalar datasets: " << particle->GetNumScalarDatasets() << endl;
		for (i=0;i<particle->GetNumScalarDatasets();i++)
	    {
	    	cout << " - Name: " << particle->scalarDataSets[i].name 
	    		 << ", Unit Label: " << particle->scalarDataSets[i].unitLabel 
	    		 << ", path: " << particle->scalarDataSets[i].path 
	    		 << ", unitSI: " << particle->scalarDataSets[i].unitSI
	    		 << endl;
	    }
    	cout << " Number of vector datasets: " << particle->GetNumVectorDatasets() << endl;
		for (i=0;i<particle->GetNumVectorDatasets();i++)
	    {
	    	cout << " - Name: " << particle->vectorDataSets[i].name 
	    	     << ", Unit Label: " << particle->vectorDataSets[i].unitLabel 
	    	     << ", path: " << particle->vectorDataSets[i].path
	    	     << ", scalar datasets: " << particle->vectorDataSets[i].dataSetId[0] 
	    	     << " " << particle->vectorDataSets[i].dataSetId[1] 
	    	     << " " << particle->vectorDataSets[i].dataSetId[2] 
	    	     << endl;
	    }
    }
}