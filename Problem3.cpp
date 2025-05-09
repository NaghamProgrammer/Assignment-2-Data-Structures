#include <iostream>
using namespace std;

class Vector
{
private:
    int size , ind;
    int* arr;
public:
    Vector()
    {
        size = 0 , ind = 0;
        arr = new int[size];
    }
    Vector(int siz , int init = 0)
    {
        size = siz;
        ind = 0;
        arr = new int[size];
        Fill(init);
    }
    void Push_Back (int element)
    {
        if (ind >= size)
        {
            if (!size)
            {
                size++;
                delete[] arr;
                arr = new int[size];
            }
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
        arr[ind++] = element;
    }

    void Fill(int val)
    {
        for (int i = 0; i < Capacity(); ++i)
        {
            Push_Back(0);
        }
    }

    int& operator [](int index)
    {
        return arr[index];
    }

    int Size()
    {
        return ind;
    }
    int Capacity()
    {
        return size;
    }
    ~Vector()
    {
        delete[] arr;
    }
};

int main() {
    cout << "Enter the length of the array :" <<endl;
    int len; cin >> len;

    Vector v;
    for (int i = 0; i < len; ++i) {
        cout << "Enter the #" << i+1 << " element :" <<endl;
        int val; cin >> val;
        v.Push_Back(val);
    }

    cout << "Enter k :" <<endl;
    int k; cin >> k;

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



//    O(n) solution
    int lstFlip = 0;
    Vector freq(len+1);

    for (int i = 0; i < len; ++i) {
        lstFlip+=freq[i];
        if (lstFlip&1)
            v[i] = 1 - v[i];
        if (!v[i])
        {
            if (i+k > len)
            {
                cout << -1 <<endl;
                return 0;
            }
            ans++ , lstFlip++ , freq[i+k]--;
        }

    }

    cout << ans <<endl;
    return 0;
}

