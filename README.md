# SecureUnionID

Secure ECC-based DID intersection in Go.

**INSTALLION**
```
go get -u github.com/volcengine/SecureUnionID
```

**ABSTRACT**

This project is used to protect device ID using Elliptic Curve Cryptography algorithm.

**RUN DEMOS**

We provide three demos without http communication (/demo/demo.go, /demo/demo1.go, /demo/demo2.go). Run demo with the following command：
```
go run demo.go
go run demo1.go
go run demo2.go
```
We also provide a demo with http communication (/httpdemo/http_client_demo.go, /httpdemo/http_sever_demo.go). Run this demo with the following command in two terminals:
```
go run http_client_demo.go
```
```
go run http_sever_demo.go
```

**BENCHMARK**

We provide a benchmark file, the path of which is /benchmark/benchmark.go. The performance on a 2.6 GHz core i7, 16G DDR4 mac with 2 medias is as follows: 
```
MasterKeyGen -    39076 iterations       0.26 ms per iteration
KeyGen -    13788 iterations       0.73 ms per iteration
SystemKeygen -    84716 iterations       0.12 ms per iteration
Blind -    20564 iterations       0.49 ms per iteration
Enc -    43715 iterations       0.23 ms per iteration
Unblind -    33308 iterations       0.30 ms per iteration
Verify -     5071 iterations       1.97 ms per iteration

```

Released under the [Apache](https://github.com/volcengine/SecureUnionID/blob/main/LICENSE) License.