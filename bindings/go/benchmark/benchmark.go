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

import (
	"fmt"
	"time"

	"github.com/volcengine/SecureUnionID/bindings/go/core"
)

const (
	MIN_TIME   int = 10
	MIN_ITERS  int = 10
	NUMOFMEDIA int = 2
	NUMOFDID   int = 1
)

func main() {
	var dspID, did, randVal, M, bt, cipheri string
	var masterKey [64]byte
	var keyPair core.KeysArray
	var sysPk core.Group
	dspID = "1234567890"
	did = "123456789012345"
	var KeyPairs []core.KeysArray
	var masterKeys [][64]byte

	for i := 0; i < NUMOFMEDIA; i++ {
		if i == 0 {
			start := time.Now()
			iterations := 0
			elapsed := time.Since(start)
			for (int(elapsed/time.Second)) < MIN_TIME || iterations < MIN_ITERS {
				seed, _ := core.RandomSeed()
				masterKey, _ = core.GenMasterKey(seed)
				iterations++
				elapsed = time.Since(start)
			}
			dur := float64(elapsed/time.Millisecond) / float64(iterations)
			fmt.Printf("GenMasterKey - %8d iterations  ", iterations)
			fmt.Printf(" %8.2f ms per iteration\n", dur)

			start = time.Now()
			iterations = 0
			elapsed = time.Since(start)
			for (int(elapsed/time.Second)) < MIN_TIME || iterations < MIN_ITERS {
				keyPair, _ = core.Keygen(masterKey, dspID)
				iterations++
				elapsed = time.Since(start)
			}
			dur = float64(elapsed/time.Millisecond) / float64(iterations)
			fmt.Printf("KeyGen - %8d iterations  ", iterations)
			fmt.Printf(" %8.2f ms per iteration\n", dur)

			masterKeys = append(masterKeys, masterKey)
			KeyPairs = append(KeyPairs, keyPair)
		} else {
			seed, _ := core.RandomSeed()
			masterKey, _ := core.GenMasterKey(seed)
			keyPair, _ := core.Keygen(masterKey, dspID)
			masterKeys = append(masterKeys, masterKey)
			KeyPairs = append(KeyPairs, keyPair)
		}
	}

	var pki []core.Group
	for i := 0; i < NUMOFMEDIA; i++ {
		pki = append(pki, KeyPairs[i].PK)
	}

	start := time.Now()
	iterations := 0
	elapsed := time.Since(start)
	for (int(elapsed/time.Second)) < MIN_TIME || iterations < MIN_ITERS {
		sysPk, _ = core.SystemKeygen(pki)
		iterations++
		elapsed = time.Since(start)
	}
	dur := float64(elapsed/time.Millisecond) / float64(iterations)
	fmt.Printf("SystemKeygen - %8d iterations  ", iterations)
	fmt.Printf(" %8.2f ms per iteration\n", dur)

	var sevs []core.PSIServer
	clt := core.NewClientFromInput(sysPk)
	for i := 0; i < NUMOFMEDIA; i++ {
		sevs = append(sevs, core.NewSeverFromInput(KeyPairs[i].SK))
	}

	start = time.Now()
	iterations = 0
	elapsed = time.Since(start)
	for (int(elapsed/time.Second)) < MIN_TIME || iterations < MIN_ITERS {
		for i := 0; i < NUMOFDID; i++ {
			seed, _ := core.RandomSeed()
			randVal, M, _ = clt.Blindv2(seed, did)
		}
		iterations++
		elapsed = time.Since(start)
	}
	dur = float64(elapsed/time.Millisecond) / float64(iterations)
	fmt.Printf("Blindv2 - %8d iterations  ", iterations)
	fmt.Printf(" %8.2f ms per iteration\n", dur)

	var cipheris []string
	for i := 0; i < NUMOFMEDIA; i++ {
		if i == 0 {
			start = time.Now()
			iterations = 0
			elapsed = time.Since(start)
			for (int(elapsed/time.Second)) < MIN_TIME || iterations < MIN_ITERS {
				for j := 0; j < NUMOFDID; j++ {
					cipheri, _ = sevs[i].Enc(M)
				}
				iterations++
				elapsed = time.Since(start)
			}
			dur = float64(elapsed/time.Millisecond) / float64(iterations)
			fmt.Printf("Enc - %8d iterations  ", iterations)
			fmt.Printf(" %8.2f ms per iteration\n", dur)
			cipheris = append(cipheris, cipheri)
		} else {
			cipher, _ := sevs[i].Enc(M)
			cipheris = append(cipheris, cipher)
		}
	}

	start = time.Now()
	iterations = 0
	elapsed = time.Since(start)
	for (int(elapsed/time.Second)) < MIN_TIME || iterations < MIN_ITERS {
		for i := 0; i < NUMOFDID; i++ {
			bt, _ = clt.Unblind(randVal, cipheris)
		}
		iterations++
		elapsed = time.Since(start)
	}
	dur = float64(elapsed/time.Millisecond) / float64(iterations)
	fmt.Printf("Unblind - %8d iterations  ", iterations)
	fmt.Printf(" %8.2f ms per iteration\n", dur)

	var cipher []string
	var dids []string
	var randVals []string
	for i := 0; i < NUMOFDID; i++ {
		cipher = append(cipher, bt)
		dids = append(dids, did)
		randVals = append(randVals, randVal)
	}

	start = time.Now()
	iterations = 0
	elapsed = time.Since(start)
	for (int(elapsed/time.Second)) < MIN_TIME || iterations < MIN_ITERS {
		clt.Verify(cipheris, pki, cipher, dids, randVals)
		iterations++
		elapsed = time.Since(start)
	}
	dur = float64(elapsed/time.Millisecond) / float64(iterations)
	fmt.Printf("Verify - %8d iterations  ", iterations)
	fmt.Printf(" %8.2f ms per iteration\n", dur)
}
