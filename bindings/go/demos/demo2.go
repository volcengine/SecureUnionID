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

// This demo simulates the situation where Bytedance is a requester and
// communicates with Company A. In this demo, Bytedance is both client
// `clt4dspId` and server `sevbd`. `clt4dspId` is responsible for blinding,
// unblinding and verifying, `sevbd` is responsible for encryption. Company A
// is server `sevCompanyA` and is responsible for encryption.
// 本样例模拟字节为发起端时，与公司A完成求交所进行的流程，字节是客户端clt4dspId也是服务端
// sevbd，clt4dspId负责加盲去盲和验证,sevbd负责加密, 公司A为服务端sevCompanyA，负责加密

import (
	"fmt"

	"github.com/volcengine/SecureUnionID/bindings/go/core"
)

func main() {
	var dspID, did, randVal, M, bt string
	dspID = "1234567890"
	did = "123456789012345"

	// Bytedance generates keys.
	seed, _ := core.SeedGen()
	masterKeyBT, _ := core.MasterKeyGen(seed)
	keyPairBT, _ := core.Keygen(masterKeyBT, dspID)

	// Company A generates keys.
	seed, _ = core.SeedGen()
	masterKeyCompanyA, _ := core.MasterKeyGen(seed)
	keyPairCompanyA, _ := core.Keygen(masterKeyCompanyA, dspID)

	// Bytedance generates the system key.
	var pki []core.Group
	pki = append(pki, keyPairBT.PK)
	pki = append(pki, keyPairCompanyA.PK)
	sysPk, _ := core.SystemKeygen(pki)

	// Generate instances of client and servers.
	cltBT4dspId := core.NewClientFromInput(sysPk)
	sevBT := core.NewSeverFromInput(keyPairBT.SK)
	sevCompanyA := core.NewSeverFromInput(keyPairCompanyA.SK)

	// Bytedance conducts blinding operation.
	seed, _ = core.SeedGen()
	randVal, M, _ = cltBT4dspId.Blind(seed, did)

	// Bytedance and Company A encrypts the received message.
	cipherBT, _ := sevBT.Enc(M)
	cipherCompanyA, _ := sevCompanyA.Enc(M)

	// After receiving all the ciphers, Bytedance unblinds them.
	var cipheri []string
	cipheri = append(cipheri, cipherBT)
	cipheri = append(cipheri, cipherCompanyA)
	bt, _ = cltBT4dspId.Unblind(randVal, cipheri)

	// Verifying.
	var cipher []string
	cipher = append(cipher, bt)
	var dids []string
	dids = append(dids, did)
	var randVals []string
	randVals = append(randVals, randVal)
	result, result1, _ := cltBT4dspId.Verify(cipheri, pki, cipher, dids, randVals)
	if result == 2 {
		fmt.Println("no one cheat!")
	} else if result == 0 || result == 1 {
		fmt.Println("verify error!")
	} else {
		fmt.Printf("No.%d media cheat on %dth did!\n", -result, -result1)
	}
}
