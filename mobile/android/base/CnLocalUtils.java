/* -*- Mode: Java; c-basic-offset: 4; tab-width: 20; indent-tabs-mode: nil; -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.mozilla.gecko;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Date;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.json.JSONObject;
import org.mozilla.gecko.util.ThreadUtils;
import org.mozilla.gecko.util.UiAsyncTask;

import android.text.TextUtils;
import android.content.SharedPreferences; 
import android.preference.PreferenceManager;  
import android.util.Log;

public class CnLocalUtils {
	
	private static String baiduPrefix = "cn_baidu_";
	private static String mBaiduTrack = "http://m.g-fox.cn/cfbaidu.gif?";
	private static String mTabTrack = "http://m.g-fox.cn/cftab.gif?";
	private static String cmmanagerId = "000000";
	
    public static boolean isBaiduUrl(String url) {
    	if (!TextUtils.isEmpty(url)) {
	    	Pattern p = Pattern.compile("(www|m)\\.baidu\\.com/(s|baidu)");
	        Matcher m = p.matcher(url);
	        if (m.find()) {
	            return true;
	        }
    	}
    	return false;
    }
    
    public static boolean addTabCount() {
    	int num = PrefUtils.getInt("cn_tab_open", 0);
    	PrefUtils.putInt("cn_tab_open", num + 1);
    	Log.d("CnUtilAddTab", "true");
    	return true;
    }
    
    public static boolean clearTabCount() {
    	PrefUtils.putInt("cn_tab_open", 0);
    	Log.d("CnUtilClearTab", "true");
    	return true;
    }
    
    public static boolean addPrivateTabCount() {
    	int num = PrefUtils.getInt("cn_private_tab_open", 0);
    	PrefUtils.putInt("cn_private_tab_open", num + 1);
    	Log.d("CnUtilAddPrivateTab", "true");
    	return true;
    }
    
    public static boolean clearPrivateTabCount() {
    	PrefUtils.putInt("cn_private_tab_open", 0);
    	Log.d("CnUtilClearPrivateTab", "true");
    	return true;
    }
    
    public static boolean addBaiduCount(String type) {
    	int num = PrefUtils.getInt(baiduPrefix + type, 0);
    	PrefUtils.putInt(baiduPrefix + type, num + 1);
    	Log.d("CnUtilAddBaidu", type);
    	return true;
    }
    
    public static boolean clearBaiduCount() {
    	PrefUtils.putInt(baiduPrefix + "baiduTab", 0);
    	PrefUtils.putInt(baiduPrefix + "bookmark", 0);
    	PrefUtils.putInt(baiduPrefix + "history", 0);
    	PrefUtils.putInt(baiduPrefix + "hotwords", 0);
    	PrefUtils.putInt(baiduPrefix + "suggestion", 0);
    	PrefUtils.putInt(baiduPrefix + "location", 0);
    	PrefUtils.putInt(baiduPrefix + "all", 0);
    	return true;
    }
    
    private static Boolean sendTrack (String url) {
    	try {
    		URL aurl = new URL(url);
    		InputStream is = null;
    		HttpURLConnection conn = (HttpURLConnection) aurl.openConnection();
	    	conn.setConnectTimeout(3000);
	    	conn.setReadTimeout(5000);
	    	conn.setDoInput(true);
	    	conn.setUseCaches(false);
	    	conn.connect();
            if (conn.getResponseCode() == 200) {
	             is = conn.getInputStream();
	             if (is != null) {
	            	 return true;
	             }
	 		}
    	} catch (IOException e) {
    	} catch (Exception e) {
    	}
    	return false;
    }
    
    private static void tabTrack() {
    	final String trackUrl = mTabTrack + "cmmanagerId=" + cmmanagerId
    							+ "&tab=" + PrefUtils.getInt("cn_tab_open", 0)
    							+ "&private_tab=" + PrefUtils.getInt("cn_private_tab_open", 0) 
    							+ "&r=" + Math.random();
    	Log.d("CnUtilTabTrack", trackUrl);
    	
    	(new UiAsyncTask<Void, Void, Boolean>(ThreadUtils.getBackgroundHandler()) {
            @Override
            public Boolean doInBackground(Void ... params) {
            	return sendTrack(trackUrl);
            }
            
            @Override
            public void onPostExecute(Boolean sucess) {
            	Date today = new Date();
            	Long current_day = (today.getTime() / (1000 * 60 * 60 * 24));  
            	PrefUtils.putInt("cn_tab_track_time", current_day.intValue());
            	if (sucess) {
            		clearTabCount();
            		clearPrivateTabCount();
            	}
            }

        }).execute();
    }
    
    private static void baiduTrack() {
    	final String trackUrl = mBaiduTrack + "cmmanagerId=" + cmmanagerId
    							+ "baidu_tab=" + PrefUtils.getInt(baiduPrefix + "baiduTab", 0)
								+ "&baidu_bookmark=" + PrefUtils.getInt(baiduPrefix + "bookmark", 0) 
								+ "&baidu_history=" + PrefUtils.getInt(baiduPrefix + "history", 0)
								+ "&baidu_hotwords=" + PrefUtils.getInt(baiduPrefix + "hotwords", 0)
								+ "&baidu_suggestion=" + PrefUtils.getInt(baiduPrefix + "suggestion", 0)
								+ "&baidu_location=" + PrefUtils.getInt(baiduPrefix + "location", 0)
								+ "&baidu_all=" + PrefUtils.getInt(baiduPrefix + "all", 0)
								+ "&r=" + Math.random();
    	Log.d("CnUtilBaiduTrack", trackUrl);
    	
    	(new UiAsyncTask<Void, Void, Boolean>(ThreadUtils.getBackgroundHandler()) {
            @Override
            public Boolean doInBackground(Void... params) {
            	return sendTrack(trackUrl);
            }
            
            @Override
            public void onPostExecute(Boolean sucess) {
            	Date today = new Date();
            	Long current_day = (today.getTime() / (1000 * 60 * 60 * 24));  
            	PrefUtils.putInt("cn_baidu_track_time", current_day.intValue());
            	if (sucess) {
            		clearBaiduCount();
            	}
            }

        }).execute();
    }
    
    private static void checkBaiduTrack() {
    	int last_check = PrefUtils.getInt("cn_baidu_track_time", 0);
    	Date today = new Date();
    	Long current_day = (today.getTime() / (1000 * 60 * 60 * 24));
		if (last_check < current_day.intValue()) {
    		if (cmmanagerId.equals("000000")) {
    			 PrefsHelper.getPref("extensions.cmmanager.cmmanagerId", new PrefsHelper.PrefHandlerBase() {
    				 @Override public void prefValue(String pref, String value) {
	    				 if (!TextUtils.isEmpty(value)) {
	    					 cmmanagerId = value;
	    				 } 
    				 }
    				 @Override public void finish() { 
    					 baiduTrack();
    				 }
    			});
    			 
    		} else {
    			baiduTrack();
    		}
		} 
    }
    
    private static void checkTabTrack() {
    	int last_check = PrefUtils.getInt("cn_tab_track_time", 0);
    	Date today = new Date();
    	Long current_day = (today.getTime() / (1000 * 60 * 60 * 24));
		if (last_check < current_day.intValue()) {
	    	if (cmmanagerId.equals("000000")) {
				 PrefsHelper.getPref("extensions.cmmanager.cmmanagerId", new PrefsHelper.PrefHandlerBase() {
					 @Override 
					 public void prefValue(String pref, String value) {
		   				 if (!TextUtils.isEmpty(value)) {
		   					 cmmanagerId = value;
						 }
					 }
					 @Override
					 public void finish() { 
						 tabTrack();
					 }
				});
			} else {
				tabTrack();
			}
		} 
    }
    
    public static void checkDailyTrack() {
    	checkBaiduTrack();
    	checkTabTrack();
    }
    
}
