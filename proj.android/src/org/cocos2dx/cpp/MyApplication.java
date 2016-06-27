package org.cocos2dx.cpp;

import android.app.Application;
import android.content.Context;
import android.widget.Toast;

import com.umeng.common.message.Log;
import com.umeng.message.PushAgent;
import com.umeng.message.UmengNotificationClickHandler;
import com.umeng.message.entity.UMessage;

public class MyApplication extends Application {
	
	private static final String TAG = MyApplication.class.getName();
	private PushAgent mPushAgent;
    public static native void DealWithPushMessage();
    public static native  void JChangeToScene();
    public static String room_id = "";
    public static String group_id = "";
    public static int m_bAppLaunched = 0;
    public static void setAppStatus(int IsLaunched)
    {
    		m_bAppLaunched = IsLaunched;
    }
    public static String getPushData()
    {
    		String s = room_id;
    		room_id = "";
    		return s;
    }
    public static String getGroupID()
    {
    		String s = group_id;
    		group_id = "";
		return s;
    }
    public static void setPushData()
    {
    		room_id = "";
    		group_id = "";
    }
	@Override
	public void onCreate() {
		super.onCreate();
		Log.e("mark", "MyApplication");
		mPushAgent = PushAgent.getInstance(this);
		mPushAgent.setDebugMode(true);
			
		UmengNotificationClickHandler notificationClickHandler = new UmengNotificationClickHandler(){
			
			//*****************************************
			@Override
			public void launchApp(Context context, UMessage msg)
			{
				Log.e("guojiabin", msg.custom);
				Toast.makeText(context, msg.extra.toString(), Toast.LENGTH_LONG).show();
				room_id = msg.extra.get("room_id");
				group_id = msg.extra.get("group_id");
				if (m_bAppLaunched == 1)
				{
					DealWithPushMessage();
				}
				else 
				{
					super.launchApp(context, msg);
				}
				
			}
			//******************************************
			
			
		};
		mPushAgent.setNotificationClickHandler(notificationClickHandler);
	}

}
