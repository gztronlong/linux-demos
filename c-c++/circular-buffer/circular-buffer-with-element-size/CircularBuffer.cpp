
#include "CircularBuffer.h"
#include <string.h>
#include <algorithm> // for std::min

using namespace std;

CircularBuffer::CircularBuffer(size_t capacity)
    : beg_index_(0),
    end_index_(0),
    size_(0),
    capacity_(capacity),
    element_size_queue() {
    data_ = new char[capacity];
    pthread_rwlock_init(&rw_lock_, NULL);
}

CircularBuffer::~CircularBuffer() {
    delete [] data_;
    pthread_rwlock_destroy(&rw_lock_);
}

size_t CircularBuffer::size() const {
    return size_;
}

size_t CircularBuffer::capacity() const {
    return capacity_;
}

void CircularBuffer::clear() {
    pthread_rwlock_wrlock(&rw_lock_);

    beg_index_ = 0;
    end_index_ = 0;
    size_      = 0;

    // clear the queue.
    queue<size_t> empty;
    swap(element_size_queue, empty);

    pthread_rwlock_unlock(&rw_lock_);
}

bool CircularBuffer::push(const char *data, size_t bytes) {
    if (bytes == 0)
        return true;

    pthread_rwlock_wrlock(&rw_lock_);

    if (bytes > capacity_ - size_)
        return false;

    // Write in a single step
    if (bytes <= capacity_ - end_index_) {
        memcpy(data_ + end_index_, data, bytes);
        end_index_ += bytes;
        if (end_index_ == capacity_)
            end_index_ = 0;
    }
    // Write in two steps
    else {
        size_t size_1 = capacity_ - end_index_;
        memcpy(data_ + end_index_, data, size_1);
        size_t size_2 = bytes - size_1;
        memcpy(data_, data + size_1, size_2);
        end_index_ = size_2;
    }

    size_ += bytes;
    element_size_queue.push(bytes);

    pthread_rwlock_unlock(&rw_lock_);

    return true;
}

#include <iostream>
bool CircularBuffer::pop(char *data, size_t &bytes) {
    pthread_rwlock_rdlock(&rw_lock_);

    if (element_size_queue.empty()) {
        bytes = 0;
        return false;
    }

    bytes = element_size_queue.front();

    // Read in a single step
    if (bytes <= capacity_ - beg_index_) {
        memcpy(data, data_ + beg_index_, bytes);
        beg_index_ += bytes;
        if (beg_index_ == capacity_)
            beg_index_ = 0;
    }
    // Read in two steps
    else {
        size_t size_1 = capacity_ - beg_index_;
        memcpy(data, data_ + beg_index_, size_1);
        size_t size_2 = bytes - size_1;
        memcpy(data + size_1, data_, size_2);
        beg_index_ = size_2;
    }

    size_ -= bytes;
    element_size_queue.pop();

    pthread_rwlock_unlock(&rw_lock_);

    return true;
}
