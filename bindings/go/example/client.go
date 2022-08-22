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

package example

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"net/http"
	"sync"
	"time"

	"github.com/golang/protobuf/proto"
	"github.com/volcengine/SecureUnionID/bindings/go/core"
)

var httpClient = &http.Client{Timeout: time.Duration(100 * time.Second)}

func callSingleServerEnc(sender string, outId string, msgs []string, destination string) (signedMsgs []string, err error) {
	req := &SignRequest{Sender: &sender, OutId: &outId, BlindedMessages: msgs}
	data, err := proto.Marshal(req)
	if err != nil {
		return nil, err
	}
	httpRequest, err := http.NewRequest("POST", destination, bytes.NewReader(data))
	if err != nil {
		return nil, err
	}
	httpRsp, err := httpClient.Do(httpRequest)
	if httpRsp != nil {
		defer httpRsp.Body.Close()
	}
	if err != nil {
		return nil, err
	}
	if httpRsp.StatusCode != 200 {
		return nil, fmt.Errorf("http failed")
	}
	bytesRsp, err := ioutil.ReadAll(httpRsp.Body)
	if err != nil {
		return nil, err
	}
	rsp := &SignResponse{}
	err = proto.Unmarshal(bytesRsp, rsp)
	if err != nil {
		return nil, err
	}
	if rsp.GetRspCode() != ResponseCode_Success {
		return nil, fmt.Errorf("server business failed code:%v", rsp.GetRspCode())
	}
	return rsp.GetSignedMessages(), nil
}

type StoreFunc func(dids []string, vals []string)

/*
	pki: public keys (assigned by medias)
	sender: the identity that represents DSP
	destination : the server address of media
	dids: the set of DIDs
	storeFunc : store the mapping of DID and encrypted DID
*/
func DoSignAndStoreJob(pki []core.Group, sender string, outId string, destination string, dids []string, storeFunc StoreFunc) {
	// seed
	seed, _ := core.RandomSeed()
	// random value
	randValAll := make([]string, len(dids))
	// Blinded DID
	blindedMsgs := make([]string, len(dids))
	// the encrypted DID to be stored
	resBtAll := make([]string, len(dids))
	// generate system key
	sysPk, _ := core.SystemKeygen(pki)
	// client instance init
	clt := core.NewClientFromInput(sysPk)
	// used for verifying
	//var concatVerify string
	wg := sync.WaitGroup{}

	// step 1 : blinding
	start := time.Now()
	for i, did := range dids {
		wg.Add(1)
		go func(i int,did string){
			randVal, M, err := clt.Blindv2(seed, did)
			fmt.Printf("i: %v randVal:%v M: %v seed: %v did: %v err:%v \n", i, randVal, M, seed, did, err)
			randValAll[i]=randVal
			blindedMsgs[i]=M
			wg.Done()
		}(i,did)
	}
	wg.Wait()
	elapsed := time.Since(start)
	fmt.Printf("%d did costs %f ms on Blinding\n", len(dids), float64(elapsed/time.Millisecond))

	// step 2 : ask for ciphers
	cipherAll, err := callSingleServerEnc(sender, outId, blindedMsgs, destination)
	if err != nil {
		fmt.Println("callSingleServerEnc err ", err)
		return
	}

	// step 3 : Unblinding
	start = time.Now()
	for i, cipher := range cipherAll {
		wg.Add(1)
		go func(i int,cipher string){
			bt, _ := clt.Unblind(randValAll[i], []string{cipher})
			resBtAll[i]=bt
			wg.Done()
			//concatVerify += bt
		}(i,cipher)
	}
	wg.Wait()
	elapsed = time.Since(start)
	fmt.Printf("%d did costs %f ms on Unblinding\n", len(dids), float64(elapsed/time.Millisecond))

	// step 4 : Verifying, which can be ignored when there is only one media
	/*checkResult, r1, _ := clt.Verify([]string{concatVerify}, pki, resBtAll, dids, randValAll)
	if checkResult == 2 {
		fmt.Println("[Verify] Successful!")
	} else if checkResult == 0 || checkResult == 1 {
		fmt.Println("[Verify] Error!")
		return
	} else {
		fmt.Printf("[Verify] Error , No.%d media cheat on %dth did!", -checkResult, -r1)
		return
	}*/

	// step 5 : store the mapping relationship
	storeFunc(dids, resBtAll)
}
