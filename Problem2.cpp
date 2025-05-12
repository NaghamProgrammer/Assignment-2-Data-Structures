#include <utility>

#include "iostream"
#include "regex"
#include "fstream"
using namespace std;

/// @brief Represents a contact with a name, phone number, and email address.
struct Contact {
    string name;
    string phone;
    string email;
};

/// @brief Represents a node in an AVL tree that stores a contact as its value.
class Node {
private:
    int key;
    int height;
    Contact Value;
    Node* left;
    Node* right;

public:
    /// @brief Constructor to initialize a node with a key and contact value.
    /// @param id The unique key for the node.
    /// @param value The contact to store in this node.
    Node(const int& id, Contact  value)
        : key(id), height(1), Value(std::move(value)), left(nullptr), right(nullptr) {}

    /// @brief Gets the node's key.
    int getKey() const {
        return key;
    }

    /// @brief Sets the node's key.
    void setKey(int key) {
        this->key = key;
    }

    /// @brief Gets the contact value stored in the node.
    Contact getValue() const {
        return Value;
    }

    /// @brief Sets the contact value stored in the node.
    void setValue(const Contact& value) {
        Value = value;
    }

    /// @brief Gets the left child pointer.
    Node* getLeft() const {
        return left;
    }

    /// @brief Sets the left child pointer.
    void setLeft(Node* left) {
        this->left = left;
    }

    /// @brief Gets the right child pointer.
    Node* getRight() const {
        return right;
    }

    /// @brief Sets the right child pointer.
    void setRight(Node* right) {
        this->right = right;
    }

    /// @brief Gets the height of the node.
    int getHeight() const {
        return height;
    }

    /// @brief Sets the height of the node.
    void setHeight(const int& height) {
        this->height = height;
    }

    ~Node() = default;
};


class AVLTree {
private:
    Node* root;

    int height(Node* node) {
        if (node == nullptr) return 0;
        return node->getHeight();
    }

    int balanceFactor(Node* node) {
        if (node == nullptr) return 0;
        return height(node->getLeft()) - height(node->getRight());
    }

    void updateHeight(Node* node) {
        if (node == nullptr) return;
        node->setHeight(1 + max(height(node->getLeft()), height(node->getRight())));
    }

    Node* rightRotate(Node* node) {
        Node* x = node->getLeft();
        Node* y = x->getRight();

        x->setRight(node);
        node->setLeft(y);

        updateHeight(node);
        updateHeight(x);

        return x;
    }

    Node* leftRotate(Node* node) {
        Node* y = node->getRight();
        Node* x = y->getLeft();

        y->setLeft(node);
        node->setRight(x);

        updateHeight(node);
        updateHeight(y);

        return y;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->getLeft() != nullptr)
            current = current->getLeft();
        return current;
    }


    void deleteTree(Node* node) {
        if (node == nullptr) return;
        deleteTree(node->getLeft());
        deleteTree(node->getRight());
        delete node;
    }

public:
    AVLTree() {
        root = nullptr;
    }

    Node* insertNode(Node* parent, Node* NewNode) {
        if (parent == nullptr)
            return NewNode;

        if (NewNode->getKey() < parent->getKey())
            parent->setLeft(insertNode(parent->getLeft(), NewNode));
        else if (NewNode->getKey() > parent->getKey())
            parent->setRight(insertNode(parent->getRight(), NewNode));
        else
            throw runtime_error("Duplicate key");

        updateHeight(parent);
        int balance = balanceFactor(parent);

        if (balance > 1) {
            if (NewNode->getKey() < parent->getLeft()->getKey())
                return rightRotate(parent);
            parent->setLeft(leftRotate(parent->getLeft()));
            return rightRotate(parent);
        }

        if (balance < -1) {
            if (NewNode->getKey() > parent->getRight()->getKey())
                return leftRotate(parent);
            parent->setRight(rightRotate(parent->getRight()));
            return leftRotate(parent);
        }

        return parent;
    }

    Node* deleteNode(const int &id, Node* node) {
        if (node == nullptr)
            throw runtime_error("Empty Tree");

        if (node->getKey() > id)
            node->setLeft(deleteNode(id, node->getLeft()));
        else if (node->getKey() < id)
            node->setRight(deleteNode(id, node->getRight()));
        else {
            if (node->getLeft() == nullptr || node->getRight() == nullptr) {
                Node* temp = node->getLeft() ? node->getLeft() : node->getRight();
                if (temp == nullptr) {
                    delete node;
                    return nullptr;
                } else {
                    *node = *temp;
                    delete temp;
                }
            } else {
                Node* temp = minValueNode(node->getRight());
                node->setKey(temp->getKey());
                node->setValue(temp->getValue());
                node->setRight(deleteNode(temp->getKey(), node->getRight()));
            }
        }

        updateHeight(node);
        int balance = balanceFactor(node);

        if (balance > 1 && balanceFactor(node->getLeft()) >= 0)
            return rightRotate(node);
        if (balance > 1 && balanceFactor(node->getLeft()) < 0) {
            node->setLeft(leftRotate(node->getLeft()));
            return rightRotate(node);
        }
        if (balance < -1 && balanceFactor(node->getRight()) <= 0)
            return leftRotate(node);
        if (balance < -1 && balanceFactor(node->getRight()) > 0) {
            node->setRight(rightRotate(node->getRight()));
            return leftRotate(node);
        }

        return node;
    }

    Node* searchNode(const int &id, Node* node) {
        if (node == nullptr)
            throw runtime_error("Empty Tree");

        if (node->getKey() > id)
            return searchNode(id, node->getLeft());
        if (node->getKey() < id)
            return searchNode(id, node->getRight());
        if (node->getKey() == id)
            return node;
        throw runtime_error("No such node");
    }

    void ListIDs(Node* node) {
        if (root == nullptr)
            throw runtime_error("Address Book is empty");
        if (node == nullptr)
            return;

        ListIDs(node->getLeft());
        Contact contact = node->getValue();

        cout << "ID:" << node->getKey()
             << ", Name:" << contact.name
             << ", Phone:" << contact.phone
             << ", Email:" << contact.email << endl;

        ListIDs(node->getRight());
    }

    void displayTree(Node* root) {
        if (root == nullptr) {
            std::cout << "Tree is empty." << std::endl;
            return;
        }

        // Constants for the display
        const int MAX_HEIGHT = 10;  // Maximum tree height to display
        const int MAX_NODES = 1023; // 2^MAX_HEIGHT - 1 (max nodes possible)

        // Arrays to store tree data
        Node* nodes[MAX_NODES];
        int levels[MAX_NODES];
        bool isRight[MAX_NODES];

        // Initialize arrays
        for (int i = 0; i < MAX_NODES; i++) {
            nodes[i] = nullptr;
            levels[i] = 0;
            isRight[i] = false;
        }

        // Root is the first node to process
        int count = 0;
        nodes[count] = root;
        levels[count] = 0;
        isRight[count] = false;
        count++;

        // Breadth-first traversal to fill the arrays
        int index = 0;
        while (index < count) {
            Node* current = nodes[index];
            int level = levels[index];

            // Add left child if it exists
            if (current->getLeft() != nullptr) {
                nodes[count] = current->getLeft();
                levels[count] = level + 1;
                isRight[count] = false;
                count++;
            }

            // Add right child if it exists
            if (current->getRight() != nullptr) {
                nodes[count] = current->getRight();
                levels[count] = level + 1;
                isRight[count] = true;
                count++;
            }

            index++;
        }

        // Find the maximum level
        int maxLevel = 0;
        for (int i = 0; i < count; i++) {
            if (levels[i] > maxLevel) {
                maxLevel = levels[i];
            }
        }

        // Display the tree level by level
        std::cout << "Current AVL Tree:" << std::endl;

        // Process each level
        for (int level = 0; level <= maxLevel; level++) {
            // For the first level (root), no indentation
            if (level == 0) {
                // Find and print the root
                for (int i = 0; i < count; i++) {
                    if (levels[i] == 0) {
                        std::cout << "    " << nodes[i]->getKey() << std::endl;
                        break;
                    }
                }
            }
            // For other levels
            else {
                // Print connecting lines first
                std::cout << "    ";
                bool hasNodeAtThisLevel = false;

                // Check if there are any nodes at this level
                for (int i = 0; i < count; i++) {
                    if (levels[i] == level) {
                        hasNodeAtThisLevel = true;
                        break;
                    }
                }

                if (hasNodeAtThisLevel) {
                    std::cout << "/ \\" << std::endl;
                }

                // Print node values
                std::cout << "   ";
                bool firstNodePrinted = false;

                for (int i = 0; i < count; i++) {
                    if (levels[i] == level) {
                        if (firstNodePrinted) {
                            std::cout << " ";
                        }
                        std::cout << nodes[i]->getKey();
                        firstNodePrinted = true;
                    }
                }
                std::cout << std::endl;
            }
        }
    }

    Node* getRoot() {
        return root;
    }

    void setRoot(Node* root) {
        this->root = root;
    }

    ~AVLTree() {
        deleteTree(root);
    }
};


/// @brief Manages a contact address book using an AVL tree for efficient operations.
class AddressLibrary {
private:
    AVLTree* tree;  ///< Pointer to the AVL tree storing the contacts.

    /// @brief Reads an integer ID from the input stream.
    static int getID(ifstream& in) {
        int value;
        try {
            in >> value;
            in.ignore();  // Ignore the newline
            return value;
        } catch (exception& e) {
            cout << "Please enter a correct number: ";
        }
        return 0;
    }

    /// @brief Reads and validates an email address from the input stream.
    static string getEmail(ifstream& in) {
        const regex pattern(R"(^[a-zA-Z0-9.]+@[a-zA-Z]+\.[a-zA-Z]{2,}$)");
        string email;
        getline(in, email);
        if (regex_match(email, pattern))
            return email;
        cout << "Please enter a correct email.\n";
        return "";
    }

    /// @brief Reads and validates a phone number from the input stream.
    /// Expected format: 01X-XXX-XXXX
    static string getPhoneNumber(ifstream& in) {
        const regex pattern("^01\\d-\\d{3}-\\d{4}$");
        string number;
        getline(in, number);
        if (regex_match(number, pattern))
            return number;
        cout << "Please enter a correct phone number.\n";
        return "";
    }

    /// @brief Reads the user's name from the input stream.
    static string getUserName(ifstream& in) {
        string userName;
        getline(in, userName);
        return userName;
    }

public:
    /// @brief Constructor: initializes the AVL tree.
    AddressLibrary() {
        tree = new AVLTree();
    }

    /// @brief Adds a new contact to the AVL tree using data from the input stream.
    void addContact(ifstream& input) const {
        Contact contact;
        int id = getID(input);
        contact.name = getUserName(input);
        contact.phone = getPhoneNumber(input);
        contact.email = getEmail(input);

        try {
            Node* newNode = new Node(id, contact);
            tree->setRoot(tree->insertNode(tree->getRoot(), newNode));
            cout << "This contact is added successfully.\n";
        } catch (exception& e) {
            cout << "Error: Contact with ID " << id << " already exists.\n";
        }

    }

    /// @brief Searches for a contact by ID and prints its data.
    void searchContact(ifstream& input) {
        cout << "Which ID do you want to search for?\n";
        int id = getID(input);

        try {
            Node* node = tree->searchNode(id, tree->getRoot());
            Contact contact = node->getValue();

            cout << "ID: " << node->getKey() << endl;
            cout << "Name: " << contact.name << endl;
            cout << "Phone: " << contact.phone << endl;
            cout << "Email: " << contact.email << endl;
        } catch (exception& e) {
            cout << "Contact not found.\n";
        }
    }

    /// @brief Deletes a contact by ID from the tree.
    void deleteContact(ifstream& input) {
        cout << "Which ID do you want to delete?\n";
        int id = getID(input);
        try {
            tree->setRoot(tree->deleteNode(id, tree->getRoot()));
            cout << "This contact is deleted.\n";
        } catch (exception& e) {
            cout << "Contact not found.\n";
        }
    }

    /// @brief Lists all contact IDs in the AVL tree.
    void listContact() {
        try {
            tree->ListIDs(tree->getRoot());
        } catch (exception& e) {
            cout << e.what() << endl;
        }
    }

    /// @brief Displays the full contact list with details.
    void DisplayContact() {
        try {
            tree->displayTree(tree->getRoot());
        }
        catch (exception& e) {
            cout << e.what() << endl;
        }
    }

    /// @brief Destructor: deallocates the AVL tree.
    ~AddressLibrary() {
        delete tree;
    }
};


int main() {
    ifstream inputFile;
    string input, fileName;
    AddressLibrary addressLibrary;

    while (true) {
        cout << "# === Welcome To Our Address Book Tree === #\n";
        cout << "1. Add New Contact"
        << "\n2. Search for Contact"
        << "\n3. Delete Contact"
        << "\n4. List All Contact"
        << "\n5. Display Contact tree structure\n";
        cout << "Enter Your File Path(Enter 0 here to close program): ";
        getline(cin, fileName);

        if (fileName == "0") {
            cout << "Thank you for using our address library.\n";
            return 0;
        }

        try {
            inputFile.open(fileName);
            while (getline(inputFile, input)) {
                if (input == "1")
                    addressLibrary.addContact(inputFile);
                else if (input == "2")
                    addressLibrary.searchContact(inputFile);
                else if (input == "3")
                    addressLibrary.deleteContact(inputFile);
                else if (input == "4")
                    addressLibrary.listContact();
                else if (input == "5")
                    addressLibrary.DisplayContact();
            }
            inputFile.close();
            cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
        }
        catch (exception& e) {
            cout << "Wrong FileName" << endl;
        }
    }
    return 0;
}
