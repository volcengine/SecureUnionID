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

func DoEncryption(dids []string, skStr string) (vals []string) {
	vals = make([]string, 0, len(dids))
	ski, _ := example.StringToSk(skStr)
	server := core.NewSeverFromInput(ski)
	for _, did := range dids {
		hashdid := core.HashToG1(did)
		encDid, _ := server.Enc(hashdid)
		vals = append(vals, encDid)
	}
	return vals
}

func main() {
	dids := []string{
		"123456789012345",
		"123456789054321",
	}
	skStr := "pcl3fdcZy9FxDfBGk64B8fPlDEpIRtiz17ZhkW3Kgug="
	encDids := DoEncryption(dids, skStr)
	for i := range dids {
		fmt.Printf("[online req] did:%v enc_did:%v \n", dids[i], encDids[i])
	}
}