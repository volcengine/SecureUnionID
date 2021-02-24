class Demo {
	public static void main(String[] args) {
        // load the library libsecureunionid.so. It should be in the java library path
		System.loadLibrary("secureunionid");

        secureunionid o = new secureunionid();
        byte masterkey[] = new byte[64];
        int x = o.MasterKeygen(5, masterkey);

        String dspID = "1234567890";

        byte pkg1string[] = new byte[300];
        byte pkg2string[] = new byte[300];
        byte sk[] = new byte[200];

        o.Keygen(masterkey, dspID, pkg1string, pkg2string, sk);

        String pkig1string[] = new String[1];
        String pkig2string[] = new String[1];
        pkig1string[0] = new String(pkg1string);
        pkig2string[0] = new String(pkg2string);

        byte sysg1string[] = new byte[300];
        byte sysg2string[] = new byte[300];
        o.System_Keygen(pkig1string, pkig2string, 1, sysg1string, sysg2string);


        String did = "123456789012345";
        String did1 = "123456789054321";

        byte betastring[] = new byte[300];
        byte Mstring[] = new byte[300];


        o.Blinding(did, 1, betastring, Mstring);

        byte betastring1[] = new byte[300];
        byte Mstring1[] = new byte[300];

        o.Blinding(did1, 1, betastring1, Mstring1);

        byte cipherstring[] = new byte[300];
        o.Enc(sk, Mstring, cipherstring);

        byte cipherstring1[] = new byte[300];
        o.Enc(sk, Mstring1, cipherstring1);

        byte cipher[] = new byte[300];
        String[] cipherstrings = new String[1];
        cipherstrings[0] = new String(cipherstring);
        o.Unblinding(cipherstrings, 1, betastring, sysg1string, cipher);


        byte cipher1[] = new byte[300];
        cipherstrings[0] = new String(cipherstring1);
        o.Unblinding(cipherstrings, 1, betastring1, sysg1string, cipher1);

        String ciphers[] = new String[2];
        ciphers[0] = new String(cipher);
        ciphers[1] = new String(cipher1);

        String dids[] = new String[2];
        dids[0] = did;
        dids[1] = did1;
        int r = o.batch_verify(ciphers, dids, sysg2string, 2);
        System.out.println(r);
	}
}
