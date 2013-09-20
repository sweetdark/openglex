ABOUT THE OPENEXR LIBRARIES
---------------------------

Half is a class that encapsulates our 16-bit floating-point format.

IlmThread is a thread abstraction library for use with IlmImf.  It
currently supports pthreads and Windows threads.

IlmImf is our "EXR" file format for storing 16-bit FP images.

Imath is a math library.  IlmImf only uses a subset of it,
but we're releasing the full library because it's easier for us to 
maintain, and we think it'll be useful to others.

Iex is an exception-handling library.

If you have questions about using the OpenEXR libraries, you may want
to join our developer mailing list.  See http://openexr.com for
details.  The web site also contains full developer documentation for
using OpenEXR in your applications.


LICENSE
-------

OpenEXR is free software.  See the file named COPYING (included in
this distribution) for details.


WHAT'S INCLUDED
---------------

This distribution contains the header files and pre-compiled
executables, DLLs and static libraries for OpenEXR 1.4.0.

The DLLs and static libraries included in this distribution were
compiled with Visual Studio .NET 2003 and are intended for use in 
other programs compiled with VS 2003.

If you are using Visual Studio 2005, you cannot link your
application against the static libraries or DLLs included in this
distribution.  Please see the OpenEXR web page at http://openexr.com
for the source code distribution and compile OpenEXR from source.
Alternatively, you may find at that site a pre-compiled distribution
of OpenEXR for Visual Studio 2005, if we have provided one.

The header files for the OpenEXR libraries are located in the include\
directory.  The DLLs are in bin\, and the static libraries are in
lib\.

Besides the core OpenEXR libraries, the distribution includes several
utilities for reading, writing, viewing, and manipulating OpenEXR
images.  These include:

  * exrdisplay, an image viewer.
  * exrheader, a utility for dumping header information.
  * exrstdattr, a utility for modifying OpenEXR standard attributes.
  * exrmaketiled, for generating tiled and rip/mipmapped images.
  * exrenvmap, for creating OpenEXR environment maps.
  * exrmakepreview, for creating preview images for OpenEXR files.

These utilities can be found in the bin\ directory.  They are
command-line applications, so you must run them from the cmd.exe
prompt.  If you invoke a utility with no arguments, it will print a
help message which explains its usage.

See the examples\ directory for some code that demonstrates how
to use the IlmImf library to read and write OpenEXR files.


USING OPENEXR IN YOUR APPLICATION (IMPORTANT)
---------------------------------------------

If you are linking your application aginst OpenEXR DLLs, you must
define OPENEXR_DLL in your project's Preprocessor Definitions in order
to get the correct signature for some of OpenEXR's symbols.

If you are linking against OpenEXR static libs, you should *not* define
the OPENEXR_DLL macro.

You do not need to define any other preprocessor macros to use OpenEXR
in your application.

