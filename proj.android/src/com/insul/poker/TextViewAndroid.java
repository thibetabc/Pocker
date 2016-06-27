package com.insul.poker;

import android.graphics.Color;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.TextView;

import java.util.HashMap;

public class TextViewAndroid {
	class TextViewData{
		public int index = -1;
		public String text;
		public TextView control = null;
	}
	
	static TextViewAndroid __instence = null;
	
	public static TextViewAndroid sharedInstence() {
		if (__instence == null) {
			__instence = new TextViewAndroid();
		}
		return __instence;
	}

	Integer index = 0;
	HashMap<Integer, TextViewData> editText_container = new HashMap<Integer, TextViewData>();

	TextViewAndroid() {

	}

	int _createTextView() {
		final TextViewData data = new TextViewData();
		data.index = index++;
		editText_container.put(data.index, data);
		
		AppActivity.__instence.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				EditText e = new EditText(AppActivity.getContext());
				e.setBackgroundColor(Color.WHITE);//默认白色  huangfeng 2016-05-24
//				e.setMaxLines(1);
				e.setSingleLine(false);
				e.setFocusable(false);
				e.addTextChangedListener(new TextWatcher() {
					@Override
					public void onTextChanged(CharSequence s, int start, int before, int count) {
					}
					
					@Override
					public void beforeTextChanged(CharSequence s, int start, int count,
							int after) {						
					}
					
					@Override
					public void afterTextChanged(Editable s) {
						data.text = s.toString();
					}
				});
				FrameLayout.LayoutParams par = new FrameLayout.LayoutParams(
						ViewGroup.LayoutParams.MATCH_PARENT,
						ViewGroup.LayoutParams.MATCH_PARENT);
				e.setLayoutParams(par);
				data.control = e;
				FrameLayout layout = AppActivity.sharedInstence().rootLayout();
				layout.addView(e);
			}
		});
		
		return data.index;
	}

	TextViewData _getTextView(int idx) {
		if (editText_container.containsKey(idx)) {
			TextViewData e = editText_container.get(idx);
			return e;
		}
		return null;
	}

	void _removeTextView(final int idx) {
		final TextViewData data = getTextView(idx);
		if (data != null && data.control != null) {
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					FrameLayout layout = AppActivity.sharedInstence().rootLayout();
					layout.removeView(data.control);
					editText_container.remove(idx);
				}
			});
		}
	}
	
	public static TextViewData getTextView(int idx){
		TextViewData txt = TextViewAndroid.sharedInstence()._getTextView(idx);
		return txt;
	}
	
	public static int createTextView(){
		return TextViewAndroid.sharedInstence()._createTextView();
	}
	
	public static void removeTextView(int idx){
		TextViewAndroid.sharedInstence()._removeTextView(idx);
	}
	
	public static boolean is_focus(int idx){
		TextViewData e = getTextView(idx);
		if (e != null) {
			return e.control != null && e.control.hasFocus();
		}
		return false;
	}
	
	public static void set_string(int idx, final String s){
		final TextViewData e = getTextView(idx);
		if (e != null) {
			e.text = s;
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					e.control.setText(s);
				}
			});
		}
	}
	
	public static void set_size(int idx, final int width, final int height) {
		Log.e("", "set_size:"+width+":"+height);
		final TextViewData data = getTextView(idx);
		if (data != null) {			
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					FrameLayout.LayoutParams par = (LayoutParams) data.control.getLayoutParams();
					par.width = width;
					par.height = height;
					data.control.setLayoutParams(par);
				}
			});
		}
	}
	
	public static void set_position(int idx, final int x, final int y) {
		Log.e("", "set_position:"+x+":"+y);
		final TextViewData data = getTextView(idx);
		if (data != null) {		
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					FrameLayout.LayoutParams par = (LayoutParams) data.control.getLayoutParams();
					par.leftMargin = x;
					par.topMargin = y;
					data.control.setLayoutParams(par);
				}
			});
		}
	}

}
