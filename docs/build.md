# Building Xalan-C++

## Overview

The build process is divided into several parts: configuration and
building, followed by (optional) testing and installation. The
configuration part is performed with CMake.  The build part is
performed by invoking the chosen build tool such as `make` or `ninja`,
or by opening the generated project files in your IDE, and building
from within the IDE.

## Prerequisites

The following are required before building Xalan-C++

* A C++ compiler such as GCC, LLVM or MSVC (provided with Microsoft
  Visual Studio)
* CMake
* Chosen build tool and/or IDE
* Xalan-C++ sources (see [Downloading Xalan-C++](download.md))
* Xerces-C++ library and headers
* ICU C++ libraries and headers (optional)

Xalan-C++ uses the [CMake](https://cmake.org/) build generator and
requires that be installed. Additionally, a build tool such as [GNU
make](http://www.gnu.org/software/make/make.html) or
[Ninja](https://ninja-build.org/) is required for building.  CMake
supports a wide range of generators for several different compilers,
build tools and popular IDEs, including Eclipse, Kate, Visual Studio,
Sublime Text and more. Any of these may be used to build Xalan-C++.
Run `cmake --help` to display the full list of supported generators
for your platform.

Xalan-C++ is built upon the Xerces-C++ XML Parser library.  You can
either install a compatible binary distribution of Xerces-C/C++ or
build your own from a source distribution of Xerces-C/C++.
You can get more information on the Xerces-C/C++ distributions at the
[Xerces-C++ website](http://xerces.apache.org/xerces-c).

If you want to take advantage of the support for number formatting,
sorting, and encoding the ICU provides, you should also download and
install the
[International Components for Unicode (ICU)](http://icu-project.org);
see [Using the ICU](usagepatterns.md#using-the-icu).

## Configuration

Besides the standard CMake
[variables](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html),
Xalan-C++ provides a number of project-specific options. You can
specify one option for each category outlined below. If you do not
specify anything for a particular category then CMake will select the
most appropriate default, based upon the available options for your
system. At the end of its execution, CMake prints the selected values
for each category.

Transcoder (used to convert between internal UTF-16 and other
encodings):

|Option|Description|
|---|---|
|`-Dtranscoder=default`|use built-in transcoder (default)|
|`-Dtranscoder=icu`|use the ICU library|

Message Loader (used to access diagnostics messages):

|Option|Description|
|---|---|
|`-Dmessage-loader=inmemory`|store the messages in memory (default)|
|`-Dmessage-loader=icu-broken`|store the messages using the ICU resource bundles (broken, do not use)|
|`-Dmessage-loader=nls-broken`|store the messages in the NLS message catalog using catopen/catgets (broken, do not use)|

System character-set encoding:

|Option|Description|
|---|---|
|`-Dsystem-encoding=ascii`|use ASCII system encoding (default for most platforms)|
|`-Dsystem-encoding=ebcdic`|use EBCDIC system encoding (default for OS390)|

Thread support is required and can not be disabled.

Shared libraries are built by default. You can use the
`-DBUILD_SHARED_LIBS:BOOL=OFF` option to build static libraries.

If you need to specify compiler executables that should be used to
build Xalan-C++, you can set the `CC` and `CXX` environment variables
when invoking `cmake`. Similarly, if you need to specify additional
compiler or linker options, you can set the `CFLAGS`, `CXXFLAGS`, and
`LDFLAGS` environment variables.  For example:

```
CC=gcc-8.4 CXX=g++-8.4 CFLAGS=-O3 CXXFLAGS=-O3 cmake …
```

If building on Windows, the specific Visual Studio version may be
selected with some generators, and this may be run from a normal
command prompt.  If using a generic generator such as `Ninja`, then
`cmake` should be run from a Visual Studio command prompt, or in a
suitably configured environment, so that the correct compiler will be
detected.

## Building

Once the configuration part is complete you can run the build tool of
choice.  This may be done generically using `cmake --build
. [--config=Debug|Release]`.  Alternatively, a specific build tool,
e.g. `make`, `gmake`, `ninja` or `msbuild` corresponding to the chosen
generator may be used directly.  When invoked without a specific
target, it will build the Xalan-C++ library, all examples and all unit
tests.

## Testing

If you would like to run the automated test suite, run `ctest [-V] [-C
Debug|Release]`.  This will run all tests.  Additional
[options](https://cmake.org/cmake/help/latest/manual/ctest.1.html) are
available, such as running a subset of the tests and running the tests
in parallel.

## Installation

Finally, install the library and examples.  This may be done
generically using `cmake --build . --target install
[--config=Debug|Release]`.  Alternatively, a specific build tool may
be used, e.g. `make install`.  To change the installation directory,
use the `-DCMAKE_INSTALL_PREFIX=prefix` CMake option.

## Additional notes

### Extra CMake options

Some platforms and configurations may require extra `cmake` options.
Run `cmake -LH` to list the additional options, along with a short
description for each.  For each of the selection categories mentioned
above, the help text will list the valid choices detected for your
platform.  Run `cmake -LAH` for all the additional advanced settings.

### Shared libraries

Note that different UNIX platforms use different system environment
variables for finding shared libraries. On Linux and Solaris, the
environment variable name is `LD_LIBRARY_PATH`, on AIX it is
`LIBPATH`, on Mac OS X it is `DYLD_FALLBACK_LIBRARY_PATH`, and on
HP-UX it is `SHLIB_PATH`.

Note that Windows (including Cygwin and MinGW), is different from the
UNIX platforms in the way it finds shared libraries at run time.
While UNIX platforms may use the `LD_LIBRARY_PATH` environment
variable, Windows uses the `PATH` environment variable if the library
is not in the same directory as the executable.

## Examples

Several examples of configuring, building, testing and installing with
CMake using different platforms, generators, and installation options
are shown below:

### Ninja generator (any platform)

```sh
mkdir build
cd build
cmake -G Ninja -DCMAKE_INSTALL_PREFIX=/opt/Xalan-c -DCMAKE_BUILD_TYPE=Release -Dnetwork-accessor=curl /path/to/Xalan-c/source
ninja
ctest -V -j 8
ninja install
```

### Unix Makefiles generator (UNIX)

```sh
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=/opt/Xalan-c -DCMAKE_BUILD_TYPE=Debug -Dmessage-loader=icu /path/to/Xalan-c/source
make -j8
make test
make install
```

### MSBuild with Visual Studio 2017 (Windows)

In a command shell:

```sh
mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX:PATH=D:\xalan-c \path\to\Xalan-c\source
cmake --build . --config Debug
ctest -V -C Debug -j 4
cmake --build . --config Debug --target install
```

### Ninja with Visual Studio 2019 and vcpkg (Windows)

At a Visual Studio 2019 command prompt:

```sh
mkdir build
cd build
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE:PATH=D:\vcpkg\scripts\buildsystems\vcpkg.cmake -DCMAKE_INSTALL_PREFIX:PATH=D:\xalan-c \path\to\Xalan-c\source
ninja
ctest -V -j 4
ninja install
```
