# How to build secureunionid.jar

After compiling the secureunionidjni library, compile the jar file

The build requires the following components:

-   Apache Maven 3.5 or newer
-   JDK 1.8 or newer

### Build jar

``` bash
$ mvn clean package
```

### Run the demo

``` bash
$ java -jar target/secureunionid-0.1.jar
```


