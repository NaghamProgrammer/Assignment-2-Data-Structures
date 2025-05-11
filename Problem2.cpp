#include "iostream"
using namespace std;

struct Contact {
    string name;
    string phone;
    string email;
};

class Node {
private:
    int key;
    int height;
    Contact Value;
    Node* left;
    Node* right;
public:
    Node(const int &id,const Contact &value) {
        int height=1;
        this->key = id;
        this->Value = value;
        this->left = nullptr;
        this->right = nullptr;
    }
    int getKey() const {
        return key;
    }

    void setKey(int key) {
        this->key = key;
    }

    Contact getValue() const {
        return Value;
    }

    void setValue(const Contact &value) {
        Value = value;
    }

    Node *getLeft() const {
        return left;
    }

    void setLeft(Node *left) {
        this->left = left;
    }

    Node *getRight() const {
        return right;
    }

    void setRight(Node *right) {
        this->right = right;
    }
    int getHeight() const {
        return height;
    }
    void setHeight(const int & height) {
        this->height = height;
    };
};

int main() {
}