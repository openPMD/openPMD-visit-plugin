#### NERSC

To use Visit on NERSC machines, it is recommended to run Visit on your local computer in client-server mode. Intructions can be found [by clicking on this link](http://www.nersc.gov/users/data-analytics/data-visualization/visit-2/).

First, setup your environment:

```
module swap PrgEnv-intel PrgEnv-gnu-VisIt
module unload darshan
module unload craype
```

Then, load the visit module:

```
module load visit/<your favorite version>
```

Then load the last version of cmake:

```
module load cmake/<the last version number>
```

Then, enter to generate the file `CMakeLists.txt`:

```
xml2cmake OpenPMD.xml
```

If you have the permission, you can create a common installation by doing:

```
xml2cmake -public -clobber OpenPMD.xml
```

Then, prepare the makefile by doing

```
cmake -DCMAKE_BUILD_TYPE:STRING=Debug
```

This step generates the files `CMakeFiles`, `CMakeCache.txt` and `cmake_install.cmake`.

To compile and generates the library:

```
make
```

The libraries (`libEOpenPMDDatabase_par.so`, `libEOpenPMDDatabase_ser.so`, `libIOpenPMDDatabase.so`, `libMOpenPMDDatabase.so`) are located in the following directory:

```
/global/homes/<first login letter>/<login>/.visit/<VisIt version>/linux-x86_64/plugins/databases/
```
