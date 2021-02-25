package main

import (
	"fmt"
	"github.com/volcengine/SecureUnionID/bindings/go/core"
	"github.com/volcengine/SecureUnionID/bindings/go/example"
)

func main() {
	base64SkString := "pcl3fdcZy9FxDfBGk64B8fPlDEpIRtiz17ZhkW3Kgug="
	G1 := "032118e933581b8f17aa6523665430175d5ee3fdb20800e1a58865ab17e04bc56c"
	G2 := "18706713498c174f2725acf1047a5b310a9bae78f0aef95b23d65df42543d87b08b650e5f472020588e7f40b497f25241327ef79cb3bc82115c3435f869fc4ff0d152aa4038c4694c961489b9c1f218ecbd02ceb6df4b272498678c92b7ca94e098cfe3d95880a3f2f122fc9aca2a9b61ac28a9a4a924be1fc9c197bcdd52519"
	CheckSkPkValid(base64SkString, G1, G2)
}

func CheckSkPkValid(base64SkString string, G1 string, G2 string) {
	sk, _ := example.StringToSk(base64SkString)
	g := core.Group{G1: G1, G2: G2}
	result := core.Test(sk, g)
	fmt.Printf("CheckSkPkValid result:%v", result)
}
