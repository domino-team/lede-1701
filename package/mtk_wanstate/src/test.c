#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <inttypes.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <linux/types.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <linux/switch.h>
#include <swlib.h>

static int use_syslog;

#define TIME_FORMAT "%F %T"
#define USE_SYSLOG(ident)                          \
    do {                                           \
        use_syslog = 1;                            \
        openlog((ident), LOG_CONS | LOG_PID, 0); } \
    while (0)

#define LOGI(format, ...)                                                        \
    do {                                                                         \
        if (use_syslog) {                                                        \
            syslog(LOG_INFO, format, ## __VA_ARGS__);                            \
        } else {                                                                 \
            time_t now = time(NULL);                                             \
            char timestr[20];                                                    \
            strftime(timestr, 20, TIME_FORMAT, localtime(&now));                 \
                fprintf(stdout, "%s INFO: " format "\n", timestr,               \
                        ## __VA_ARGS__);                                         \
        }                                                                        \
    }                                                                            \
    while (0)

#define LOGE(format, ...)                                                         \
    do {                                                                          \
        if (use_syslog) {                                                         \
            syslog(LOG_ERR, format, ## __VA_ARGS__);                              \
        } else {                                                                  \
            time_t now = time(NULL);                                              \
            char timestr[20];                                                     \
            strftime(timestr, 20, TIME_FORMAT, localtime(&now));                  \
                fprintf(stderr, "%s ERROR: " format "\n", timestr,               \
                        ## __VA_ARGS__);                                          \
        } }                                                                       \
    while (0)

/**
 * @return: -1 = error, 1 = link up, 0 = link down
 */
int probe_port_status(struct switch_dev *dev, int port)
{
	int ret = 0;
	struct switch_val val;
	struct switch_attr *attr;
	struct switch_port_link *link;

	if (!dev || port < 0 || port > 6) {
		LOGE("Invalid port or dev specified");
		return -1;
	}

	/* get link attribute */
	attr = swlib_lookup_attr(dev, SWLIB_ATTR_GROUP_PORT, "link");
	if (!attr) {
		LOGE("Unknown attribute");
		return -1;
	}

	/* specific port */
	val.port_vlan = port;

	ret = swlib_get_attr(dev, attr, &val);
	if (ret < 0) {
		LOGE("Failed to get attribute");
		return -1;
	}

	if (attr->type == SWITCH_TYPE_LINK) {
		link = val.value.link;
		if(link->link) {
			return 1;
		} else {
			return 0;
		}
	}

	return ret;
}

void print_usage() 
{
	printf("Usage: wanstate [-d <switch_dev>] [-p <port>] [-h]\n\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	int c = 0;
	int ret = 0;
	int port = 0;
	int cur_status = 0;
	int port_status = 0;
	char *cdev = NULL;

	struct switch_dev *dev;

	while ((c = getopt(argc, argv, "d:p:h")) != -1) {
		switch (c) {
		case 'd':
			cdev = optarg;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		case 'h':
			print_usage();
			break;
		case '?':
			/* LOGE("Unrecognized option: %s", optarg); */
			print_usage();
			break;
		}
	}

	if (!cdev) {
		cdev = strdup("switch0");
	}

	/* port 0 is wan by default */
	port = port?port:0;

	/* enable syslog */
	USE_SYSLOG(argv[0]);

	dev = swlib_connect(cdev);
	if (!dev) {
		LOGE("Failed to connect to the switch\n");
		return ret;
	}

	swlib_scan(dev);

	/* run loop */
	while(1) {
		/* port 0 is wan port */
		cur_status = probe_port_status(dev, 0);
		if (cur_status < 0) {
			goto out;
		}

		if ((cur_status == 1) && (port_status == 0)) {
			/* port link status: down->up */
			port_status = 1;
			LOGI("wan link up");
		} else if ((cur_status == 0) && (port_status == 1)) {
			port_status = 0;
			LOGI("wan link down");
			/* renew dhcp */
			system("killall -16 udhcpc");
		} else {
			/* nothing to do */
		}
		sleep(2);
	}

out:
	swlib_free_all(dev);
	return ret;
}
