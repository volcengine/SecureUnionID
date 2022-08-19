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

package core

/*
#include "../../../src/psi/encryption.c"
#include "../../../src/crypto/ecp_BN254.c"
#include "../../../src/crypto/ecp2_BN254.c"
#include "../../../src/crypto/big_256_56.c"
#include "../../../src/crypto/hash.c"
#include "../../../src/crypto/fn_BN254.c"
#include "../../../src/crypto/pair_BN254.c"
#include "../../../src/crypto/fp12_BN254.c"
#include "../../../src/crypto/fp_BN254.c"
#include "../../../src/crypto/fp2_BN254.c"
#include "../../../src/crypto/fp4_BN254.c"
#include "../../../src/crypto/oct.c"
#include "../../../src/crypto/rand.c"
#include "../../../src/crypto/randapi.c"
#include "../../../src/crypto/hkdf.c"
#include "../../../src/crypto/rom_curve_BN254.c"
#include "../../../src/crypto/rom_field_BN254.c"
#include "../../../src/crypto/rom_order_BN254.c"
*/
import "C"
import (
	"bytes"
	"encoding/binary"
	"errors"
	"os"
	"unsafe"
)

const (
	G1LENTH  = 33
	G2LENTH  = 128
	BIGLENTH = 32
)

//PSIClientHandler只针对特定dsp
//每个dsp都自己生成一个client
//每个媒体都会为每个dsp单独生成一个client
type PSIClientHandler interface {
	// client blinding H(did) * randVal
	// [in]: seed       seed
	//		 did        device ID
	// [out]: the randVal used in blinding and the result of blinding
	Blind(seed uint32, did string) (string, string, error)
	// client blinding H(did) * randVal
	// [in]: seed       string
	//		 did        device ID
	// [out]: the randVal used in blinding and the result of blinding
	Blindv2(seed string, did string) (string, string, error)
	// client unblinding, get Enc(sk, H(did))
	// [in]: cipheri    the final cipher of did
	//       randVal    the randVal used in blinding
	// [out]: the final cipher
	// Note: cipheri and randVal correspond one to one, please don't input wrong parameters
	Unblind(randVal string, cipheri []string) (string, error)
}

type PSIServerHandler interface {
	// server encrypt, get Enc(ski, H(did) *randVal)
	// [in]: message     blinded did received from client
	// [out]: the encrypted result of blinded did
	Enc(message string) (string, error)
}

type PSIClient struct {
	sysPk Group
}

type PSIServer struct {
	sk [BIGLENTH]byte
}

type Group struct {
	G1 string
	G2 string
}

type KeysArray struct {
	PK Group
	SK [BIGLENTH]byte
}

func SeedGen() (uint32, error) {
	var ran uint32
	file, err := os.OpenFile("/dev/random", os.O_RDONLY, 0444)
	if err != nil {
		return 0, err
	}
	defer file.Close()

	buf := make([]byte, 4)
	_, err = file.Read(buf)
	if err != nil {
		return 0, err
	}
	bytesBuffer := bytes.NewBuffer(buf)
	err = binary.Read(bytesBuffer, binary.BigEndian, &ran)
	if err != nil {
		return 0, err
	}
	return ran, nil
}

func RandomSeed() ([64]byte, error) {
	var rand [64]byte
	randPtr := (*C.char)(unsafe.Pointer(&rand[0]))
	result := C.genRandSeed(randPtr)
	if result != 64 {
		return rand, errors.New("generate random error!\n")
	}
	return rand, nil
}

// generate master key
// [in]: seed,  the seed of random number
// [out]: master key
func GenMasterKey(seed [64]byte) ([64]byte, error) {
	var masterKey [64]byte
	keyPtr := (*C.char)(unsafe.Pointer(&masterKey[0]))
	rndSeed := (*C.char)(unsafe.Pointer(&seed[0]))
	result := C.genMasterKey(rndSeed, keyPtr)
	if result != 2 {
		return masterKey, errors.New("Set para null pointer!\n")
	}
	return masterKey, nil
}

// generate master key
// [in]: seed,  the seed of random number
// [out]: master key
func MasterKeyGen(seed uint32) ([64]byte, error) {
	var masterKey [64]byte
	keyPtr := (*C.char)(unsafe.Pointer(&masterKey[0]))
	result := C.MasterKeygen(C.ulong(seed), keyPtr)
	if result != 2 {
		return masterKey, errors.New("Set para null pointer!\n")
	}
	return masterKey, nil
}

// generate key pair for dspId
// [in]: masterKey,  master key
//       dspId,      the identity of dsp
// [out]: key pair
func Keygen(masterKey [64]byte, dspId string) (KeysArray, error) {
	dspIdChar := C.CString(dspId)
	defer C.free(unsafe.Pointer(dspIdChar))

	pkG1Buffer := make([]byte, 2*G1LENTH+1)
	pkG1Ptr := (*C.char)(unsafe.Pointer(&pkG1Buffer[0]))

	pkG2Buffer := make([]byte, 2*G2LENTH+1)
	pkG2Ptr := (*C.char)(unsafe.Pointer(&pkG2Buffer[0]))

	skBuffer := make([]byte, BIGLENTH)
	skPtr := (*C.char)(unsafe.Pointer(&skBuffer[0]))

	keyPtr := (*C.char)(unsafe.Pointer(&masterKey[0]))

	result := C.Keygen(keyPtr, dspIdChar, pkG1Ptr, pkG2Ptr, skPtr)
	if result != 2 {
		var sk [32]byte
		return KeysArray{Group{"", ""}, sk}, errors.New("Genpara null pointer!\n")
	}

	var keys4DspId KeysArray
	keys4DspId.PK.G1 = C.GoString(pkG1Ptr)
	keys4DspId.PK.G2 = C.GoString(pkG2Ptr)
	for i := 0; i < BIGLENTH; i++ {
		keys4DspId.SK[i] = skBuffer[i]
	}
	return keys4DspId, nil
}

// generate system public key corresponding to dspId
// [in]: pki    the slice consists of the public keys of all the medias corresponding to dspId
// [out]: system public key
func SystemKeygen(pki []Group) (Group, error) {
	pkiG1Ptr := make([](*C.char), 0, len(pki))
	pkiG2Ptr := make([](*C.char), 0, len(pki))
	sysG1Buffer := make([]byte, 2*G1LENTH+1)
	sysG2Buffer := make([]byte, 2*G2LENTH+1)
	sysG1Ptr := (*C.char)(unsafe.Pointer(&sysG1Buffer[0]))
	sysG2Ptr := (*C.char)(unsafe.Pointer(&sysG2Buffer[0]))
	for i := 0; i < len(pki); i++ {
		char1 := C.CString(pki[i].G1)
		strptr1 := (*C.char)(unsafe.Pointer(char1))
		pkiG1Ptr = append(pkiG1Ptr, strptr1)
		defer C.free(unsafe.Pointer(char1))
		char2 := C.CString(pki[i].G2)
		strptr2 := (*C.char)(unsafe.Pointer(char2))
		pkiG2Ptr = append(pkiG2Ptr, strptr2)
		defer C.free(unsafe.Pointer(char2))
	}

	result := C.System_Keygen((**C.char)(unsafe.Pointer(&pkiG1Ptr[0])), (**C.char)(unsafe.Pointer(&pkiG2Ptr[0])), C.int(len(pki)), sysG1Ptr, sysG2Ptr)
	if result == 0 {
		return Group{"", ""}, errors.New("SystemKeygen null pointer!\n")
	} else if result == 1 {
		return Group{"", ""}, errors.New("SystemKeygen malloc error!\n")
	}
	var sysPk Group
	sysPk.G1 = C.GoString(sysG1Ptr)
	sysPk.G2 = C.GoString(sysG2Ptr)
	return sysPk, nil
}

func HashToG1(did string) string {
	var didHash string
	didChar := C.CString(did)
	defer C.free(unsafe.Pointer(didChar))

	hashBuffer := make([]byte, 2*G1LENTH+1)
	hashPtr := (*C.char)(unsafe.Pointer(&hashBuffer[0]))

	C.HASHIT(hashPtr, didChar)

	didHash = C.GoString(hashPtr)
	return didHash
}

func (clt *PSIClient) Blind(seed uint32, did string) (string, string, error) {
	var randVal string
	didChar := C.CString(did)
	defer C.free(unsafe.Pointer(didChar))

	randValBuffer := make([]byte, 2*BIGLENTH+1)
	randValPtr := (*C.char)(unsafe.Pointer(&randValBuffer[0]))

	cipherBuffer := make([]byte, 2*G1LENTH+1)
	cipherPtr := (*C.char)(unsafe.Pointer(&cipherBuffer[0]))

	result := C.Blinding(didChar, C.ulong(seed), randValPtr, cipherPtr)
	if result != 2 {
		return "", "", errors.New("Blinding null pointer!\n")
	}

	randVal = C.GoString(randValPtr)
	cipher := C.GoString(cipherPtr)
	return randVal, cipher, nil
}

func (clt *PSIClient) Blindv2(seed [64]byte, did string) (string, string, error) {
	var randVal string
	didChar := C.CString(did)
	defer C.free(unsafe.Pointer(didChar))

	seedPtr := (*C.char)(unsafe.Pointer(&seed[0]))

	randValBuffer := make([]byte, 2*BIGLENTH+1)
	randValPtr := (*C.char)(unsafe.Pointer(&randValBuffer[0]))

	cipherBuffer := make([]byte, 2*G1LENTH+1)
	cipherPtr := (*C.char)(unsafe.Pointer(&cipherBuffer[0]))

	result := C.Blind(didChar, seedPtr, randValPtr, cipherPtr)
	if result != 2 {
		return "", "", errors.New("Blinding null pointer!\n")
	}

	randVal = C.GoString(randValPtr)
	cipher := C.GoString(cipherPtr)
	return randVal, cipher, nil
}

func (clt *PSIClient) Unblind(randVal string, cipheri []string) (string, error) {
	randValChar := C.CString(randVal)
	defer C.free(unsafe.Pointer(randValChar))
	sysPkG1Char := C.CString(clt.sysPk.G1)
	defer C.free(unsafe.Pointer(sysPkG1Char))

	cipheriPtr := make([](*C.char), 0, len(cipheri))
	for i := 0; i < len(cipheri); i++ {
		char := C.CString(cipheri[i])
		defer C.free(unsafe.Pointer(char))
		strptr := (*C.char)(unsafe.Pointer(char))
		cipheriPtr = append(cipheriPtr, strptr)
	}

	cipherBuffer := make([]byte, 2*G1LENTH+1)
	cipherPtr := (*C.char)(unsafe.Pointer(&cipherBuffer[0]))

	result := C.Unblinding((**C.char)(unsafe.Pointer(&cipheriPtr[0])), C.int(len(cipheri)), randValChar, sysPkG1Char, cipherPtr)
	if result == 0 {
		return "", errors.New("Unblinding null pointer!\n")
	} else if result == 1 {
		return "", errors.New("Unblinding malloc error!\n")
	}

	cipher := C.GoString(cipherPtr)

	return cipher, nil
}

func (sev *PSIServer) Enc(message string) (string, error) {
	if len(message) != 2*G1LENTH {
		return "", errors.New("Wrong message format!\n")
	}

	messageChar := C.CString(message)
	defer C.free(unsafe.Pointer(messageChar))

	cipherBuffer := make([]byte, 2*G1LENTH+1)
	cipherPtr := (*C.char)(unsafe.Pointer(&cipherBuffer[0]))

	result := C.Enc((*C.char)(unsafe.Pointer(&sev.sk[0])), messageChar, cipherPtr)
	if result != 2 {
		return "", errors.New("Enc null pointer!\n")
	}
	return C.GoString(cipherPtr), nil
}

func NewClientFromInput(sysPk Group) PSIClient {
	return PSIClient{sysPk}
}

func NewSeverFromInput(sk [BIGLENTH]byte) PSIServer {
	return PSIServer{sk}
}

func Test(sk [BIGLENTH]byte, pk Group) int {
	g1char := C.CString(pk.G1)
	defer C.free(unsafe.Pointer(g1char))
	g2char := C.CString(pk.G2)
	defer C.free(unsafe.Pointer(g2char))
	result := C.test((*C.char)(unsafe.Pointer(&sk[0])), g1char, g2char)

	return (int)(result)
}
