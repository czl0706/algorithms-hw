#include <iostream>
#include <sstream>
#include <climits>
using namespace std;

class Solver;

template <typename T>
class RBTree {
    friend class Solver;
    enum Color { BLACK, RED };
    struct Node {
        T val;
        Node *left, *right, *parent;
        bool color;
    };
    typedef Node* NodePtr;

public:
    void insert(T val) { 
        NodePtr z = new Node { val, nil, nil, nil, RED };
        NodePtr y = nil, x = root;
        while (x != nil) {
            y = x;
            if (val < x->val) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        
        z->parent = y;

        if (y == nil) {
            root = z;
        } else if (val < y->val) {
            y->left = z;
        } else {
            y->right = z;
        }

        insertFixup(z);
    }
    inline bool find(T val) { return findNode(val) != nil; }
    bool remove(T val) { 
        NodePtr z = findNode(val);
        if (z == nil) { return false; }
        
        removeNode(z);
        
        return true;
    }

private:
    NodePtr nil = new Node { 0, nullptr, nullptr, nullptr, BLACK }, root = nil;
    void removeNode(NodePtr z) {
        if (z->left == nil || z->right == nil) {
            NodePtr y = z;
            NodePtr x = (z->left == nil) ? z->right : z->left;
            x->parent = y->parent;
            if (y->parent == nil) {
                root = x;
            } else if (y == y->parent->left) {
                y->parent->left = x;
            } else {
                y->parent->right = x;
            }
            if (y->color == BLACK) {
                removeFixup(x);
            }
            delete y;
        } else {
            NodePtr y = z->right;
            while (y->left != nil) {
                y = y->left;
            }
            z->val = y->val;
            removeNode(y);
        }
    }
    NodePtr findNode(T val) {
        NodePtr x = root;
        while (x != nil) {
            if (val < x->val) {
                x = x->left;
            } else if (val > x->val) {
                x = x->right;
            } else {
                return x;
            }
        }
        return nil;
    }
    void rotateLeft(NodePtr x) { 
        NodePtr y = x->right;
        x->right = y->left;
        if (y->left != nil) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }
    void rotateRight(NodePtr x) { 
        NodePtr y = x->left;
        x->left = y->right;
        if (y->right != nil) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }
    void insertFixup(NodePtr z) {
        while (isRed(z->parent)) {
            if (z->parent == z->parent->parent->left) {
                NodePtr y = z->parent->parent->right;
                if (isRed(y)) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateRight(z->parent->parent);
                }
            } else {
                NodePtr y = z->parent->parent->left;
                if (isRed(y)) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }
    void removeFixup(NodePtr x) {
        while (x != root && !isRed(x)) {
            if (x == x->parent->left) {
                NodePtr w = x->parent->right;
                if (isRed(w)) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if (!isRed(w->left) && !isRed(w->right)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (!isRed(w->right)) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            } else {
                NodePtr w = x->parent->left;
                if (isRed(w)) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if (!isRed(w->left) && !isRed(w->right)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (!isRed(w->left)) {
                        w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }
    inline void flipColors(NodePtr z) { z->color = !z->color; }
    inline bool isRed(NodePtr z) { return z->color; }
};

class Solver {
public:
    Solver(RBTree<int> &tree) : tree(tree) {}
    inline string preOrder() {
        return preOrderNode(tree.root);
    }
    inline int maxPathSum() {
        maxPathSumResult = INT_MIN;
        maxPathSumNode(tree.root);
        return maxPathSumResult;
    }
    
private:
    RBTree<int> tree;
    int maxPathSumResult;
    string preOrderNode(RBTree<int>::NodePtr x) {
        if (x == tree.nil) { return ""; }
        return to_string(x->val) + " " + preOrderNode(x->left) + preOrderNode(x->right);
    }
    int maxPathSumNode(RBTree<int>::NodePtr x) {
        if (x == tree.nil) { return 0; }
        int left = max(0, maxPathSumNode(x->left));
        int right = max(0, maxPathSumNode(x->right));
        maxPathSumResult = max(maxPathSumResult, left + right + x->val);
        return max(left, right) + x->val;
    }
};

int main() {    
    RBTree<int> tree;
    
    string inputString;
    getline(cin, inputString);

    int x;
    stringstream ss(inputString);
    while (ss >> x) { tree.insert(x); }

    Solver solver(tree);
    cout << solver.preOrder() << endl;
    cout << solver.maxPathSum() << endl;

    return 0;
}