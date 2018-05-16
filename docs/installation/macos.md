Installation instructions for Ubuntu
================================================================================

## MacOs using executables

First, download and install the MacOs executable (.dmg image)
[VisIt](https://wci.llnl.gov/simulation/computer-codes/visit/executables).
(Please note that we recommend version 2.11: this has been used successfully with this plugin
on High Sierra.)

After downloading/cloning the plugin source, we will use the `xml2cmake` tool located in
```
<Visit application directory>/Contents/Resources/bin/
```
where `<Visit application directory>`
would usually be `/Applications/VisIt.app`. You can either use this directly, or add an alias
in your `~/.profile`.

To generate the `CMakeLists.txt`, run: 

```
xml2cmake -public -clobber OpenPMD.xml
```

Note that you may need to prepend a path to a Python 2 interpreter for this step.
This step generates the file `CMakeLists.txt` in the source directory.

If your system uses cmake version >= 3 (you can check using `cmake --version`) then it is
necessary to add `SET(CMAKE_MACOSX_RPATH 1)` to the head of `CMakeLists.txt`.

Then, prepare the makefile by doing

```
cmake -DCMAKE_BUILD_TYPE:STRING=Debug
```

This step generates the files `CMakeFiles/`, `CMakeCache.txt`, and `cmake_install.cmake`.

To compile and generate the library, use:

```
make
```

with the optional flag `-j N` with `N` the number of threads to use to speed up compilation.

You have almost finished, make sure that the libraries have been successfully created in the
plugins directory, usually
```
/Applications/VisIt.app/Contents/Resources/<VisIt version>/darwin-x86_64/plugins/databases/
```
You should be able to find the files: `libEOpenPMDDatabase_ser.dylib`, `libMOpenPMDDatabase.dylib`,
and `libEOpenPMDDatabase_par.dylib`.

## Using Visit with openPMD
--------------------------------------------------------------------------------

Once you have installed the OpenPMD viewer, Visit will automatically recognize it.

To start Visit, you can use the launcher but we recommend to start Visit by command line:

```
/Applications/VisIt.app/Contents/MacOS/VisIt -np <number of cores>
```

You can specify the number of cores you want to use in parallel with VisIt.
OpenPMD files are read and treated in parallel if more than one core are used.
