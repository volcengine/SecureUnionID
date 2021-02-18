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

// This demo simulates the situation where Bytedance and Company A communicate
// with a DSP. In this demo, the DSP is client and is responsible for blinding,
// unblinding and verifying. Bytedance and Company A are servers and are
// responsible for encryption.
// 本样例模拟字节和公司A外加一个DSP所进行的流程，DSP为客户端也即发起端，负责加盲去盲和验证
// 字节和公司A分别为两个服务端，负责加密

import (
	"fmt"

	"github.com/volcengine/SecureUnionID/core"
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
	masterKeyTX, _ := core.MasterKeyGen(seed)
	keyPairTX, _ := core.Keygen(masterKeyTX, dspID)

	// DSP generates the system key.
	var pki []core.Group
	pki = append(pki, keyPairBT.PK)
	pki = append(pki, keyPairTX.PK)
	sysPk, _ := core.SystemKeygen(pki)

	// Generate instances of client and servers.
	clt := core.NewClientFromInput(sysPk)
	sevBT := core.NewSeverFromInput(keyPairBT.SK)
	sevTX := core.NewSeverFromInput(keyPairTX.SK)

	// DSP conducts blinding operation.
	seed, _ = core.SeedGen()
	randVal, M, _ = clt.Blind(seed, did)

	// Bytedance and Company A encrypt received messages.
	cipherBT, _ := sevBT.Enc(M)
	cipherTX, _ := sevTX.Enc(M)

	// DSP unblinds received ciphers.
	var cipheri []string
	cipheri = append(cipheri, cipherBT)
	cipheri = append(cipheri, cipherTX)
	bt, _ = clt.Unblind(randVal, cipheri)

	// Verifying
	var cipher []string
	cipher = append(cipher, bt)
	var dids []string
	dids = append(dids, did)
	var randVals []string
	randVals = append(randVals, randVal)
	result, result1, _ := clt.Verify(cipheri, pki, cipher, dids, randVals)
	if result == 2 {
		fmt.Println("no one cheat!")
	} else if result == 0 || result == 1 {
		fmt.Println("verify error!")
	} else {
		fmt.Printf("No.%d media cheat on %dth!", result, result1)
	}
}
