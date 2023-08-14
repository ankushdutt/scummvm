package org.scummvm.scummvm;

import android.content.Context;
import android.widget.Toast;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import com.google.android.play.core.assetpacks.AssetPackManager;
import com.google.android.play.core.assetpacks.AssetPackManagerFactory;
import com.google.android.play.core.assetpacks.AssetPackStateUpdateListener;
import com.google.android.play.core.assetpacks.AssetPackState;
import com.google.android.play.core.assetpacks.AssetPackLocation;
import com.google.android.play.core.assetpacks.model.AssetPackStatus;

public class DLCPlaystore {

	public static class DLCDesc {
		public String name;
		public int size = 0;
	}

	private Context _context;
	private String _currentPackDownload;
	private String _dlcspath;
	AssetPackManager _assetPackManager;
	boolean waitForWifiConfirmationShown = false;

	AssetPackStateUpdateListener _assetPackStateUpdateListener = new AssetPackStateUpdateListener() {
		@Override
		public void onStateUpdate(AssetPackState assetPackState) {
			switch (assetPackState.status()) {
				case AssetPackStatus.PENDING:
					Log.i(ScummVM.LOG_TAG, "Pending");
					break;

				case AssetPackStatus.DOWNLOADING:
					long downloaded = assetPackState.bytesDownloaded();
					long totalSize = assetPackState.totalBytesToDownload();
					double percent = 100.0 * downloaded / totalSize;
					ScummVM.updateDownloadedBytes(downloaded, totalSize);

					Log.i(ScummVM.LOG_TAG, "PercentDone=" + String.format("%.2f", percent));

					break;

				case AssetPackStatus.TRANSFERRING:
					// 100% downloaded and assets are being transferred.
					// Notify user to wait until transfer is complete.
					break;

				case AssetPackStatus.COMPLETED:
					// Asset pack is ready to use.
					AssetPackLocation location = _assetPackManager.getPackLocation(_currentPackDownload);
					File sourceFolder = new File(location.path() + "/assets");
					File destFolder = new File(_dlcspath + "/" + _currentPackDownload);
					try {
						copyFolder(sourceFolder, destFolder);
					} catch (IOException e) {
						e.printStackTrace();
					}
					ScummVM.addEntryToConfig(_dlcspath + "/" + _currentPackDownload);
					// After removing, play store will not re-download the game upon updating
					_assetPackManager.removePack(_currentPackDownload);
					ScummVM.processNextDownload();
					break;

				case AssetPackStatus.FAILED:
					// Request failed. Notify user.
					Log.e(ScummVM.LOG_TAG, assetPackState.errorCode() + "");
					break;

				case AssetPackStatus.CANCELED:
					// Request canceled. Notify user.
					Log.w(ScummVM.LOG_TAG, "JAVA: Download is Canceled");
					break;

				case AssetPackStatus.NOT_INSTALLED:
					// Asset pack is not downloaded yet.
					break;
				case AssetPackStatus.UNKNOWN:
					Log.wtf(ScummVM.LOG_TAG, "Asset pack status unknown");
					break;
			}
		}
	};

	// Constructor
	public DLCPlaystore(Context context) {
		_context = context;
		_assetPackManager = AssetPackManagerFactory.getInstance(_context);
	}

	public void startDownload(String packname) {
		_currentPackDownload = packname;
		Log.i(ScummVM.LOG_TAG, "JAVA: startDownload " + packname);
		_assetPackManager.registerListener(_assetPackStateUpdateListener);
		List<String> assetPackList = new ArrayList<>();
		assetPackList.add(packname);
		// start download
		_assetPackManager.fetch(assetPackList);
	}

	public void cancelDownload() {
		Log.i(ScummVM.LOG_TAG, "JAVA: cancelDownload");
		List<String> assetPackList = new ArrayList<>();
		assetPackList.add(_currentPackDownload);
		_assetPackManager.cancel(assetPackList);
		_currentPackDownload = "";
	}

	public void setDLCPath(String path) {
		_dlcspath = path;
	}

	public void copyFolder(File sourceFolder, File destFolder) throws IOException {
        if (sourceFolder.isDirectory()) {
            if (!destFolder.exists()) {
                destFolder.mkdirs();
            }

            String[] files = sourceFolder.list();
            if (files != null) {
                for (String file : files) {
                    File srcFile = new File(sourceFolder, file);
                    File destFile = new File(destFolder, file);

                    copyFolder(srcFile, destFile);
                }
            }
        } else {
            InputStream in = null;
			OutputStream out = null;

			try {
				in = new FileInputStream(sourceFolder);
				out = new FileOutputStream(destFolder);

				byte[] buffer = new byte[1024];
				int length;
				while ((length = in.read(buffer)) > 0) {
					out.write(buffer, 0, length);
				}
			} finally {
				in.close();
				out.close();
			}

		}
    }
}
