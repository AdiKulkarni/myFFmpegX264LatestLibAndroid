package com.android.myffmpegx264lib;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends Activity {

	public static final String TAG = "com.android.myffmpegx264lib.MainActivity";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		/*byte[] data = "Aditya".getBytes();
		int outHeight = 640;
		int outWidth = 480;

		// do something here
		frameEncoder encoder = new frameEncoder();
		encoder.setFps(25);
		encoder.setOutHeight(outHeight);
		encoder.setInHeight(outHeight);
		encoder.setOutWidth(outWidth);
		encoder.setInWidth(outWidth);
		encoder.setParams();

		if (encoder.validateSettings()) {
			Log.i(TAG, "Encoder Seems ok");
		} else {
			Log.i(TAG, "Encoder has problems");
			finish();
		}

		if (encoder.open())
			if (data.toString().length() != 0) {
				encoder.encode(data.toString());
				Log.i(TAG, encoder.getOutStreamBuffer());
			} else {
				Log.i(TAG, "Encoder is not open");
				encoder.close();
				encoder.delete();
				finish();
			}*/
	}

	/*static {
		System.loadLibrary("ffmpeg-x264");
	}*/
}
