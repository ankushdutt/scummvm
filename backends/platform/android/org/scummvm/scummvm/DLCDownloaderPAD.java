package org.scummvm.scummvm;

import android.content.Context;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import com.google.android.play.core.assetpacks.AssetPackManager;
import com.google.android.play.core.assetpacks.AssetPackManagerFactory;
import com.google.android.gms.tasks.Task;

// @RequiresApi(api = Build.VERSION_CODES.N)
public class DLCDownloaderPAD {

	private Context _context;
	private AssetPackManager _assetPackManager;
	private String _onDemandAssetPack = "on_demand_asset_pack";

	// Constructor
	public DLCDownloaderPAD(Context context) {
		// todo Read more about Context
		_context = context;
	}

	private void initAssetPackManager() {
		// todo handle internet connectivity
		// if (isInternetConnected()) {
			if (_assetPackManager == null) {
				_assetPackManager = AssetPackManagerFactory.getInstance(_context);
			}
			registerListener();
		// } else {
		//     Error "Please Connect to Internet"
		// }
	}

	private String getAssetPath(String assetPack) {
		AssetPackLocation assetPackPath = _assetPackManager.getPackLocation(assetPack);

		if (assetPackPath == null) {
			// asset pack is not ready
			return null;
		}

		String assetsFolderPath = assetPackPath.assetsPath();
		return assetsFolderPath;
	}

	private void initOnDemand() {
        String assetsPath = getAssetPath(_onDemandAssetPack);
        if (assetsPath == null) {
            getPackStates(_onDemandAssetPack);
        }
        if (assetsPath != null) {
            File file = new File(assetsPath + File.separator + videoFileName);
            if (file.exists()) {
                playVideoInExoplayer(file);
            }
        }
    }

	private void registerListener() {
		String onDemandAssetPackPath = getAssetPath(_onDemandAssetPack);
		if (onDemandAssetPackPath == null) {
			assetPackManager.registerListener(assetPackStateUpdateListener);
			List<String> assetPackList = new ArrayList<>();
			assetPackList.add(_onDemandAssetPack);
			// async 
			_assetPackManager.fetch(assetPackList);
		} else {
			initOnDemand();
		}
	}

	private void getPackStates(String assetPackName) {
		assetPackManager.getPackStates(Collections.singletonList(assetPackName))
		.addOnCompleteListener(new OnCompleteListener<AssetPackStates>() {
			@Override
			public void onComplete(Task<AssetPackStates> task) {
				AssetPackStates assetPackStates;
				try {
					assetPackStates = task.getResult();
					assetPackState =
							assetPackStates.packStates().get(assetPackName);

					if (assetPackState != null) {
						if (assetPackState.status() == AssetPackStatus.WAITING_FOR_WIFI) {
							totalSizeToDownloadInBytes = assetPackState.totalBytesToDownload();
							if (totalSizeToDownloadInBytes > 0) {
								long sizeInMb = totalSizeToDownloadInBytes / (1024 * 1024);
								if (sizeInMb >= 150) {
									// 
									showWifiConfirmationDialog();
								} else {
									registerListener();
								}
							}
						}
					}
				} catch (Exception e) {
					Log.d("DLC", e.getMessage());
				}
			}
		});
	}

	// Task - https://developers.google.com/android/reference/com/google/android/gms/tasks/Task
	private Task<AssetPackStates> getPackStatesAsync(List<String> packNames) {
		// AssetPackStates have method packStates() -> map<String, AssetPackState>
	}

	private void temp() {
		_assetPackManager
		.getPackStates(Collections.singletonList(_onDemandAssetPack))
		.addOnCompleteListener(new OnCompleteListener<AssetPackStates>() {
			@Override
			public void onComplete(Task<AssetPackStates> task) {
				AssetPackStates assetPackStates;
				try {
					assetPackStates = task.getResult();
					AssetPackState assetPackState =
						assetPackStates.packStates().get(_onDemandAssetPack);
				} catch (RuntimeExecutionException e) {
					Log.d("DLC", e.getMessage());
					return;
				}
			}
		});
	}

}