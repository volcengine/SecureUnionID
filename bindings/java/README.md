This is the java bindings for Secure ECC-based DID intersection.

It supports Linux, Mac and Windows platforms.

The secureunionidjni directory is used for building the native share library.

The secureunionid directory is used for building the jar package.

First, you need go to the secureunionidjni and build the library following its

READEME.md file. It generates the libsecureunionidjni-linux64.so on Linux,

libsecureunionidjni-osx.dylib on Mac or libsecureunionidjni-win64.dll on Windows.

Then you need go to the secureunionid directory to build the jar package following

its README.md file.