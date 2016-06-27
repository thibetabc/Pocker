package org.cocos2dx.cpp;

import android.media.MediaPlayer;
import android.util.Log;

import java.io.IOException;

public class SoundPlayer
{
	static MediaPlayer player;
	public static void play_sound(final String filename) throws IllegalArgumentException, SecurityException, IllegalStateException, IOException {
		if(player != null) {
			player.stop();
		}
		player = new MediaPlayer();
		player.setDataSource(filename);
		player.prepare();
		player.start();
	}
	
	public static void stop_play() {
		if(player != null) {
			Log.d("stop_play", "stop_play");
			player.stop();
		}
	}
	
	public static boolean is_playing()
	{
		if( player != null) {
			return player.isPlaying();
		}
		return false;
	}
}