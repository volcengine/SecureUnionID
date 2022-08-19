# SecureUnionID

Secure ECC-based DID intersection.

## ABSTRACT

This project is used to protect device ID using Elliptic Curve Cryptography algorithm. The details of this project can be found in [INTRODUCTION](https://github.com/volcengine/SecureUnionID/blob/main/doc/SecureUnionID_Intro.pdf).

## INSTALLION

**For Golang User**
```
go get -u github.com/volcengine/SecureUnionID
```

**For Java User**
The Java source code is under the bindings/java directory.

Please see the `README.md` under the bindings/java directory.

## RUN DEMOS

**For Golang User**

We provide three demos without http communication (/bingdings/go/demo/demo.go, /bingdings/go/demo/demo1.go, /bingdings/go/demo/demo2.go). Run demo with the following command：
```
go run demo.go
go run demo1.go
go run demo2.go
```
We also provide a demo with http communication (/bingdings/go/httpdemo/http_client_demo.go, /bingdings/go/httpdemo/http_sever_demo.go). Run this demo with the following command in two terminals:
```
go run http_client_demo.go
```
```
go run http_sever_demo.go
```

**For Java User**

Please see the `README.md` under the bindings/java directory.

## BENCHMARK

We provide a benchmark file for Golang user, the path of which is /bindings/go/benchmark/benchmark.go. The performance on a 2.3 GHz core i7, 32G DDR4 mac with 2 media is as follows: 
```
GenMasterKey -    35572 iterations       0.28 ms per iteration
KeyGen -    13785 iterations       0.73 ms per iteration
SystemKeygen -    87311 iterations       0.11 ms per iteration
Blindv2 -    16344 iterations       0.61 ms per iteration
Enc -    43323 iterations       0.23 ms per iteration
Unblind -    34110 iterations       0.29 ms per iteration

```

## Doc

We provide API documents of [C](https://github.com/volcengine/SecureUnionID/blob/main/doc/c_doc_cn.rst), [Go](https://github.com/volcengine/SecureUnionID/blob/main/doc/go_doc_cn.rst), [Java](https://github.com/volcengine/SecureUnionID/blob/main/doc/java_doc_cn.rst), [Python](https://github.com/volcengine/SecureUnionID/blob/main/doc/python_doc_cn.rst) (English version in [C_EN](https://github.com/volcengine/SecureUnionID/blob/main/doc/c_doc_en.rst), [Go_EN](https://github.com/volcengine/SecureUnionID/blob/main/doc/go_doc_en.rst), [Java_EN](https://github.com/volcengine/SecureUnionID/blob/main/doc/java_doc_en.rst), [Python_EN](https://github.com/volcengine/SecureUnionID/blob/main/doc/python_doc_en.rst)). Other contributors can develop interfaces in other languages according to this document.

Released under the [Apache](https://github.com/volcengine/SecureUnionID/blob/main/LICENSE) License.