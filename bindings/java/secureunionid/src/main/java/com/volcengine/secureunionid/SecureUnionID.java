/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.2
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.volcengine.secureunionid;

import com.volcengine.secureunionid.util.Environment;

import java.io.IOException;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

public class SecureUnionID {
  // master key length.
  public static final int MASTER_KEY_LEN = 64;
  // private key length.
  public static final int PRIVATE_KEY_LEN = 32;

  public static final int G1_LEN = 33;
  public static final int G2_LEN = 128;

  // the length of public key on G1 group
  public static final int PUBKEY_G1_LEN = G1_LEN * 2 + 1;
  // the length of blind result
  public static final int BLIND_RESULT_LEN_STRIM = PUBKEY_G1_LEN - 1;
  // the length of public key on G2 group
  public static final int PUBKEY_G2_LEN = G2_LEN * 2 + 1;

  public static final int MAX_MEDIA_NUMBER = 1000;

  // error code definition
  public static final int SUCCESS = 0;
  public static final int FAIL = -1;
  public static final int JAVA_NULL_OR_LENGTH_ERROR = -2;
  public static final int C_NULL_POINTER_ERROR = -3;
  public static final int C_MALLOC_ERROR = -4;

  public static final int NOT_FOUND = -1;

  private enum LibraryState {
    NOT_LOADED,
    LOADING,
    LOADED
  }

  private static AtomicReference<LibraryState> libraryLoaded = new AtomicReference<>(LibraryState.NOT_LOADED);

  static {
    SecureUnionID.loadLibrary();
  }

  /**
   * Loads the necessary library files.
   * Calling this method twice will have no effect.
   * By default the method extracts the shared library for loading at
   * java.io.tmpdir, however, you can override this temporary location by
   * setting the environment variable SECUREUNIONID_SHAREDLIB_DIR.
   */
  public static void loadLibrary() {
    if (libraryLoaded.get() == LibraryState.LOADED) {
      return;
    }

    if (libraryLoaded.compareAndSet(LibraryState.NOT_LOADED,
            LibraryState.LOADING)) {
      final String tmpDir = System.getenv("SECUREUNIONID_SHAREDLIB_DIR");

      try {
        NativeLibraryLoader.getInstance().loadLibrary(tmpDir);
      } catch (final IOException e) {
        libraryLoaded.set(LibraryState.NOT_LOADED);
        throw new RuntimeException("Unable to load the SecureUnionID shared library", e);
      }

      libraryLoaded.set(LibraryState.LOADED);
      return;
    }
    while (libraryLoaded.get() == LibraryState.LOADING) {
      try {
        Thread.sleep(10);
      } catch(final InterruptedException e) {
        //ignore
      }
    }
  }

  /**
   * Tries to load the necessary library files from the given list of
   * directories.
   *
   * @param paths a list of strings where each describes a directory
   *     of a library.
   */
  public static void loadLibrary(final List<String> paths) {
    if (libraryLoaded.get() == LibraryState.LOADED) {
      return;
    }

    if (libraryLoaded.compareAndSet(LibraryState.NOT_LOADED,
            LibraryState.LOADING)) {
      boolean success = false;
      UnsatisfiedLinkError err = null;
      for (final String path : paths) {
        try {
          System.load(path + "/" +
                  Environment.getJniLibraryFileName("secureunionidjni"));
          success = true;
          break;
        } catch (final UnsatisfiedLinkError e) {
          err = e;
        }
      }
      if (!success) {
        libraryLoaded.set(LibraryState.NOT_LOADED);
        throw err;
      }

      libraryLoaded.set(LibraryState.LOADED);
      return;
    }

    while (libraryLoaded.get() == LibraryState.LOADING) {
      try {
        Thread.sleep(10);
      } catch(final InterruptedException e) {
        //ignore
      }
    }
  }


  public static void HASHIT(byte[] hashstring, String m) {
    SecureUnionIDJNI.HASHIT(hashstring, m);
  }

  public static long randomSeed() {
    return SecureUnionIDJNI.randomSeed();
  }

  public static int randomSeed(byte[] BYTE) {
    if (BYTE == null || BYTE.length < MASTER_KEY_LEN)
      return JAVA_NULL_OR_LENGTH_ERROR;
    int r = SecureUnionIDJNI.genRandSeed(BYTE);
    if (r == MASTER_KEY_LEN) return SUCCESS;
    if (r == 0) return C_NULL_POINTER_ERROR;
    return r;
  }

  public static int MasterKeygen(byte[] ran, byte[] BYTE) {
    if (ran == null || ran.length < MASTER_KEY_LEN)
    if (BYTE == null || BYTE.length < MASTER_KEY_LEN)
      return JAVA_NULL_OR_LENGTH_ERROR;
    int r = SecureUnionIDJNI.genMasterKey(ran, BYTE);
    if (r == 2) return SUCCESS;
    if (r == 0) return C_NULL_POINTER_ERROR;
    return r;
  }

  public static int MasterKeygen(long ran, byte[] BYTE) {
    if (BYTE == null || BYTE.length < MASTER_KEY_LEN)
      return JAVA_NULL_OR_LENGTH_ERROR;
    int r = SecureUnionIDJNI.MasterKeygen(ran, BYTE);
    if (r == 2) return SUCCESS;
    if (r == 0) return C_NULL_POINTER_ERROR;
    return r;
  }

  public static int Keygen(byte[] arg0, String dspid, byte[] arg2, byte[] arg3, byte[] arg4) {
    if (arg0 == null || arg0.length < MASTER_KEY_LEN ||
        arg2 == null || arg2.length < PUBKEY_G1_LEN  ||
        arg3 == null || arg3.length < PUBKEY_G2_LEN  ||
        arg4 == null || arg4.length < PRIVATE_KEY_LEN)
      return JAVA_NULL_OR_LENGTH_ERROR;
    int r = SecureUnionIDJNI.Keygen(arg0, dspid, arg2, arg3, arg4);
    if (r == 2) return SUCCESS;
    if (r == 0) return C_NULL_POINTER_ERROR;
    return r;
  }

  public static int System_Keygen(String[] arg0, String[] arg1, int numofmedia, byte[] arg3, byte[] arg4) {
    if (arg3 == null || arg3.length < PUBKEY_G1_LEN ||
        arg4 == null || arg4.length < PUBKEY_G2_LEN ||
        numofmedia < 0 || numofmedia > MAX_MEDIA_NUMBER)
        return JAVA_NULL_OR_LENGTH_ERROR;
    int r = SecureUnionIDJNI.System_Keygen(arg0, arg1, numofmedia, arg3, arg4);
    if (r == 2) return SUCCESS;
    if (r == 3) return FAIL;
    if (r == 0) return C_NULL_POINTER_ERROR;
    if (r == 1) return C_MALLOC_ERROR;
    return r;
  }

  public static int Blind(String did, byte[] seed, byte[] arg2, byte[] arg3) {
    if (seed == null || seed.length <  MASTER_KEY_LEN ||
        arg2 == null || arg2.length < (2 * PRIVATE_KEY_LEN + 1) ||
        arg3 == null || arg3.length < PUBKEY_G1_LEN)
        return JAVA_NULL_OR_LENGTH_ERROR;
    int r = SecureUnionIDJNI.Blind(did, seed, arg2, arg3);
    if (r == 2) return SUCCESS;
    if (r == 3) return FAIL;
    if (r == 0) return C_NULL_POINTER_ERROR;
    return r;
  }

  public static int Blinding(String did, long seed, byte[] arg2, byte[] arg3) {
    if (arg2 == null || arg2.length < (2 * PRIVATE_KEY_LEN + 1) ||
        arg3 == null || arg3.length < PUBKEY_G1_LEN)
        return JAVA_NULL_OR_LENGTH_ERROR;
    int r = SecureUnionIDJNI.Blinding(did, seed, arg2, arg3);
    if (r == 2) return SUCCESS;
    if (r == 3) return FAIL;
    if (r == 0) return C_NULL_POINTER_ERROR;
    return r;
  }

  public static int Enc(byte[] arg0, byte[] arg1, byte[] arg2) {
    if (arg0 == null || arg0.length < PRIVATE_KEY_LEN ||
        arg1 == null || arg1.length < BLIND_RESULT_LEN_STRIM ||
        arg2 == null || arg2.length < PUBKEY_G1_LEN)
        return JAVA_NULL_OR_LENGTH_ERROR;
    int r = SecureUnionIDJNI.Enc(arg0, arg1, arg2);
    if (r == 2) return SUCCESS;
    if (r == 3) return FAIL;
    if (r == 0) return C_NULL_POINTER_ERROR;
    return r;
  }

  public static int Unblinding(String[] STRING_ARRAY, int numofmedia, byte[] arg2, byte[] arg3, byte[] arg4) {
    if (arg2 == null || arg2.length < (2 * PRIVATE_KEY_LEN + 1) ||
        arg3 == null || arg3.length < PUBKEY_G1_LEN ||
        arg4 == null || arg4.length < PUBKEY_G1_LEN ||
        numofmedia < 0 || numofmedia > MAX_MEDIA_NUMBER)
        return JAVA_NULL_OR_LENGTH_ERROR;
    int r = SecureUnionIDJNI.Unblinding(STRING_ARRAY, numofmedia, arg2, arg3, arg4);
    if (r == 2) return SUCCESS;
    if (r == 3) return FAIL;
    if (r == 0) return C_NULL_POINTER_ERROR;
    if (r == 1) return C_MALLOC_ERROR;
    return r;
  }

  public static int verify_individual(String[] arg0, String[] arg1, String[] arg2, String did, int numofmedia, byte[] BYTE) {
    if (BYTE == null || BYTE.length < (PRIVATE_KEY_LEN * 2 + 1) ||
        numofmedia < 0 || numofmedia > MAX_MEDIA_NUMBER)
      return JAVA_NULL_OR_LENGTH_ERROR;
    int r = SecureUnionIDJNI.verify_individual(arg0, arg1, arg2, did, numofmedia, BYTE);
    if (r == 2) return SUCCESS;
    if (r == 3) return FAIL;
    if (r == 0) return C_NULL_POINTER_ERROR;
    if (r == 1) return C_MALLOC_ERROR;
    if (r < 0) return -r;
    return r;
  }

  public static int batch_verify(String[] arg0, String[] arg1, byte[] BYTE, int numofdid) {
    if (BYTE == null || BYTE.length < PUBKEY_G2_LEN)
      return JAVA_NULL_OR_LENGTH_ERROR;
    int r = SecureUnionIDJNI.batch_verify(arg0, arg1, BYTE, numofdid);
    if (r == 2) return SUCCESS;
    if (r == 3) return FAIL;
    if (r == 0) return C_NULL_POINTER_ERROR;
    if (r == 1) return C_MALLOC_ERROR;
    return r;
  }

}
