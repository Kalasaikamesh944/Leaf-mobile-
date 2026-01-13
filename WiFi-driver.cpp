#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

/* =========================
   DRIVER INFO
   ========================= */
#define DRV_NAME "kala_wifi"

/* =========================
   DEVICE STRUCTURE
   ========================= */
struct kala_wifi_priv {
    struct net_device *netdev;
};

/* =========================
   OPEN INTERFACE
   ========================= */
static int kala_wifi_open(struct net_device *dev) {
    netif_start_queue(dev);
    printk(KERN_INFO "[KalaWiFi] Interface UP\n");
    return 0;
}

/* =========================
   STOP INTERFACE
   ========================= */
static int kala_wifi_stop(struct net_device *dev) {
    netif_stop_queue(dev);
    printk(KERN_INFO "[KalaWiFi] Interface DOWN\n");
    return 0;
}

/* =========================
   PACKET TRANSMIT
   ========================= */
static netdev_tx_t kala_wifi_xmit(struct sk_buff *skb,
                                  struct net_device *dev) {
    printk(KERN_INFO "[KalaWiFi] Packet sent: %u bytes\n", skb->len);
    dev_kfree_skb(skb);
    return NETDEV_TX_OK;
}

/* =========================
   NET DEVICE OPS
   ========================= */
static const struct net_device_ops kala_wifi_ops = {
    .ndo_open       = kala_wifi_open,
    .ndo_stop       = kala_wifi_stop,
    .ndo_start_xmit = kala_wifi_xmit,
};

/* =========================
   SETUP DEVICE
   ========================= */
static void kala_wifi_setup(struct net_device *dev) {
    ether_setup(dev);
    dev->netdev_ops = &kala_wifi_ops;
    dev->flags |= IFF_NOARP;
    dev->features |= NETIF_F_HW_CSUM;
}

/* =========================
   MODULE INIT
   ========================= */
static struct net_device *kala_dev;

static int __init kala_wifi_init(void) {
    kala_dev = alloc_netdev(sizeof(struct kala_wifi_priv),
                            "kwlan%d",
                            NET_NAME_UNKNOWN,
                            kala_wifi_setup);

    if (register_netdev(kala_dev)) {
        printk(KERN_ERR "[KalaWiFi] Registration failed\n");
        return -1;
    }

    printk(KERN_INFO "[KalaWiFi] Driver loaded\n");
    return 0;
}

/* =========================
   MODULE EXIT
   ========================= */
static void __exit kala_wifi_exit(void) {
    unregister_netdev(kala_dev);
    free_netdev(kala_dev);
    printk(KERN_INFO "[KalaWiFi] Driver unloaded\n");
}

module_init(kala_wifi_init);
module_exit(kala_wifi_exit);

/* =========================
   MODULE META
   ========================= */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dr. Sai Kamesh Yadavalli");
MODULE_DESCRIPTION("Kala Linux WiFi Driver Skeleton");
