#pragma once

#include <stdexcept>
#include <iostream>

// Node Class
// A fundamental building block for linked data structures.
template <typename T>
class Node
{
private:
    T data;        // The value stored in this node
    Node<T> *next; // Pointer to the next node in the sequence (or nullptr)

public:
    // Constructor initializes data and sets next to nullptr
    Node(const T &value) : data(value), next(nullptr) {}

    T getData() const { return data; }
    void setData(const T &value) { data = value; }

    Node<T> *getNext() const { return next; }
    void setNext(Node<T> *nextNode) { next = nextNode; }
};

// MyVector (Dynamic Array)
// A custom implementation of a dynamic array.
// Features:
// - Contiguous memory allocation for O(1) random access.
// - Automatic resizing when capacity is reached.
template <typename T>
class MyVector
{
private:
    T *data;           // Pointer to the dynamic array
    size_t m_size;     // Current number of elements
    size_t m_capacity; // Total available slots in memory

    // Resizes the underlying array when we run out of space.
    // Creates a new, larger array, copies elements, and deletes the old array.
    void resize(size_t newCapacity)
    {
        T *newData = new T[newCapacity];
        for (size_t i = 0; i < m_size; ++i)
        {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        m_capacity = newCapacity;
    }

public:
    MyVector() : m_size(0), m_capacity(2)
    {
        data = new T[m_capacity]; // Initial allocation
    }

    // Copy Constructor: Deep copy to prevent double-free issues
    MyVector(const MyVector &other) : m_size(other.m_size), m_capacity(other.m_capacity)
    {
        data = new T[m_capacity];
        for (size_t i = 0; i < m_size; ++i)
        {
            data[i] = other.data[i];
        }
    }

    // Assignment Operator: Deep copy with self-assignment check
    MyVector &operator=(const MyVector &other)
    {
        if (this != &other)
        {
            delete[] data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            data = new T[m_capacity];
            for (size_t i = 0; i < m_size; ++i)
            {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~MyVector()
    {
        delete[] data; // Clean up memory
    }

    // Adds an element to the end. Time Complexity: Amortized O(1).
    void push_back(const T &value)
    {
        if (m_size == m_capacity)
        {
            resize(m_capacity * 2); // Double capacity strategy
        }
        data[m_size++] = value;
    }

    // Removes the last element. Time Complexity: O(1).
    void pop_back()
    {
        if (m_size > 0)
        {
            m_size--;
        }
    }

    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    bool isEmpty() const { return m_size == 0; }

    // Array subscript operator for read/write access.
    T &operator[](size_t index)
    {
        return data[index];
    }

    const T &operator[](size_t index) const
    {
        return data[index];
    }

    // Safe access with bounds checking.
    T &at(size_t index)
    {
        if (index >= m_size)
        {
            throw std::out_of_range("MyVector index out of bounds");
        }
        return data[index];
    }

    void clear()
    {
        m_size = 0; // Logically clear without deallocating immediately
    }
};

// MyQueue (Circular Queue)
// Uses a circular buffer to efficiently manage space without shifting elements.
template <typename T>
class MyQueue
{
private:
    T *data;
    size_t frontIdx; // Index of the front element
    size_t rearIdx;  // Index where the next element will be inserted
    size_t m_size;
    size_t m_capacity;

    // Resizes the queue when full, realigning the circular data into a linear sequence.
    void resize()
    {
        size_t newCapacity = m_capacity * 2;
        T *newData = new T[newCapacity];

        for (size_t i = 0; i < m_size; ++i)
        {
            newData[i] = data[(frontIdx + i) % m_capacity];
        }

        delete[] data;
        data = newData;
        frontIdx = 0;
        rearIdx = m_size;
        m_capacity = newCapacity;
    }

public:
    MyQueue() : frontIdx(0), rearIdx(0), m_size(0), m_capacity(4)
    {
        data = new T[m_capacity];
    }

    ~MyQueue()
    {
        delete[] data;
    }

    // Enqueue: Add to rear. Time Complexity:O(1).
    void enqueue(const T &value)
    {
        if (m_size == m_capacity)
        {
            resize();
        }
        data[rearIdx] = value;
        rearIdx = (rearIdx + 1) % m_capacity; // Wrap around
        m_size++;
    }

    // Dequeue: Remove from front. Time Complexity: O(1).
    void dequeue()
    {
        if (isEmpty())
            return;
        frontIdx = (frontIdx + 1) % m_capacity;
        m_size--;
    }

    // Returns the front element.
    T &front()
    {
        if (isEmpty())
            throw std::underflow_error("Queue is empty");
        return data[frontIdx];
    }

    bool isEmpty() const { return m_size == 0; }
    size_t size() const { return m_size; }
};

// MyStack (Dynamic Array Stack)
// A Last-In-First-Out (LIFO) data structure.
// Implemented using composition with MyVector.
template <typename T>
class MyStack
{
private:
    MyVector<T> container; // Uses MyVector to handle storage

public:
    // Push onto top. Time Complexity: O(1).
    void push(const T &value)
    {
        container.push_back(value);
    }

    // Remove from top. Time Complexity: O(1).
    void pop()
    {
        container.pop_back();
    }

    // Access top element.
    T &top()
    {
        if (container.isEmpty())
            throw std::underflow_error("Stack is empty");
        return container[container.size() - 1];
    }

    bool isEmpty() const { return container.isEmpty(); }
    size_t size() const { return container.size(); }
};

// MyLinkedList (Singly Linked List)
// A dynamic linked list that provides flexible insertion and traversal.
// Unlike MyVector, this doesn't require contiguous memory and excels at insertions/deletions.
template <typename T>
class MyLinkedList
{
private:
    Node<T> *head; // Pointer to the first node
    Node<T> *tail; // Pointer to the last node (for efficient push_back)
    size_t m_size;

    // Helper function to clear all nodes
    void clearAll()
    {
        while (head != nullptr)
        {
            Node<T> *temp = head;
            head = head->getNext();
            delete temp;
        }
        tail = nullptr;
        m_size = 0;
    }

public:
    MyLinkedList() : head(nullptr), tail(nullptr), m_size(0) {}

    // Copy Constructor
    MyLinkedList(const MyLinkedList &other) : head(nullptr), tail(nullptr), m_size(0)
    {
        Node<T> *current = other.head;
        while (current != nullptr)
        {
            push_back(current->getData());
            current = current->getNext();
        }
    }

    // Assignment Operator
    MyLinkedList &operator=(const MyLinkedList &other)
    {
        if (this != &other)
        {
            clearAll();
            Node<T> *current = other.head;
            while (current != nullptr)
            {
                push_back(current->getData());
                current = current->getNext();
            }
        }
        return *this;
    }

    ~MyLinkedList()
    {
        clearAll();
    }

    // Add element to the front. Time Complexity: O(1)
    void push_front(const T &value)
    {
        Node<T> *newNode = new Node<T>(value);
        newNode->setNext(head);
        head = newNode;

        if (tail == nullptr)
        {
            tail = head; // First element
        }

        m_size++;
    }

    // Add element to the back. Time Complexity: O(1)
    void push_back(const T &value)
    {
        Node<T> *newNode = new Node<T>(value);

        if (tail == nullptr)
        {
            // Empty list
            head = tail = newNode;
        }
        else
        {
            tail->setNext(newNode);
            tail = newNode;
        }

        m_size++;
    }

    // Insert in sorted order (descending: highest to lowest). Time Complexity: O(n)
    // Assumes the list is already sorted in descending order.
    void insert_sorted_descending(const T &value)
    {
        Node<T> *newNode = new Node<T>(value);

        // Empty list or value is greater than head
        if (head == nullptr || value > head->getData())
        {
            newNode->setNext(head);
            head = newNode;
            if (tail == nullptr)
            {
                tail = head;
            }
            m_size++;
            return;
        }

        // Find position to insert
        Node<T> *current = head;
        while (current->getNext() != nullptr && current->getNext()->getData() > value)
        {
            current = current->getNext();
        }

        // Insert after current
        newNode->setNext(current->getNext());
        current->setNext(newNode);

        // Update tail if inserted at end
        if (newNode->getNext() == nullptr)
        {
            tail = newNode;
        }

        m_size++;
    }

    // Remove element at a specific index. Time Complexity: O(n)
    void removeAt(size_t index)
    {
        if (index >= m_size || head == nullptr)
        {
            return; // Out of bounds
        }

        if (index == 0)
        {
            // Remove head
            Node<T> *temp = head;
            head = head->getNext();
            delete temp;

            if (head == nullptr)
            {
                tail = nullptr; // List is now empty
            }

            m_size--;
            return;
        }

        // Find node before the one to remove
        Node<T> *current = head;
        for (size_t i = 0; i < index - 1; i++)
        {
            current = current->getNext();
        }

        Node<T> *toDelete = current->getNext();
        current->setNext(toDelete->getNext());

        if (toDelete == tail)
        {
            tail = current; // Update tail if we removed the last element
        }

        delete toDelete;
        m_size--;
    }

    // Get element at index. Time Complexity: O(n)
    T getAt(size_t index) const
    {
        if (index >= m_size)
        {
            throw std::out_of_range("MyLinkedList index out of bounds");
        }

        Node<T> *current = head;
        for (size_t i = 0; i < index; i++)
        {
            current = current->getNext();
        }

        return current->getData();
    }

    // Get head node for manual iteration
    Node<T> *getHead() const { return head; }

    // Get tail node
    Node<T> *getTail() const { return tail; }

    size_t size() const { return m_size; }
    bool isEmpty() const { return m_size == 0; }

    void clear()
    {
        clearAll();
    }
};
