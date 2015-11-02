/*
 * Common driver-related functions
 * Copyright (c) 2003-2011, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "includes.h"
#include "utils/common.h"
#include "driver.h"
#include "driver_wext.h"

/*BEGIN 3052902934 shenxiong 2013-05-28 added*/
/* 下表是所有5G的信道和频率之间的对应关系                       *
*  应用下发5G信道的bitmap时，bit0对应183，bit1对应184，以此类推 */
CHAN_FREQ_STRU g_chan_freq_24G_table[] = 
{
    {CHAN_24G_1, FREQ_24G_1},
    {CHAN_24G_2, FREQ_24G_2},
    {CHAN_24G_3, FREQ_24G_3},
    {CHAN_24G_4, FREQ_24G_4},
    {CHAN_24G_5, FREQ_24G_5},
    {CHAN_24G_6, FREQ_24G_6},
    {CHAN_24G_7, FREQ_24G_7},
    {CHAN_24G_8, FREQ_24G_8},
    {CHAN_24G_9, FREQ_24G_9},
    {CHAN_24G_10, FREQ_24G_10},
    {CHAN_24G_11, FREQ_24G_11},
    {CHAN_24G_12, FREQ_24G_12},
    {CHAN_24G_13, FREQ_24G_13},
    {CHAN_24G_14, FREQ_24G_14},
};

CHAN_FREQ_STRU g_chan_freq_5G_table[] = 
{
    {CHAN_5G_7, FREQ_5G_7 },
    {CHAN_5G_8, FREQ_5G_8 },
    {CHAN_5G_9, FREQ_5G_9 },
    {CHAN_5G_11, FREQ_5G_11 },
    {CHAN_5G_12, FREQ_5G_12 },
    {CHAN_5G_16, FREQ_5G_16 },
    {CHAN_5G_34, FREQ_5G_34 },
    {CHAN_5G_36, FREQ_5G_36 },
    {CHAN_5G_38, FREQ_5G_38 },
    {CHAN_5G_40, FREQ_5G_40 },
    {CHAN_5G_42, FREQ_5G_42 },
    {CHAN_5G_44, FREQ_5G_44 },
    {CHAN_5G_46, FREQ_5G_46 },
    {CHAN_5G_48, FREQ_5G_48 },
    {CHAN_5G_52, FREQ_5G_52 },
    {CHAN_5G_56, FREQ_5G_56 },
    {CHAN_5G_60, FREQ_5G_60 },
    {CHAN_5G_64, FREQ_5G_64 },
    {CHAN_5G_100, FREQ_5G_100},
    {CHAN_5G_104, FREQ_5G_104},
    {CHAN_5G_108, FREQ_5G_108},
    {CHAN_5G_112, FREQ_5G_112},
    {CHAN_5G_116, FREQ_5G_116},
    {CHAN_5G_120, FREQ_5G_120},
    {CHAN_5G_124, FREQ_5G_124},
    {CHAN_5G_128, FREQ_5G_128},
    {CHAN_5G_132, FREQ_5G_132},
    {CHAN_5G_136, FREQ_5G_136},
    {CHAN_5G_140, FREQ_5G_140},
    {CHAN_5G_149, FREQ_5G_149},
    {CHAN_5G_153, FREQ_5G_153},
    {CHAN_5G_157, FREQ_5G_157},
    {CHAN_5G_161, FREQ_5G_161},
    {CHAN_5G_165, FREQ_5G_165},
    {CHAN_5G_183, FREQ_5G_183},
    {CHAN_5G_184, FREQ_5G_184},
    {CHAN_5G_185, FREQ_5G_185},  
    {CHAN_5G_187, FREQ_5G_187},  
    {CHAN_5G_188, FREQ_5G_188},  
    {CHAN_5G_189, FREQ_5G_189},  
    {CHAN_5G_192, FREQ_5G_192},  
    {CHAN_5G_196, FREQ_5G_196},  
};
/*END 3052902934 shenxiong 2013-05-28 added*/
/* BEGIN PN: 3061809061 ,Added by s00208871, 2013/7/4 */
/* 将下发信道bit位转换为对应的信道值 */
void dump_scan_channel(struct scan_channel *channels)
{
    CHAN_FREQ_STRU *table = NULL;
    unsigned long long ch_set, tmp;  
    int i, j, result, total, count, len;
    char buf[1024]={0}, *p = NULL; //1024为输出字符的缓冲区大小

    p = buf;
    for (i = 0; i < BAND_TOTAL; i++){
        ch_set = channels->ch_set[i];
        count = 0;
        if(0 != ch_set) {
            if (i == BAND_24G) {
                table = g_chan_freq_24G_table; 
                total = sizeof(g_chan_freq_24G_table)/sizeof(CHAN_FREQ_STRU);
            } else {
                table = g_chan_freq_5G_table; 
                total = sizeof(g_chan_freq_5G_table)/sizeof(CHAN_FREQ_STRU);
            }
            for (j = 0; j < total; j++) {
                result  = 0;
                if (j < MACHINE_BIT_LENGTH) {
                    result = (ch_set >> j) & 1;
                } else {
                    tmp = *(((unsigned int*)&ch_set) + 1);
                    result = (tmp >> (j - MACHINE_BIT_LENGTH)) & 1;
                 }
                 if (1 == result) {
                     len = snprintf(p, buf + sizeof(buf) - p, " %d", table[j].channel);
                     p += len;
                     count++;
                 }
            }
            wpa_printf(MSG_INFO, "band:%d(0:2.4G, 1:5G) support (%d) channel:%s", i, count, buf);
            os_memset(buf, 0, sizeof(buf));
            p = buf;
        }
    }

}

/* 获得频段对应的信道编号*/
static unsigned long long get_num_from_freq(u16 freq)
{
    CHAN_FREQ_STRU *table = NULL;
    unsigned long long num = 0;
    int total, i, flag = 0;
    
    if (FREQ_24G_14 >= freq) {
        table = g_chan_freq_24G_table;
        total = sizeof(g_chan_freq_24G_table)/sizeof(CHAN_FREQ_STRU);
    } else {
        table = g_chan_freq_5G_table;
        total = sizeof(g_chan_freq_5G_table)/sizeof(CHAN_FREQ_STRU);
    }
    for (i = 0; i < total; i++) {
        if (table[i].freq == freq) {
            if (i >= MACHINE_BIT_LENGTH) {
                *(((unsigned int*)&num) + 1) = 1 << (i - MACHINE_BIT_LENGTH);
            } else {
                num = 1 << i;
            }
            flag = 1;
            break;
        }
    }
    if (flag) {
        wpa_printf(MSG_DEBUG, "freq=%u(%d), number=0x%llx", table[i].freq, table[i].channel, num);
    } else {
        wpa_printf(MSG_DEBUG, "fail to find freq %u in table", freq);
    }
    
    return num;  
}
/* 判断信道是否需要过滤 */
Boolean wpa_freq_is_filter(struct scan_channel *channels, int freq)
{
    unsigned long long  num, ch_set, result;

    if (NULL == channels) {
       wpa_printf(MSG_DEBUG, "filter rule is empty");      
       return FALSE;
    }
    
    num = get_num_from_freq((u16)freq);
    if(0 == num) {
        return TRUE;
    }
    if (freq <= FREQ_24G_14) {
        ch_set = channels->ch_set[BAND_24G];
    } else {
        ch_set = channels->ch_set[BAND_5G];
    }

    result = ch_set & num;
    wpa_printf(MSG_DEBUG, "ch_set(0x%llx)& num(0x%llx)=0x%llx", ch_set, num, result);
    if (result) {
        return FALSE;
    }
    return TRUE;
}

int  wext_set_chan_list(char *buf, struct scan_channel *channels)
{
  int bp = 0;
  /* Set list of channels */
  if (channels->status) {
      /* set specified channel set */
      int i,j, count, total;
      unsigned long long ch_set;
      CHAN_FREQ_STRU *table = NULL;
      count = 0;
      unsigned int result = 0;
      unsigned int tmp;
      for (i = 0; i < BAND_TOTAL; i++) {
          ch_set = channels->ch_set[i];
		      count = 0;
          /* 如果指定的信道不为空，需要处理指定信道 */
          if(0 != ch_set) {
              if (i == BAND_24G) {
                  table = g_chan_freq_24G_table; 
                  total = sizeof(g_chan_freq_24G_table)/sizeof(CHAN_FREQ_STRU);
              } else {
                  table = g_chan_freq_5G_table; 
                  total = sizeof(g_chan_freq_5G_table)/sizeof(CHAN_FREQ_STRU);
              }
              for (j = 0; j < total; j++) {
                  result  = 0;
                  if (j < MACHINE_BIT_LENGTH) {
                      result = (ch_set >> j) & 1;
                      wpa_printf(MSG_INFO, "result:%d\n", result);
                  } else {
                      tmp = *(((unsigned int*)&ch_set) + 1);
                      result = (tmp >> (j - MACHINE_BIT_LENGTH)) & 1;
                      wpa_printf(MSG_INFO, "over 32, tmp:%u, result:%d\n", tmp, result);
                  }
                  if (1 == result) {
                      buf[bp++] = WEXT_CSCAN_CHANNEL_SECTION;
                      buf[bp++] = (unsigned char)table[j].channel;
                      wpa_printf(MSG_INFO, "band:%d(0:2.4G, 1:5G) scan channel %d\n", i, buf[bp - 1]);
                      count++;
                  }
              }
              wpa_printf(MSG_INFO, "band %d(0:2.4G, 1:5G) total scan %d channels\n", i, count);
          }
      }
  } else {
      wpa_printf(MSG_INFO, "no scan channels list\n");
      /* has not set channel list */
    	buf[bp++] = WEXT_CSCAN_CHANNEL_SECTION; 
	    buf[bp++] = 0; 
  }
  return bp;
}
/* END PN: 3061809061 ,Added by s00208871, 2013/7/4*/


void wpa_scan_results_free(struct wpa_scan_results *res)
{
	size_t i;

	if (res == NULL)
		return;

	for (i = 0; i < res->num; i++)
		os_free(res->res[i]);
	os_free(res->res);
	os_free(res);
}


const char * event_to_string(enum wpa_event_type event)
{
#define E2S(n) case EVENT_ ## n: return #n
	switch (event) {
	E2S(ASSOC);
	E2S(DISASSOC);
	E2S(MICHAEL_MIC_FAILURE);
	E2S(SCAN_RESULTS);
	E2S(ASSOCINFO);
	E2S(INTERFACE_STATUS);
	E2S(PMKID_CANDIDATE);
	E2S(STKSTART);
	E2S(TDLS);
	E2S(FT_RESPONSE);
	E2S(IBSS_RSN_START);
	E2S(AUTH);
	E2S(DEAUTH);
	E2S(ASSOC_REJECT);
	E2S(AUTH_TIMED_OUT);
	E2S(ASSOC_TIMED_OUT);
	E2S(FT_RRB_RX);
	E2S(WPS_BUTTON_PUSHED);
	E2S(TX_STATUS);
	E2S(RX_FROM_UNKNOWN);
	E2S(RX_MGMT);
	E2S(RX_ACTION);
	E2S(REMAIN_ON_CHANNEL);
	E2S(CANCEL_REMAIN_ON_CHANNEL);
	E2S(MLME_RX);
	E2S(RX_PROBE_REQ);
	E2S(NEW_STA);
	E2S(EAPOL_RX);
	E2S(SIGNAL_CHANGE);
	E2S(INTERFACE_ENABLED);
	E2S(INTERFACE_DISABLED);
	E2S(CHANNEL_LIST_CHANGED);
	E2S(INTERFACE_UNAVAILABLE);
	E2S(BEST_CHANNEL);
	E2S(UNPROT_DEAUTH);
	E2S(UNPROT_DISASSOC);
	E2S(STATION_LOW_ACK);
	E2S(P2P_DEV_FOUND);
	E2S(P2P_GO_NEG_REQ_RX);
	E2S(P2P_GO_NEG_COMPLETED);
	E2S(P2P_PROV_DISC_REQUEST);
	E2S(P2P_PROV_DISC_RESPONSE);
	E2S(P2P_SD_REQUEST);
	E2S(P2P_SD_RESPONSE);
	E2S(IBSS_PEER_LOST);
	E2S(DRIVER_GTK_REKEY);
	E2S(SCHED_SCAN_STOPPED);
	E2S(DRIVER_CLIENT_POLL_OK);
	E2S(EAPOL_TX_STATUS);
	E2S(CH_SWITCH);
	E2S(WNM);
	E2S(CONNECT_FAILED_REASON);
	E2S(DFS_RADAR_DETECTED);
	E2S(DFS_CAC_FINISHED);
	E2S(DFS_CAC_ABORTED);
	E2S(DFS_NOP_FINISHED);
	E2S(SURVEY);
	E2S(SCAN_STARTED);
	E2S(AVOID_FREQUENCIES);
	}

	return "UNKNOWN";
#undef E2S
}
