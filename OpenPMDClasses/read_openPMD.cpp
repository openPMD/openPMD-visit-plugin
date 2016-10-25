/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at the  *
 * root level of an installed copy of the electronic HDF5 document set and   *
 * is linked from the top-level documents page.  It can also be found at     *
 * https://support.hdfgroup.org/HDF5/doc/Copyright.html.  If you do not have          *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * This example creates a group in the file and dataset in the group.
 * Hard link to the group object is created and the dataset is accessed
 * under different names.
 * Iterator function is used to find the object names in the root group.
 * Note that the C++ API iterator function is not completed yet, thus
 * the C version is used in this example.
 */

#include "PMDFile.h"

#ifdef OLD_HEADER_FILENAME
#include <iostream.h>
#else
#include <iostream>
#endif
#include <string>
#include <stdlib.h>

#include <stdio.h>

#ifndef H5_NO_NAMESPACE
#ifndef H5_NO_STD
    using std::cout;
    using std::endl;
#endif  // H5_NO_STD
#endif

#include "H5Cpp.h"
//#include "hdf5.h"
//#include <visit-hdf5.h>

#ifndef H5_NO_NAMESPACE
    using namespace H5;
#endif

const H5std_string FILE_NAME( "data00001000.pmd" );
const int	   RANK = 2;

// Operator function
extern "C" herr_t get_iteration_info(hid_t loc_id, const char *name, const H5L_info_t *linfo,
    void *opdata);

// Operator function
extern "C" herr_t get_field_info(hid_t loc_id, const char *name, const H5L_info_t *linfo,
    void *opdata);

int main(void)
{

    hsize_t  	dims[2];
    hsize_t  	cdims[2];
    hsize_t  	nobj;
    hid_t    	fileId;
    hid_t    	groupId;
    hid_t  		datasetId;

    // Try block to detect exceptions raised by any of the calls inside it
    try
    {
	/*
	 * Turn off the auto-printing when failure occurs so that we can
	 * handle the errors appropriately
	 */
	Exception::dontPrint();

    //hid_t fileAccessPropListID = H5Pcreate(H5P_FILE_ACCESS);

    //herr_t err = H5Pset_fclose_degree(fileAccessPropListID, H5F_CLOSE_SEMI);

	//H5File * file = new H5File(FILE_NAME, H5F_ACC_RDWR);
	//fileId = H5Fopen("data00001000.pmd", H5F_ACC_RDONLY, H5P_DEFAULT);

	// scanIterations(fileId);

	// printIterations();

	// for (std::vector<PMDIteration>::iterator it = iterations.begin() ; it != iterations.end(); ++it)
	// {
	// 	it->ScanFields(fileId);
	// 	it->PrintInfo();
	// }

	char path[64];
	strcpy(path, "data00001000.pmd");

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
	//float * array = new float[1016];
	//char dataSetPath[128] = "/data/1000/particles/beam/position/x";
	//openPMDFile.ReadScalarDataSet(array,1016,H5T_FLOAT,dataSetPath);
	//for (int i=0;i<1016;i++)
	//{
	//	cout << array[i] << endl;	
	//}

	// Print the content
	openPMDFile.Print();

	// Close the file
	openPMDFile.CloseFile();

	/* ______________________________________________________________ */
	/* Test and examples */

	/* Example: How to read iteration attributes */

	// OpenPMD files always contain a data group at the root
	//Group* group_iterations = new Group(file->openGroup("data"));
	// groupId = H5Gopen(fileId, "/data",H5P_DEFAULT);

	//H5Gget_num_objs(group_iterations->getId(), &nobj);
	// H5Gget_num_objs(groupId, &nobj);
	// cout << "Number of iterations: " << nobj << endl;

	/*
	 * Use iterator to see all iterations contained in the file
	 */
	//cout << endl << "List of iterations" << endl;
	//herr_t idx = H5Literate(group_iterations->getId(), H5_INDEX_NAME, H5_ITER_INC, NULL, get_iteration_info, NULL);
	// herr_t idx = H5Literate(groupId, H5_INDEX_NAME, H5_ITER_INC, NULL, get_iteration_info, NULL);
	// cout << endl;

	/* Example: Iterate recursively in fields */
	// cout << " Example: Iterate recursively in fields" << endl;
	// groupId = H5Gopen(fileId, "/data/1000/fields/",H5P_DEFAULT);
	// herr_t errId = H5Lvisit(groupId, H5_INDEX_NAME, H5_ITER_INC, get_field_info, NULL);
	// if (errId < 0)
	// {
	// 	cout << "H5visit error" << endl;
	// }
	// cout << endl;

	/* Example: Read information from dataSet rho */
	// datasetId = H5Dopen(fileId,"/data/1000/fields/rho",H5P_DEFAULT);
	// int num_attr = H5Aget_num_attrs(datasetId);
    // cout << " Number of attributes for rho: " << num_attr << endl;
    // // Data space
    // hid_t datasetSpace = H5Dget_space(datasetId);
    // // Data type
    // hid_t datasetType  = H5Dget_type(datasetId);
    // // Storage size (in bytes)
    // hsize_t datasetStorageSize = H5Dget_storage_size(datasetId);
    // // Dimension from the data space
    // int ndims        = H5Sget_simple_extent_ndims(datasetSpace);
    // // Class
    // hid_t datasetClass = H5Tget_class(datasetType) ;

	// cout << " datasetStorageSize: " << datasetStorageSize << " bytes" << endl;
	// cout << " ndims: " << ndims << endl;
	// cout << " dataSpace: " << datasetSpace << endl;


	/*
	 * Close the group and file.
	 */
	//delete file;
	//delete group_iterations;
    }  // end of try block

    // catch failure caused by the H5File operations
    catch( FileIException error )
    {
	error.printError();
	return -1;
    }

    // catch failure caused by the DataSet operations
    catch( DataSetIException error )
    {
	error.printError();
	return -1;
    }

    // catch failure caused by the DataSpace operations
    catch( DataSpaceIException error )
    {
	error.printError();
	return -1;
    }

    // catch failure caused by the Attribute operations
    catch( AttributeIException error )
    {
	error.printError();
	return -1;
    }
    return 0;
}

/*
 * Operator function.
 */
herr_t
get_iteration_info(hid_t loc_id, const char *name, const H5L_info_t *linfo, void *opdata)
{
    hid_t group;
    int na;
    int i;
    hid_t aid;
    char buf_attr_name[20]; 
    char buf_attr_val[20]; 
    double val;
    hid_t atype;
	hid_t aspace;

    /*
     * Open the group using its name.
     */
    group = H5Gopen2(loc_id, name, H5P_DEFAULT);

    /*
     * Display group information.
     */
    cout << "Iteration" <<endl;
    cout << " Name: " << name << endl;

    /*
     * List iteration group attributes
	 */
    na = H5Aget_num_attrs(group);
    cout << " Number of attributes: " << na << endl;
    for (i = 0; i < na; i++)
    {

		aid = H5Aopen_idx(group, (unsigned int)i );
		
		H5Aget_name(aid, 20, buf_attr_name );

		/* the dimensions of the attribute data */
		aspace = H5Aget_space(aid);

		/* The type of the attribute */
		atype  = H5Aget_type(aid); 

		/* Read attribute */
		H5Aread (aid, atype, &val);
		cout << " " << buf_attr_name << "=" << val << endl;
		H5Aclose(aid);
	}
    H5Gclose(group);
    return 0;
}

herr_t
get_field_info(hid_t loc_id, const char *name, const H5L_info_t *linfo, void *opdata)
{

	H5O_info_t object_info;
	herr_t err;

	cout << " Name: " << name << " in " << loc_id << endl;

	// Get info
	err = H5Oget_info_by_name(loc_id, name, &object_info, H5P_DEFAULT);

	// types
	// This function is depreciated int type = H5Gget_objtype_by_idx(loc_id, (size_t)i );
	//cout << object_info.type << endl;

	// Check the type (group or dataset)

	switch(object_info.type)
	{
	// If group
	case H5O_TYPE_GROUP:
		cout << "This is a group" << endl;;

	break;
	// If dataset
	case H5O_TYPE_DATASET:
		cout << "This is a dataset" << endl;;
	break;

	default:
	cout << "visitLinks: node '" << name <<
	"' has an unknown type " << object_info.type << std::endl;
	break;
	}

	return 0;
}

