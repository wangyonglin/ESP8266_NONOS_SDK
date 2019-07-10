/*
 * wifi.c
 *
 *  Created on: Dec 30, 2014
 *      Author: Minh
 */
#include "../../app/include/modules/wifi.h"

#include "../../app/include/modules/config.h"
#include "../../app/include/mqtt/debug.h"
#include "../../app/include/mqtt/mqtt_msg.h"
#include "../../app/include/user_config.h"
#include "user_interface.h"
#include "osapi.h"
#include "espconn.h"
#include "os_type.h"
#include "mem.h"
#include "logger.h"
static ETSTimer WiFiLinker;		// 定时器：WIFI连接

WifiCallback wifiCb = NULL;		// WIFI连接成功回调函数

static uint8_t wifiStatus = STATION_IDLE, lastWifiStatus = STATION_IDLE;	// 初始化WIFI状态：STA未接入WIFI


// 定时函数：检查IP获取情况
//==============================================================================
static void ICACHE_FLASH_ATTR wifi_check_ip(void *arg)
{
	struct ip_info ipConfig;

	os_timer_disarm(&WiFiLinker);	// 关闭WiFiLinker定时器

	wifi_get_ip_info(STATION_IF, &ipConfig);		// 获取IP地址

	wifiStatus = wifi_station_get_connect_status();	// 获取接入状态


	// 获取到IP地址
	//-------------------------------------------------------------------------
	if (wifiStatus == STATION_GOT_IP && ipConfig.ip.addr != 0)
	{

		// 获取IP后，每2秒检查一次WIFI连接的正确性【防止WIFI掉线等情况】
		//------------------------------------------------------------------
		os_timer_setfn(&WiFiLinker, (os_timer_func_t *)wifi_check_ip, NULL);
		os_timer_arm(&WiFiLinker, 2000, 0);
	}
	//-------------------------------------------------------------------------


	// 未获取到IP地址
	//-------------------------------------------------------------------------
	else
	{
		if(wifi_station_get_connect_status() == STATION_WRONG_PASSWORD)
		{
			INFO("STATION_WRONG_PASSWORD\r\n");	// 密码错误
			logger(error,"wifi password fail");
			wifi_station_connect();
		}
		else if(wifi_station_get_connect_status() == STATION_NO_AP_FOUND)
		{
			INFO("STATION_NO_AP_FOUND\r\n");	// 未发现对应AP
			logger(error,"AP not found");
			wifi_station_connect();
		}
		else if(wifi_station_get_connect_status() == STATION_CONNECT_FAIL)
		{
			INFO("STATION_CONNECT_FAIL\r\n");	// 连接失败
			logger(error," wifi connect fait");
			wifi_station_connect();
		}else if(wifi_station_get_connect_status() ==STATION_CONNECTING){
			INFO("STATION_CONNECTING\r\n");	// 连接中
			logger(none," wifi connecting");
		}
		else
		{
			INFO("STATION_IDLE\r\n");
			logger(error,"STA is not connected to WIFI");
		}

		// 再次开启定时器
		//------------------------------------------------------------------
		os_timer_setfn(&WiFiLinker, (os_timer_func_t *)wifi_check_ip, NULL);
		os_timer_arm(&WiFiLinker, 500, 0);		// 500Ms定时
	}
	//-------------------------------------------------------------------------

	// 如果WIFI状态改变，则调用[wifiConnectCb]函数
	//-------------------------------------------------------------------------
	if(wifiStatus != lastWifiStatus)
	{
		lastWifiStatus = wifiStatus;	// WIFI状态更新

		if(wifiCb)				// 判断是否设置了[wifiConnectCb]函数
		wifiCb(wifiStatus);		// wifiCb(wifiStatus)=wifiConnectCb(wifiStatus)
	}
}
//==============================================================================


// 连接WIFI：SSID【...】、PASSWORD【...】、WIFI连接成功回调函数【wifiConnectCb】
//====================================================================================================================
void ICACHE_FLASH_ATTR WIFI_Connect(uint8_t* ssid, uint8_t* pass, WifiCallback cb)
{
	struct station_config stationConf;

	INFO("WIFI_INIT\r\n");
	wifi_set_opmode_current(STATION_MODE);		// 设置ESP8266为STA模式

	//wifi_station_set_auto_connect(FALSE);		// 上电不自动连接已记录的AP(已注释，即：上电自动连接已记录的AP(默认))


	//--------------------------------------------------------------------------------------
	wifiCb = cb;	// 函数名赋值：wifiCb可以作为函数名使用，wifiCb(..) == wifiConnectCb(..)


	// 设置STA参数
	//--------------------------------------------------------------------------
	os_memset(&stationConf, 0, sizeof(struct station_config));	// STA信息 = 0
	os_sprintf(stationConf.ssid, "%s", ssid);					// SSID赋值
	os_sprintf(stationConf.password, "%s", pass);				// 密码赋值
	wifi_station_set_config_current(&stationConf);	// 设置STA参数


	// 设置WiFiLinker定时器
	//-------------------------------------------------------------------------------------------------------
	os_timer_disarm(&WiFiLinker);	// 定时器：WIFI连接
	os_timer_setfn(&WiFiLinker, (os_timer_func_t *)wifi_check_ip, NULL);	// wifi_check_ip：检查IP获取情况
	os_timer_arm(&WiFiLinker, 1000, 0);		// 1秒定时(1次)

	//wifi_station_set_auto_connect(TRUE);	// 上电自动连接已记录AP


	wifi_station_connect();		// ESP8266接入WIFI
}
//====================================================================================================================

