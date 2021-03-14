import SecureUnionID
masterKey = SecureUnionID.MasterKeygen(1)
keys = SecureUnionID.Keygen(masterKey[1], "1234567890")
print(keys[0])
publicKeyOfG1Array = [keys[1]]
publicKeyOfG2Array = [keys[2]]
systemKey = SecureUnionID.System_Keygen(publicKeyOfG1Array, publicKeyOfG2Array , 1)
print(systemKey[0])
blind = SecureUnionID.Blinding("1", 1)
enc = SecureUnionID.Enc(keys[3], blind[2])
cipherTextArray = [enc[1]]
unblind = SecureUnionID.Unblinding(cipherTextArray, 1, blind[1], systemKey[1])

unblindCipherArray = [unblind[1]]
allDeviceIds = ["123456789012345"]
r = SecureUnionID.batch_verify(unblindCipherArray, allDeviceIds, systemKey[2], 1)
print(r)


