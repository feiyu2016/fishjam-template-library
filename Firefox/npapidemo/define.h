#ifndef _DEFINE_H_
#define _DEFINE_H_

#define  UM_SHOWCAPTUREDOCMENT WM_USER+1 
#define  UM_SETCOMMAND         WM_USER+2
#define  UM_SETGESTURELINE     WM_USER+4


enum ACTION_TYPE
{
	ACTIONTYPE_LOCATION_URL = 0,
	ACTIONTYPE_LOCATION_SEARCH,
	ACTIONTYPE_NAVIGATE_CLICK,
	ACTIONTYPE_NAVIGATE_JSCRIPT,
	ACTIONTYPE_NAVIGATE_METAREFRESH,
	ACTIONTYPE_NAVIGATE_REDIRECT,
	ACTIONTYPE_NAVIGATE_MENUMOVE,
	ACTIONTYPE_NAVIGATE_UNKNWN,
	ACTIONTYPE_BUTTON_FORWARD,
	ACTIONTYPE_BUTTON_BACK,
	ACTIONTYPE_BUTTON_REFRESH,
	ACTIONTYPE_BUTTON_HOME,
	ACTIONTYPE_BUTTON_SEARCH,
	ACTIONTYPE_BUTTON_IE7_SP,
	ACTIONTYPE_BUTTON_FAVORITE,
	ACTIONTYPE_BUTTON_HISTORY,
	ACTIONTYPE_BUTTON_CONN,
	ACTIONTYPE_BUTTON_UNKWN,
	ACTIONTYPE_BHO_NAVERTOOLBAR,
	ACTIONTYPE_BHO_FLASH,
	ACTIONTYPE_BHO_UNKNWN,
	ACTIONTYPE_ETC,
	ACTIONTYPE_NOT_INIT,
};

#endif