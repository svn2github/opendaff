
# OpenDAFF - A free, open source software package for directional audio data

OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL) with an exception for static linking permission.

Copyright (c) Institute of Technical Acoustics, RWTH Aachen University, 2009-2016
Visit the OpenDAFF homepage: http://www.opendaff.org


## License and warranty notes

OpenDAFF is free software, distributed under the terms of the GNU Lesser General Public License (LGPL) version 3. You can redistribute it and/or modify it under the terms of the GNU Lesser General Public License (LGPL) version 3, as published by the Free Software Foundation. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see http://www.gnu.org/licenses.


## Quick build guide for Linux developers

OpenDAFF uses CMake to generate project files for your desired development environment and platform. Install a current version of CMake according to the information found on http://www.cmake.org
In the source folder of OpenDAFF that contains the CMakeLists.txt, run 

cmake .
make
make install

Now, a copy of the OpenDAFF static library *DAFF* has been installed on your system. To activate further optional parts of OpenDAFF, see the OPENDAFF_* switches within the CMake configuration.


## OpenDAFF content

OpenDAFF comprises ...

1. two C++ programming libraries
	+ *DAFF*: reader interface for DAFF files
	+ *DAFFViz*: DAFF content visualization
2. two PC applications
	+ *DAFFTool*: a command line tool to investigate file contents (metadata) and extract directional data
	+ *DAFFViewer*: a graphical application to visualize DAFF directivities, show metadata and extract directional data
3. a binding for Matlab (a Matlab executable) to read DAFF files
4. a collection of Matlab scripts to generate or compile DAFF content
5. some tests for validation.


## OpenDAFF dependencies

There is no dependency required for the DAFF library itself, however the additional visualization library DAFFViz and the provided applications require third party projects. To build the entire OpenDAFF project, the following is required:

+ FFTW3
+ SNDFILE
+ Qt
+ VTK with Qt Widgets
+ Doxygen
+ Matlab


## General build guide

To build all parts of the OpenDAFF project, the corresponding switches have to be activated in CMake. Have a look at the *OPENDAFF* group or variables with this prefix in the configuration list.

OpenDAFF uses a "D" postfix to distinguish between libraries and binaries that are build with debug information, i.e. the DAFFD.lib (or DAFFD.dll or libDAFFD.so) refers to the debug version.

The common way is to first activate everything you desire to build on your machine. Hit configure. Then, when CMake looks for the dependencies, it will come up with errors if a package could not be found (vagely spoken: are not installed on your computer). Here, one can help and set the pathes that appear in CMake called *PACKAGE*_DIR, for example if you downloaded and extracted a package to some arbitrary folder. Also, have a look at the FindXXX.cmake in the OpenDAFF/cmake folder to extend the search paths for CMake.

### Dependency overview

- DAFFViz
	+ DAFF
	+ VTK
- DAFFTool
	+ DAFF
	+ FFTW3
	+ SNDFILE
- DAFFViewer
	+ DAFF
	+ DAFFViz
	+ FFTW3
	+ SNDFILE
	+ Qt
	+ VTK with Qt Widgets
	
You may have to set the VTK_DIR appropriately to tell CMake where to look for the VTKConfig.cmake file.
The DAFFTool requires the FFTW3 library and libsndfile library. Again, you may have to tell CMake where to find the package.
The DAFFViewer also requires the Qt and VTK.


## Build guide for Visual Studio users on Windows

### Prerequisites

OpenDAFF uses CMake to generate project files for your desired development environment and platform. Install a current version of CMake according to the information found on http://www.cmake.org

Let's assume you have donwloaded the source code of OpenDAFF to the folder *D:/dev/OpenDAFF* and this README along with a file named CMakeLists.txt can be found here.

Start the CMake GUI and enter or browse to your source code folder: *D:/dev/OpenDAFF*
Now, provide a place where the project files shall be genereted, I would choose something like *D:/dev/OpenDAFF/build/win32-x64.vc12* to indicate platform (x64 in this case) and compiler version (Visual C++ Compiler version 12).
Hit the Configure button and select the desired Visual Studio compiler, build platform and environment version (i.e. Visual Studio 12 2013 Win64).
For starters, change the variable CMAKE_INSTALL_PREFIX to something like *D:/dev/OpenDAFF/dist* to prevent installation of OpenDAFF into the system's installation path. I would chose something like *D:/dev/OpenDAFF/dist/win32-x64.vc12*.
Now you can generate the project files and you will find the OpenDAFF.sln solution file in the target folder *D:/dev/OpenDAFF/build/win32-x64.vc12*, which you can open with Visual Studio.

### Build DAFF library

In Visual Studio, you can now build the entire solution with Debug or Release mode. It will automatically install the headers and library into the CMAKE_INSTALL_PREFIX folder, if you have write permissions here. 

### Building DAFFViz library

To build the visualization library *DAFFViz*, you will have to first build VTK with Qt support (QtWidgets). Find appropriate build/installation tutorials online.
Let's assume you have used CMake to build and install VTK-7.0 (from sources) into the folder *D:/dev/VTK/dist/win32-x64.vc12*. To tell CMake within the OpenDAFF generation process the location, provide the path to the CMake-related files generated by VTK, which is for this case example located here: *D:/dev/VTK/dist/win32-x64.vc12/lib/cmake/vtk-7.0*
To do so, set the variable VTK_DIR to the respective folder *D:/dev/VTK/dist/win32-x64.vc12/lib/cmake/vtk-7.0*, and CMake will know how to handle the dependencies.

You can now activate the switch OPENDAFF_BUILD_DAFFVIZ to generate the DAFF visualization library *DAFFViz*. Configure, Generate. Your Visual Studio project will now include this library.

If you want to build the applications, namely the DAFFViewer and the DAFFTool, you will have to resolve further third party dependencies, as stated above.You can find Windows binary packages on the websites of the projects. Again, tell CMake where to find the packages inside the CMake GUI window.
You can then activate the switches for the applications. Configure, Generate. Your Visual Studio project will now include the applications.


### Matlab binding

To generate the Matlab executable (mex) binary, use your preferred compiler within Matlab. You can execute the build_Matlab_DAFF.m script in bindings/matlab.
