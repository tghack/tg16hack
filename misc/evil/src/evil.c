/*
 * TODO: cleanup this shit
 */
#include <stdio.h>
#include <stdlib.h>
#include <libnet.h>
#include <stdint.h>

#define flag "TG16{evil_Packets_everywhere_y0u_better_be_SCARED_y0}"

static inline int send_icmp(uint8_t *payload, size_t payload_size,
			    uint32_t ip_addr, int evil, libnet_t *l)
{
	size_t size = LIBNET_IPV4_H + LIBNET_ICMPV4_ECHO_H + sizeof(payload);
	uint8_t tos = 0;
	uint8_t id = 0;
	uint16_t frag = evil ? 1 << 15 : 0; /* set evil bit, or not*/
	uint8_t ttl = 64;
	uint8_t prot = IPPROTO_ICMP;
	uint16_t sum = 0; /* autofill */
	uint32_t src_ip = ip_addr; /* TODO: fix this shit */
	uint32_t dst_ip = ip_addr;

	libnet_clear_packet(l);

	/* build ICMP header */
	if (libnet_build_icmpv4_echo(ICMP_ECHO, 0, 0, id, 1,
				payload, payload_size, l, 0) == -1) {
		fprintf(stderr, "Error building ICMP header: %s\n",
				libnet_geterror(l));
		return -1;
	}

	if (libnet_build_ipv4(size, tos, id, frag, ttl, prot, sum,
				src_ip, dst_ip, 0, 0, l, 0) == -1) {
		fprintf(stderr, "Error building IP header: %s\n",
				libnet_geterror(l));
		return -1;
	}

	int written = libnet_write(l);
	if (written != -1) {
		printf("Wrote %d bytes.\n", written);
	} else {
		fprintf(stderr, "Error writing packet: %s\n",
				libnet_geterror(l));
		return -1;
	}

	return 0;
}

/*
 * Send an ICMP packet with 4 bytes random payload
 */
static inline int send_random_icmp(uint32_t ip_addr, libnet_t *l)
{
	uint8_t payload[4];

	for (int i = 0; i < sizeof(payload); i++)
		payload[i] = libnet_get_prand(LIBNET_PR8);

	if (send_icmp(payload, sizeof(payload), ip_addr, 0, l))
		return -1;

	return 0;
}

static inline int send_flag_part(uint32_t ip_addr, libnet_t *l)
{
	/*
	 * send 1-6 random packets
	 * 1 flag packet (4 bytes)
	 * rinse and repeat
	 */
	size_t flag_pos = 0;
	size_t max_pos = sizeof(flag) / 4;
	size_t rest = sizeof(flag) % 4;

	while (flag_pos < max_pos * 4) {
		uint8_t num = libnet_get_prand(LIBNET_PR8) % 6;

		fprintf(stderr, "Sending %d random packets.\n", num);

		for (int i = 0; i < num; i++)
			send_random_icmp(ip_addr, l);

		char *ptr = flag + flag_pos;

		send_icmp((uint8_t *)ptr, 4, ip_addr, 1, l);

		flag_pos += 4;
	}

	char tmp[4] = { 0 };
	memcpy(tmp, flag + flag_pos, rest);
	send_icmp((uint8_t *)tmp, 4, ip_addr, 1, l);

	return 0;
}

libnet_t *init_libnet(void)
{
	char errbuf[LIBNET_ERRBUF_SIZE];

	libnet_t *l = libnet_init(LIBNET_RAW4, NULL, errbuf);
	if (!l) {
		fprintf(stderr, "libnet_init() failed: %s\n", errbuf);
		return NULL;
	}

	libnet_seed_prand(l);

	return l;
}

int main(void)
{
	libnet_t *l;
	char ip_addr_str[16];
	u_int32_t ip_addr;
	u_int16_t id, seq;
	const char payload[] = "fuck off";
	int written;

	l = init_libnet();
	if (!l)
		exit(EXIT_FAILURE);

	id = (u_int16_t)libnet_get_prand(LIBNET_PR16);

	/* destination IP address */
	printf("Destination IP: ");
	scanf("%15s", ip_addr_str);

	ip_addr = libnet_name2addr4(l, ip_addr_str, LIBNET_DONT_RESOLVE);
	if (ip_addr == -1) {
		fprintf(stderr, "Error converting IP.\n");
		goto fail;
	}

	send_flag_part(ip_addr, l);
	
	libnet_destroy(l);
	return 0;
fail:
	libnet_destroy(l);
	exit(EXIT_FAILURE);
}
