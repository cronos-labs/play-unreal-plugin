package com.cronos.play;

import android.content.Context;
import android.util.Log;
import androidx.security.crypto.EncryptedFile;
import androidx.security.crypto.MasterKey;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

public class SecureStorage {

  public static String readSecureStorage(Context context, String key) {
    ByteArrayOutputStream outputStream = null;
    InputStream inputStream = null;
    String result;
    try {
      MasterKey masterKey =
          new MasterKey.Builder(context).setKeyScheme(MasterKey.KeyScheme.AES256_GCM).build();

      File file = new File(context.getFilesDir(), key);
      EncryptedFile encryptedFile =
          new EncryptedFile.Builder(
                  context, file, masterKey, EncryptedFile.FileEncryptionScheme.AES256_GCM_HKDF_4KB)
              .build();

      if (file.exists()) {
        inputStream = encryptedFile.openFileInput();
        outputStream = new ByteArrayOutputStream();
        // prepare buffer to read per iteration
        byte[] buffer = new byte[1024];
        int len;
        while ((len = inputStream.read(buffer)) != -1) {
          outputStream.write(buffer, 0, len);
        }
        byte[] data = outputStream.toByteArray();
        result = new String(data, StandardCharsets.UTF_8);
        result = "{\"result\":\"" + result + "\",\"success\":\"1\",\"error\":\"\"}";
      } else {
        result = "{\"result\":\"\",\"success\":\"0\",\"error\":\"encrypt file not found\"}";
      }
    } catch (Exception e) {
      Log.e("readSecureStorage exception error", "Failed to read secure storage: " + e);
      result = "{\"result\":\"\",\"success\":\"0\",\"error\":\"" + e + "\"}";
    } finally {
      // if outputStream is not null, close
      if (outputStream != null) {
        try {
          outputStream.close();
        } catch (IOException e) {
          result = "{\"result\":\"\",\"success\":\"0\",\"error\":\"" + e + "\"}";
          Log.e("readSecureStorage exception error", "Failed to close output stream: " + e);
        }
      }
      // if inputStream is not null, close
      if (inputStream != null) {
        try {
          inputStream.close();
        } catch (IOException e) {
          result = "{\"result\":\"\",\"success\":\"0\",\"error\":\"" + e + "\"}";
          Log.e("readSecureStorage exception error", "Failed to close input stream: " + e);
        }
      }
    } // end of finally
    return result;
  }

  public static int writeSecureStorage(Context context, String key, String value) {
    FileOutputStream outputStream = null;
    int ret = 0;
    try {
      MasterKey masterKey =
          new MasterKey.Builder(context).setKeyScheme(MasterKey.KeyScheme.AES256_GCM).build();
      File file = new File(context.getFilesDir(), key);
      EncryptedFile encryptedFile =
          new EncryptedFile.Builder(
                  context, file, masterKey, EncryptedFile.FileEncryptionScheme.AES256_GCM_HKDF_4KB)
              .build();

      if (file.exists()) {
        file.delete();
      }
      outputStream = encryptedFile.openFileOutput();
      byte[] utf8bytes = value.getBytes(StandardCharsets.UTF_8);

      outputStream.write(utf8bytes);
      // flush, close necessary to prevent `no matching keyfound error`
      outputStream.flush();
      ret = 1;
    } catch (Exception e) {
      ret = 0;
      Log.e("writeSecureStorage exception error", "Failed to write secure storage: " + e);
    } finally {
      // if outputStream is not null, close
      if (outputStream != null) {
        try {
          outputStream.close();
        } catch (IOException e) {
          ret = 0;
          Log.e("writeSecureStorage exception error", "Failed to close output stream: " + e);
        }
      }
    } // end of finally
    return ret;
  }
}
