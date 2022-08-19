// Copyright 2021 The SecureUnionID Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package main

// This demo simulates the situation where Bytedance communicates with a DSP.
// In this demo, the DSP is client and is responsible for blinding, unblinding
// and verifying. Bytedance is server and is responsible for encryption.
// 本样例模拟字节和一个DSP所进行的流程，DSP为客户端也即发起端，负责加盲去盲和验证, 字节为服
// 务端，负责加密。

import (
	"fmt"

	"github.com/volcengine/SecureUnionID/bindings/go/core"
	"github.com/volcengine/SecureUnionID/bindings/go/example"
)

func main() {
	var dspID, did, did1, randVal, M, bt string
	dspID = "1234567890"
	did = "123456789012345"
	did1 = "123456789054321"

	// Bytedance generates keys.
	seed, _ := core.RandomSeed()
	masterKeyBT, _ := core.GenMasterKey(seed)
	keyPairBT, _ := core.Keygen(masterKeyBT, dspID)

	keyPairBT.PK = core.Group{
		"032118e933581b8f17aa6523665430175d5ee3fdb20800e1a58865ab17e04bc56c",
		"18706713498c174f2725acf1047a5b310a9bae78f0aef95b23d65df42543d87b08b650e5f472020588e7f40b497f25241327ef79cb3bc82115c3435f869fc4ff0d152aa4038c4694c961489b9c1f218ecbd02ceb6df4b272498678c92b7ca94e098cfe3d95880a3f2f122fc9aca2a9b61ac28a9a4a924be1fc9c197bcdd52519",
	}
	keyPairBT.SK, _ = example.StringToSk("pcl3fdcZy9FxDfBGk64B8fPlDEpIRtiz17ZhkW3Kgug=")

	// DSP generates the system key.
	var pki []core.Group
	pki = append(pki, keyPairBT.PK)
	sysPk, _ := core.SystemKeygen(pki)

	// Generate instances of client and server.
	clt := core.NewClientFromInput(sysPk)
	sevBT := core.NewSeverFromInput(keyPairBT.SK)

	// DSP conducts blinding operation.
	seed, _ = core.RandomSeed()
	randVal, M, _ = clt.Blindv2(seed, did)
	fmt.Printf("i: %v randVal:%v M: %v seed: %v did: %v \n", 0, randVal, M, seed, did)
	seed, _ = core.RandomSeed()
	randVal1, M1, _ := clt.Blindv2(seed, did1)
	fmt.Printf("i: %v randVal:%v M: %v seed: %v did: %v \n", 1, randVal1, M1, seed, did1)

	// Bytedance encrypts the received messages.
	cipherBT1, _ := sevBT.Enc(M)
	cipherBT2, _ := sevBT.Enc(M1)

	// DSP unblinds the received ciphers.
	var cipheri []string
	cipheri = append(cipheri, cipherBT1)
	bt, _ = clt.Unblind(randVal, cipheri)

	cipheri = cipheri[0:0]
	cipheri = append(cipheri, cipherBT2)
	bt1, _ := clt.Unblind(randVal1, cipheri)
	cipheri = cipheri[0:0]
	cipheri = append(cipheri, cipherBT1+cipherBT2)

	// Verifying.
	var cipher []string
	cipher = append(cipher, bt)
	cipher = append(cipher, bt1)
	storeFunc := func(dids []string, msgs []string) {
		tpl := "did: %s, enc_did: %s"
		for i := range dids {
			fmt.Println(fmt.Sprintf(tpl, dids[i], msgs[i]))
		}
	}
	storeFunc([]string{did, did1}, cipher)

}
