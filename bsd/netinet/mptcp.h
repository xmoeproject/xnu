/*
 * Copyright (c) 2012-2014 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */

#ifndef	_NETINET_MPTCP_H_
#define	_NETINET_MPTCP_H_

#ifdef BSD_KERNEL_PRIVATE

#include <machine/endian.h>

#if BYTE_ORDER == BIG_ENDIAN
#define	mptcp_hton64(x)  (x)
#define	mptcp_ntoh64(x)  (x)
#else /* LITTLE_ENDIAN */
#define	mptcp_hton64(x)  __DARWIN_OSSwapInt64(x)
#define	mptcp_ntoh64(x)  __DARWIN_OSSwapInt64(x)
#endif

/*
 * MPTCP Option Subtype Field values
 */
#define	MPO_CAPABLE	0x0
#define	MPO_JOIN	0x1
#define	MPO_DSS		0x2
#define	MPO_ADD_ADDR	0x3
#define	MPO_REMOVE_ADDR	0x4
#define	MPO_PRIO	0x5
#define	MPO_FAIL	0x6
#define	MPO_FASTCLOSE	0x7

/* MPTCP Protocol version */
#define	MPTCP_STD_VERSION_0	0x0

/*
 * MPTCP MP_CAPABLE TCP Option definitions
 *
 * Used to establish an MPTCP connection and first subflow.
 */
struct mptcp_mpcapable_opt_common {
	u_int8_t	mmco_kind;
	u_int8_t	mmco_len;
#if BYTE_ORDER == LITTLE_ENDIAN
	u_int8_t	mmco_version:4,
			mmco_subtype:4;
#else /* BIG_ENDIAN */
	u_int8_t	mmco_subtype:4,
			mmco_version:4;
#endif
#define	MPCAP_PROPOSAL_SBIT	0x01	/* SHA1 Algorithm */
#define	MPCAP_HBIT		0x01	/* alias of MPCAP_PROPOSAL_SBIT */
#define	MPCAP_GBIT		0x02	/* must be 0 */
#define	MPCAP_FBIT		0x04	/* must be 0 */
#define	MPCAP_EBIT		0x08	/* must be 0 */
#define	MPCAP_DBIT		0x10	/* must be 0 */
#define	MPCAP_CBIT		0x20	/* must be 0 */
#define	MPCAP_BBIT		0x40	/* Extensibility bit, must be 0 */
#define	MPCAP_ABIT		0x80	/* alias of MPCAP_CHECKSUM_CBIT */
#define	MPCAP_CHECKSUM_CBIT	0x80	/* DSS Checksum bit */
	u_int8_t	mmco_flags;
} __attribute__((__packed__));

struct mptcp_mpcapable_opt_rsp {
	struct mptcp_mpcapable_opt_common mmc_common;
	mptcp_key_t mmc_localkey;
} __attribute__((__packed__));

struct mptcp_mpcapable_opt_rsp1 {
	struct mptcp_mpcapable_opt_common mmc_common;
	mptcp_key_t mmc_localkey;
	mptcp_key_t mmc_remotekey;
} __attribute__((__packed__));

/*
 * MPTCP MP_JOIN TCP Option definitions
 *
 * Used to add subflows to an existing MP_CAPABLE connection.
 */

/* MP_JOIN Option for SYN */
struct mptcp_mpjoin_opt_req {
	u_int8_t	mmjo_kind;
	u_int8_t	mmjo_len;
#define	MPTCP_BACKUP	0x1
	u_int8_t	mmjo_subtype_bkp;
	u_int8_t	mmjo_addr_id;
	u_int32_t	mmjo_peer_token;
	u_int32_t	mmjo_rand;
} __attribute__((__packed__));

/* MP_JOIN Option for SYN/ACK */
struct mptcp_mpjoin_opt_rsp {
	u_int8_t	mmjo_kind;
	u_int8_t	mmjo_len;
#define	MPTCP_BACKUP	0x1
	u_int8_t	mmjo_subtype_bkp;
	u_int8_t 	mmjo_addr_id;
	u_int64_t	mmjo_mac; /* Truncated message auth code */
	u_int32_t	mmjo_rand;
} __attribute__((__packed__));

/* MP_Join Option for ACK */
struct mptcp_mpjoin_opt_rsp2 {
	u_int8_t	mmjo_kind;
	u_int8_t	mmjo_len;
#if BYTE_ORDER == LITTLE_ENDIAN
	u_int8_t	mmjo_reserved1:4,
			mmjo_subtype:4;
#else /* BIG_ENDIAN */
	u_int8_t	mmjo_subtype:4,
			mmjo_reserved1:4;
#endif
	u_int8_t	mmjo_reserved2;
	u_int8_t	mmjo_mac[20]; /* This is 160 bits HMAC SHA-1 per RFC */
} __attribute__((__packed__));


/*
 * MPTCP ADD_ADDR and REMOVE_ADDR TCP Options
 *
 * ADD_ADDR option shall be ignored by this implementation
 * REMOVE_ADDR option shall be sent to help flush dead subflows
 */

/* Add Address Option */
struct mptcp_addaddr_opt {
	u_int8_t	ma_kind;
	u_int8_t	ma_len;
#if BYTE_ORDER == LITTLE_ENDIAN
	u_int8_t	ma_ipver:4,
			ma_subtype:4;
#else /* BIG_ENDIAN */
	u_int8_t	ma_subtype:4,
			ma_ipver:4;
#endif
#define	MA_IPVer_V4 4	/* IPv4 Address tagged to the option */
#define	MA_IPVer_V6 6	/* IPv6 Address tagged to the option */
	u_int8_t	ma_addr_id;
} __attribute__((__packed__));

/* Address sent in the ADD_ADDR option */
struct mptcp_addr_family_val {
	union {
		struct in_addr	ma_v4_addr;
		struct in6_addr	ma_v6_addr;
	} ma_addr;
	/* u_int16_t ma_ports; */	/* optional field */
} __attribute__((__packed__));

/* Remove Address Option */
struct mptcp_remaddr_opt {
	u_int8_t	mr_kind;
	u_int8_t	mr_len;
#if BYTE_ORDER == LITTLE_ENDIAN
	u_int8_t	mr_rest:4,
			mr_subtype:4;
#else /* BIG_ENDIAN */
	u_int8_t	mr_subtype:4,
			mr_rest:4;
#endif
	u_int8_t	mr_addr_id;
} __attribute__((__packed__));

/*
 * MPTCP Data Sequence Signal (DSS) TCP Options
 *
 * Used to map subflow sequence space to MPTCP data sequence space.
 * Used to send Data ACKs
 */

/*
 * DSS Option variants coded as flags in the DSS option flags field
 */
#define	MDSS_A 0x01	/* Data ACK present if set */
#define	MDSS_a 0x02	/* 64-bit Data ACK present if set */
#define	MDSS_M 0x04	/* Data Sequence Number present if set */
#define	MDSS_m 0x08	/* 64-bit Data Sequence Number present if set */
#define	MDSS_F 0x10	/* Data FIN present */

/* DSS fields common to all DSS option variants */
struct mptcp_dss_copt {
	u_int8_t	mdss_kind;
	u_int8_t	mdss_len;
#if BYTE_ORDER == LITTLE_ENDIAN
	u_int8_t	mdss_reserved1:4,
			mdss_subtype:4;
#else /* BIG_ENDIAN */
	u_int8_t	mdss_subtype:4,
			mdss_reserved1:4;
#endif
	u_int8_t	mdss_flags;
}__attribute__((__packed__));

/* 32-bit DSS option */
struct mptcp_dsn_opt {
	struct mptcp_dss_copt	mdss_copt;
	u_int32_t	mdss_dsn;		/* Data Sequence Number */
	u_int32_t	mdss_subflow_seqn;	/* Relative Subflow Seq Num */
	u_int16_t	mdss_data_len;		/* Data Length */
	/* u_int16_t	mdss_xsum; */		/* Data checksum - optional */

}__attribute__((__packed__));

/* 64-bit DSS option */
struct mptcp_dsn64_opt {
	struct mptcp_dss_copt   mdss_copt;
	u_int64_t	mdss_dsn;		/* Data Sequence Number */
	u_int32_t	mdss_subflow_seqn;	/* Relative Subflow Seq Num */
	u_int16_t	mdss_data_len;		/* Data Length */
	/* u_int16_t	mdss_xsum; */		/* Data checksum - optional */
}__attribute__((__packed__));

/* 32-bit DSS Data ACK option */
struct mptcp_data_ack_opt {
	struct mptcp_dss_copt   mdss_copt;
	u_int32_t		mdss_ack;
}__attribute__((__packed__));

/* 64-bit DSS Data ACK option */
struct mptcp_data_ack64_opt {
	struct mptcp_dss_copt   mdss_copt;
	u_int64_t		mdss_ack;
}__attribute__((__packed__));

/* 32-bit DSS+Data ACK option */
struct mptcp_dss_ack_opt {
	struct mptcp_dss_copt   mdss_copt;
	u_int32_t	mdss_ack;		/* Data ACK */
	u_int32_t	mdss_dsn;		/* Data Sequence Number */
	u_int32_t	mdss_subflow_seqn;	/* Relative Subflow Seq Num */
	u_int16_t	mdss_data_len;		/* Data Length */
	/* u_int16_t mdss_xsum; */		/* Data checksum - optional */
}__attribute__((__packed__));

/* 64-bit DSS+Data ACK option */
struct mptcp_dss64_ack64_opt {
	struct mptcp_dss_copt   mdss_copt;
	u_int64_t	mdss_ack;		/* Data ACK */
	u_int64_t	mdss_dsn;		/* Data Sequence Number */
	u_int32_t	mdss_subflow_seqn;	/* Relative Subflow Seq Num */
	u_int16_t	mdss_data_len;		/* Data Length */
	/* u_int16_t mdss_xsum; */		/* Data checksum - optional */
}__attribute__((__packed__));

/* DSS+Data ACK mixed option variants */
struct mptcp_dss32_ack64_opt {
	struct mptcp_dss_copt   mdss_copt;
	u_int64_t	mdss_ack;		/* Data ACK */
	u_int32_t	mdss_dsn;		/* Data Sequence Number */
	u_int32_t	mdss_subflow_seqn;	/* Relative Subflow Seq Num */
	u_int16_t	mdss_data_len;		/* Data Length */
	/* u_int16_t mdss_xsum; */		/* Data checksum - optional */
}__attribute__((__packed__));

struct mptcp_dss64_ack32_opt {
	struct mptcp_dss_copt   mdss_copt;
	u_int32_t	mdss_ack;		/* Data ACK */
	u_int64_t	mdss_dsn;		/* Data Sequence Number */
	u_int32_t	mdss_subflow_seqn;	/* Relative Subflow Seq Num */
	u_int16_t	mdss_data_len;		/* Data Length */
	/* u_int16_t mdss_xsum; */		/* Data checksum - optional */
}__attribute__((__packed__));


/*
 * MPTCP Fast Close Option
 *
 * MPTCP connection is aborted if the FastClose option is received.
 * In future, we may send this option if a MPTCP socket level abort
 * API is supported.
 */
struct mptcp_fastclose_opt {
	u_int8_t	mfast_kind;
	u_int8_t 	mfast_len;
#if BYTE_ORDER == LITTLE_ENDIAN
	u_int8_t	mfast_reserved:4,
			mfast_subtype:4;
#else /* BIG_ENDIAN */
	u_int8_t	mfast_subtype:4,
			mfast_reserved:4;
#endif
	u_int8_t	mfast_reserved1;
	u_int64_t	mfast_key;		/* Option receiver's key */
}__attribute__((__packed__));

/*
 * MPTCP MP_FAIL Option
 *
 * When DSS checksum is ON, and checksum fails, remote peer may send
 * this option to indicate the failure. Likewise, we may send this
 * option.
 */
struct mptcp_mpfail_opt {
	u_int8_t	mfail_kind;
	u_int8_t	mfail_len;
#if BYTE_ORDER == LITTLE_ENDIAN
	u_int8_t	mfail_reserved:4,
			mfail_subtype:4;
#else /* BIG_ENDIAN */
	u_int8_t	mfail_subtype:4,
			mfail_reserved:4;
#endif
	u_int8_t	mfail_reserved1:8;
	u_int64_t	mfail_dsn;
}__attribute__((__packed__));


/*
 * MPTCP MP_PRIO Option
 *
 * When a subflow becomes unusable (due to bad radio coverage) or
 * it is the costlier path or it is not the preferred path, the receiver may
 * use this option to let the sender know of its path preference.
 */

/* Option to change priority of self */
struct mptcp_mpprio_opt {
	u_int8_t	mpprio_kind;
	u_int8_t	mpprio_len;
#define	MPTCP_MPPRIO_BKP	0x1
#if BYTE_ORDER == LITTLE_ENDIAN
	u_int8_t	mpprio_flags:4,
			mpprio_subtype:4;
#else /* BIG_ENDIAN */
	u_int8_t	mpprio_subtype:4,
			mpprio_flags:4;
#endif
}__attribute__((__packed__));

/* Option to change priority of some other subflow(s) using addr_id */
struct mptcp_mpprio_addr_opt {
	u_int8_t	mpprio_kind;
	u_int8_t	mpprio_len;
#define	MPTCP_MPPRIO_BKP	0x1
#if BYTE_ORDER == LITTLE_ENDIAN
	u_int8_t	mpprio_flags:4,
			mpprio_subtype:4;
#else /* BIG_ENDIAN */
	u_int8_t	mpprio_subtype:4,
			mpprio_flags:4;
#endif
	u_int8_t	mpprio_addrid;
}__attribute__((__packed__));

/*
 * MPTCP Checksum Psuedo Header
 *
 */
struct mptcp_pseudohdr {
	u_int64_t	mphdr_dsn;	/* Data Sequence Number */
	u_int32_t	mphdr_ssn;	/* Subflow Sequence Number */
	u_int16_t	mphdr_len;	/* Data-Level Length */
	u_int16_t	mphdr_xsum;	/* MPTCP Level Checksum */
}__attribute__((__packed__));

#endif /* BSD_KERNEL_PRIVATE */

#endif /* _NETINET_MPTCP_H_ */
