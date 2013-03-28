/*****************************************************************************
 * Copyright (C) 2011-2013 Michael Krufky
 *
 * Author: Michael Krufky <mkrufky@linuxtv.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#ifndef __PARSE_H__
#define __PARSE_H__

#include <stdint.h>
#include <string.h>

#include "decode.h"
#include "demux.h"
#include "output.h"
#include "stats.h"

#define LIBDVBTEE_VERSION "0.1.9"

#define USE_STATIC_DECODE_MAP 1

#include <map>
typedef std::map<uint16_t, dvbpsi_handle> map_dvbpsi;
typedef std::map<uint16_t, decode> map_decoder;
#if 0 // moved to output.h
typedef std::map<uint16_t, uint16_t> map_pidtype;
#endif

typedef struct {
	unsigned int channel;
	uint32_t frequency;
	const char *modulation;
} channel_info_t;
typedef std::map<uint16_t, channel_info_t> map_channel_info;

typedef void (*addfilter_callback)(void *, uint16_t);

typedef struct {
	uint16_t lcn;
	uint16_t major;
	uint16_t minor;
	uint16_t vpid;
	uint16_t apid;
	uint16_t program_number;
	uint16_t physical_channel;
	uint32_t freq;
	const char *modulation;
	unsigned char *service_name;
} parsed_channel_info_t;

typedef const char * (*chandump_callback)(void *context, parsed_channel_info_t *c);

class parse
{
public:
	parse();
	~parse();

	unsigned int get_fed_pkt_count() const { return fed_pkt_count; };
	uint16_t get_ts_id() const { return ts_id; };
	uint16_t get_ts_id(unsigned int channel);

	void add_service_pids(uint16_t service_id, map_pidtype &pids);
	void add_service_pids(char* service_ids, map_pidtype &pids);
	void add_service_pids(map_pidtype &pids);

	void reset_output_pids(int target_id = -1) { out.reset_pids(target_id); };

	void set_service_ids(char *ids);

	int feed(int, uint8_t*);
	void reset();
	void stop();

	int add_output(char*);
	int add_output(int, unsigned int);
	int add_output(void* priv, stream_callback);

	int add_output(char*, map_pidtype&);
	int add_output(int, unsigned int, map_pidtype&);
	int add_output(void* priv, stream_callback, map_pidtype&);

	int add_output(char*, uint16_t);
	int add_output(int, unsigned int, uint16_t);
	int add_output(void* priv, stream_callback, uint16_t);

	int add_output(char*, char*);
	int add_output(int, unsigned int, char*);
	int add_output(void* priv, stream_callback, char*);

	unsigned int xine_dump(chandump_callback chandump_cb = NULL, void* chandump_context = NULL); /* full channel dump  */
	void epg_dump(decode_report *reporter = NULL); /* full channel dump  */

	void set_channel_info(unsigned int channel, uint32_t frequency, const char *modulation)
	{ new_channel_info.channel = channel; new_channel_info.frequency = frequency; new_channel_info.modulation = modulation; };

	void set_scan_mode(bool onoff) { scan_mode = onoff; };
	void set_epg_mode(bool onoff)  { epg_mode = onoff; };
	void enable(bool onoff)  { enabled = onoff; };
//got_all_eit()
	bool is_psip_ready();
	bool is_epg_ready();

	void cleanup();

	void limit_eit(int limit) { eit_collection_limit = limit; }

	void process_error_packets(bool yesno) { process_err_pkts = yesno; }

	void set_addfilter_callback(addfilter_callback cb, void* context) { addfilter_context = context; addfilter_cb = cb; reset_filters(); }

	output out;

	bool check();
	bool is_enabled() { return enabled; }

	stats statistics;
private:
#if !USE_STATIC_DECODE_MAP
	map_decoder   decoders;
#endif
	static void take_pat(void*, dvbpsi_pat_t*);
	static void take_pmt(void*, dvbpsi_pmt_t*);
	static void take_eit(void*, dvbpsi_eit_t*);
	static void take_nit_actual(void*, dvbpsi_nit_t*);
	static void take_nit_other(void*,  dvbpsi_nit_t*);
	static void take_sdt_actual(void*, dvbpsi_sdt_t*);
	static void take_sdt_other(void*,  dvbpsi_sdt_t*);
	static void take_tot(void*, dvbpsi_tot_t*);
	static void take_vct(void*, dvbpsi_atsc_vct_t*);
	static void take_eit(void*, dvbpsi_atsc_eit_t*);
	static void take_ett(void*, dvbpsi_atsc_ett_t*);
	static void take_stt(void*, dvbpsi_atsc_stt_t*);
	static void take_mgt(void*, dvbpsi_atsc_mgt_t*);

	static void attach_table(void*, dvbpsi_handle, uint8_t, uint16_t);

	bool take_pat(dvbpsi_pat_t*, bool);
	bool take_pmt(dvbpsi_pmt_t*, bool);
	bool take_eit(dvbpsi_eit_t*, bool);
	bool take_nit_actual(dvbpsi_nit_t*, bool);
	bool take_nit_other(dvbpsi_nit_t*,  bool);
	bool take_sdt_actual(dvbpsi_sdt_t*, bool);
	bool take_sdt_other(dvbpsi_sdt_t*,  bool);
	bool take_tot(dvbpsi_tot_t*, bool);
	bool take_vct(dvbpsi_atsc_vct_t*, bool);
	bool take_eit(dvbpsi_atsc_eit_t*, bool);
	bool take_ett(dvbpsi_atsc_ett_t*, bool);
	bool take_stt(dvbpsi_atsc_stt_t*, bool);
	bool take_mgt(dvbpsi_atsc_mgt_t*, bool);

	void attach_table(dvbpsi_handle, uint8_t, uint16_t);

	unsigned int xine_dump(uint16_t ts_id, chandump_callback chandump_cb, void* chandump_context)
	{ return xine_dump(ts_id, &channel_info[ts_id], chandump_cb, chandump_context); };
	unsigned int xine_dump(uint16_t, channel_info_t*, chandump_callback, void* chandump_context);

	void set_ts_id(uint16_t);
	void set_service_id(uint16_t id) { service_ids[id] = 0; }
	void detach_demux();

	channel_info_t new_channel_info;
	map_channel_info channel_info;

	unsigned int fed_pkt_count;

	dvbpsi_handle h_pat;
	map_dvbpsi    h_pmt;
	map_dvbpsi    h_demux;

	time_t stream_time;
	uint16_t ts_id;
	map_pidtype service_ids; // ignore the type name used here

	bool epg_mode;
	bool scan_mode;
	bool dont_collect_ett;
	bool has_pat;
	bool has_mgt;
	bool has_vct;
	bool has_sdt;
	bool has_nit;
	bool expect_vct;

//	uint8_t grab_next_eit(uint8_t current_eit_x);
	map_pidtype eit_pids; /* pid, eit-x */

	int dumped_eit;
	int eit_collection_limit;

	bool process_err_pkts;
	unsigned int tei_count;
	map_pidtype payload_pids;

	addfilter_callback addfilter_cb;
	void* addfilter_context;
	void add_filter(uint16_t pid) { if ((addfilter_context) && (addfilter_cb)) addfilter_cb(addfilter_context, pid); }
	void clear_filters() { if ((addfilter_context) && (addfilter_cb)) addfilter_cb(addfilter_context, 0xffff); }
	void reset_filters();

	bool enabled;

	uint8_t pat_pkt[188];

	uint8_t rewritten_pat_ver_offset, rewritten_pat_cont_ctr;
	void rewrite_pat();
	void process_pat(const decoded_pat_t *);
	void process_pmt(const decoded_pmt_t *);
#ifdef DVBTEE_DEMUXER
	demux demuxer;
#endif
	map_pidtype out_pids;
};

#endif //__PARSE_H__
