#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>
/*
Node Implementation of Union of int, float and bool
*/
union operand
{
    int data_int;
    float data_float;
    // decide the data type of following by: 0 -> int, 1 -> float
    bool data_bool;
};

template <class T>
class Node
{
public:
    T data;
    Node<T> *next;
};

/*
Stack Implementation
*/
template <class T>
class Stack
{
private:
    Node<T> *head;
    int count;

public:
    Stack() : head(NULL), count(0){};
    ~Stack()
    {
        Node<T> *temp;
        while (head)
        {
            temp = head;
            head = head->next;
            delete temp;
        }
        head = nullptr;
        count = 0;
    }
    bool empty()
    {
        return count == 0;
    }
    int size()
    {
        return count;
    }
    T top()
    {
        if (head)
            return head->data;
        else
            exit(1);
    }
    void push(const T &val)
    {
        if (head == nullptr)
        {
            head = new Node<T>;
            head->next = nullptr;
            head->data = val;
        }
        else
        {
            Node<T> *temp = new Node<T>;
            temp->data = val;
            temp->next = head;
            head = temp;
        }
        ++count;
    }
    void pop()
    {
        if (head == nullptr)
        {
            return;
        }
        else
        {
            Node<T> *temp = head;
            head = head->next;
            delete temp;
            --count;
        }
    }
    bool isFull(int size)
    {
        return count == size;
    }
};

/*
AVLNode Implementation of Union of int, float
*/
union variable
{
    int data_int;
    float data_float;
};

#define LH +1
#define EH 0
#define RH -1

template <class K, class V>
class AVLNode
{
public:
    K key;            // variable name
    bool isFloat = 0; // 1 - float, 0 - int
    V val;            // variable value
    AVLNode<K, V> *left;
    AVLNode<K, V> *right;
    int balance;

    AVLNode<K, V>(K key, bool isFloat, V val)
        : key(key), isFloat(isFloat), val(val),
          left(nullptr), right(nullptr), balance(EH){};
};
/*
AVLTree Implementation
*/

template <class K, class V>
class AVLTree
{
private:
    AVLNode<K, V> *root;
    int size;

    // AVLTree();

    void destroyAVL(AVLNode<K, V> *node)
    {
        if (node)
        {
            destroyAVL(node->left);
            destroyAVL(node->right);
            delete node;
        }
        return;
    }
    // ~AVLTree();

    // int count();

    AVLNode<K, V> *rotateLeft(AVLNode<K, V> *node)
    {
        AVLNode<K, V> *temp = node->right;
        node->right = temp->left;
        temp->left = node;
        return temp;
    }
    AVLNode<K, V> *rotateRight(AVLNode<K, V> *node)
    {
        AVLNode<K, V> *temp = node->left;
        node->left = temp->right;
        temp->right = node;
        return temp;
    }
    AVLNode<K, V> *leftBalance(AVLNode<K, V> *node, bool &taller)
    {
        AVLNode<K, V> *rightTree;
        AVLNode<K, V> *leftTree;
        leftTree = node->left;
        switch (leftTree->balance)
        {
        case LH:
            node->balance = EH;
            leftTree->balance = EH;

            node = rotateRight(node);
            taller = false;
            break;

        case EH:
            exit(100);

        case RH:
            rightTree = leftTree->right;
            switch (rightTree->balance)
            {
            case LH:
                node->balance = RH;
                leftTree->balance = EH;
                break;
            case EH:
                node->balance = EH;
                leftTree->balance = EH;
                break;
            case RH:
                node->balance = EH;
                leftTree->balance = LH;
                break;
            }

            rightTree->balance = EH;
            // Rotate Left
            node->left = rotateLeft(leftTree);
            // Rotate Right
            node = rotateRight(node);
            taller = false;
        }
        return node;
    }
    AVLNode<K, V> *rightBalance(AVLNode<K, V> *node, bool &taller)
    {
        AVLNode<K, V> *rightTree;
        AVLNode<K, V> *leftTree;

        rightTree = node->right;

        switch (rightTree->balance)
        {
        case LH:
            leftTree = rightTree->left;

            switch (leftTree->balance)
            {
            case RH:
                node->balance = LH;
                rightTree->balance = EH;
                break;
            case EH:
                node->balance = EH;
                rightTree->balance = EH;
                break;
            case LH:
                node->balance = EH;
                rightTree->balance = RH;
                break;
            }
            leftTree->balance = EH;

            node->right = rotateRight(rightTree);
            node = rotateLeft(node);
            taller = false;
            break;

        case EH:
            node->balance = EH;
            taller = false;
            break;

        case RH:
            node->balance = EH;
            rightTree->balance = EH;

            node = rotateLeft(node);
            taller = false;
            break;
        }
        return node;
    }

    AVLNode<K, V> *insertRec(AVLNode<K, V> *node, AVLNode<K, V> *newPtr, bool &taller)
    {
        if (!node)
        {
            node = newPtr;
            taller = true;
            return node;
        }
        if (newPtr->key < node->key)
        {
            node->left = insertRec(node->left, newPtr, taller);
            // left subtree taller case
            if (taller)
            {
                if (node->balance == LH)
                {
                    node = leftBalance(node, taller);
                }
                else if (node->balance == EH)
                {
                    node->balance = LH;
                }
                else
                {
                    node->balance = EH;
                    taller = false;
                }
            }
        }
        else
        {
            node->right = insertRec(node->right, newPtr, taller);
            // right subtree taller case
            if (taller)
            {
                if (node->balance == LH)
                {
                    node->balance = EH;
                    taller = false;
                }
                else if (node->balance == EH)
                {
                    node->balance = RH;
                }
                else
                {
                    node = rightBalance(node, taller);
                }
            }
        }
        return node;
    }
    // bool insert(const K &key, const bool &isFloat, const V &val)

    AVLNode<K, V> *findRec(AVLNode<K, V> *node, const K &key)
    {
        if (!node)
        {
            return nullptr;
        }
        if (key < node->key)
        {
            return findRec(node->left, key);
        }
        else if (key > node->key)
        {
            return findRec(node->right, key);
        }
        else
        {
            return node;
        }
    }
    // AVLNode<K, V> *find(const K &key);

    // bool isFull(int size);

public:
    AVLTree() : root(nullptr), size(0){};

    ~AVLTree()
    {
        if (root)
        {
            destroyAVL(root);
        }
        // cout << "Tree has been destructed.";
    }

    int count()
    {
        return size;
    }

    bool insert(const K &key, const bool &isFloat, const V &val)
    {
        bool taller;
        AVLNode<K, V> *newPtr;
        if (!(newPtr = new AVLNode<K, V>(key, isFloat, val)))
        {
            return false;
        }
        root = insertRec(root, newPtr, taller);
        size += 2;
        return true;
    }

    AVLNode<K, V> *find(const K &key)
    {
        return findRec(root, key);
    }

    bool isFull(int count)
    {
        return size == count;
    };

    AVLNode<K, V> *findParentRec(const K &key, AVLNode<K, V> *node)
    {
        if (!node)
            return nullptr;
        if (!node->left && !node->right)
        {
            return nullptr;
        }
        if ((node->left && node->left->key == key) ||
            (node->right && node->right->key == key))
        {
            return node;
        }
        if (node->key > key)
        {
            return findParentRec(key, node->left);
        }
        if (node->key < key)
        {
            return findParentRec(key, node->right);
        }
        return nullptr;
    }
    AVLNode<K, V> *findParent(const K &key)
    {
        return findParentRec(key, root);
    }

    // inOrder traversal
};

/*
StackFrame declaration
*/
class StackFrame
{
private:
    int opStackMaxSize;    // max size of operand stack
    int localVarSpaceSize; // size of local variable array

    Stack<operand> *OperandStack;
    AVLTree<std::string, variable> *LocalVariableSpace;

public:
    // Program counter
    unsigned int PC;
    // Exception flag
    bool IsExcep;

    /*
    Constructor of StackFrame
    */
    StackFrame();

    /*
    Destruction of StackFrame
    */
    ~StackFrame();

    void runloadstore(std::string instr, std::string var, operand val);
    void runLocalVariableManagement(std::string instr, std::string var);
    void runOperandStackManagement(std::string instr);
    void runTypeConversion(std::string instr);
    void runArithmetic(std::string instr);

    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void run(std::string filename);

    /*
    1. TypeMisMatch
    2. DivideByZero
    3. StackFull
    4. StackEmpty
    5. LocalSpaceFull
    6. UndefinedVariable
    @param numberOfexception an integer in range of [1, 6]
    @return An instance of your exceptions
    */
    void raiseException(int numberOfexception);
};

#endif // !__STACK_FRAME_H__