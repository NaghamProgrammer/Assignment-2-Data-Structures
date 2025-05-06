#include <bits\stdc++.h>
using namespace std;

template <class T>
class MyVector {
private:
    int sz;
    int capacity;
    T *data;

public:
    MyVector() : sz(0) , capacity(1)
    {
        data = new T[capacity];
    }

    MyVector(int n) : sz(n) , capacity(n)
    {
        data = new T[capacity];
    }

    bool empty() { return !sz; }

    int size() { return sz; }

    void push_back(T val)
    {
        if(sz == capacity)
        {
            capacity <<= 1;
            T *newData = new T[capacity];
            for (int i = 0; i < sz; ++i)
                newData[i] = data[i];
            delete [] data;
            data = newData;
        }
        data[sz++] = val;
    }

    void pop_back()
    {
        if(empty())
            throw out_of_range("Vector is empty");
        sz--;
    }

    T& operator[](int index)
    {
        return data[index];
    }

    T& front()
    {
        if(empty())
            throw out_of_range("Vector is empty");
        return data[0];
    }

    T& back()
    {
        if(empty())
            throw out_of_range("Vector is empty");
        return data[sz-1];
    }
};


struct Patient {
    string name;
    int severity;
    int arrivalTime;

    bool operator>(const Patient &other) const
    {
        // If equal severity, Make earlier one first
        if(severity == other.severity)
            return arrivalTime > other.arrivalTime;
        // Make high severity first
        return severity < other.severity;
    }
};

template <class T>
class MaxHeap {
protected:
    MyVector <Patient> heap;
    int left(int i) { return 2*i + 1; }
    int right(int i) { return 2*i + 2; }
    int parent(int i) { return (i-1) / 2; }
    bool compare(T a, T b) { return a > b; }

    void buildHeap() // O(nlog(n))
    {
        for (int i = heap.size()/2 - 1; i >= 0; --i)
            heapify(heap, i, heap.size());
    }

    void heapify(MyVector <T> & heap, int i, int n)
    {
        int l = left(i);
        int r = right(i);
        int max = i;
        if(l < n && compare(heap[max],heap[l]))
            max = l;
        if(r < n && compare(heap[max],heap[r]))
            max = r;
        if(max != i)
        {
            swap(heap[i], heap[max]);
            heapify(heap, max, n);
        }
    }

public:
    MaxHeap() {}

    bool isEmpty() { return heap.empty(); }

    int size() { return heap.size(); }

    void print_heap()
    {
        cout << "Heap: [";
        for (int i = 0; i < heap.size(); ++i)
            cout << '\'' << heap[i].name << '\'' <<  ((i != heap.size()-1) ? ", " : "");
        cout << "]\n";
    }

    T peek()
    {
        if(heap.empty())
            throw out_of_range("Heap is empty");
        return heap[0];
    }

    void deleteTop()
    {
        if(heap.empty())
            throw out_of_range("Heap is empty");
        swap(heap.front(), heap.back());
        heap.pop_back();
        heapify(heap, 0, heap.size());
    }

    void insert(T val)
    {
        int i = heap.size();
        heap.push_back(val);
        while(i > 0 && compare(heap[parent(i)], heap[i]))
        {
            swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }

    T extract_max()
    {
        if(heap.empty())
            throw out_of_range("Heap is empty");
        T top = peek();
        deleteTop();
        return top;
    }
};


signed main()
{
    MaxHeap <Patient> heap;
    cout << "Heap After Insertions:\n";
    Patient p = {"Alice", 80, 1};
    heap.insert(p);
    cout << "Inserting: " << p.name << '\n';
    heap.print_heap();

    p = {"Bob", 90, 2};
    heap.insert(p);
    cout << "Inserting: " << p.name << '\n';
    heap.print_heap();

    p = {"Charlie", 70, 3};
    heap.insert(p);
    cout << "Inserting: " << p.name << '\n';
    heap.print_heap();

    p = {"David", 85, 4};
    heap.insert(p);
    cout << "Inserting: " << p.name << '\n';
    heap.print_heap();

    p = {"Eve", 90, 5};
    heap.insert(p);
    cout << "Inserting: " << p.name << '\n';
    heap.print_heap();

    p = {"Frank", 75, 6};
    heap.insert(p);
    cout << "Inserting: " << p.name << '\n';
    heap.print_heap();

    p = {"Grace", 95, 7};
    heap.insert(p);
    cout << "Inserting: " << p.name << '\n';
    heap.print_heap();

    p = {"Henry", 80, 8};
    heap.insert(p);
    cout << "Inserting: " << p.name << '\n';
    heap.print_heap();

    p = {"Ivy", 70, 9};
    heap.insert(p);
    cout << "Inserting: " << p.name << '\n';
    heap.print_heap();

    p = {"Jack", 100, 10};
    heap.insert(p);
    cout << "Inserting: " << p.name << '\n';
    heap.print_heap();

    cout << "\nTreatment Order:\n";
    int heapSize = heap.size();
    for (int i = 0; i < heapSize; ++i)
    {
        Patient p = heap.extract_max();
        cout << "Treating: " << p.name << '\n';
    }
}

