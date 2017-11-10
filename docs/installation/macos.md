Installation instructions for Ubuntu
================================================================================

## MacOs using executables

First, download and install the MacOs executable (.dmg image)
[VisIt](https://wci.llnl.gov/simulation/computer-codes/visit/executables).

On MacOs, you can choose to install the plugin only for you or all users.
For this aim, we will first use the xml2cmake tool located in `<Visit application directory>/Contents/Resources/bin/`.
We recommend to add an alias in your `~/.profile`.

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
If you used the .dmg image, you will find the files in:

```
/Applications/VisIt.app/Contents/Resources/<VisIt version>/darwin-x86_64/plugins/databases/
```

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
their corresponding directories. You should be able to find the files
`libEOpenPMDDatabase_ser.dylib`, `libMOpenPMDDatabase.dylib`,
 `libEOpenPMDDatabase_par.dylib`.

## Using Visit with openPMD
--------------------------------------------------------------------------------

Once you have installed the OpenPMD viewer, Visit will automatically recognize it.

To start Visit, you can use the launcher but we recommend to start Visit by command line:

```
/Applications/VisIt.app/Contents/MacOS/VisIt -np <number of cores>
```

You can specify the number of cores you want to use in parallel with VisIt.
OpenPMD files are read and treated in parallel if more than one core are used.
