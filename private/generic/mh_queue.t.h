#ifndef CASYNC_MH_QUEUE_T_H
#define CASYNC_MH_QUEUE_T_H
#include <mh_queue.h>
typedef _Atomic(struct mh_queue_node *) QueueNode;

struct mh_queue {
    QueueNode first;
    QueueNode current;
};

typedef struct mh_queue_node {
    QueueNode next;
    Any data;
} mh_queue_node_t;

#endif //CASYNC_MH_QUEUE_T_H
