package com.insul.poker;

import android.graphics.Color;
import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;
import android.text.method.PasswordTransformationMethod;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;

import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Timer;
import java.util.TimerTask;

public class EditBoxPool {
	class EditBoxData {
		public int index = -1;
		public String text;
		public EditText control = null;
		public boolean is_password = false;
		public int return_type = 0;
		public int keyborad_type = 0;
		public int input_flag = -1;
		public boolean visible = true;
		public boolean editable = true;
	}

	static final int ANY = 0;
	static final int EMAIL_ADDRESS = 1;
	static final int NUMERIC = 2;
	static final int PHONE_NUMBER = 3;
	static final int URL = 4;
	static final int DECIMAL = 5;
	static final int SINGLE_LINE = 6;

	static final int PASSWORD = 0;
	static final int SENSITIVE = 1;
	static final int INITIAL_CAPS_WORD = 2;
	static final int INITIAL_CAPS_SENTENCE = 3;
	static final int INTIAL_CAPS_ALL_CHARACTERS = 4;

	static final int kKeyboardReturnTypeDefault = 0;
	static final int kKeyboardReturnTypeDone = 1;
	static final int kKeyboardReturnTypeSend = 2;
	static final int kKeyboardReturnTypeSearch = 3;
	static final int kKeyboardReturnTypeGo = 4;

	// public static native void send(int idx);
	public static native void editBoxEditingDidBegin(int idx);

	public static native void editBoxEditingDidEnd(int idx);

	public static native void editBoxReturn(int idx);

	public static native void editBoxTextChanged(int idx, String text);

	static EditBoxPool __instence = null;

	public static EditBoxPool sharedInstence() {
		if (__instence == null) {
			__instence = new EditBoxPool();
		}
		return __instence;
	}

	Integer index = 0;
	HashMap<Integer, EditBoxData> editText_container = new HashMap<Integer, EditBoxData>();

	EditBoxPool() {

	}

	int _createEditBox() {
		final EditBoxData data = new EditBoxData();
		data.index = index++;
		editText_container.put(data.index, data);

		AppActivity.__instence.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				final EditText e = new EditText(AppActivity.getContext());
				Log.d("_createEditBox", "_createEditBox");
				data.control = e;
				e.setBackgroundColor(Color.TRANSPARENT);
				e.setMaxLines(1);
				e.addTextChangedListener(new TextWatcher() {
					@Override
					public void onTextChanged(CharSequence s, int start,
							int before, int count) {
						Log.d("onTextChanged", "onTextChanged");
						editBoxTextChanged(data.index, s.toString());
					}

					@Override
					public void beforeTextChanged(CharSequence s, int start,
							int count, int after) {
						Log.d("beforeTextChanged", "beforeTextChanged");
					}

					@Override
					public void afterTextChanged(Editable s) {
						Log.d("afterTextChanged", "afterTextChanged");
						data.text = s.toString();
					}
				});

				e.setOnFocusChangeListener(new android.view.View.OnFocusChangeListener() {

					@Override
					public void onFocusChange(View v, boolean hasFocus) {

						if (hasFocus) {
							// �˴�Ϊ�õ�����ʱ�Ĵ�������
							editBoxEditingDidBegin(data.index);

							final Timer timer = new Timer(true);

							TimerTask task = new TimerTask() {
								public void run() {
									for (Entry<Integer, EditBoxData> entry : editText_container
											.entrySet()) {
										EditBoxData data = entry.getValue();
										final EditText control = data.control;
										if (control != null) {
											AppActivity.__instence
													.runOnUiThread(new Runnable() {
														@Override
														public void run() {
															control.getParent()
																	.bringChildToFront(
																			control);
														}
													});
										}
									}
								}
							};

							timer.schedule(task, 50, 1000); // ��ʱ1000ms��ִ�У�1000msִ��һ��
						} else {
							// �˴�Ϊʧȥ����ʱ�Ĵ�������submitRequest
							editBoxEditingDidEnd(data.index);
						}

					}

				});

				FrameLayout.LayoutParams par = new FrameLayout.LayoutParams(
						ViewGroup.LayoutParams.MATCH_PARENT,
						ViewGroup.LayoutParams.MATCH_PARENT);
				e.setLayoutParams(par);

				FrameLayout layout = AppActivity.sharedInstence().rootLayout();
				layout.addView(e);

			}
		});

		return data.index;
	}

	EditBoxData _getEditBox(int idx) {
		if (editText_container.containsKey(idx)) {
			EditBoxData e = editText_container.get(idx);
			return e;
		}
		return null;
	}

	void _removeEditBox(final int idx) {
		final EditBoxData data = getEditBox(idx);
		if (data != null && data.control != null) {
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					FrameLayout layout = AppActivity.sharedInstence()
							.rootLayout();
					layout.removeView(data.control);
					editText_container.remove(idx);
				}
			});
		}
	}

	public static EditBoxData getEditBox(int idx) {
		EditBoxData txt = EditBoxPool.sharedInstence()._getEditBox(idx);
		return txt;
	}

	public static int createEditBox() {
		return EditBoxPool.sharedInstence()._createEditBox();
	}

	public static void removeEditBox(int idx) {
		EditBoxPool.sharedInstence()._removeEditBox(idx);
	}

	public static boolean is_focus(int idx) {
		EditBoxData e = getEditBox(idx);
		if (e != null) {
			return e.control != null && e.control.hasFocus();
		}
		return false;
	}

	public static boolean is_password(int idx) {
		EditBoxData e = getEditBox(idx);
		if (e != null) {
			return e.is_password;
		}
		return false;
	}

	public static boolean is_hidden(int idx) {
		EditBoxData e = getEditBox(idx);
		if (e != null) {
			return e.visible;
		}
		return false;
	}

	public static void set_text(int idx, final String s) {
		final EditBoxData e = getEditBox(idx);
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

	public static void set_background_color(int idx, final int r, final int g,
			final int b) {
		final EditBoxData e = getEditBox(idx);
		if (e != null) {
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					Log.e("", "e.control.setBackgroundColor:");
					e.control.setBackgroundColor(Color.rgb(r, g, b));
				}
			});
		}
	}

	public static void set_text_color(int idx, final int r, final int g,
			final int b) {
		final EditBoxData e = getEditBox(idx);
		if (e != null) {
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					e.control.setTextColor(Color.rgb(r, g, b));
				}
			});
		}
	}

	public static void set_mult_line(int idx, final boolean mult) {
		final EditBoxData e = getEditBox(idx);
		if (e != null) {
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					e.control.setSingleLine(!mult);
				}
			});
		}
	}

	public static void set_keyborad_type(int idx, final int type) {
		final EditBoxData e = getEditBox(idx);
		if (e != null) {
			e.keyborad_type = type;

			if (e.control != null) {
				AppActivity.__instence.runOnUiThread(new Runnable() {
					@Override
					public void run() {
						switch (type) {
						case EMAIL_ADDRESS:
							// _systemControl.textField.keyboardType =
							// UIKeyboardTypeEmailAddress;
							e.control
									.setInputType(InputType.TYPE_CLASS_TEXT
											| InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS);
							break;
						case NUMERIC:
							e.control.setInputType(InputType.TYPE_CLASS_NUMBER
									| InputType.TYPE_NUMBER_FLAG_SIGNED);
							break;
						case PHONE_NUMBER:
							e.control.setInputType(InputType.TYPE_CLASS_PHONE);
							break;
						case URL:
							e.control.setInputType(InputType.TYPE_CLASS_TEXT
									| InputType.TYPE_TEXT_VARIATION_URI);
							break;
						case DECIMAL:
							e.control.setInputType(InputType.TYPE_CLASS_NUMBER
									| InputType.TYPE_NUMBER_FLAG_DECIMAL
									| InputType.TYPE_NUMBER_FLAG_SIGNED);
							break;
						case SINGLE_LINE:
							// _systemControl.textField.keyboardType =
							// UIKeyboardTypeDefault;
							e.control.setInputType(InputType.TYPE_CLASS_TEXT);
							break;
						default:
							// _systemControl.textField.keyboardType =
							// UIKeyboardTypeDefault;
							break;
						}
					}
				});

			}

		}
	}

	public static void set_keyboard_input_flag(int idx, final int flag) {
		Log.d("TYPE_TEXT_VARIATION_PASSWORD", "" + flag);
		final EditBoxData e = getEditBox(idx);
		if (e != null) {
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					e.input_flag = flag;
					if (e.control != null) {

						switch (flag) {
						case PASSWORD:
							Log.d("TYPE_TEXT_VARIATION_PASSWORD",
									"TYPE_TEXT_VARIATION_PASSWORD");

							e.control
									.setInputType(InputType.TYPE_TEXT_VARIATION_PASSWORD);
							e.control
									.setTransformationMethod(PasswordTransformationMethod
											.getInstance());
							break;
						case SENSITIVE:
							e.control
									.setInputType(InputType.TYPE_TEXT_FLAG_AUTO_COMPLETE);
							break;
						case INITIAL_CAPS_WORD:
							e.control
									.setInputType(InputType.TYPE_TEXT_FLAG_CAP_SENTENCES);
							break;
						case INITIAL_CAPS_SENTENCE:
							e.control
									.setInputType(InputType.TYPE_TEXT_FLAG_AUTO_COMPLETE
											| InputType.TYPE_TEXT_FLAG_CAP_SENTENCES);
							break;
						case INTIAL_CAPS_ALL_CHARACTERS:
							e.control
									.setInputType(InputType.TYPE_TEXT_FLAG_AUTO_COMPLETE);
							break;
						default:

							break;

						}
					} else {
						Log.d("TYPE_TEXT_VARIATION_PASSWORD",
								"e.control is null");
					}
				}
			});
		}
	}

	public static void set_return_type(int idx, final int type) {
		final EditBoxData e = getEditBox(idx);
		if (e != null) {
			e.return_type = type;
			if (e.control != null) {
				AppActivity.__instence.runOnUiThread(new Runnable() {
					@Override
					public void run() {
						switch (type) {
						case kKeyboardReturnTypeDefault:
							e.control.setImeOptions(EditorInfo.IME_ACTION_NONE);
							break;
						case kKeyboardReturnTypeDone:
							e.control.setImeOptions(EditorInfo.IME_ACTION_DONE);
							break;
						case kKeyboardReturnTypeSend:
							e.control.setImeOptions(EditorInfo.IME_ACTION_SEND);
							break;
						case kKeyboardReturnTypeSearch:
							e.control
									.setImeOptions(EditorInfo.IME_ACTION_SEARCH);
							break;
						case kKeyboardReturnTypeGo:
							e.control.setImeOptions(EditorInfo.IME_ACTION_GO);
							break;
						default:

							break;

						}

						android.view.View.OnKeyListener listener = new android.view.View.OnKeyListener() {

							@Override
							public boolean onKey(View arg0, int keyCode,
									KeyEvent arg2) {
								// TODO Auto-generated method stub
								if (keyCode == KeyEvent.KEYCODE_ENTER
										&& arg2.getAction() == KeyEvent.ACTION_UP) {
									if (e.return_type == kKeyboardReturnTypeSend) {
										Log.d("kKeyboardReturnTypeSend",
												"kKeyboardReturnTypeSend");
										// send(e.index);
										editBoxReturn(e.index);
									} else {
									}
								}
								return false;
							}
						};
						e.control.setOnKeyListener(listener);
					}

				});

			}
		}
	}

	public static void set_editable(int idx, final boolean able) {
		final EditBoxData e = getEditBox(idx);
		e.editable = able;
		if (e != null) {
//			e.control.setFocusable(able);
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					Log.d("set_editable", "" + able);
					e.control.setFocusableInTouchMode(able);
					e.control.setFocusable(able);
				}
			});
		}
	}
	
	public static boolean is_editable(int idx) {
		final EditBoxData e = getEditBox(idx);
		if (e != null) {
			return e.editable;
		}
		return false;
	}

	public static void set_visible(int idx, final boolean visible) {
		final EditBoxData e = getEditBox(idx);
		if (e != null) {
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					e.control.setVisibility(visible ? View.VISIBLE
							: View.INVISIBLE);
				}
			});
		}
	}

	public static void set_placeholder(int idx, final String s) {
		Log.e("", "set_placeholder:" + s);
		final EditBoxData e = getEditBox(idx);
		if (e != null) {
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					e.control.setHint(s);
				}
			});
		}
	}

	public static void set_size(int idx, final int width, final int height) {
		Log.e("", "set_size:" + width + ":" + height);
		final EditBoxData data = getEditBox(idx);
		if (data != null) {
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					FrameLayout.LayoutParams par = (LayoutParams) data.control
							.getLayoutParams();
					par.width = width;
					par.height = height;
					data.control.setLayoutParams(par);
				}
			});
		}
	}

	public static void set_position(int idx, final int x, final int y) {
		Log.e("", "set_position:" + x + ":" + y);
		final EditBoxData data = getEditBox(idx);
		if (data != null) {
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					FrameLayout.LayoutParams par = (LayoutParams) data.control
							.getLayoutParams();
					par.leftMargin = x;
					par.topMargin = y;
					data.control.setLayoutParams(par);
				}
			});
		}
	}

	public static String text(int idx) {
		EditBoxData e = getEditBox(idx);
		if (e != null) {
			return e.text;
		}
		return "";
	}

	public static void open_keyborad(int idx) {
		final EditBoxData data = getEditBox(idx);
		if (data != null) {
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					data.control.requestFocus();
				}
			});
		}
	}

	public static void close_keyborad(int idx) {
		final EditBoxData e = getEditBox(idx);
		if (e != null && e.control != null) {
			AppActivity.__instence.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					e.control.clearFocus();
					e.control.setVisibility(View.INVISIBLE);
				}
			});
		}
	}
}
