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

package com.volcengine.secureunionid;

import java.util.Arrays;

class Demo {
    // master key length.
    public static final int MASTER_KEY_LEN = 64;
    // private key length.
    public static final int PRIVATE_KEY_LEN = 32;

    public static final int G1_LEN = 33;
    public static final int G2_LEN = 128;

    // the length of public key on G1 group
    public static final int PUBKEY_G1_LEN = G1_LEN * 2 + 1;
    // the length of public key on G2 group
    public static final int PUBKEY_G2_LEN = G2_LEN * 2 + 1;

    // borrowed from
    // https://stackoverflow.com/questions/9655181/how-to-convert-a-byte-array-to-a-hex-string-in-java
    private static final char[] HEX_ARRAY = "0123456789ABCDEF".toCharArray();
    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for (int j = 0; j < bytes.length; j++) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = HEX_ARRAY[v >>> 4];
            hexChars[j * 2 + 1] = HEX_ARRAY[v & 0x0F];
        }
        return new String(hexChars);
    }

    public static void main(String[] args) {
        SecureUnionID secureUnionID = new SecureUnionID();

        // generate masterkey.
        byte randSeed [] = new byte[MASTER_KEY_LEN];
        byte masterKey[] = new byte[MASTER_KEY_LEN];

        System.out.println("--------------------------------------------------");
        System.out.println("Step 0: generate master key.");

        int r = secureUnionID.randomSeed(randSeed);
        if (r != 0) {
            System.out.printf("generate random seed error %d\n", r);
            return;
        }
        //System.out.printf("random seed is %d\n", randSeed);
        r = secureUnionID.MasterKeygen(randSeed, masterKey);
        if (r != 0) {
            System.out.printf("generate master key error %d\n", r);
            return;
        }
        System.out.printf("masterKey is: %s\n", bytesToHex(masterKey));

        String dspID = "1234567890";

        // generate public/private keys.
        System.out.println("--------------------------------------------------");
        System.out.println("Step 1: generate public key and private key");
        byte publicKeyOfG1[] = new byte[PUBKEY_G1_LEN];
        byte publicKeyOfG2[] = new byte[PUBKEY_G2_LEN];
        byte privateKey[] = new byte[PRIVATE_KEY_LEN];
        r = secureUnionID.Keygen(masterKey, dspID, publicKeyOfG1, publicKeyOfG2, privateKey);
        if (r != 0) {
            System.out.printf("generate key error %d\n", r);
            return;
        }

        System.out.printf("publicKey of G1: is %s\n", bytesToHex(publicKeyOfG1));
        System.out.printf("publicKey of G2: is %s\n", bytesToHex(publicKeyOfG2));

        // generate system keys.
        System.out.println("--------------------------------------------------");
        System.out.println("Step 2: generate the system key");
        String publicKeyOfG1Array[] = new String[1];
        String publicKeyOfG2Array[] = new String[1];
        publicKeyOfG1Array[0] = new String(publicKeyOfG1);
        publicKeyOfG2Array[0] = new String(publicKeyOfG2);
        byte systemKeyOfG1[] = new byte[PUBKEY_G1_LEN];
        byte systemKeyOfG2[] = new byte[PUBKEY_G2_LEN];
        // only make systemKeyOfG1 provied, others should be kept secret !!!
        r = secureUnionID.System_Keygen(publicKeyOfG1Array, publicKeyOfG2Array, 1, systemKeyOfG1, systemKeyOfG2);
        if (r != 0) {
            System.out.printf("generate system key error %d\n", r);
            return;
        }

        // the device id number to protect.
        String deviceId0 = "123456789012345";
        String deviceId1 = "123456789054321";

        // blinding.
        System.out.println("--------------------------------------------------");
        System.out.println("Step 3: blind");
        byte betaValue0[] = new byte[2 * PRIVATE_KEY_LEN + 1];
        byte blindResult0[] = new byte[PUBKEY_G1_LEN];
        r = secureUnionID.randomSeed(randSeed);
        if (r != 0) {
            System.out.printf("generate random seed error %d\n", r);
            return;
        }
        r = secureUnionID.Blind(deviceId0, randSeed, betaValue0, blindResult0);
        if (r != 0) {
            System.out.printf("blinding error %d\n", r);
            return;
        }
        System.out.printf("blind result for device id 0: %s\n", bytesToHex(blindResult0));
        System.out.println( new String( blindResult0 ) );
        System.out.println( "-------------------------------------------------");

        byte betaValue1[] = new byte[2 * PRIVATE_KEY_LEN + 1];
        byte blindResult1[] = new byte[PUBKEY_G1_LEN];
        r = secureUnionID.randomSeed(randSeed);
        if (r != 0) {
            System.out.printf("generate random seed error %d\n", r);
            return;
        }
        r = secureUnionID.Blind(deviceId1, randSeed, betaValue1, blindResult1);
        if (r != 0) {
            System.out.printf("blinding error %d\n", r);
            return;
        }
        System.out.printf("blind result for device id 1: %s\n", bytesToHex(blindResult1));

        // encryption
        System.out.println("--------------------------------------------------");
        System.out.println("Step 4: encrypt");
        byte cipherText0[] = new byte[PUBKEY_G1_LEN];

        System.out.printf("blind result length is %d\n", blindResult0.length);
        // for the go server, the blind result size is 66 instead of 67.
        String blindResultStr0 = new String(blindResult0).trim();
        System.out.printf("After strim blind result length is %d\n", blindResultStr0.length());
        r = secureUnionID.Enc(privateKey, blindResultStr0.getBytes(), cipherText0);
        if (r != 0) {
            System.out.printf("encrypt error %d\n", r);
            return;
        }
        System.out.printf("encrypt result for device id 0: %s\n", bytesToHex(blindResult0));

        byte cipherText1[] = new byte[PUBKEY_G1_LEN];
        System.out.printf("blind result length is %d\n", blindResult1.length);
        // for the go server, the blind result size is 66 instead of 67.
        String blindResultStr1 = new String(blindResult1).trim();
        System.out.printf("After strim blind result length is %d\n", blindResultStr1.length());


        r = secureUnionID.Enc(privateKey, blindResultStr1.getBytes(), cipherText1);
        if (r != 0) {
            System.out.printf("encrypt error %d\n", r);
            return;
        }
        System.out.printf("encrypt result for device id 1: %s\n", bytesToHex(blindResult1));

        // unblinding
        System.out.println("--------------------------------------------------");
        System.out.println("Step 5: unblind");
        byte unblindCipherText0[] = new byte[PUBKEY_G1_LEN];
        String[] cipherTextArray = new String[1];
        cipherTextArray[0] = new String(cipherText0);
        r = secureUnionID.Unblinding(cipherTextArray, 1, betaValue0, systemKeyOfG1, unblindCipherText0);
        if (r != 0) {
            System.out.printf("unblinding error %d\n", r);
            return;
        }
        System.out.printf("unblind result for device id 0: %s\n", bytesToHex(unblindCipherText0));


        byte unblindCipherText1[] = new byte[PUBKEY_G1_LEN];
        cipherTextArray[0] = new String(cipherText1);
        r = secureUnionID.Unblinding(cipherTextArray, 1, betaValue1, systemKeyOfG1, unblindCipherText1);
        if (r != 0) {
            System.out.printf("unblinding error %d\n", r);
            return;
        }
        System.out.printf("unblind result for device id 1: %s\n", bytesToHex(unblindCipherText1));

        System.out.println("--------------------------------------------------");
        System.out.println("Step 6: verify");
        String unblindCipherArray[] = new String[2];
        unblindCipherArray[0] = new String(unblindCipherText0);
        unblindCipherArray[1] = new String(unblindCipherText1);

        String allDeviceIds[] = new String[2];
        allDeviceIds[0] = deviceId0;
        allDeviceIds[1] = deviceId1;

        // directly hash and enc
        byte hash[] = new byte[PUBKEY_G1_LEN];
        byte hashCipherText0[] = new byte[PUBKEY_G1_LEN];
        System.out.println( "-------------------------------------------------");
        SecureUnionID.HASHIT(hash, deviceId0);
        r = SecureUnionID.Enc(privateKey, hash, hashCipherText0);
        if(Arrays.equals(unblindCipherText0, hashCipherText0)) {
            System.out.println("blind-> enc->unblind = enc");
        }
        else {
            System.out.println("blind-> enc->unblind != enc");
        }
    }
}
