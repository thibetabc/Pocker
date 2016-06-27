package com.insul.poker;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.provider.ContactsContract;
import android.provider.MediaStore;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.WindowManager;
import android.widget.Toast;

import com.insul.poker.utils.Util;
import com.tencent.mm.sdk.modelmsg.SendMessageToWX;
import com.tencent.mm.sdk.modelmsg.WXImageObject;
import com.tencent.mm.sdk.modelmsg.WXMediaMessage;
import com.tencent.mm.sdk.modelmsg.WXTextObject;
import com.tencent.mm.sdk.modelmsg.WXWebpageObject;
import com.tencent.mm.sdk.modelpay.PayReq;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.umeng.message.IUmengRegisterCallback;
import com.umeng.message.PushAgent;
import com.umeng.mobclickcpp.MobClickCppHelper;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.Locale;

//import com.tencent.mm.sdk.modelpay.PayReq;

//import umcom.umeng.social.CCUMSocialController;
//import com.umeng.message.example.MyPushIntentService;

public class AppActivity extends Cocos2dxActivity {
	final boolean debugBoolean = false;
	final static int REQUEST_CODE_PICK_IMAGE = 0;
	final static int REQUEST_CODE_CAPTURE_CAMEIA = 1;
	static AppActivity __instence = null;
	private static IWXAPI msgApi;
	String path = null;
	String device_token = "";
	private PushAgent mPushAgent;
	static boolean landscape_mode = false;
	static AppActivity app;
	private static final int START_DOWNLOAD = 100;
	private static final int PERMISSIONS_REQUEST_CODE_ACCESS_READ_CONTACTS = 0x110;
	private static final String TAG = "HJFAppActivity";
	public static final String WEI_XIN_APP_ID = "wx2109735a7ec9925b";
	private static final int THUMB_SIZE = 150;

	//call c++ method
	public static native void allContents(String tJson);
	public static native void clickBack();
	public static native void changeHeadPic(String name);
	public static native void sendDeviceToken(String deviceToken);

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		app = this;
		mPushAgent = PushAgent.getInstance(this);
		mPushAgent.onAppStart();
		mPushAgent.enable(new IUmengRegisterCallback() {

			@Override
			public void onRegistered(String registrationId) {
				//onRegistered方法的参数registrationId即是device_token
				Log.e("==device_token==", registrationId);
				Log.e("==device_token==", "sendDeviceToken");
				sendDeviceToken(registrationId);
			}
		});
		msgApi= WXAPIFactory.createWXAPI(this, null);
		msgApi.registerApp(WEI_XIN_APP_ID);

		MobClickCppHelper.init(this);
		__instence = this;

	}

	public boolean fileIsExists(String filename){
        try{
                File f=new File(filename);
                if(!f.exists()){
                        return false;
                }
                
        }catch (Exception e) {
                // TODO: handle exception
                return false;
        }
        return true;
	}
	public void showNoticeDialog()
	{
		// 构造对话框
		Log.e("showNoticeDialog","showNoticeDialog");
		AlertDialog.Builder builder = new Builder(this);
		builder.setTitle(R.string.soft_update_title);
		builder.setMessage(R.string.soft_update_info);
		
		// 更新
		
		builder.setPositiveButton(R.string.soft_update_updatebtn, new OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				dialog.dismiss();
				// 显示下载对话框
				UpdateManager update_manager = new UpdateManager(AppActivity.this);
				Log.e("installapk", update_manager.toString());
				update_manager.showDownloadDialog();
				
				
			}
		});
		// 稍后更新
		builder.setNegativeButton("cancel", new OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				dialog.dismiss();
			}
		});
		Dialog noticeDialog = builder.create();
		noticeDialog.show();
	}
	public  void installAPK(String apkUrl) 
	{ 
		Message msg = new Message();
		msg.what = AppActivity.START_DOWNLOAD;
		this.handler.sendMessage(msg);
		 
	}  
	

	

	
	static 
	{
        MobClickCppHelper.loadLibrary();
	}
    public Handler handler = new Handler(){
    	public void handleMessage(Message msg)
    	{
    		switch(msg.what)
    		{
    			case AppActivity.START_DOWNLOAD:
    			{
    				showNoticeDialog();
    			}
    		}
    	}
    	};
    public void print()
    {
        device_token = mPushAgent.getRegistrationId();
        Log.e("==token==", device_token);
        //JChangeToScene();
        Log.e("==token==", device_token);
    };
    public IUmengRegisterCallback mRegisterCallback = new IUmengRegisterCallback() {
	    @Override
	    public void onRegistered(String registrationId) 
	    {
	    	Log.e("Test", "Registered");
	    	print();
	    	// TODO Auto-generated method stub
	    	handler.post(new Runnable() 
	    			{
	        
	    				@Override
	    				public void run() 
	    				{
	    					Log.e("onRegistered","==registered==");
	    					// TODO Auto-generated method stub
	    					print();
	    				}
	    			});
	    }
    };

	public static int getStatusBarHeight() {
		int result = 0;
		int resourceId = sharedInstence().getResources().getIdentifier(
				"status_bar_height", "dimen", "android");
		if (resourceId > 0) {
			result = sharedInstence().getResources().getDimensionPixelSize(
					resourceId);
		}
		return result;
	}

	public static int getScreenWidth() {
		DisplayMetrics metric = new DisplayMetrics();
		sharedInstence().getWindowManager().getDefaultDisplay()
				.getMetrics(metric);
		int width = metric.widthPixels;
		Log.d("screent_width", "" + width);
		return width;
	}

	public static int getScreenHeight() {
		DisplayMetrics metric = new DisplayMetrics();
		sharedInstence().getWindowManager().getDefaultDisplay()
				.getMetrics(metric);
		int height = metric.heightPixels;
		Log.d("screent_height", "" + height);
		return height;
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
		Log.d("onConfigurationChanged", "onConfigurationChanged");
	}

	private static void full(boolean enable) {
		if (enable) {

			// sharedInstence().getWindow().clearFlags(
			// WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
			// sharedInstence().getWindow().addFlags(
			// WindowManager.LayoutParams.FLAG_FULLSCREEN);
			WindowManager.LayoutParams attrs = sharedInstence().getWindow()
					.getAttributes();
			attrs.flags |= WindowManager.LayoutParams.FLAG_FULLSCREEN;
			sharedInstence().getWindow().setAttributes(attrs);

		} else {
			// sharedInstence().getWindow().clearFlags(
			// WindowManager.LayoutParams.FLAG_FULLSCREEN);
			// sharedInstence().getWindow().addFlags(
			// WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
			WindowManager.LayoutParams attrs = sharedInstence().getWindow()
					.getAttributes();
			attrs.flags &= (~WindowManager.LayoutParams.FLAG_FULLSCREEN);
			sharedInstence().getWindow().setAttributes(attrs);
		}
	}

//	@Override
//	public void onWindowFocusChanged(boolean hasFocus) {
//		// TODO Auto-generated method stub
//		super.onWindowFocusChanged(hasFocus);
//		full(AppActivity.landscape_mode);
//	}

	public static void landscape() {
		AppActivity.landscape_mode = true;
		
//		sharedInstence().setRequestedOrientation(
//				ActivityInfo.SCREEN_ORIENTATION_USER_LANDSCAPE);
		
		AppActivity.__instence.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				sharedInstence().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
				full(AppActivity.landscape_mode);
			}
		});
		
//		sharedInstence().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		// sharedInstence().getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
		// WindowManager.LayoutParams.FLAG_FULLSCREEN);
	}

	public static void portrait() {
		AppActivity.landscape_mode = false;
//		
		AppActivity.__instence.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				sharedInstence().setRequestedOrientation(
						ActivityInfo.SCREEN_ORIENTATION_USER_PORTRAIT);
				full(AppActivity.landscape_mode);
			}
		});
//		sharedInstence().setRequestedOrientation(
//				ActivityInfo.SCREEN_ORIENTATION_USER_PORTRAIT);
		// sharedInstence().getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
		// WindowManager.LayoutParams.FLAG_FULLSCREEN);
	}

	public static AppActivity sharedInstence() {
		return __instence;
	}

	public static void start_capture(String path, int type) {
		__instence.path = path;
		if (type == 0) {
			__instence.getImageFromCamera();
		} else {
			__instence.getImageFromAlbum();
		}
	}

	protected void getImageFromAlbum() {
		Intent intent = new Intent(Intent.ACTION_PICK);
		intent.setType("image/*");
		startActivityForResult(intent, REQUEST_CODE_PICK_IMAGE);
	}

	protected void getImageFromCamera() {
		String state = Environment.getExternalStorageState();
		if (state.equals(Environment.MEDIA_MOUNTED)) {
			Intent getImageByCamera = new Intent(
					"android.media.action.IMAGE_CAPTURE");
			startActivityForResult(getImageByCamera,
					REQUEST_CODE_CAPTURE_CAMEIA);
		}
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		Log.e("onActivityResult", "onActivityResult");
		super.onActivityResult(requestCode, resultCode, data);
		Log.e("ok", "requestCode:" + String.valueOf(requestCode));
		Log.e("ok", "resultCode:" + String.valueOf(resultCode));
		//取消拍照
		if (resultCode == 0) 
		{
			clickBack();
			return;
		}
		if (data != null) {
			if (data.getData() != null) {
				Uri uri = data.getData();
				Log.e("ok", "uri.getPath():" + uri.getPath());
				try {
					Bitmap bitmap = MediaStore.Images.Media.getBitmap(
							this.getContentResolver(), uri);
					saveImage(bitmap, path);
				} catch (FileNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			} else {
				Bundle bundle = data.getExtras();
				if (bundle != null) {
//					Bitmap photo = (Bitmap) bundle.get("data");
//					Log.e("ok", "save data");
//					saveImage(photo, path);

					Bitmap photo = getResizedBitmap((Bitmap) bundle.get("data"), 600, 960);
					saveImage(photo, path);
//					photo.recycle();
				} else {
				}
			}
		}
	}
	
	private Bitmap getResizedBitmap(Bitmap bm, int newWidth, int newHeight) {
	    int width = bm.getWidth();
	    int height = bm.getHeight();
	    float scaleWidth = ((float) newWidth) / width;
	    float scaleHeight = ((float) newHeight) / height;
	    // CREATE A MATRIX FOR THE MANIPULATION
	    Matrix matrix = new Matrix();
	    // RESIZE THE BIT MAP
	    matrix.postScale(scaleWidth, scaleHeight);
	
	    // "RECREATE" THE NEW BITMAP
	    Bitmap resizedBitmap = Bitmap.createBitmap(
	        bm, 0, 0, width, height, matrix, false);
	    bm.recycle();
	    return resizedBitmap;
	}

//	@Override
//	public boolean onKeyUp(final int keyCode, KeyEvent event) {
//		Log.e("==onkeyup1==", "onkeyup1");
//		AppActivity.__instence.runOnGLThread(new Runnable() {
//			@Override
//			public void run() {
//				if (keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE) {
//					clickBack();
//					Log.e("==onkeyup2==", "onkeyup2");
//				} 
//			}
//		});
//		return false;
//	}
	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {
		
        if(event.getKeyCode() == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_UP)
        {
        	Log.e("==click_back==", "click_back");
        	clickBack();
        }
        return super.dispatchKeyEvent(event);
    }
	
	public static Bitmap scaleImage(Bitmap photo) {
		Matrix matrix = new Matrix();
		// int width = photo.getWidth();
		// int height = photo.getHeight();
		// int newWidth = 120;
		// int newHeight = 120;
		float scaleWidth = 1.0f;
		float scaleHeight = 1.0f;
		matrix.postScale(scaleWidth, scaleHeight);
		Bitmap resizeBmp = Bitmap.createBitmap(photo, 0, 0, photo.getWidth(),
				photo.getHeight(), matrix, true);
		return resizeBmp;
	}

	

//	public static void saveImage(Bitmap photo, String spath) {
//		try {
//			BufferedOutputStream bos = new BufferedOutputStream(
//					new FileOutputStream(spath, false));
//
//			Bitmap new_photo = scaleImage(photo);
//			new_photo.compress(Bitmap.CompressFormat.JPEG, 100, bos);
//			bos.flush();
//			bos.close();
//
//			changeHeadPic(spath);
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
//	}
	public static void saveImage(Bitmap image, String spath) {
		try {
			ByteArrayOutputStream os = new ByteArrayOutputStream();
			// scale
			int options = 100;
			// Store the bitmap into output stream(no compress)
	        image.compress(Bitmap.CompressFormat.JPEG, options, os);  
	        // Compress by loop
	        while (os.toByteArray().length / 1024 > 200) {
	            // Clean up os
	        	os.reset();
	        	// interval 10
	            options -= 10;
	            image.compress(Bitmap.CompressFormat.JPEG, options, os);
	        }
	        
	        // Generate compressed image file
	        FileOutputStream fos = new FileOutputStream(spath);  
	        fos.write(os.toByteArray());  
	        fos.flush();  
	        fos.close();  
	        changeHeadPic(spath);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	
	/*
     * 读取联系人的信息
     */
    @SuppressLint("NewApi")
	@TargetApi(23)
	public void ReadAllContacts()
    {
//        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.M &&
//           checkSelfPermission(Manifest.permission.READ_CONTACTS) != PackageManager.PERMISSION_GRANTED)
//        {
//            requestPermissions(new String[]{Manifest.permission.READ_CONTACTS},
//                               PERMISSIONS_REQUEST_CODE_ACCESS_READ_CONTACTS);
//            
//        }
//        else
//        {
//            //android6.0以下直接读取
//            ToReadAllContacts();
//        }
    	
		if (Build.VERSION.SDK_INT >= 23)
		{
//			Android 6.0 release removes support for the Apache HTTP client. If your app is using this client and targets Android 2.3 (API level 9) or higher, use theHttpURLConnection class instead. This API is more efficient because it reduces network use through transparent compression and response caching, and minimizes power consumption.
//
//			简单翻译一下就是： Android 6.0不再支持 Apache HTTP client。 Utils.java
		} else {
			// android6.0以下直接读取
			ToReadAllContacts();
		}
	}
    
    //授权回调
//    @Override
//	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults)
//    {
//		switch (requestCode) 
//		{
//		case PERMISSIONS_REQUEST_CODE_ACCESS_READ_CONTACTS:
//			if (grantResults[0] == PackageManager.PERMISSION_GRANTED) 
//			{
//				// 获得授权后处理方法
//				ToReadAllContacts();
//			}
//			else
//			{
//				//拒绝授权后处理方法
//			}
//			break;
//		}
//		
//	}
    
    
    
    /*
     * 读取联系人的信息
     */
    public void ToReadAllContacts() {
        
        class ContactsData
        {
            String id;
            public String name;
            public Collection<String> telephone;
            public Collection<String> email;
        }
        
        Collection<ContactsData> collections = new ArrayList<ContactsData>();
        
        Cursor cursor = getContentResolver().query(ContactsContract.Contacts.CONTENT_URI,
                                                   null, null, null, null);
        int contactIdIndex = 0;
        int nameIndex = 0;
        if(cursor.getCount() > 0) {
            contactIdIndex = cursor.getColumnIndex(ContactsContract.Contacts._ID);
            nameIndex = cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME);
        }
        while(cursor.moveToNext()) {
            ContactsData contactsData = new ContactsData();
            String contactId = cursor.getString(contactIdIndex);
            String name = cursor.getString(nameIndex);
            contactsData.id = contactId;
            contactsData.name = name;
            /*
             * 查找该联系人的phone信息
             */
            Cursor phones = getContentResolver().query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
                                                       null,
                                                       ContactsContract.CommonDataKinds.Phone.CONTACT_ID + "=" + contactId,
                                                       null, null);
            int phoneIndex = 0;
            if(phones.getCount() > 0) {
                phoneIndex = phones.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER);
            }
            contactsData.telephone = new ArrayList<String>();
            while(phones.moveToNext()) {
                String phoneNumber = phones.getString(phoneIndex);
                contactsData.telephone.add(phoneNumber);
            }
            phones.close();
            /*
             * 查找该联系人的email信息
             */
            Cursor emails = getContentResolver().query(ContactsContract.CommonDataKinds.Email.CONTENT_URI,
                                                       null,
                                                       ContactsContract.CommonDataKinds.Email.CONTACT_ID + "=" + contactId,
                                                       null, null);
            int emailIndex = 0;
            if(emails.getCount() > 0) {
                emailIndex = emails.getColumnIndex(ContactsContract.CommonDataKinds.Email.DATA);
            }
            contactsData.email = new ArrayList<String>();
            while(emails.moveToNext()) {
                String email = emails.getString(emailIndex);
                contactsData.email.add(email);
            }
            emails.close();
            collections.add(contactsData);
            
            
        }
        cursor.close();
        // JSON格式
        //        {
        //            "all_contacts": [
        //            {
        //                "id": "112000",
        //                    "name": "赵四",
        //                    "all_telephone": [
        //                {
        //                    "telephone": "112200"
        //                },
        //                {
        //                    "telephone": "112200"
        //                }
        //                ],
        
        
        //                "all_email": [
        //                {
        //                    "email": "abc@gmail.com"
        //                },
        //                {
        //                    "email": "abc@gmail.com"
        //                }
        //                ]
        //            }
        //            ]
        //        }
        String json = "{\"all_contacts\":[";
        String tmpJson = "";
        String tempJson1 = "";
        Log.i(TAG, "=====找到通讯录数量=======> collections.size  = " + collections.size());
        int i = 0;



		for(Iterator itContact = collections.iterator(); itContact.hasNext();)
        {
			ContactsData cd = (ContactsData)itContact.next();
            tempJson1 = "";
            tmpJson += "{\"id\":\"" + cd.id + "\"" + ",\"name\":\"" + cd.name + "\", \"all_telephone\":[";
            Log.i(TAG, "i = " + (++i) + ", id = " + cd.id + ", name = " + cd.name);
			for(Iterator itTelephone = cd.telephone.iterator(); itTelephone.hasNext();)
            {
				String telephone = (String)itTelephone.next();
                tempJson1 += "{\"telephone\":\"" + telephone + "\"}";
				if(itTelephone.hasNext())
				{
					tempJson1 += ",";
				}
                Log.i(TAG, "telephone = " + telephone);
            }
            tmpJson += tempJson1 + "], \"all_email\": [";
            tempJson1 = "";
			for(Iterator itEmail = cd.email.iterator(); itEmail.hasNext();)
            {
				String email = (String)itEmail.next();
                tempJson1 += "{\"email\": \"" + email + "\"}";
				if (itEmail.hasNext())
				{
					tempJson1 += ",";
				}
                Log.i(TAG, "email = " + email);
            }
            tmpJson += tempJson1 + "]}";
			if (itContact.hasNext())
			{
				tmpJson += ",";
			}


        }
        json += tmpJson + "]}";
        Log.i(TAG, "json ===> " + json);
       if(collections.size() > 0)
       {
    	   allContents(json);
       }
        
    }
    
    //发短信
    void sendMessageView(String phone, String title, String msg)
    {
    	Uri uri = Uri.parse("smsto:"+phone);
    	Intent it = new Intent(Intent.ACTION_SENDTO, uri);     
    	it.putExtra("sms_body", msg);
    	startActivity(it); 
    }

//	//分享平台
//	enum TargetShare
//	{
//		WeiXinWeChat,
//		WeiXinCircle,
//	};
//	//分享类型
//	enum ShareType
//	{
//		Text,  //文字类型
//		Image, //图片类型
//		Music, //音乐类型
//		Video, //视频类型
//		Web,   //网页类型
//
//	};
	void shareWeixin(int targetShare,  int shareType, String title, String description, String linkUrl, String imagePath)
	{

		if(!msgApi.isWXAppInstalled())
		{
			Toast.makeText(this, "您没有安装微信App", Toast.LENGTH_SHORT).show();
			return;
		}

		int scene;

		switch(targetShare)
		{
//			case TargetShare::WeiXinWeChat:
			case 0:
				scene = SendMessageToWX.Req.WXSceneSession;
				break;
//			case TargetShare::WeiXinCircle:
			case 1:
				scene = SendMessageToWX.Req.WXSceneTimeline;
				break;
			default:
				scene = SendMessageToWX.Req.WXSceneSession;
				break;
		}

//		if(shareType == ShareType::Text)
		if(shareType == 0)
		{
			WXTextObject textObject = new WXTextObject();
			textObject.text = description;

			WXMediaMessage msg = new WXMediaMessage();
			msg.mediaObject = textObject;
			msg.description = textObject.text;

			SendMessageToWX.Req req = new SendMessageToWX.Req();
			req.transaction = String.valueOf(System.currentTimeMillis());
			req.message = msg;
			req.scene = scene;
			msgApi.sendReq(req);
		}
//		else if(shareType == ShareType::Image)
		else if(shareType == 1)
		{
			Log.i(TAG, "android微信分享-图片保存路径" + imagePath);
			WXImageObject imgObj = new WXImageObject();
			imgObj.setImagePath(imagePath);

			WXMediaMessage msg = new WXMediaMessage();
			msg.mediaObject = imgObj;

			Bitmap bmp = BitmapFactory.decodeFile(imagePath);
			Bitmap thumbBmp = Bitmap.createScaledBitmap(bmp, THUMB_SIZE, THUMB_SIZE, true);
			bmp.recycle();
			msg.thumbData = Util.bmpToByteArray(thumbBmp, true);

			SendMessageToWX.Req req = new SendMessageToWX.Req();
			req.transaction = buildTransaction("img");
			req.message = msg;
			req.scene = scene;
//			req.openId = getOpenId();
			boolean isSuccss = msgApi.sendReq(req);
			Log.i(TAG, isSuccss ? "微信请求成功" : "微信请求失败");
		}

//		else if(shareType == ShareType::Web)
		else if(shareType == 4)
		{
			WXWebpageObject webpage = new WXWebpageObject();
			webpage.webpageUrl = linkUrl;
			WXMediaMessage msg = new WXMediaMessage(webpage);
			msg.title = title;
			msg.description = description;
			Bitmap thumb = getImageFromAssetsFile(imagePath);
			if(thumb == null)
			{
				Log.i(TAG, "读取文件[" + imagePath + "]失败");
			}
			else
			{
				Log.i(TAG, "读取文件[" + imagePath + "]成功");
				msg.thumbData = Util.bmpToByteArray(thumb, true);
			}
			SendMessageToWX.Req req = new SendMessageToWX.Req();
			req.transaction = buildTransaction("webpage");
			req.message = msg;
			req.scene = scene;
			msgApi.sendReq(req);
		}
	}
	private String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis()) : type + System.currentTimeMillis();
	}
	public static IWXAPI getWxApi()
	{
		return msgApi;
	}

	public static Object GetAppActivity()
	{
		return app;
	}
	public void SendPayWX(String appID,String partnerID,String prepayId , String nonceStr , String timeStamp , String sign)
	{
		if(!msgApi.isWXAppInstalled())
		{
			Toast.makeText(this, "您没有安装微信App", Toast.LENGTH_SHORT).show();
			return;
		}

		PayReq req = new PayReq();
		req.appId = appID;
		req.partnerId = partnerID;
		req.prepayId = prepayId;
		req.packageValue = "Sign=WXPay";
		req.nonceStr = nonceStr;
		req.timeStamp = timeStamp;
		req.sign = sign;

		Log.e(TAG, "充值——android----- req.appIdd = "+ req.appId);
		Log.e(TAG, "充值——android----- req.partnerId = " + req.partnerId);
		Log.e(TAG, "充值——android----- req.prepayId = " + req.prepayId);
		Log.e(TAG, "充值——android----- req.packageValue = " + req.packageValue);
		Log.e(TAG, "充值——android----- req.nonceStr = " + req.nonceStr);
		Log.e(TAG, "充值——android----- req.timeStamp = " + req.timeStamp);
		Log.e(TAG, "充值——android----- req.sign = " + req.sign);
		msgApi.sendReq(req);
	}
	//多语言-获取地区
	public String getCurrentCountry()
	{
		return Locale.getDefault().getCountry();
	}
	//从Assets中读取图片
	private Bitmap getImageFromAssetsFile(String fileName)
	{
		Bitmap image = null;
		AssetManager am = getResources().getAssets();
		try
		{
			InputStream is = am.open(fileName);
			image = BitmapFactory.decodeStream(is);
			is.close();
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}

		return image;

	}

}