# Cristal

The Cristal cluster (BULL) is a machine of scientific post-treatment and visualization.
Cristal has several nodes containing GPUs with a large quantity of memory.

See [the Cristal documentation](https://www.cines.fr/calcul/materiels/la-machine-de-pre-post-traitement-cristal/)

First, setup your environment:

```
module load gcc/4.9.3
module load bullxmpi/1.2.9.1
module load python/gcc/2.7.11
module load visit/2.12.
module load cmake/3.7.1
```

Then, enter to generate the file `CMakeLists.txt`:
```
xml2cmake OpenPMD.xml
```
The plugin compiled files will be placed in `/home/lobetm/.visit/2.12.1/linux-x86_64/plugins/databases/`.

If you have the permission, you can create a common installation by doing:
```
xml2cmake -public -clobber OpenPMD.xml
```
The plugin compiled files will be placed in the databases directory of the VisIt installation folder.

Then, prepare the makefile by doing

```
cmake -DCMAKE_BUILD_TYPE:STRING=Debug
```

This step generates the files `CMakeFiles`, `CMakeCache.txt` and `cmake_install.cmake`.

To compile and generates the library:

```
make
```

The libraries (`libEOpenPMDDatabase_par.so`, `libEOpenPMDDatabase_ser.so`, `libIOpenPMDDatabase.so`, `libMOpenPMDDatabase.so`) are generated in the corresponding Visit directories.
