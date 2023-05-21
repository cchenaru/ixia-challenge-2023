#include <linux/skbuff.h>
#include <net/pkt_sched.h>

static int ixia_qdisc_enqueue(struct sk_buff *skb, struct Qdisc *sch,
		struct sk_buff **to_free)
{
    qdisc_enqueue(skb, sch, to_free);

    return NET_XMIT_SUCCESS;
}

static struct sk_buff *ixia_qdisc_dequeue(struct Qdisc *sch)
{
    struct sk_buff *skb;

    skb = qdisc_dequeue_head(sch);
    if (!skb)
        return NULL;

    return skb;
}

static int ixia_qdisc_init(struct Qdisc *sch, struct nlattr *opt,
		  struct netlink_ext_ack *extack)
{
    sch->limit = 1024;

    sch->flags &= ~TCQ_F_CAN_BYPASS;

    return 0;
}

static struct Qdisc_ops ixia_qdisc_ops __read_mostly = {
    .id          = "ixia_qdisc",
    .priv_size   = 0,
    .enqueue     = ixia_qdisc_enqueue,
    .dequeue     = ixia_qdisc_dequeue,
    .peek        = qdisc_peek_head,
    .init        = ixia_qdisc_init,
    .owner       = THIS_MODULE,
};

static int __init ixia_qdisc_mod_init(void)
{
    return register_qdisc(&ixia_qdisc_ops);
}

static void __exit ixia_qdisc_mod_exit(void)
{
    unregister_qdisc(&ixia_qdisc_ops);
}

module_init(ixia_qdisc_mod_init);
module_exit(ixia_qdisc_mod_exit);

MODULE_LICENSE("GPL");
