# How to build secureunionidjni library on Linux

Supported platforms:

-   x86_64

The build requires the following components:

-   CMake 3.10 or newer
-   Make
-   C++ compiler: gcc 9 or newer
-   Linker: lld or gold (the classic GNU ld won’t work)
-   JDK 1.8 or newer (JAVA_HOME should be set. For instance, export JAVA_HOME="/usr/lib/jvm/java-8-openjdk-amd64")

### Build secureunionidjni

``` bash
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

Output the following info:

```
[ 95%] Building C object CMakeFiles/secureunionidjni-linux64.dir/src/SecureUnionID_wrap.c.o
[100%] Linking C shared library ....../SecureUnionID/bindings/java/secureunionid/src/main/resources/libsecureunionidjni-linux64.so
```



# How to build secureunionidjni library on Mac

Supported platforms:

-   x86_64

The build requires the following components:

-   CMake 3.10 or newer
-   Make
-   Apple clang
-   JDK 1.8 or newer  (JAVA_HOME should be set. For instance, export JAVA_HOME="/Library/Java/JavaVirtualMachines/jdk1.8.0_261.jdk/Contents/Home")

### Build secureunionidjni

``` bash
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

Output the following info:

```
[ 95%] Building C object CMakeFiles/secureunionidjni-osx.dir/src/SecureUnionID_wrap.c.o
[100%] Linking C shared library ....../SecureUnionID/bindings/java/secureunionid/src/main/resources/libsecureunionidjni-osx.dylib
```



# How to build secureunionidjni library on windows

Supported platforms:

-   x86_64

The build requires the following components:

-   mingw-w64 x86_64-8.1.0-posix-seh-rt_v6-rev0 or newer
-   CLion 2018.1 or newer
-   JDK 1.8 or newer  (JAVA_HOME should be set. For instance, JAVA_HOME="C:\Program Files\Unity\Hub\Editor\2019.3.0a3\Editor\Data\PlaybackEngines\AndroidPlayer\Tools\OpenJDK\Windows")

### Build secureunionidjni

**1.** Open this directory on the CLion

File -> Open

**1.** Set the compiler of the CLion to MinGW

File -> Settings -> Build,Execution,Deployment -> Toolchains

**3.** Build the library

Set CMake setting to be `Release` and click the `Reload CMake Project` button, which generates the Make files. And then Run -> Build.

Output the following info:
```
[ 95%] Building C object CMakeFiles/secureunionidjni-win64.dir/src/SecureUnionID_wrap.c.obj
[100%] Linking C shared library ......SecureUnionID\bindings\java\secureunionid\src\main\resources\libsecureunionidjni-win64.dll
[100%] Built target secureunionidjni-win64
```




