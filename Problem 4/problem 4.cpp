#include <bits/stdc++.h>
using namespace std;

// Template class for a dynamic array similar to std::vector
template <class T>
class MyVector
{
private:
    int sz;       // current size of vector
    int capacity; // total capacity of vector
    T *data;      // pointer to dynamic array

public:
    // Default constructor initializes with capacity 1
    MyVector() : sz(0), capacity(1)
    {
        data = new T[capacity];
    }

    // Constructor with initial size
    MyVector(int n) : sz(n), capacity(n)
    {
        data = new T[capacity];
    }

    // Check if vector is empty
    bool empty() { return !sz; }

    // Return current size
    int size() { return sz; }

    // Add new element to the end
    void push_back(T val)
    {
        if (sz == capacity)
        {
            capacity <<= 1; // Double the capacity
            T *newData = new T[capacity];
            for (int i = 0; i < sz; ++i)
                newData[i] = data[i];
            delete[] data;
            data = newData;
        }
        data[sz++] = val;
    }

    // Remove last element
    void pop_back()
    {
        if (empty())
            throw out_of_range("Vector is empty");
        sz--;
    }

    // Access element at index
    T &operator[](int index)
    {
        return data[index];
    }

    // Access first element
    T &front()
    {
        if (empty())
            throw out_of_range("Vector is empty");
        return data[0];
    }

    // Access last element
    T &back()
    {
        if (empty())
            throw out_of_range("Vector is empty");
        return data[sz - 1];
    }

    // Destructor to free memory
    ~MyVector()
    {
        delete[] data;
    }
};

// Struct to store patient info
struct Patient
{
    string name;
    int severity;
    int arrivalTime;

    // Custom comparator for max-heap
    bool operator>(const Patient &other) const
    {
        // If severity is same, earlier arrival gets priority
        if (severity == other.severity)
            return arrivalTime > other.arrivalTime;
        // Higher severity has higher priority
        return severity < other.severity;
    }
};

// Template class for MaxHeap
template <class T>
class MaxHeap
{
protected:
    MyVector<Patient> heap;

    // Utility functions for heap indexing
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }
    int parent(int i) { return (i - 1) / 2; }

    // Use '>' operator to maintain max-heap property
    bool compare(T a, T b) { return a > b; }

    // Build heap from array - O(n log n)
    void buildHeap()
    {
        for (int i = heap.size() / 2 - 1; i >= 0; --i)
            heapify(heap, i, heap.size());
    }

    // Ensure heap property from index i downwards
    void heapify(MyVector<T> &heap, int i, int n)
    {
        int l = left(i);
        int r = right(i);
        int max = i;

        // Compare with left and right children
        if (l < n && compare(heap[max], heap[l]))
            max = l;
        if (r < n && compare(heap[max], heap[r]))
            max = r;

        // Swap and recurse if needed
        if (max != i)
        {
            swap(heap[i], heap[max]);
            heapify(heap, max, n);
        }
    }

public:
    MaxHeap() {}

    // Check if heap is empty
    bool isEmpty() { return heap.empty(); }

    // Get size of heap
    int size() { return heap.size(); }

    // Print heap contents (names only)
    void print_heap()
    {
        cout << "Heap: [";
        for (int i = 0; i < heap.size(); ++i)
            cout << '\'' << heap[i].name << '\'' << ((i != heap.size() - 1) ? ", " : "");
        cout << "]\n";
    }

    // Get top element (highest priority)
    T peek()
    {
        if (heap.empty())
            throw out_of_range("Heap is empty");
        return heap[0];
    }

    // Delete top element and restore heap
    void deleteTop()
    {
        if (heap.empty())
            throw out_of_range("Heap is empty");
        swap(heap.front(), heap.back());
        heap.pop_back();
        heapify(heap, 0, heap.size());
    }

    // Insert new element and maintain heap
    void insert(T val)
    {
        int i = heap.size();
        heap.push_back(val);
        // Bubble up the new element
        while (i > 0 && compare(heap[parent(i)], heap[i]))
        {
            swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }

    // Remove and return top element
    T extract_max()
    {
        if (heap.empty())
            throw out_of_range("Heap is empty");
        T top = peek();
        deleteTop();
        return top;
    }
};

// Main function
signed main()
{
    cout << "----- Welcome to Emergency Room Priority -----\n";

    MaxHeap<Patient> heap;
    Patient p;
    string filename;

    cout << "Enter test cases file name (without extensions):\n";
    cin >> filename;

    // Read patients from file
    ifstream file(filename + ".txt");
    while (!file.eof())
    {
        file >> p.name >> p.severity >> p.arrivalTime;
        cout << "Heap After Insertions:\n";
        heap.insert(p);
        cout << "Inserting: " << p.name << '\n';
        heap.print_heap();
    }

    // Extract and display treatment order
    cout << "\nTreatment Order:\n";
    int heapSize = heap.size();
    for (int i = 0; i < heapSize; ++i)
    {
        Patient p = heap.extract_max();
        cout << "Treating: " << p.name << '\n';
    }
}
