#pragma once
#include "priNode.h"
#include <iostream>

//This class impelements the priority queue as a sorted list (Linked List)
//The item with highest priority is at the front of the queue
template <typename T>
class priQueue
{
protected:
    priNode<T>* head;
    int count;

public:
    priQueue() : head(nullptr), count(0) {}

    ~priQueue() {
        T tmp;
        int p;
        while (dequeue(tmp, p)); // Properly cleanup all nodes
    }

    // Copy constructor to prevent shallow copies
    priQueue(const priQueue& other) : head(nullptr), count(0) {
        priNode<T>* current = other.head;
        int pri;
        while (current) {
            T item = current->getItem(pri);
            enqueue(item, pri);
            current = current->getNext();
        }
    }

    // Assignment operator
    priQueue& operator=(const priQueue& other) {
        if (this != &other) {
            // Clear existing nodes
            T tmp;
            int p;
            while (dequeue(tmp, p));
            
            // Copy other's nodes
            priNode<T>* current = other.head;
            int pri;
            while (current) {
                T item = current->getItem(pri);
                enqueue(item, pri);
                current = current->getNext();
            }
        }
        return *this;
    }

    //insert the new node in its correct position according to its priority
    void enqueue(const T& data, int priority) {
        priNode<T>* newNode = new priNode<T>(data, priority);

        if (head == nullptr || priority > head->getPri()) {
            
            newNode->setNext(head);
            head = newNode;
            count++;  // Increment count
            return;
        }
       
        priNode<T>* current = head;        
        while (current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }
        newNode->setNext( current->getNext());
        current->setNext( newNode);        
        count++;  // Increment count
    }

    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);  // Fixed: passing pri parameter
        priNode<T>* temp = head;
        head = head->getNext();
        delete temp;
        count--;  // Decrement count
        return true;
    }

    bool peek(T& topEntry, int& pri) const {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);  // Fixed: passing pri parameter
        return true;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    int getCount() const { return count; }

    void print(std::ostream& out = std::cout) const {
        priNode<T>* current = head;
        int pri;
        while (current) {
            T item = current->getItem(pri);
            out << item;
            if (current->getNext()) out << ", ";
            current = current->getNext();
        }
    }
};

// Stream operator overload for priQueue
template <typename T>
std::ostream& operator<<(std::ostream& out, const priQueue<T>& queue) {
    queue.print(out);
    return out;
}
