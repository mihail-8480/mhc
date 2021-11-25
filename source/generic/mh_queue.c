#include <generic/mh_queue.t.h>
#include <generic/mh_alloc.h>

Queue mh_queue_new(void) {
    return mh_calloc(1, sizeof(mh_queue_t));
}

void mh_queue_destroy(Queue queue) {
    mh_assert(queue != MH_NULL);
    mh_free(queue);
}

Any mh_queue_take(Queue queue) {
    mh_assert(queue != MH_NULL);
    mh_queue_node_t *node = queue->first;
    if (node == MH_NULL) {
        return MH_NULL;
    }
    queue->first = queue->first->next;
    Any data = node->data;
    return data;
}

void mh_queue(Queue queue, Any data, Any handle) {
    mh_assert(queue != MH_NULL);
    mh_assert(handle != MH_NULL);
    mh_queue_node_t *node = handle;
    node->data = data;
    if (queue->first == MH_NULL) {
        queue->first = node;
    } else {
        queue->current->next = node;
    }
    queue->current = node;
}
