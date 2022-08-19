import SecureUnionID
import random
import sys

print("--------------------------------------------------")
print("Step 0: generate master key.")


# generate random seed.
r = SecureUnionID.genRandSeed()
if r[0] !=64:
    print ("generate random number error, error number: %d" % (r[0]))
    exit(1)

# generate master key.
masterKey = SecureUnionID.genMasterKey(r[1])
r = masterKey[0]
if r != 2:
    print ("generate master key error, error number: %d" % (r))
    exit(1)
print("OK")


# generate public/private keys.
print("--------------------------------------------------")
print("Step 1: generate public key and private key")
# generate public/private keys.
sys.version_info[0]
dspId = b"1234567890"
if (sys.version_info.major == 2):
    dspId = "1234567890"

keys = SecureUnionID.Keygen(masterKey[1], dspId)
r = keys[0]
if r != 2:
    print ("generate public private keys error, error number: %d" % (r))
    exit(1)
print("OK")

# generate system keys.
print("--------------------------------------------------")
print("Step 2: generate the system key")

publicKeyOfG1Array = [keys[1]]
publicKeyOfG2Array = [keys[2]]

# generate system keys.
systemKey = SecureUnionID.System_Keygen(publicKeyOfG1Array, publicKeyOfG2Array , 1)
r = systemKey[0]
if r != 2:
    print ("generate system key error, error number: %d" % (r))
    exit(1)
print("OK")

# blinding.
print("--------------------------------------------------")
print("Step 3: blind")
# blinding.
plaintext = b"123456789012345"
if (sys.version_info.major == 2):
    plaintext = "123456789012345"

r = SecureUnionID.genRandSeed()
blind = SecureUnionID.Blind(plaintext, r[1])
r = blind[0]
if r != 2:
    print ("blind error, error number: %d" % (r))
    exit(1)
print("OK")

# encryption
print("--------------------------------------------------")
print("Step 4: encrypt")
enc = SecureUnionID.Enc(keys[3], blind[2])
r = enc[0]
if r != 2:
    print ("encrypt error, error number: %d" % (r))
    exit(1)
print("OK")

# unblinding
print("--------------------------------------------------")
print("Step 5: unblind")
cipherTextArray = [enc[1]]
unblind = SecureUnionID.Unblinding(cipherTextArray, 1, blind[1], systemKey[1])
r = unblind[0]
if r != 2:
    print ("unbind error, error number: %d" % (r))
    exit(1)

print("OK")

