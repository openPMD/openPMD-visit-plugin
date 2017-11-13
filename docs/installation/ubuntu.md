Installation instructions for Ubuntu
================================================================================

#### Ubuntu using executables

Download and install the Ubuntu executable of VisIt:
[VisIt](https://wci.llnl.gov/simulation/computer-codes/visit/executables).

On MacOs, you can choose to install the plugin only for you or all users.
For this aim, we will first use the xml2cmake tool located in
`<Visit application directory>/Contents/Resources/bin/`.

To install the plugin for you only:

```
xml2cmake OpenPMD.xml
```

In this case, the compiled library will be generated in the directory `~/.visit/`.

To install the plugin for all users:

```
xml2cmake -public -clobber OpenPMD.xml
```

In this case, the plugin will be located where you have installed VisIt.

This step generates the file `CMakeLists.txt` in the source directory.

Then, prepare the makefile by doing

```
cmake -DCMAKE_BUILD_TYPE:STRING=Debug
```

This step generates the files `CMakeFiles`, `CMakeCache.txt` and `cmake_install.cmake`.

To compile and generates the library:

```
make
```

You have almost finished, make sure that the libraries are well generated in
their corresponding directories. You should be able to find the
files `libEOpenPMDDatabase_ser.dylib`, `libMOpenPMDDatabase.dylib`,
 `libEOpenPMDDatabase_par.dylib`.

#### Ubuntu using the sources

In this section, we present how to install OpenPMD with the VisIt sources.

First, you need to install the following packages

```
sudo apt-get install subversion gcc gdb valgrind cmake
sudo apt-get install mpich
sudo apt-get install libgl1-mesa-dev
sudo apt-get install mesa-utils
sudo apt-get install libxt-dev
sudo apt-get install freeglut3-dev
```

In order to test that OpenGL is well installed, the following command will
launch a small test window.

```
glxgears
```

If you get this error during the installation of VisIt:
```
src/freeglut_internal.h:111:39: fatal error: X11/extensions/XInput.h: No such file or directory
```

You will have to do the following operation:
```
cd /usr/include/X11/extensions && sudo ln -s XI.h XInput.h
```

Create a directory for VisIt where you want to make the installation
(for instance your home directory).

```
mkdir visit
cd visit
```

Then, download useful VisIt files from the svn repository:

```
svn co http://visit.ilight.com/svn/visit/trunk/src/
svn co http://visit.ilight.com/svn/visit/trunk/test/
svn co http://visit.ilight.com/svn/visit/trunk/data
```

Then create the directory `deps`:

```
mkdir deps
cd deps
```

Build the files for VisIt:
```
../src/svn_bin/build_visit --system-cmake --python --hdf5 --szip --zlib --no-visit --parallel --makeflags -j8 --no-pyside --console --arch x86_64
```

Useful libraries for visit are built in `deps/visit`.
You may have to define some path by hand in case of trouble.

Copy the OpenPMD plugin in the VisIt database:

```
cd ../src/databases
git clone git@github.com:openPMD/openPMD-viewer.git
```

You can rename the plugin folder like OpenPMD.
```
mv openpmd-visit-plugin OpenPMD
```

Check that `/CMakeLists.txt` located in the plugin folder is well configured.
For this aim, look at the `CMakeLists.txt` file of the other plugins.
You can also just replace the file by `CMakeLists/CMakeLists.txt`.

Then, open the file `CMakeLists.txt` located in the `src/databases` directory and add the openPMD plugin
name to the list after `SET(REQUIRED_DATABASE_PLUGINS`.

Prepare the makefile:

```
cd ../..
mkdir build
cd build
cmake -DVISIT_CONFIG_SITE=../deps/*.cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../install  ../src
```

Make VisIt:

```
make -j 8
```

Use this command to launch VisIt with Valgrind:

```
./bin/visit -xterm -valgrind engine_ser
```
