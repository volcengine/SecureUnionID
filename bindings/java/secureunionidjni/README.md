# How to Build secureunionidjni.so on Linux

Supported platforms:

-   x86_64

The build requires the following components:

-   CMake 3.10 or newer
-   Make
-   C++ compiler: gcc 9 or newer
-   Linker: lld or gold (the classic GNU ld won’t work)

### Build secureunionidjni

``` bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Output the following info:

```
[ 95%] Building C object CMakeFiles/secureunionidjni-linux64.dir/src/SecureUnionID_wrap.c.o
[100%] Linking C shared library ....../SecureUnionID/bindings/java/secureunionid/src/main/resources/libsecureunionidjni-linux64.so
```



# How to Build secureunionidjni.dll on windows

Supported platforms:

-   x86_64

The build requires the following components:

-   mingw-w64 x86_64-8.1.0-posix-seh-rt_v6-rev0 or newer
-   CLion 2018.1 or newer

### Build secureunionidjni

**1.** Set the compiler of the CLion to MinGW
File -> Settings -> Build,Execution,Deployment -> Toolchains

**2.** Open this directory on the CLion
File -> Open

**3.** Build dll
Run -> Build

Output the following info:
```
[ 95%] Building C object CMakeFiles/secureunionidjni-win64.dir/src/SecureUnionID_wrap.c.obj
[100%] Linking C shared library ......SecureUnionID\bindings\java\secureunionid\src\main\resources\libsecureunionidjni-win64.dll
[100%] Built target secureunionidjni-win64
```




