package org.cocos2dx.cpp;

import android.media.MediaRecorder;
import android.util.Log;

import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

public class VoiceRecorder {
	static MediaRecorder recorder = null;
	static Timer timer = null;
	static int record_time = 0;
	static TimerTask task = null;

	public static void start_record(String path) {
		stop_record();
		recorder = new MediaRecorder();
		recorder.setAudioSource(MediaRecorder.AudioSource.MIC);
		recorder.setOutputFormat(MediaRecorder.OutputFormat.AMR_NB);
		recorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
		Log.e("ok", "record path:" + path);
		recorder.setOutputFile(path);
		try {
			recorder.prepare();
			recorder.start();

			record_time = 0;
			timer = new Timer();
			task = new TimerTask() {
				
				@Override
				public void run() {
					record_time++;
				}
			};
			timer.schedule(task, 0, 1000);
		} catch (IllegalStateException e) {
			Log.e("ok", "some error happen1");
			e.printStackTrace();
		} catch (IOException e) {
			Log.e("ok", "some error happen2");
			e.printStackTrace();
		}
	}

	public static int stop_record() {
		if (timer != null) {
			timer.cancel();
			task.cancel();
			timer = null;
		}
		if (recorder != null) {
			recorder.stop();
			recorder.reset();
			recorder.release();
			recorder = null;
		}
		Log.e("ok", "record time:" + String.valueOf(record_time));
		return record_time;
	}
}
