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

	// DSP持有的公钥，由媒体生成好之后派发给DSP
	pki := []core.Group{
		{
			G1: "032118e933581b8f17aa6523665430175d5ee3fdb20800e1a58865ab17e04bc56c",
			G2: "18706713498c174f2725acf1047a5b310a9bae78f0aef95b23d65df42543d87b08b650e5f472020588e7f40b497f25241327ef79cb3bc82115c3435f869fc4ff0d152aa4038c4694c961489b9c1f218ecbd02ceb6df4b272498678c92b7ca94e098cfe3d95880a3f2f122fc9aca2a9b61ac28a9a4a924be1fc9c197bcdd52519",
		},
	}

	// 标识dsp的identity，需媒体和dsp达成一致
	sender := "dsp_identity_xxx"

	// 外部id和sender需match，用于验签
	outId := "dsp_adv_id_xxxx"

	// 媒体的签名server地址
	destination := "http://127.0.0.1:8008/encryption"

	// dsp需要得到加密did的原始did列表，通过列表序号对应
	dids := []string{
		"123456789012345",
		"123456789054321",
	}

	// dsp最终要存储的映射关系，即dids[i]与msgs[i]是一对映射关系
	storeFunc := func(dids []string, msgs []string) {
		tpl := "did: %s, enc_did: %s"
		for i := range dids {
			fmt.Println(fmt.Sprintf(tpl, dids[i], msgs[i]))
		}
	}

	// 单边PSI的具体实现
	example.DoSignAndStoreJob(pki, sender, outId, destination, dids, storeFunc)

}
