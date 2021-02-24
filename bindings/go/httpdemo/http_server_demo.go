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
	"github.com/volcengine/SecureUnionID/bindings/go/example"
)

func main() {

	// 标识媒体的identity
	serverIdentity := "bytedance"

	// 媒体针对某个固定的dsp所持有的私钥，不公开
	skStr := "pcl3fdcZy9FxDfBGk64B8fPlDEpIRtiz17ZhkW3Kgug="

	// 媒体针对某个固定的dsp签名使用密钥的版本，需要公开
	skVersion := "bytedance_v1"

	// server参数初始化
	example.InitServer(serverIdentity, skStr, skVersion)

	// 启动，监听请求
	example.StartServer()

}
