LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	wrapper/wrapper.cpp\
        amr_nb/common/src/add.cpp \
 	amr_nb/common/src/az_lsp.cpp \
 	amr_nb/common/src/bitno_tab.cpp \
 	amr_nb/common/src/bitreorder_tab.cpp \
 	amr_nb/common/src/c2_9pf_tab.cpp \
 	amr_nb/common/src/div_s.cpp \
 	amr_nb/common/src/gains_tbl.cpp \
 	amr_nb/common/src/gc_pred.cpp \
 	amr_nb/common/src/get_const_tbls.cpp \
 	amr_nb/common/src/gmed_n.cpp \
 	amr_nb/common/src/grid_tbl.cpp \
 	amr_nb/common/src/gray_tbl.cpp \
 	amr_nb/common/src/int_lpc.cpp \
 	amr_nb/common/src/inv_sqrt.cpp \
 	amr_nb/common/src/inv_sqrt_tbl.cpp \
 	amr_nb/common/src/l_shr_r.cpp \
 	amr_nb/common/src/log2.cpp \
 	amr_nb/common/src/log2_norm.cpp \
 	amr_nb/common/src/log2_tbl.cpp \
 	amr_nb/common/src/lsfwt.cpp \
 	amr_nb/common/src/lsp.cpp \
 	amr_nb/common/src/lsp_az.cpp \
 	amr_nb/common/src/lsp_lsf.cpp \
 	amr_nb/common/src/lsp_lsf_tbl.cpp \
 	amr_nb/common/src/lsp_tab.cpp \
 	amr_nb/common/src/mult_r.cpp \
 	amr_nb/common/src/norm_l.cpp \
 	amr_nb/common/src/norm_s.cpp \
 	amr_nb/common/src/overflow_tbl.cpp \
 	amr_nb/common/src/ph_disp_tab.cpp \
 	amr_nb/common/src/pow2.cpp \
 	amr_nb/common/src/pow2_tbl.cpp \
 	amr_nb/common/src/pred_lt.cpp \
 	amr_nb/common/src/q_plsf.cpp \
 	amr_nb/common/src/q_plsf_3.cpp \
 	amr_nb/common/src/q_plsf_3_tbl.cpp \
 	amr_nb/common/src/q_plsf_5.cpp \
 	amr_nb/common/src/q_plsf_5_tbl.cpp \
 	amr_nb/common/src/qua_gain_tbl.cpp \
 	amr_nb/common/src/reorder.cpp \
 	amr_nb/common/src/residu.cpp \
 	amr_nb/common/src/round.cpp \
 	amr_nb/common/src/shr.cpp \
 	amr_nb/common/src/shr_r.cpp \
 	amr_nb/common/src/sqrt_l.cpp \
 	amr_nb/common/src/sqrt_l_tbl.cpp \
 	amr_nb/common/src/sub.cpp \
 	amr_nb/common/src/syn_filt.cpp \
 	amr_nb/common/src/weight_a.cpp \
 	amr_nb/common/src/window_tab.cpp\
        amr_nb/dec/src/decoder_gsm_amr.cpp \
 	amr_nb/dec/src/a_refl.cpp \
 	amr_nb/dec/src/agc.cpp \
 	amr_nb/dec/src/amrdecode.cpp \
 	amr_nb/dec/src/b_cn_cod.cpp \
 	amr_nb/dec/src/bgnscd.cpp \
 	amr_nb/dec/src/c_g_aver.cpp \
 	amr_nb/dec/src/d1035pf.cpp \
 	amr_nb/dec/src/d2_11pf.cpp \
 	amr_nb/dec/src/d2_9pf.cpp \
 	amr_nb/dec/src/d3_14pf.cpp \
 	amr_nb/dec/src/d4_17pf.cpp \
 	amr_nb/dec/src/d8_31pf.cpp \
 	amr_nb/dec/src/d_gain_c.cpp \
 	amr_nb/dec/src/d_gain_p.cpp \
 	amr_nb/dec/src/d_plsf.cpp \
 	amr_nb/dec/src/d_plsf_3.cpp \
 	amr_nb/dec/src/d_plsf_5.cpp \
 	amr_nb/dec/src/dec_amr.cpp \
 	amr_nb/dec/src/dec_gain.cpp \
 	amr_nb/dec/src/dec_input_format_tab.cpp \
 	amr_nb/dec/src/dec_lag3.cpp \
 	amr_nb/dec/src/dec_lag6.cpp \
 	amr_nb/dec/src/dtx_dec.cpp \
 	amr_nb/dec/src/ec_gains.cpp \
 	amr_nb/dec/src/ex_ctrl.cpp \
 	amr_nb/dec/src/if2_to_ets.cpp \
 	amr_nb/dec/src/int_lsf.cpp \
 	amr_nb/dec/src/lsp_avg.cpp \
 	amr_nb/dec/src/ph_disp.cpp \
 	amr_nb/dec/src/post_pro.cpp \
 	amr_nb/dec/src/preemph.cpp \
 	amr_nb/dec/src/pstfilt.cpp \
 	amr_nb/dec/src/qgain475_tab.cpp \
 	amr_nb/dec/src/sp_dec.cpp \
 	amr_nb/dec/src/wmf_to_ets.cpp\
        amr_nb/enc/src/amrencode.cpp \
 	amr_nb/enc/src/autocorr.cpp \
 	amr_nb/enc/src/c1035pf.cpp \
 	amr_nb/enc/src/c2_11pf.cpp \
 	amr_nb/enc/src/c2_9pf.cpp \
 	amr_nb/enc/src/c3_14pf.cpp \
 	amr_nb/enc/src/c4_17pf.cpp \
 	amr_nb/enc/src/c8_31pf.cpp \
 	amr_nb/enc/src/calc_cor.cpp \
 	amr_nb/enc/src/calc_en.cpp \
 	amr_nb/enc/src/cbsearch.cpp \
 	amr_nb/enc/src/cl_ltp.cpp \
 	amr_nb/enc/src/cod_amr.cpp \
 	amr_nb/enc/src/convolve.cpp \
 	amr_nb/enc/src/cor_h.cpp \
 	amr_nb/enc/src/cor_h_x.cpp \
 	amr_nb/enc/src/cor_h_x2.cpp \
 	amr_nb/enc/src/corrwght_tab.cpp \
 	amr_nb/enc/src/div_32.cpp \
 	amr_nb/enc/src/dtx_enc.cpp \
 	amr_nb/enc/src/enc_lag3.cpp \
 	amr_nb/enc/src/enc_lag6.cpp \
 	amr_nb/enc/src/enc_output_format_tab.cpp \
 	amr_nb/enc/src/ets_to_if2.cpp \
 	amr_nb/enc/src/ets_to_wmf.cpp \
 	amr_nb/enc/src/g_adapt.cpp \
 	amr_nb/enc/src/g_code.cpp \
 	amr_nb/enc/src/g_pitch.cpp \
 	amr_nb/enc/src/gain_q.cpp \
 	amr_nb/enc/src/gsmamr_encoder_wrapper.cpp \
 	amr_nb/enc/src/hp_max.cpp \
 	amr_nb/enc/src/inter_36.cpp \
 	amr_nb/enc/src/inter_36_tab.cpp \
 	amr_nb/enc/src/l_abs.cpp \
 	amr_nb/enc/src/l_comp.cpp \
 	amr_nb/enc/src/l_extract.cpp \
 	amr_nb/enc/src/l_negate.cpp \
 	amr_nb/enc/src/lag_wind.cpp \
 	amr_nb/enc/src/lag_wind_tab.cpp \
 	amr_nb/enc/src/levinson.cpp \
 	amr_nb/enc/src/lpc.cpp \
 	amr_nb/enc/src/ol_ltp.cpp \
 	amr_nb/enc/src/p_ol_wgh.cpp \
 	amr_nb/enc/src/pitch_fr.cpp \
 	amr_nb/enc/src/pitch_ol.cpp \
 	amr_nb/enc/src/pre_big.cpp \
 	amr_nb/enc/src/pre_proc.cpp \
 	amr_nb/enc/src/prm2bits.cpp \
 	amr_nb/enc/src/q_gain_c.cpp \
 	amr_nb/enc/src/q_gain_p.cpp \
 	amr_nb/enc/src/qgain475.cpp \
 	amr_nb/enc/src/qgain795.cpp \
 	amr_nb/enc/src/qua_gain.cpp \
 	amr_nb/enc/src/s10_8pf.cpp \
 	amr_nb/enc/src/set_sign.cpp \
 	amr_nb/enc/src/sid_sync.cpp \
 	amr_nb/enc/src/sp_enc.cpp \
 	amr_nb/enc/src/spreproc.cpp \
 	amr_nb/enc/src/spstproc.cpp \
 	amr_nb/enc/src/ton_stab.cpp \
 	amr_nb/enc/src/vad1.cpp


LOCAL_MODULE := libopencore-amrnb

LOCAL_ARM_MODE := arm
#LOCAL_LDLIBS := libpv_amr_nb_common_lib libpvdecoder_gsmamr libpvencoder_gsmamr
#LOCAL_STATIC_LIBRARIES := libpv_amr_nb_common_lib libpvdecoder_gsmamr libpvencoder_gsmamr

LOCAL_SHARED_LIBRARIES := 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/amr_nb/common/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/amr_nb/dec/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/amr_nb/common/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/common/dec/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/amr_nb/enc/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/amr_nb/dec/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/amr_nb/enc/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../oscl/osclbase/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../oscl/config/android
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../oscl/config/shared
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../oscl/osclmemory/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../oscl/osclerror/src
LOCAL_MODULE_TAGS := eng
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)
