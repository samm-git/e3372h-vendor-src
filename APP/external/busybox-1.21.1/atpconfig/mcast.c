//usage:#define mcast_trivial_usage 
//usage:	    "[OPTION]... HOST"
//usage:#define mcast_full_usage 
//usage:	    "mcast download a file through multicast channel\n\n" 
//usage:       "\t-g, --download         Download\n" 
//usage:       "\t-s, --upload           Upload\n" 
//usage:       "\t-v, --verbose          Verbose\n" 
//usage:       "\t-u, --username         Username to be used\n" 
//usage:       "\t-p, --password         Password to be used\n" 
//usage:       "\t-l, --local            Local file path\n" 
//usage:       "\t-r, --remote           Remote file path\n" 
//usage:       "\t-P, --port             Port to be used, optional\n" 
//usage:       "\t-B, --bind             Bind local ip, optional\n" 
//usage:       "\t-A, --addr             Remote resolved ip, optional\n" 
//usage:       "\t-b, --begin            Transfer start position\n" 
//usage:       "\t-e, --len              Transfer length\n" 
//usage:       "\t-m, --max              Max transfer size\n" 
//usage:       "\t-c, --compress         Compress downloaded file" 
        

#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netpacket/packet.h>

#include <getopt.h>	/* for struct option */
#include "libbb.h"
#include "atpcommon.h"

#if 0
#define DBG_PRINT    printf
#else
#define DBG_PRINT(x...)
#endif

/*
 *  We add an ip address collision detection function here
 */

struct ip_collision_detector {
    struct in_addr      src;
	struct in_addr      dst;
	struct sockaddr_ll  me;
	struct sockaddr_ll  he;
    int                 fd;
    int                 count;
    char                *target_ip;
};

#define detector ((struct ip_collision_detector *)(bb_common_bufsiz1 + sizeof(struct atp_trans_globals)))

static int send_arp_packet(struct in_addr *src_addr,
			struct in_addr *dst_addr, struct sockaddr_ll *ME,
			struct sockaddr_ll *HE)
{
	int err;
	unsigned char buf[256];
	struct arphdr *ah = (struct arphdr *) buf;
	unsigned char *p = (unsigned char *) (ah + 1);

	ah->ar_hrd = htons(ME->sll_hatype);
	ah->ar_hrd = htons(ARPHRD_ETHER);
	ah->ar_pro = htons(ETH_P_IP);
	ah->ar_hln = ME->sll_halen;
	ah->ar_pln = 4;
	ah->ar_op  = htons(ARPOP_REQUEST);

	memcpy(p, &ME->sll_addr, ah->ar_hln);
	p += ME->sll_halen;

	memcpy(p, src_addr, 4);
	p += 4;

	memcpy(p, &HE->sll_addr, ah->ar_hln);
	p += ah->ar_hln;

	memcpy(p, dst_addr, 4);
	p += 4;

	err = sendto(detector->fd, buf, p - buf, 0,
                 (struct sockaddr *) HE, sizeof(*HE));
	if (err == p - buf) {
		detector->count -= 1;
	}

	return err;
}

static int recv_arp_packet(unsigned char *buf, int len, struct sockaddr_ll *FROM)
{
	struct arphdr *ah = (struct arphdr *) buf;
	unsigned char *p = (unsigned char *) (ah + 1);
	struct in_addr src_ip, dst_ip;

	/* Filter out wild packets */
	if (FROM->sll_pkttype != PACKET_HOST
	 && FROM->sll_pkttype != PACKET_BROADCAST
	 && FROM->sll_pkttype != PACKET_MULTICAST)
		return 0;

	/* Only these types are recognised */
	if (ah->ar_op != htons(ARPOP_REQUEST) && ah->ar_op != htons(ARPOP_REPLY))
		return 0;

	/* ARPHRD check and this darned FDDI hack here :-( */
	if (ah->ar_hrd != htons(FROM->sll_hatype)
	 && (FROM->sll_hatype != ARPHRD_FDDI || ah->ar_hrd != htons(ARPHRD_ETHER)))
		return 0;

	/* Protocol must be IP. */
	if (ah->ar_pro != htons(ETH_P_IP))
		return 0;
	if (ah->ar_pln != 4)
		return 0;
	if (ah->ar_hln != detector->me.sll_halen)
		return 0;
	if (len < sizeof(*ah) + 2 * (4 + ah->ar_hln))
		return 0;
	memcpy(&src_ip, p + ah->ar_hln, 4);
	memcpy(&dst_ip, p + ah->ar_hln + 4 + ah->ar_hln, 4);

	/* DAD packet was:
	   src_ip = 0 (or some src)
	   src_hw = ME
	   dst_ip = tested address
	   dst_hw = <unspec>

	   We fail, if receive request/reply with:
	   src_ip = tested_address
	   src_hw != ME
	   if src_ip in request was not zero, check
	   also that it matches to dst_ip, otherwise
	   dst_ip/dst_hw do not matter.
	 */
	if (src_ip.s_addr != detector->dst.s_addr)
		return 0;
	if (memcmp(p, &detector->me.sll_addr, detector->me.sll_halen) == 0)
		return 0;
	if (detector->src.s_addr && detector->src.s_addr != dst_ip.s_addr)
		return 0;

	return 1;
}

//static void finish_detection(void) ATTRIBUTE_NORETURN;
static void finish_detection(void)
{
	close(detector->fd);
}

static void sender(void)
{
    if (detector->count <= 0) {
	    finish_detection();
	    return;
    }
    send_arp_packet(&(detector->src), &(detector->dst), &(detector->me), &(detector->he));
	alarm(1);
}

static int atp_detect_ip_collision(struct ip_collision_detector *detect)
{
    unsigned char *packet;

    detect->fd = xsocket(AF_PACKET, SOCK_DGRAM, 0);

    // Check local interface
    {
		struct ifreq ifr;

		memset(&ifr, 0, sizeof(ifr));
		strncpy(ifr.ifr_name, "br0", sizeof(ifr.ifr_name) - 1);
		/* We use ifr.ifr_name in error msg so that problem
		 * with truncated name will be visible */
		ioctl_or_perror_and_die(detect->fd, SIOCGIFINDEX, &ifr, "interface %s not found", ifr.ifr_name);
		detect->me.sll_ifindex = ifr.ifr_ifindex;

		xioctl(detect->fd, SIOCGIFFLAGS, (char *)&ifr);

		if (!(ifr.ifr_flags & IFF_UP)) {
            printf("interface br0 is down\n");
            close(detect->fd);
            detect->fd = 0;
            return -1;
		}
		if (ifr.ifr_flags & (IFF_NOARP | IFF_LOOPBACK)) {
            printf("interface br0 is not ARPable\n");
            close(detect->fd);
            detect->fd = 0;
            return -1;
		}
	}

    // Setup dest address
    if (NULL != detect->target_ip) {
		len_and_sockaddr *lsa;
		lsa = xhost_and_af2sockaddr(detect->target_ip, 0, AF_INET);
		memcpy(&detect->dst, &lsa->sin.sin_addr.s_addr, 4);
		if (ENABLE_FEATURE_CLEAN_UP)
			free(lsa);
	} else {
	    memcpy(&detect->dst, &detect->src, 4);
	}

    // Fill me and bind
    detect->me.sll_family = AF_PACKET;
	//me.sll_ifindex = ifindex; - done before
	detect->me.sll_protocol = htons(ETH_P_ARP);
	xbind(detect->fd, (struct sockaddr *) &detect->me, sizeof(detect->me));
    {
		socklen_t alen = sizeof(detect->me);

		if (getsockname(detect->fd, (struct sockaddr *) &detect->me, &alen) == -1) {
			bb_perror_msg_and_die("getsockname");
		}
	}
    detect->he = detect->me;
	memset(detect->he.sll_addr, -1, detect->he.sll_halen);

    // Setup signals
    {
		struct sigaction sa;

		memset(&sa, 0, sizeof(sa));
		sa.sa_flags = SA_RESTART;

		//sa.sa_handler = (void (*)(int)) finish_detection;
		//sigaction(SIGINT, &sa, NULL);

		sa.sa_handler = (void (*)(int)) sender;
		sigaction(SIGALRM, &sa, NULL);
	}

    // Sent the first packet now
    sender();

    // Loop to recv
	packet = xmalloc(4096);
	while (1) {
		//sigset_t sset, osset;
		struct sockaddr_ll from;
		socklen_t alen = sizeof(from);
		int cc;

		cc = recvfrom(detect->fd, packet, 4096, 0, (struct sockaddr *) &from, &alen);
        detect->count = 0;
		if (cc < 0) {
            close(detect->fd);
            detect->fd = 0;
			free(packet);
			return 0;
		}
		//sigemptyset(&sset);
		//sigaddset(&sset, SIGALRM);
		//sigaddset(&sset, SIGINT);
		//sigprocmask(SIG_BLOCK, &sset, &osset);
		if (recv_arp_packet(packet, cc, &from) > 0) {
		    printf("Packet recved %d!\n", cc);
            break;
        }
        else {
            printf("Fake packet!\n");
        }
		//sigprocmask(SIG_SETMASK, &osset, NULL);
	}

    close(detect->fd);
    detect->fd = 0;
	free(packet);
	return 1;
}

static void init_mcast_socket(int fd, const char *localIp, const char *dstIp)
{
    struct in_addr          mcastInterfaceAddr;
    struct ip_mreq          mreq;

    // Set reuseaddress
    setsockopt_reuseaddr(fd);

    // Set multicast
    memset(&mcastInterfaceAddr, 0, sizeof(mcastInterfaceAddr));
    if (NULL != localIp)
        mcastInterfaceAddr.s_addr = inet_addr(localIp);
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &mcastInterfaceAddr,
               sizeof(mcastInterfaceAddr));

    // Add to group
    mreq.imr_multiaddr.s_addr = inet_addr(dstIp);
    mreq.imr_interface.s_addr = 0;
    if (NULL != localIp)
        mreq.imr_interface.s_addr = inet_addr(localIp);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) < 0)
        bb_perror_msg_and_die("Join mcast group failed: %s!", localIp);
}

/* 子频道组播报文头信息 */
typedef struct tagATP_UPG_MULT_PACKET
{
    unsigned char           aucPacketLen[4];        // 当前组播包的长度
    unsigned char           aucCrc[4];              // 当前组播包的校验和
    unsigned char           aucSeqNum[4];           // 当前包的序号，从0开始
    unsigned char           aucImgLen[4];           // 文件总长度
    unsigned char           aucPkgType[4];          // 当前镜像文件类型
    unsigned char           aucVersion[32];         // 版本信息
    unsigned char           aucProductInfo[32];     // 产品信息
} ATP_UPG_MULT_PACKET;

#define NET_INT_TO_HOST(x, buf) {memcpy((void *)&(x), (void *)(buf), sizeof(unsigned int));\
                                 (x) = ntohl(x);}

#define CRC32_INIT_VALUE            (0xffffffff)
static unsigned int m_aulCrc32Table[256] =
{
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
	0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
	0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
	0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
	0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
	0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
	0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
	0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
	0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
	0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
	0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
	0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
	0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
	0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
	0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
	0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

	0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
	0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
	0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
	0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
	0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
	0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
	0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
	0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
	0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
	0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
	0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
	0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
	0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
	0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
	0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
	0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

	0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
	0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
	0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
	0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
	0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
	0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
	0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
	0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
	0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
	0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
	0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
	0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
	0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
	0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
	0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
	0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

	0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
	0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
	0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
	0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
	0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
	0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
	0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
	0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
	0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
	0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
	0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
	0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
	0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
	0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
	0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
	0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
};

static unsigned int upg_calc_crc(
                                const unsigned char     *pucSrcBuf,
                                unsigned int            ulSrcLen)
{
    unsigned int      ulCrc;

    // Calc crc
    ulCrc = CRC32_INIT_VALUE;
    while (ulSrcLen-- > 0)
    {
        ulCrc = (ulCrc >> 8) ^ m_aulCrc32Table[(ulCrc ^ *pucSrcBuf++) & 0xff];
    }

    return ulCrc;
}

#define MCAST_PACKET_LEN        (1024 - sizeof(ATP_UPG_MULT_PACKET))

static int mcast_write_dummy_file(int fd, int len)
{
    unsigned char       aucBuf[1024];
    int                 tmpLen;

    memset((void *)aucBuf, 0, sizeof(aucBuf));

    while (1) {
        if (len >= sizeof(aucBuf)) {
            tmpLen = sizeof(aucBuf);
        } else {
            tmpLen = len;
        }
        if (write(fd, (void *)aucBuf, (size_t)tmpLen) < tmpLen) {
            return -1;
        }
        len -= tmpLen;
        if (len <= 0) {
            break;
        }
    }

    return 0;
}

static int mcast_subchannel_data_proc(char *pcBuf, unsigned int len,
                                             const char *localFile)
{
    int                         fd;
    ATP_UPG_MULT_PACKET         *pstPkg;
    unsigned int                ulCurSeqNum;
    unsigned int                ulTmpInt;
    unsigned int                ulCalcCrc;

    if (len <= sizeof(ATP_UPG_MULT_PACKET))
    {
        DBG_PRINT("Subchannel data too short!\n");
        return 0;
    }
    pstPkg = (ATP_UPG_MULT_PACKET *)pcBuf;

    // Get current packet len
    NET_INT_TO_HOST(ulTmpInt, pstPkg->aucPacketLen);
    if (len != ulTmpInt)
    {
        DBG_PRINT("Subchannel data len check err!\n");
        return 0;
    }
    // Get current crc
    NET_INT_TO_HOST(ulTmpInt, pstPkg->aucCrc);
    ulCalcCrc = upg_calc_crc((pcBuf + 8), (len - 8));
    if (ulCalcCrc != ulTmpInt)
    {
        DBG_PRINT("Subchannel crc check err!\n");
        return 0;
    }
    // Get current seq num
    NET_INT_TO_HOST(ulCurSeqNum, pstPkg->aucSeqNum);

    // Get current img len
    NET_INT_TO_HOST(ulTmpInt, pstPkg->aucImgLen);

    // Open local file for write
    if (NULL == g_pucMcastBuf) {
        //DBG_PRINT("Recv from seq num %d!\n", ulCurSeqNum);
        printf("Recv from seq num %d!\n", ulCurSeqNum);
        g_ulTotalLen    = ulTmpInt;
        g_lMcastLen     = 0;
        fd = open(localFile, O_RDWR | O_CREAT,0666);
        if ((fd <= 0) || ((0 < g_lMaxTrans) && (ulTmpInt > g_lMaxTrans))) {
            xfunc_error_retval = ATP_TRANS_FILE_ERR;
            DBG_PRINT("Open local file failed!\n");
            return 0;
        }

        // Write dummy data
        if (mcast_write_dummy_file(fd, g_ulTotalLen) < 0) {
            perror("Write dummy file err!");
            return 0;
        }

        // Do memmap
        g_pucMcastBuf = (void *)mmap(0, ulTmpInt, PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
        if (((caddr_t)(-1)) == (caddr_t)g_pucMcastBuf) {
            perror("Mmap failed!");
            return 0;
        }

        // Return directly, discard the current packet, need to recv more
        return 1;
    }
    if ((NULL == g_pucMcastBuf) || (ulTmpInt != g_ulTotalLen)) {
        DBG_PRINT("Subchannel open file err!\n");
        return 0;
    }

    // If the first one, then start from the seq num
    if (0 == g_lMcastLen) {
        g_lMcastSeqNum  = ulCurSeqNum;
        printf("Store data from seq num %d!\n", ulCurSeqNum);
    }

    // Check seq number
    if (g_lMcastSeqNum != ulCurSeqNum)
    {
        //DBG_PRINT("Subchannel data seq num %d ignored!\n", ulTmpInt);
        printf("Subchannel data seq num %d ignored!\n", ulCurSeqNum);
        return 1;
    }

    // Prepare data len and addr
    len -= sizeof(ATP_UPG_MULT_PACKET);
    pstPkg += 1;

    // Check seq num
    ulCurSeqNum = MCAST_PACKET_LEN * g_lMcastSeqNum;
    if ((ulCurSeqNum + len) > g_ulTotalLen) {
        DBG_PRINT("Seq num %d too large\n", ulCurSeqNum);
        return 1;
    // If last one, next seq num should roll back to zero
    } else if ((ulCurSeqNum + len) == g_ulTotalLen) {
        g_lMcastSeqNum = 0;
    } else {
        g_lMcastSeqNum++;
    }
    DBG_PRINT("Store seq num %d len: %d total len: %d next %d\n",
              ulCurSeqNum, len, g_lMcastLen, g_lMcastSeqNum);
    memcpy((void *)(g_pucMcastBuf + ulCurSeqNum),
           (void *)pstPkg, len);
    //fwrite((void *)pstPkg, len, 1, g_pfMcastFile);
    g_lMcastLen += len;

    // Check end of the file
    if (ulTmpInt <= g_lMcastLen)
    {
        //munmap((caddr_t)g_pucMcastBuf, g_ulTotalLen);
        //g_pucMcastBuf = NULL;
        xfunc_error_retval = ATP_TRANS_OK;
        DBG_PRINT("Subchannel OK now!\n");
        return 0;
    }

    return 1;
}

static int mcast_pubchannel_data_proc(char *pcBuf, unsigned int len,
                                             const char *localFile)
{
    static int randomized = 0;
    FILE    *pfFile;
    char acArg[1024];
    unsigned int tmp;
    unsigned int tmpRnd;

    if ((0 < g_lMaxTrans) && (len > g_lMaxTrans)) {
        DBG_PRINT("File too large err!\n");
        return 1;
    }

    // Write data to local file
    pfFile = fopen(localFile, "wb");
    if (NULL == pfFile) {
        DBG_PRINT("Pubchannel open file err!\n");
        return 0;
    }
    fwrite((void *)pcBuf, len, 1, pfFile);
    fclose(pfFile);

    // 1 Check if an upgrade is needed
     pfFile = fopen("/var/state/upg", "rb");
    if (NULL != pfFile) {
        fclose(pfFile);
        //unlink("/var/state/upg");
        sleep(60);
        return 1;
    }

    // 2 Random sleep
    if (0 == randomized) {
        srand(monotonic_us());
    }
    tmp = rand() % 10;
    sleep(tmp);

    tmp = inet_addr(g_pcLocalIP);
    // 3 Find a random ip between 2-254
    while (1)
    {
    #ifdef BUILD_LITTLE_ENDIAN
        tmpRnd = (unsigned int)(rand() & 0xFF000000);
        if ((tmpRnd == (tmp & 0xFF000000)) || (0 == tmpRnd) ||
            (0xFF == tmpRnd) || (1 == tmpRnd))
        {
            continue;
        }
        tmp = tmp & 0x00FFFFFF;
        tmp = tmp + tmpRnd;
    #else
        tmpRnd = (unsigned int)(rand() & 0x000000FF);
        if ((tmpRnd == (tmp & 0x000000FF)) || (0 == tmpRnd) ||
            (0xFF == tmpRnd) || (1 == tmpRnd))
        {
            continue;
        }
        tmp = tmp & 0xFFFFFF00;
        tmp = tmp + tmpRnd;
    #endif
        detector->dst.s_addr = tmp;
        snprintf(acArg, sizeof(acArg), "%s", inet_ntoa(detector->dst));
        memset((void *)detector, 0, sizeof(struct ip_collision_detector));
        detector->count = 3;
        detector->target_ip = acArg;
        if (atp_detect_ip_collision(detector) > 0)
        {
            continue;
        }
        break;
    }

    // 4 Change ip address
    snprintf(acArg, sizeof(acArg), "ifconfig br0 %s",
             inet_ntoa(detector->dst));
    system(acArg);

    snprintf(acArg, sizeof(acArg), "upg -a %s -t '1 Firmware Upgrade Image' -b -c mcast -l %s", localFile, inet_ntoa(detector->dst));
    system(acArg);

    // 5 Restore LAN IP Address
    snprintf(acArg, sizeof(acArg), "ifconfig br0 %s", g_pcLocalIP);
    system(acArg);

    sleep(1);

    //xfunc_error_retval = ATP_TRANS_OK;
    return 1;
}

static void mcast_listen(int fd, const char *localFile,
                           const char *pcRemoteIp, int port, int mode)
{
    fd_set                      hReadset;
    struct timeval              stTimeVal;
    int                         slct;

    char                        acRecvBuf[2048];
    struct sockaddr_in          stAddr;
    int                         lAddrLen;
    int                         lBytesRecvd;

    while (1)
    {
        FD_ZERO(&hReadset);
        FD_SET(fd, &hReadset);

        stTimeVal.tv_sec    = 5;
        stTimeVal.tv_usec   = 0;
        slct = select(FD_SETSIZE, &hReadset, NULL, NULL, &stTimeVal);
        if (-1 == slct) {
            xfunc_error_retval = ATP_TRANS_TIMEOUT;
            DBG_PRINT("Select err!\n");
            break;
        }

        // Timeout
        if (0 == slct) {

            if (mode) {
                // Normal subchannel download
                xfunc_error_retval = ATP_TRANS_TIMEOUT;
                DBG_PRINT("Subchannel data timeout!\n");
                break;
            } else {
                DBG_PRINT("Pubchannel data timeout!\n");
                continue;
            }
        }

        if (0 == FD_ISSET(fd, &hReadset)) {
            continue;
        }

        // Recv data now
        lAddrLen = sizeof(struct sockaddr_in);
        lBytesRecvd = recvfrom(fd, acRecvBuf,
                          sizeof(acRecvBuf), 0, (struct sockaddr *)(&stAddr),
                          (socklen_t *)(&lAddrLen));
        if (lBytesRecvd <= 0) {
            DBG_PRINT("Data recv err!\n");
            continue;
        }

        // Process data now
        xfunc_error_retval = ATP_TRANS_SYS_ERR;
        if (mode) {
            lAddrLen = mcast_subchannel_data_proc(acRecvBuf,
                            (unsigned int)lBytesRecvd, localFile);
        } else {
            // Close listening socket when upgrading
            close(fd);

            lAddrLen = mcast_pubchannel_data_proc(acRecvBuf,
                            (unsigned int)lBytesRecvd, localFile);

            // Create UDP socket
            //fd = create_and_bind_dgram_or_die(g_pcLocalIP, port);
            fd = create_and_bind_dgram_or_die(NULL, port);

            // Init mcast socket
            //snprintf(acRecvBuf, sizeof(acRecvBuf), "%s", inet_ntoa(detector->dst));
            init_mcast_socket(fd, g_pcLocalIP, pcRemoteIp);
        }

        // Check process result
        if (0 == lAddrLen) {
            DBG_PRINT("Need exit now\n");
            break;
        }
    }

    close(fd);
}

int mcast_main(int argc, char **argv);
int mcast_main(int argc, char **argv)
{
    int                     opt;
    int                     fd;
    const char              *dummy;
    const char              *localFile  = NULL;
    const char              *remotePort = NULL;
    const char              *maxSize    = NULL;
    int                     mode        = 0;    // Default recv all data
    const char              *pcDelay    = NULL;

    xfunc_error_retval      = ATP_TRANS_SYS_ERR;
    g_lMcastSeqNum          = 0;
    g_lMcastLen             = 0;
    g_pucMcastBuf           = NULL;
    g_pcLocalIP             = NULL;

#if ENABLE_FEATURE_ATP_MCAST_ZIP
    g_bCompress             = 0;
    #define ZIP_OPT_CHAR    "c"
#else
    #define ZIP_OPT_CHAR
#endif

    opt = getopt32(argv, "gsdu:p:r:l:P:B:A:b:e:m:"ZIP_OPT_CHAR"f:",
                         &dummy,        // Username
                         &dummy,        // Password
                         &dummy,        // Remote path
                         &localFile,    // Local file
                         &remotePort,   // Remote Port
                         &g_pcLocalIP,  // Local ip
                         &dummy,        // Resolved ip
                         &dummy,
                         &dummy,
                         &maxSize,
                         &pcDelay);
#undef ZIP_OPT_CHAR

    if ((NULL == localFile) || (NULL == argv[optind]))
        bb_show_usage();

    if (opt & TRANS_TOOL_OPT_DOWNLOAD) {
		mode = 1;       // Download from sub channel
	}

    /* Run As Deamon */
    if (opt & TRANS_TOOL_OPT_VERBOSE) {
        switch (fork()) {
        case  -1:
            return ATP_TRANS_SYS_ERR;

        case 0:     // Child
            if (setsid() == -1) {
    	        return ATP_TRANS_SYS_ERR;
            }
        break;

        default:    // Parent
            _exit(ATP_TRANS_OK);
            break;
        }
    }

    /* Delay if necessary */
    if (NULL != pcDelay) {
        opt = atoi(pcDelay);
        if (opt > 0) {
            sleep(opt);
        }
    }

    // Set default port
    opt = 37123;
    if (NULL != remotePort)
        opt = atoi(remotePort);

    // Create UDP socket
    //fd = create_and_bind_dgram_or_die(g_pcLocalIP, opt);
    fd = create_and_bind_dgram_or_die(NULL, opt);

    // Init mcast socket
    init_mcast_socket(fd, g_pcLocalIP, argv[optind]);

    // Calc max file size
    g_lMaxTrans = 0;
    if (NULL != maxSize)
        g_lMaxTrans = atoi(maxSize);

    // Start listening
    mcast_listen(fd, localFile, argv[optind], opt, mode);
    if (NULL != g_pucMcastBuf) {
        munmap((caddr_t)g_pucMcastBuf, g_ulTotalLen);
        g_pucMcastBuf = NULL;
    }

    return xfunc_error_retval;
}

