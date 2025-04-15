/*
 * MutablePriorityQueue.h
 * A simple implementation of mutable priority queues, required by Dijkstra algorithm.
 *
 * Created on: 17/03/2018
 *      Author: João Pascoal Faria
 */

#ifndef DA_TP_CLASSES_MUTABLEPRIORITYQUEUE
#define DA_TP_CLASSES_MUTABLEPRIORITYQUEUE

#include <vector>

/**
 * class T must have: (i) accessible field int queueIndex; (ii) operator< defined.
 */

template <class T>
class MutablePriorityQueue {
    std::vector<T *> H;
    void heapifyUp(unsigned i);
    void heapifyDown(unsigned i);
    inline void set(unsigned i, T * x);
public:
    MutablePriorityQueue();
    void insert(T * x);
    T * extractMin();
    void decreaseKey(T * x);
    bool empty();
};

// Index calculations
#define parent(i) ((i) / 2) ///< get the parent's index from the child's index
#define leftChild(i) ((i) * 2) ///< get the left child's index from the parent's index

/**
 * @brief Mutable Priority Queue default constructor
 */
template <class T>
MutablePriorityQueue<T>::MutablePriorityQueue() {
    H.push_back(nullptr);
    // indices will be used starting in 1
    // to facilitate parent/child calculations
}

/**
 * @brief Checks it the queue is empty
 *
 * @return true if the queue is empty, false otherwise
 */
template <class T>
bool MutablePriorityQueue<T>::empty() {
    return H.size() == 1;
}

/**
 * @brief Extracts the item with the minimum key
 *
 * @return pointer to the item extracted
 */
template <class T>
T* MutablePriorityQueue<T>::extractMin() {
    auto x = H[1];
    H[1] = H.back();
    H.pop_back();
    if(H.size() > 1) heapifyDown(1);
    x->queueIndex = 0;
    return x;
}

/**
 * @brief Inserts a new item at the end of the queue and calls heapifyUp()
 *
 * @param x - item to be inserted
 */
template <class T>
void MutablePriorityQueue<T>::insert(T *x) {
    H.push_back(x);
    heapifyUp(H.size()-1);
}

/**
 * @brief Decreases the key of the item x using heapifyUp()
 *
 * @param x - item to have the key decreased
 */
template <class T>
void MutablePriorityQueue<T>::decreaseKey(T *x) {
    heapifyUp(x->queueIndex);
}

/**
 * @brief Pushes the item with index i up the tree
 *
 * @param i - index of the item to be pushed up
 */
template <class T>
void MutablePriorityQueue<T>::heapifyUp(unsigned i) {
    auto x = H[i];
    while (i > 1 && *x < *H[parent(i)]) {
        set(i, H[parent(i)]);
        i = parent(i);
    }
    set(i, x);
}

/**
 * @brief Pushes the item with index i down the tree
 *
 * @param i - index of the item to be pushed down
 */
template <class T>
void MutablePriorityQueue<T>::heapifyDown(unsigned i) {
    auto x = H[i];
    while (true) {
        unsigned k = leftChild(i);
        if (k >= H.size())
            break;
        if (k+1 < H.size() && *H[k+1] < *H[k])
            ++k; // right child of i
        if ( ! (*H[k] < *x) )
            break;
        set(i, H[k]);
        i = k;
    }
    set(i, x);
}

/**
 * @brief Changes the value at index i to x
 *
 * @param i - index where the item will be changed
 * @param x - item to be inserted
 */
template <class T>
void MutablePriorityQueue<T>::set(unsigned i, T * x) {
    H[i] = x;
    x->queueIndex = i;
}

#endif /* DA_TP_CLASSES_MUTABLEPRIORITYQUEUE */