#include <iostream>
#include "fstream"
using namespace std;

// Create a vector class
class Vector
{
// initialization of the private attributes
private:
    int size , ind;
    int* arr;
// initialization of the public methods
public:
    // default constructor
    Vector()
    {
        size = 0 , ind = 0;
        arr = new int[size];
    }
    // parameterized constructor
    Vector(int siz , int init = 0)
    {
        size = siz;
        ind = 0;
        arr = new int[size];
        // fill the array with the given value
        Fill(init);
    }
    // push_back method to add an element to the end of the vector
    void Push_Back (int element)
    {
        // if the index is greater than or equal to the size of the array
        if (ind >= size)
        {
            // if the size is 0, create a new array of size 1
            if (!size)
            {
                size++;
                delete[] arr;
                arr = new int[size];
            }
            // if the size is not 0, double the size of the array
            else
            {
                size*=2;
                int* newArr = new int[size];
                for (int i = 0; i < size/2; ++i) {
                    newArr[i] = arr[i];
                }
                delete[] arr;
                arr = newArr;
            }
        }
        // add the element to the end of the array and increment the index
        arr[ind++] = element;
    }

    // fill method to fill the array with a given value
    void Fill(int val)
    {
        for (int i = 0; i < Capacity(); ++i)
        {
            Push_Back(0);
        }
    }

    // operator overloading to access the elements of the array using []
    int& operator [](int index)
    {
        return arr[index];
    }

    // size method to return the number of elements in the array
    int Size()
    {
        return ind;
    }

    // capacity method to return the size of the array
    int Capacity()
    {
        return size;
    }

    // destructor to delete the array
    ~Vector()
    {
        delete[] arr;
    }
};

int main() {
    // ask the user to enter the name of the file
    cout << "Enter the name of the file : " <<endl;
    string filename; cin >> filename;
    fstream file(filename);

    // check if the file is open
    if (file.is_open())
    {
        cout << "file opened successfully" <<endl;
    }
    else
    {
        cout << "file not opened" <<endl;
        return 0;
    }

    // read the number of test cases from the file
    int t; file >> t;
    while (t--)
    {
        // read the length of the vector from the file
        int len; file >> len;

        // create a vector
        Vector v;

        // read the elements of the vector from the file
        for (int i = 0; i < len; ++i) {
            int val; file >> val;
            v.Push_Back(val);
        }

        // read the value of k from the file
        int k; file >> k;

        // initialize the answer to 0
        int ans = 0;

//    O(n*k) solution
//    for (int i = 0; i < len; ++i) {
//        if (!v[i])
//        {
//            if (i+k > len)
//            {
//                cout << -1 <<endl;
//                return 0;
//            }
//            ans++;
//            for (int j = i; j < i+k; ++j) {
//                v[j] = 1-v[j];
//            }
//        }
//    }
//
//    cout << ans <<endl;



        // O(n) solution

        // initialize the last flip to 0
        int lstFlip = 0;
        // create a frequency vector of size len+1 and initialize it to 0
        Vector freq(len+1);

        for (int i = 0; i < len; ++i) {
            lstFlip+=freq[i];
            // if the last flip is odd, flip the current element
            if (lstFlip&1)
                v[i] = 1 - v[i];
            // if the current element is 0
            if (!v[i])
            {
                // if i+k > len, set ans to -1 and break (there is no solution)
                if (i+k > len)
                {
                    ans = -1;
                    break;
                }
                // flip the current element and the next k-1 elements, increment ans and lstFlip
                ans++ , lstFlip++ , freq[i+k]--;
            }
        }
        // print ans
        cout << ans <<endl;
    }

    return 0;
}

