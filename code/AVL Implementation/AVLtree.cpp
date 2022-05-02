#include <iostream>

using namespace std;

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

    AVLNode<K, V>(K key, bool isFloat, V val) : key(key), isFloat(isFloat), val(val)
    {
        left = right = nullptr;
        balance = EH;
    }
};

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
    AVLNode<K, V> *deleteLeftBalance(AVLNode<K, V> *node, bool &shorter)
    {
        AVLNode<K, V> *rightTree, *leftTree;
        switch (node->balance)
        {
        case RH:
            node->balance = EH;
            break;
        case EH:
            node->balance = LH;
            shorter = false;
            break;
        case LH:
            leftTree = node->left;
            if (leftTree->balance != RH)
            {
                if (leftTree->balance == EH)
                {
                    node->balance = LH;
                    leftTree->balance = RH;
                    shorter = false;
                }
                else
                {
                    node->balance = EH;
                    leftTree->balance = EH;
                }
                node = rotateRight(node);
            }
            else
            {
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
                node->left = rotateLeft(leftTree);
                node = rotateRight(node);
            }
        }
        return node;
    }

    AVLNode<K, V> *deleteRightBalance(AVLNode<K, V> *node, bool &shorter)
    {
        AVLNode<K, V> *rightTree, *leftTree;
        switch (node->balance)
        {
        case LH:
            node->balance = EH;
            break;
        case EH:
            node->balance = RH;
            shorter = false;
            break;
        case RH:
            rightTree = node->right;
            if (rightTree->balance == LH)
            {
                leftTree = rightTree->left;
                switch (leftTree->balance)
                {
                case LH:
                    rightTree->balance = RH;
                    node->balance = EH;
                    break;
                case EH:
                    rightTree->balance = EH;
                    node->balance = EH;
                    break;
                case RH:
                    rightTree->balance = LH;
                    node->balance = EH;
                    break;
                }
                leftTree->balance = EH;
                // Rotate Right then Left
                node->right = rotateRight(rightTree);
                node = rotateLeft(node);
            } //  if rightTree->shorter == LH
            else
            {
                switch (rightTree->balance)
                {
                case LH:
                case RH:
                    node->balance = EH;
                    rightTree->balance = EH;
                    break;
                case EH:
                    node->balance = RH;
                    rightTree->balance = LH;
                    shorter = false;
                    break;
                }
                node = rotateLeft(node);
            }
        }
        return node;
    }

    AVLNode<K, V> *eraseRec(AVLNode<K, V> *node, const K &dltkey, bool &shorter, bool &success)
    {
        if (!node)
        {
            shorter = false;
            success = false;
            return nullptr;
        }
        AVLNode<K, V> *dltPtr, *exchPtr, *newRoot;
        if (dltkey < node->key)
        {
            node->left = eraseRec(node->left, dltkey, shorter, success);
            if (shorter)
            {
                node = deleteRightBalance(node, shorter);
            }
        }
        else if (dltkey > node->key)
        {
            node->right = eraseRec(node->right, dltkey, shorter, success);
            if (shorter)
            {
                node = deleteLeftBalance(node, shorter);
            }
        }
        else
        {
            dltPtr = node;
            if (!node->right)
            {
                newRoot = node->left;
                success = true;
                shorter = true;
                delete dltPtr;
                return newRoot;
            }
            else
            {
                if (!node->left)
                {
                    newRoot = node->right;
                    success = true;
                    shorter = true;
                    delete dltPtr;
                    return newRoot;
                }
                else
                {
                    exchPtr = node->left;
                    while (exchPtr->right)
                    {
                        exchPtr = exchPtr->right;
                    }
                    node->key = exchPtr->key;
                    node->isFloat = exchPtr->isFloat;
                    node->val = exchPtr->val;
                    node->left = eraseRec(node, exchPtr->key, shorter, success);
                    if (shorter)
                    {
                        node = deleteRightBalance(node, shorter);
                    }
                }
            }
        }
        return node;
    }
    // AVLNode<K, V> *erase(AVLNode<K, V> *&node, const K &key);

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
        ++size;
        return true;
    }

    bool erase(const K &key)
    {
        bool shorter, success;
        AVLNode<K, V> *newRoot;
        newRoot = eraseRec(root, key, shorter, success);
        if (success)
        {
            root = newRoot;
            size--;
        }
        return success;
    }

    AVLNode<K, V> *find(const K &key)
    {
        return findRec(root, key);
    }

    bool isFull(int count)
    {
        return size == count;
    };
    // inOrder traversal
    void inOrderRec(AVLNode<K, V> *root)
    {
        if (!root)
            return;
        cout << root->key << '\n';
        inOrderRec(root->left);
        inOrderRec(root->right);
    }

    void inOrder()
    {
        return inOrderRec(root);
    }
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
};
// https://stackoverflow.com/questions/67602936/adding-data-pushing-pair-to-list-of-pairs-to-nodes-in-avl-tree-is-not-working
int main()
{
    AVLTree<string, float> *tree = new AVLTree<string, float>();

    tree->insert("pPre", true, 0.6);
    tree->insert("num", true, 0.6);
    tree->insert("Number", true, 0.6);
    tree->insert("number", true, 0.6);
    tree->insert("Num", true, 655);
    tree->insert("idx", false, 21);
    tree->insert("index", true, 0.6);
    tree->insert("ind", true, 0.6);
    tree->insert("temp", true, 0.6);
    AVLNode<string, float> * temp = tree->find("idx");
    cout << temp->isFloat << " " << temp->val << " " << temp->key << '\n';
    // cout << tree->findParent("num")->key << '\n';
    return 0;
}