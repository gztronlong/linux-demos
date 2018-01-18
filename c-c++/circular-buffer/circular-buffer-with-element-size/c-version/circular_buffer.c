
#include "circular_buffer.h"
#include <pthread.h>
#include <string.h>

struct circular_buffer *circular_buffer_init(size_t capacity) {
    struct circular_buffer *buf = malloc(sizeof(struct circular_buffer));

    buf->beg_index_ = 0;
    buf->end_index_ = 0;
    buf->size_ = 0;
    buf->capacity_ = capacity;

    buf->data_ = malloc(capacity);

    CIRCLEQ_INIT(&buf->size_queue_);

    pthread_rwlock_init(&buf->rw_lock_, NULL);

    return buf;
}

void circular_buffer_deinit(struct circular_buffer *buf) {
    pthread_rwlock_destroy(&buf->rw_lock_);
    free(buf->data_);
    while (buf->size_queue_.cqh_first != (void *)&buf->size_queue_)
        CIRCLEQ_REMOVE(&buf->size_queue_, buf->size_queue_.cqh_first, entries);
    buf = NULL;
}

size_t circular_buffer_size(struct circular_buffer *buf) {
    return buf->size_;
}

size_t circular_buffer_capacity(struct circular_buffer *buf) {
    return buf->capacity_;
}

void circular_buffer_clear(struct circular_buffer *buf) {
    pthread_rwlock_wrlock(&buf->rw_lock_);

    buf->beg_index_ = 0;
    buf->end_index_ = 0;
    buf->size_      = 0;

    while (buf->size_queue_.cqh_first != (void *)&buf->size_queue_)
        CIRCLEQ_REMOVE(&buf->size_queue_, buf->size_queue_.cqh_first, entries);

    pthread_rwlock_unlock(&buf->rw_lock_);
}

bool circular_buffer_push(struct circular_buffer *buf, const char *data, size_t bytes) {
    if (bytes <= 0)
        return true;

    pthread_rwlock_wrlock(&buf->rw_lock_);

    if (bytes > buf->capacity_ - buf->size_)
        return false;

    // Write in a single step
    if (bytes <= buf->capacity_ - buf->end_index_) {
        memcpy(buf->data_ + buf->end_index_, data, bytes);
        buf->end_index_ += bytes;
        if (buf->end_index_ == buf->capacity_)
            buf->end_index_ = 0;
    }
    // Write in two steps
    else {
        size_t size_1 = buf->capacity_ - buf->end_index_;
        memcpy(buf->data_ + buf->end_index_, data, size_1);
        size_t size_2 = bytes - size_1;
        memcpy(buf->data_, data + size_1, size_2);
        buf->end_index_ = size_2;
    }

    buf->size_ += bytes;

    struct entry *new_frame_size = malloc(sizeof(struct entry));
    new_frame_size->frame_size = bytes;
    CIRCLEQ_INSERT_TAIL(&buf->size_queue_, new_frame_size, entries);

    pthread_rwlock_unlock(&buf->rw_lock_);

    return true;
}

bool circular_buffer_pop(struct circular_buffer *buf, char *data, size_t *bytes) {
    int bytes_to_read;
    pthread_rwlock_rdlock(&buf->rw_lock_);

    if (buf->size_ <= 0) {
        *bytes = 0;
        return false;
    }

    struct entry *head_frame_size = buf->size_queue_.cqh_first;

    bytes_to_read = head_frame_size->frame_size;

    // Read in a single step
    if (bytes_to_read <= buf->capacity_ - buf->beg_index_) {
        memcpy(data, buf->data_ + buf->beg_index_, bytes_to_read);
        buf->beg_index_ += bytes_to_read;
        if (buf->beg_index_ == buf->capacity_)
            buf->beg_index_ = 0;
    }
    // Read in two steps
    else {
        size_t size_1 = buf->capacity_ - buf->beg_index_;
        memcpy(data, buf->data_ + buf->beg_index_, size_1);
        size_t size_2 = bytes_to_read - size_1;
        memcpy(data + size_1, buf->data_, size_2);
        buf->beg_index_ = size_2;
    }

    buf->size_ -= bytes_to_read;
    *bytes = bytes_to_read;

    CIRCLEQ_REMOVE(&buf->size_queue_, buf->size_queue_.cqh_first, entries);

    pthread_rwlock_unlock(&buf->rw_lock_);

    return true;
}
