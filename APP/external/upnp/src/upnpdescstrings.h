/* $Id: upnpdescstrings.h,v 1.5 2007/02/09 10:12:52 nanard Exp $ */
/* miniupnp project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006 Thomas Bernard
 * This software is subject to the coditions detailed in
 * the LICENCE file provided within the distribution */

   /**********************问题单修改记录******************************************
    日期              修改人         问题单号           修改内容

 2012.08.29        z00203875     2082304944    UPnP认证测试
******************************************************************************/

#ifndef __UPNPDESCSTRINGS_H__
#define __UPNPDESCSTRINGS_H__

#include "config.h"

/* strings used in the root device xml description */

#define ROOTDEV_FRIENDLYNAME		OS_NAME " router"

#ifdef FEATURE_HUAWEI_MBB_PNPX

    #define ROOTDEV_MANUFACTURER		"Vodafone(Huawei)"
    #define ROOTDEV_MANUFACTURERURL		"http://vodafonemobile.wifi"
    #define ROOTDEV_MODELNAME			"R208"
    #define ROOTDEV_MODELDESCRIPTION	"Vodafone Mobile Wi-Fi"
    #define ROOTDEV_MODELNUMBER			"1.0"
    #define ROOTDEV_MODELURL			"http://vodafonemobile.wifi"

#else

    #define ROOTDEV_MANUFACTURER		OS_NAME
    #define ROOTDEV_MANUFACTURERURL		OS_URL
    #define ROOTDEV_MODELNAME			OS_NAME " router"
    #define ROOTDEV_MODELDESCRIPTION	OS_NAME " router"
    #define ROOTDEV_MODELNUMBER			UPNP_VERSION
    #define ROOTDEV_MODELURL			OS_URL

#endif


#define WANDEV_FRIENDLYNAME			"WANDevice"

#ifdef FEATURE_HUAWEI_MBB_PNPX
#define WANDEV_MANUFACTURER			"Vodafone(Huawei)"
#define WANDEV_MANUFACTURERURL		"http://vodafonemobile.wifi"
#else
#define WANDEV_MANUFACTURER			"Huawei"
#define WANDEV_MANUFACTURERURL		"http://www.huawei.com/"
#endif

#define WANDEV_MODELNAME			"WAN Device"
#define WANDEV_MODELDESCRIPTION		"WAN Device"
#define WANDEV_MODELNUMBER			UPNP_VERSION
#define WANDEV_MODELURL				"http://www.huawei.com/"
/* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 modified */
#define WANDEV_UPC					"000000000000"
/* END   2082304944 zhoujianchun 00203875 2012.8.29 modified */

#define WANCDEV_FRIENDLYNAME		"WANConnectionDevice"
#define WANCDEV_MANUFACTURER		WANDEV_MANUFACTURER
#define WANCDEV_MANUFACTURERURL		WANDEV_MANUFACTURERURL
#define WANCDEV_MODELNAME			"WAN Connection Device"
#define WANCDEV_MODELDESCRIPTION	"WAN Connection Device"
#define WANCDEV_MODELNUMBER			UPNP_VERSION
#define WANCDEV_MODELURL			"http://www.huawei.com/"
/* BEGIN 2082304944 zhoujianchun 00203875 2012.8.29 modified */
#define WANCDEV_UPC					"000000000000"
/* END   2082304944 zhoujianchun 00203875 2012.8.29 modified */

#endif

