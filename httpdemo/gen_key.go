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
	"github.com/volcengine/SecureUnionID/core"
	"github.com/volcengine/SecureUnionID/example"
	"fmt"
)

func main() {
	dspID := "1234567890"
	seed, _ := core.SeedGen()
	masterKeyBT, _ := core.MasterKeyGen(seed)
	keyPairBT, _ := core.Keygen(masterKeyBT, dspID)

	Sk := example.SkToString(keyPairBT.SK)

	fmt.Println("G1: ", keyPairBT.PK.G1)
	fmt.Println("G2: ", keyPairBT.PK.G2)
	fmt.Println("Sk: ", Sk, "len: ",len(Sk))
}
