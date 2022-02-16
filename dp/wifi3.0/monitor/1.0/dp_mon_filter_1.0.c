/*
 * Copyright (c) 2021, The Linux Foundation. All rights reserved.
 * Copyright (c) 2021-2022 Qualcomm Innovation Center, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <hal_api.h>
#include <wlan_cfg.h>
#include "dp_types.h"
#include "dp_internal.h"
#include "dp_htt.h"
#include "dp_mon.h"
#include "dp_mon_filter.h"

#include <dp_mon_1.0.h>
#include <dp_rx_mon_1.0.h>
#include <dp_mon_filter_1.0.h>

#if defined(QCA_MCOPY_SUPPORT) || defined(ATH_SUPPORT_NAC_RSSI) \
	|| defined(ATH_SUPPORT_NAC) || defined(WLAN_RX_PKT_CAPTURE_ENH)
/**
 * dp_mon_filter_check_co_exist() - Check the co-existing of the
 * enabled modes.
 * @pdev: DP pdev handle
 *
 * Return: QDF_STATUS
 */
static QDF_STATUS dp_mon_filter_check_co_exist(struct dp_pdev *pdev)
{
	struct dp_mon_pdev *mon_pdev = pdev->monitor_pdev;
	/*
	 * Check if the Rx Enhanced capture mode, monitor mode,
	 * smart_monitor_mode and mcopy mode can co-exist together.
	 */
	if ((mon_pdev->rx_enh_capture_mode != CDP_RX_ENH_CAPTURE_DISABLED) &&
	    ((mon_pdev->neighbour_peers_added && mon_pdev->mvdev) ||
		 mon_pdev->mcopy_mode)) {
		dp_mon_filter_err("%pK:Rx Capture mode can't exist with modes:\n"
				  "Smart Monitor Mode:%d\n"
				  "M_Copy Mode:%d", pdev->soc,
				  mon_pdev->neighbour_peers_added,
				  mon_pdev->mcopy_mode);
		return QDF_STATUS_E_FAILURE;
	}

	/*
	 * Check if the monitor mode cannot co-exist with any other mode.
	 */
	if ((mon_pdev->mvdev && mon_pdev->monitor_configured) &&
	    (mon_pdev->mcopy_mode || mon_pdev->neighbour_peers_added)) {
		dp_mon_filter_err("%pK: Monitor mode can't exist with modes\n"
				  "M_Copy Mode:%d\n"
				  "Smart Monitor Mode:%d",
				  pdev->soc, mon_pdev->mcopy_mode,
				  mon_pdev->neighbour_peers_added);
		return QDF_STATUS_E_FAILURE;
	}

	/*
	 * Check if the smart monitor mode can co-exist with any other mode
	 */
	if (mon_pdev->neighbour_peers_added &&
	    ((mon_pdev->mcopy_mode) || mon_pdev->monitor_configured)) {
		dp_mon_filter_err("%pk: Smart Monitor mode can't exist with modes\n"
				  "M_Copy Mode:%d\n"
				  "Monitor Mode:%d",
				  pdev->soc, mon_pdev->mcopy_mode,
			      mon_pdev->monitor_configured);
		return QDF_STATUS_E_FAILURE;
	}

	/*
	 * Check if the m_copy, monitor mode and the smart_monitor_mode
	 * can co-exist togther.
	 */
	if (mon_pdev->mcopy_mode &&
	    (mon_pdev->mvdev || mon_pdev->neighbour_peers_added)) {
		dp_mon_filter_err("%pK: mcopy mode can't exist with modes\n"
				  "Monitor Mode:%pK\n"
				  "Smart Monitor Mode:%d",
				  pdev->soc, mon_pdev->mvdev,
				  mon_pdev->neighbour_peers_added);
		return QDF_STATUS_E_FAILURE;
	}

	/*
	 * Check if the Rx packet log lite or full can co-exist with
	 * the enable modes.
	 */
	if ((mon_pdev->rx_pktlog_mode != DP_RX_PKTLOG_DISABLED) &&
	    !mon_pdev->rx_pktlog_cbf &&
	    (mon_pdev->mvdev || mon_pdev->monitor_configured)) {
		dp_mon_filter_err("%pK: Rx pktlog full/lite can't exist with modes\n"
				  "Monitor Mode:%d", pdev->soc,
				  mon_pdev->monitor_configured);
		return QDF_STATUS_E_FAILURE;
	}
	return QDF_STATUS_SUCCESS;
}
#else
static QDF_STATUS dp_mon_filter_check_co_exist(struct dp_pdev *pdev)
{
	struct dp_mon_pdev *mon_pdev = pdev->monitor_pdev;
	/*
	 * Check if the Rx packet log lite or full can co-exist with
	 * the enable modes.
	 */
	if ((mon_pdev->rx_pktlog_mode != DP_RX_PKTLOG_DISABLED) &&
	    (mon_pdev->mvdev || mon_pdev->monitor_configured)) {
		 dp_mon_filter_err("%pK: Rx pktlog full/lite can't exist with modes\n"
				   "Monitor Mode:%d", pdev->soc,
				   mon_pdev->monitor_configured);
		return QDF_STATUS_E_FAILURE;
	}

	return QDF_STATUS_SUCCESS;
}
#endif

#ifdef QCA_ENHANCED_STATS_SUPPORT
void dp_mon_filter_setup_enhanced_stats_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	enum dp_mon_filter_mode mode = DP_MON_FILTER_ENHACHED_STATS_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	/* Enabled the filter */
	filter.valid = true;

	mon_pdev = pdev->monitor_pdev;
	dp_mon_filter_set_status_cmn(mon_pdev, &filter);

	filter.tlv_filter.enable_mo = 0;
	filter.tlv_filter.mo_mgmt_filter = 0;
	filter.tlv_filter.mo_ctrl_filter = 0;
	filter.tlv_filter.mo_data_filter = 0;

	dp_mon_filter_show_filter(mon_pdev, mode, &filter);
	mon_pdev->filter[mode][srng_type] = filter;
}

/**
 * dp_mon_filter_reset_enhanced_stats() - Reset the enhanced stats filter
 * @pdev: DP pdev handle
 */
void dp_mon_filter_reset_enhanced_stats_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	enum dp_mon_filter_mode mode = DP_MON_FILTER_ENHACHED_STATS_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;
	mon_pdev->filter[mode][srng_type] = filter;
}
#endif /* QCA_ENHANCED_STATS_SUPPORT */

#ifdef QCA_UNDECODED_METADATA_SUPPORT
/**
 * mon_filter_setup_undecoded_metadata_capture() - Setup undecoded frame
 * capture phyrx aborted frame filter setup
 * @pdev: DP pdev handle
 */
void dp_mon_filter_setup_undecoded_metadata_capture_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	enum dp_mon_filter_mode mode =
				DP_MON_FILTER_UNDECODED_METADATA_CAPTURE_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	/* Enabled the filter */
	filter.valid = true;

	mon_pdev = pdev->monitor_pdev;
	dp_mon_filter_set_status_cmn(mon_pdev, &filter);

	/* Setup the filter to subscribe to FP PHY status tlv */
	filter.tlv_filter.fp_phy_err = 1;
	filter.tlv_filter.fp_phy_err_buf_src = SW2RXDMA_BUF_SOURCE_RING;
	filter.tlv_filter.fp_phy_err_buf_dest = RXDMA2SW_RING;

	filter.tlv_filter.phy_err_filter_valid = 1;

	dp_mon_filter_show_filter(mon_pdev, mode, &filter);
	mon_pdev->filter[mode][srng_type] = filter;
}

/**
 * mon_filter_reset_undecoded_metadata_capture() - Reset undecoded frame
 * capture phyrx aborted frame filter
 * @pdev: DP pdev handle
 */
void dp_mon_filter_reset_undecoded_metadata_capture_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	enum dp_mon_filter_mode mode =
				DP_MON_FILTER_UNDECODED_METADATA_CAPTURE_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}
	mon_pdev = pdev->monitor_pdev;

	/* Enabled filter to reset to default values */
	filter.valid = true;
	/* Reset the filter and phy error mask */
	filter.tlv_filter.fp_phy_err = 0;
	filter.tlv_filter.fp_phy_err_buf_src = NO_BUFFER_RING;
	filter.tlv_filter.fp_phy_err_buf_dest = RXDMA_RELEASING_RING;

	filter.tlv_filter.phy_err_mask = 0;
	filter.tlv_filter.phy_err_mask_cont = 0;

	filter.tlv_filter.phy_err_filter_valid = 1;

	dp_mon_filter_show_filter(mon_pdev, mode, &filter);
	mon_pdev->filter[mode][srng_type] = filter;
}
#endif /* QCA_UNDECODED_METADATA_SUPPORT */

#ifdef QCA_MCOPY_SUPPORT
#ifdef QCA_MONITOR_PKT_SUPPORT
static void dp_mon_filter_set_reset_mcopy_dest(struct dp_pdev *pdev,
					       struct dp_mon_filter *pfilter)
{
	struct dp_soc *soc = pdev->soc;
	struct dp_mon_pdev *mon_pdev = pdev->monitor_pdev;
	enum dp_mon_filter_mode mode = DP_MON_FILTER_MCOPY_MODE;
	enum dp_mon_filter_srng_type srng_type;

	srng_type = ((soc->wlan_cfg_ctx->rxdma1_enable) ?
			DP_MON_FILTER_SRNG_TYPE_RXDMA_MON_BUF :
			DP_MON_FILTER_SRNG_TYPE_RXDMA_BUF);

	/* Set the filter */
	if (pfilter->valid) {
		dp_mon_filter_set_mon_cmn(mon_pdev, pfilter);

		pfilter->tlv_filter.fp_data_filter = 0;
		pfilter->tlv_filter.mo_data_filter = 0;

		dp_mon_filter_show_filter(mon_pdev, mode, pfilter);
		mon_pdev->filter[mode][srng_type] = *pfilter;
	} else /* Reset the filter */
		mon_pdev->filter[mode][srng_type] = *pfilter;
}
#else
static void dp_mon_filter_set_reset_mcopy_dest(struct dp_pdev *pdev,
					       struct dp_mon_filter *pfilter)
{
}
#endif

void dp_mon_filter_setup_mcopy_mode_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	struct dp_soc *soc = NULL;
	enum dp_mon_filter_mode mode = DP_MON_FILTER_MCOPY_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	soc = pdev->soc;
	if (!soc) {
		dp_mon_filter_err("Soc Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;
	if (!mon_pdev) {
		dp_mon_filter_err("monitor pdev Context is null");
		return;
	}
	/* Enabled the filter */
	filter.valid = true;
	dp_mon_filter_set_reset_mcopy_dest(pdev, &filter);

	/* Clear the filter as the same filter will be used to set the
	 * monitor status ring
	 */
	qdf_mem_zero(&(filter), sizeof(struct dp_mon_filter));

	/* Enabled the filter */
	filter.valid = true;
	dp_mon_filter_set_status_cmn(mon_pdev, &filter);

	/* Setup the filter */
	filter.tlv_filter.enable_mo = 1;
	filter.tlv_filter.packet_header = 1;
	filter.tlv_filter.mpdu_end = 1;
	dp_mon_filter_show_filter(mon_pdev, mode, &filter);

	srng_type = DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	mon_pdev->filter[mode][srng_type] = filter;
}

void dp_mon_filter_reset_mcopy_mode_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	struct dp_soc *soc = NULL;
	enum dp_mon_filter_mode mode = DP_MON_FILTER_MCOPY_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	soc = pdev->soc;
	if (!soc) {
		dp_mon_filter_err("Soc Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;
	dp_mon_filter_set_reset_mcopy_dest(pdev, &filter);

	srng_type = DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	mon_pdev->filter[mode][srng_type] = filter;
}
#endif

#if defined(ATH_SUPPORT_NAC_RSSI) || defined(ATH_SUPPORT_NAC)
/**
 * dp_mon_filter_setup_smart_monitor() - Setup the smart monitor mode filter
 * @pdev: DP pdev handle
 */
void dp_mon_filter_setup_smart_monitor_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	struct dp_soc *soc = NULL;
	struct dp_mon_soc *mon_soc;

	enum dp_mon_filter_mode mode = DP_MON_FILTER_SMART_MONITOR_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	soc = pdev->soc;
	if (!soc) {
		dp_mon_filter_err("Soc Context is null");
		return;
	}

	mon_soc = soc->monitor_soc;
	mon_pdev = pdev->monitor_pdev;

	/* Enabled the filter */
	filter.valid = true;
	dp_mon_filter_set_status_cmn(mon_pdev, &filter);

	filter.tlv_filter.enable_mo = 0;
	filter.tlv_filter.mo_mgmt_filter = 0;
	filter.tlv_filter.mo_ctrl_filter = 0;
	filter.tlv_filter.mo_data_filter = 0;

	if (mon_soc->hw_nac_monitor_support) {
		filter.tlv_filter.enable_md = 1;
		filter.tlv_filter.packet_header = 1;
		filter.tlv_filter.md_data_filter = FILTER_DATA_ALL;
	}

	dp_mon_filter_show_filter(mon_pdev, mode, &filter);
	mon_pdev->filter[mode][srng_type] = filter;
}

void dp_mon_filter_reset_smart_monitor_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	enum dp_mon_filter_mode mode = DP_MON_FILTER_SMART_MONITOR_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;
	mon_pdev->filter[mode][srng_type] = filter;
}
#endif /* ATH_SUPPORT_NAC_RSSI || ATH_SUPPORT_NAC */

#ifdef WLAN_RX_PKT_CAPTURE_ENH
#ifdef QCA_MONITOR_PKT_SUPPORT
static
void dp_mon_filter_set_reset_rx_enh_capture_dest(struct dp_pdev *pdev,
						 struct dp_mon_filter *pfilter)
{
	struct dp_soc *soc = pdev->soc;
	struct dp_mon_pdev *mon_pdev = pdev->monitor_pdev;
	enum dp_mon_filter_mode mode = DP_MON_FILTER_RX_CAPTURE_MODE;
	enum dp_mon_filter_srng_type srng_type;

	srng_type = ((soc->wlan_cfg_ctx->rxdma1_enable) ?
			DP_MON_FILTER_SRNG_TYPE_RXDMA_MON_BUF :
			DP_MON_FILTER_SRNG_TYPE_RXDMA_BUF);

	/* Set the filter */
	if (pfilter->valid) {
		dp_mon_filter_set_mon_cmn(mon_pdev, pfilter);

		pfilter->tlv_filter.fp_mgmt_filter = 0;
		pfilter->tlv_filter.fp_ctrl_filter = 0;
		pfilter->tlv_filter.fp_data_filter = 0;
		pfilter->tlv_filter.mo_mgmt_filter = 0;
		pfilter->tlv_filter.mo_ctrl_filter = 0;
		pfilter->tlv_filter.mo_data_filter = 0;

		dp_mon_filter_show_filter(mon_pdev, mode, pfilter);
		pdev->monitor_pdev->filter[mode][srng_type] = *pfilter;
	} else /* Reset the filter */
		pdev->monitor_pdev->filter[mode][srng_type] = *pfilter;
}
#else
static
void dp_mon_filter_set_reset_rx_enh_capture_dest(struct dp_pdev *pdev,
						 struct dp_mon_filter *pfilter)
{
}
#endif

void dp_mon_filter_setup_rx_enh_capture_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	struct dp_soc *soc = NULL;
	enum dp_mon_filter_mode mode = DP_MON_FILTER_RX_CAPTURE_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	soc = pdev->soc;
	if (!soc) {
		dp_mon_filter_err("Soc Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;

	/* Enabled the filter */
	filter.valid = true;
	dp_mon_filter_set_reset_rx_enh_capture_dest(pdev, &filter);

	/* Clear the filter as the same filter will be used to set the
	 * monitor status ring
	 */
	qdf_mem_zero(&(filter), sizeof(struct dp_mon_filter));

	/* Enabled the filter */
	filter.valid = true;
	dp_mon_filter_set_status_cmn(mon_pdev, &filter);

	/* Setup the filter */
	filter.tlv_filter.mpdu_end = 1;
	filter.tlv_filter.enable_mo = 1;
	filter.tlv_filter.packet_header = 1;

	if (mon_pdev->rx_enh_capture_mode == CDP_RX_ENH_CAPTURE_MPDU) {
		filter.tlv_filter.header_per_msdu = 0;
		filter.tlv_filter.enable_mo = 0;
	} else if (mon_pdev->rx_enh_capture_mode ==
			CDP_RX_ENH_CAPTURE_MPDU_MSDU) {
		bool is_rx_mon_proto_flow_tag_enabled =
		wlan_cfg_is_rx_mon_protocol_flow_tag_enabled(soc->wlan_cfg_ctx);
		filter.tlv_filter.header_per_msdu = 1;
		filter.tlv_filter.enable_mo = 0;
		if (mon_pdev->is_rx_enh_capture_trailer_enabled ||
		    is_rx_mon_proto_flow_tag_enabled)
			filter.tlv_filter.msdu_end = 1;
	}

	dp_mon_filter_show_filter(mon_pdev, mode, &filter);

	srng_type = DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	mon_pdev->filter[mode][srng_type] = filter;
}

void dp_mon_filter_reset_rx_enh_capture_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	struct dp_soc *soc = NULL;
	enum dp_mon_filter_mode mode = DP_MON_FILTER_RX_CAPTURE_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	soc = pdev->soc;
	if (!soc) {
		dp_mon_filter_err("Soc Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;
	dp_mon_filter_set_reset_rx_enh_capture_dest(pdev, &filter);

	srng_type = DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	mon_pdev->filter[mode][srng_type] = filter;
}
#endif /* WLAN_RX_PKT_CAPTURE_ENH */

#ifdef QCA_MONITOR_PKT_SUPPORT
static void dp_mon_filter_set_reset_mon_dest(struct dp_pdev *pdev,
					     struct dp_mon_filter *pfilter)
{
	struct dp_soc *soc = pdev->soc;
	struct dp_mon_pdev *mon_pdev = pdev->monitor_pdev;
	enum dp_mon_filter_mode mode = DP_MON_FILTER_MONITOR_MODE;
	enum dp_mon_filter_srng_type srng_type;

	srng_type = ((soc->wlan_cfg_ctx->rxdma1_enable) ?
			DP_MON_FILTER_SRNG_TYPE_RXDMA_MON_BUF :
			DP_MON_FILTER_SRNG_TYPE_RXDMA_BUF);

	/* set the filter */
	if (pfilter->valid) {
		dp_mon_filter_set_mon_cmn(mon_pdev, pfilter);

		dp_mon_filter_show_filter(mon_pdev, mode, pfilter);
		mon_pdev->filter[mode][srng_type] = *pfilter;
	} else /* reset the filter */
		mon_pdev->filter[mode][srng_type] = *pfilter;
}
#else
static void dp_mon_filter_set_reset_mon_dest(struct dp_pdev *pdev,
					     struct dp_mon_filter *pfilter)
{
}
#endif

void dp_mon_filter_setup_mon_mode_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	struct dp_soc *soc = NULL;
	enum dp_mon_filter_mode mode = DP_MON_FILTER_MONITOR_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	soc = pdev->soc;
	if (!soc) {
		dp_mon_filter_err("Soc Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;
	filter.valid = true;
	dp_mon_filter_set_reset_mon_dest(pdev, &filter);

	/* Clear the filter as the same filter will be used to set the
	 * monitor status ring
	 */
	qdf_mem_zero(&(filter), sizeof(struct dp_mon_filter));

	/* Enabled the filter */
	filter.valid = true;
	dp_mon_filter_set_status_cmn(mon_pdev, &filter);
	dp_mon_filter_show_filter(mon_pdev, mode, &filter);

	/* Store the above filter */
	srng_type = DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	mon_pdev->filter[mode][srng_type] = filter;
}

void dp_mon_filter_reset_mon_mode_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	struct dp_soc *soc = NULL;
	enum dp_mon_filter_mode mode = DP_MON_FILTER_MONITOR_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	soc = pdev->soc;
	if (!soc) {
		dp_mon_filter_err("Soc Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;
	dp_mon_filter_set_reset_mon_dest(pdev, &filter);

	srng_type = DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	mon_pdev->filter[mode][srng_type] = filter;
}

#ifdef WDI_EVENT_ENABLE
void dp_mon_filter_setup_rx_pkt_log_full_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	enum dp_mon_filter_mode mode = DP_MON_FILTER_PKT_LOG_FULL_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;
	/* Enabled the filter */
	filter.valid = true;
	dp_mon_filter_set_status_cmn(mon_pdev, &filter);

	/* Setup the filter */
	filter.tlv_filter.packet_header = 1;
	filter.tlv_filter.msdu_start = 1;
	filter.tlv_filter.msdu_end = 1;
	filter.tlv_filter.mpdu_end = 1;
	filter.tlv_filter.attention = 1;

	dp_mon_filter_show_filter(mon_pdev, mode, &filter);
	mon_pdev->filter[mode][srng_type] = filter;
}

void dp_mon_filter_reset_rx_pkt_log_full_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	enum dp_mon_filter_mode mode = DP_MON_FILTER_PKT_LOG_FULL_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;
	mon_pdev->filter[mode][srng_type] = filter;
}

void dp_mon_filter_setup_rx_pkt_log_lite_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	enum dp_mon_filter_mode mode = DP_MON_FILTER_PKT_LOG_LITE_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;
	/* Enabled the filter */
	filter.valid = true;
	dp_mon_filter_set_status_cmn(mon_pdev, &filter);

	dp_mon_filter_show_filter(mon_pdev, mode, &filter);
	mon_pdev->filter[mode][srng_type] = filter;
}

void dp_mon_filter_reset_rx_pkt_log_lite_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	enum dp_mon_filter_mode mode = DP_MON_FILTER_PKT_LOG_LITE_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;

	mon_pdev->filter[mode][srng_type] = filter;
}

#ifdef QCA_MONITOR_PKT_SUPPORT
static
void dp_mon_filter_set_reset_rx_pkt_log_cbf_dest(struct dp_pdev *pdev,
						 struct dp_mon_filter *pfilter)
{
	struct dp_soc *soc = pdev->soc;
	struct dp_mon_pdev *mon_pdev = pdev->monitor_pdev;
	enum dp_mon_filter_mode mode = DP_MON_FILTER_PKT_LOG_CBF_MODE;
	enum dp_mon_filter_srng_type srng_type;

	srng_type = ((soc->wlan_cfg_ctx->rxdma1_enable) ?
			DP_MON_FILTER_SRNG_TYPE_RXDMA_MON_BUF :
			DP_MON_FILTER_SRNG_TYPE_RXDMA_BUF);

	/*set the filter */
	if (pfilter->valid) {
		dp_mon_filter_set_cbf_cmn(pdev, pfilter);

		dp_mon_filter_show_filter(mon_pdev, mode, pfilter);
		mon_pdev->filter[mode][srng_type] = *pfilter;
	} else /* reset the filter */
		mon_pdev->filter[mode][srng_type] = *pfilter;
}
#else
static
void dp_mon_filter_set_reset_rx_pkt_log_cbf_dest(struct dp_pdev *pdev,
						 struct dp_mon_filter *pfilter)
{
}
#endif

void dp_mon_filter_setup_rx_pkt_log_cbf_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	struct dp_soc *soc = NULL;
	enum dp_mon_filter_mode mode = DP_MON_FILTER_PKT_LOG_CBF_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	struct dp_mon_pdev *mon_pdev = NULL;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return;
	}

	mon_pdev = pdev->monitor_pdev;
	soc = pdev->soc;
	if (!soc) {
		dp_mon_filter_err("Soc Context is null");
		return;
	}

	/* Enabled the filter */
	filter.valid = true;
	dp_mon_filter_set_status_cbf(pdev, &filter);
	dp_mon_filter_show_filter(mon_pdev, mode, &filter);
	mon_pdev->filter[mode][srng_type] = filter;

	/* Clear the filter as the same filter will be used to set the
	 * monitor status ring
	 */
	qdf_mem_zero(&(filter), sizeof(struct dp_mon_filter));

	filter.valid = true;
	dp_mon_filter_set_reset_rx_pkt_log_cbf_dest(pdev, &filter);
}

void dp_mon_filter_reset_rx_pktlog_cbf_1_0(struct dp_pdev *pdev)
{
	struct dp_mon_filter filter = {0};
	struct dp_soc *soc = NULL;
	enum dp_mon_filter_mode mode = DP_MON_FILTER_PKT_LOG_CBF_MODE;
	enum dp_mon_filter_srng_type srng_type =
				DP_MON_FILTER_SRNG_TYPE_RXDMA_BUF;
	struct dp_mon_pdev *mon_pdev = NULL;

	if (!pdev) {
		QDF_TRACE(QDF_MODULE_ID_MON_FILTER, QDF_TRACE_LEVEL_ERROR,
			  FL("pdev Context is null"));
		return;
	}

	mon_pdev = pdev->monitor_pdev;
	soc = pdev->soc;
	if (!soc) {
		QDF_TRACE(QDF_MODULE_ID_MON_FILTER, QDF_TRACE_LEVEL_ERROR,
			  FL("Soc Context is null"));
		return;
	}

	dp_mon_filter_set_reset_rx_pkt_log_cbf_dest(pdev, &filter);

	srng_type = DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS;
	mon_pdev->filter[mode][srng_type] = filter;
}
#endif /* WDI_EVENT_ENABLE */

#ifdef WLAN_DP_RESET_MON_BUF_RING_FILTER
/**
 * dp_mon_should_reset_buf_ring_filter() - Reset the monitor buf ring filter
 * @pdev: DP PDEV handle
 *
 * WIN has targets which does not support monitor mode, but still do the
 * monitor mode init/deinit, only the rxdma1_enable flag will be set to 0.
 * MCL need to do the monitor buffer ring filter reset always, but this is
 * not needed for WIN targets where rxdma1 is not enabled (the indicator
 * that monitor mode is not enabled.
 * This function is used as WAR till WIN cleans up the monitor mode
 * function for targets where monitor mode is not enabled.
 *
 * Returns: true
 */
static inline bool dp_mon_should_reset_buf_ring_filter(struct dp_pdev *pdev)
{
	return (pdev->monitor_pdev->mvdev) ? true : false;
}
#else
static inline bool dp_mon_should_reset_buf_ring_filter(struct dp_pdev *pdev)
{
	return false;
}
#endif

#ifdef QCA_MONITOR_PKT_SUPPORT
static QDF_STATUS dp_mon_filter_dest_update(struct dp_pdev *pdev,
					    struct dp_mon_filter *pfilter,
					    bool *pmon_mode_set)
{
	struct dp_soc *soc = pdev->soc;
	struct dp_mon_pdev *mon_pdev = pdev->monitor_pdev;
	enum dp_mon_filter_srng_type srng_type;
	QDF_STATUS status = QDF_STATUS_SUCCESS;

	srng_type = ((soc->wlan_cfg_ctx->rxdma1_enable) ?
			DP_MON_FILTER_SRNG_TYPE_RXDMA_MON_BUF :
			DP_MON_FILTER_SRNG_TYPE_RXDMA_BUF);

	dp_mon_filter_h2t_setup(soc, pdev, srng_type, pfilter);

	*pmon_mode_set = pfilter->valid;
	if (dp_mon_should_reset_buf_ring_filter(pdev) || *pmon_mode_set) {
		status = dp_mon_ht2_rx_ring_cfg(soc, pdev,
						srng_type,
						&pfilter->tlv_filter);
	} else {
		/*
		 * For WIN case the monitor buffer ring is used and it does need
		 * reset when monitor mode gets enabled/disabled.
		 */
		if (soc->wlan_cfg_ctx->rxdma1_enable) {
			if (mon_pdev->monitor_configured || *pmon_mode_set) {
				status = dp_mon_ht2_rx_ring_cfg(soc, pdev,
								srng_type,
								&pfilter->tlv_filter);
			}
		}
	}

	return status;
}

static void dp_mon_filter_dest_reset(struct dp_pdev *pdev)
{
	struct dp_soc *soc = pdev->soc;
	enum dp_mon_filter_srng_type srng_type;

	srng_type = ((soc->wlan_cfg_ctx->rxdma1_enable) ?
			DP_MON_FILTER_SRNG_TYPE_RXDMA_MON_BUF :
			DP_MON_FILTER_SRNG_TYPE_RXDMA_BUF);

	dp_mon_filter_reset_mon_srng(soc, pdev, srng_type);
}
#else
static QDF_STATUS dp_mon_filter_dest_update(struct dp_pdev *pdev,
					    struct dp_mon_filter *pfilter,
					    bool *pmon_mode_set)
{
	return QDF_STATUS_SUCCESS;
}

static void dp_mon_filter_dest_reset(struct dp_pdev *pdev)
{
}
#endif

QDF_STATUS dp_mon_filter_update_1_0(struct dp_pdev *pdev)
{
	struct dp_soc *soc;
	bool mon_mode_set = false;
	struct dp_mon_filter filter = {0};
	QDF_STATUS status = QDF_STATUS_SUCCESS;
	struct dp_mon_pdev *mon_pdev;

	if (!pdev) {
		dp_mon_filter_err("pdev Context is null");
		return QDF_STATUS_E_FAILURE;
	}

	mon_pdev = pdev->monitor_pdev;
	soc = pdev->soc;
	if (!soc) {
		dp_mon_filter_err("Soc Context is null");
		return QDF_STATUS_E_FAILURE;
	}

	status = dp_mon_filter_check_co_exist(pdev);
	if (status != QDF_STATUS_SUCCESS)
		return status;

	/*
	 * Setup the filters for the monitor destination ring.
	 */
	status = dp_mon_filter_dest_update(pdev, &filter,
					   &mon_mode_set);

	if (status != QDF_STATUS_SUCCESS) {
		dp_mon_filter_err("%pK: Monitor destination ring filter setting failed",
				  soc);
		return QDF_STATUS_E_FAILURE;
	}

	/*
	 * Setup the filters for the status ring.
	 */
	qdf_mem_zero(&(filter), sizeof(filter));
	dp_mon_filter_h2t_setup(soc, pdev,
				DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS,
				&filter);

	/*
	 * Reset the monitor filters if the all the modes for the status rings
	 * are disabled. This is done to prevent the HW backpressure from the
	 * monitor destination ring in case the status ring filters
	 * are not enabled.
	 */
	if (!filter.valid && mon_mode_set)
		dp_mon_filter_dest_reset(pdev);

	if (dp_mon_ht2_rx_ring_cfg(soc, pdev,
				   DP_MON_FILTER_SRNG_TYPE_RXDMA_MONITOR_STATUS,
				   &filter.tlv_filter) != QDF_STATUS_SUCCESS) {
		dp_mon_filter_err("%pK: Monitor status ring filter setting failed",
				  soc);
		dp_mon_filter_dest_reset(pdev);
		return QDF_STATUS_E_FAILURE;
	}

	return status;
}
