#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "nodeTree.h"
#include "leafTree.h"
#include <iostream>
#include <algorithm>

// Macro para seleccionar la implementación
// Definir USE_LEAF_TREE para usar leafTree, de lo contrario usa nodeTree
#ifdef USE_LEAF_TREE
    #define AVL_NODE node
    #define AVL_TREE leafTree
#else
    #define AVL_NODE nodeT
    #define AVL_TREE nodeTree
#endif

template<typename T, typename B>
class AVLTree {
private:
    AVL_TREE<T, B> baseTree;
    
    // Funciones auxiliares para AVL
    int getHeight(AVL_NODE<T, B>* node);
    int getBalance(AVL_NODE<T, B>* node);
    void setHeight(AVL_NODE<T, B>* node);
    
    // Rotaciones
    AVL_NODE<T, B>* rotateRight(AVL_NODE<T, B>* y);
    AVL_NODE<T, B>* rotateLeft(AVL_NODE<T, B>* x);
    
    // Operaciones AVL
    AVL_NODE<T, B>* insertAVL(T key, B val, AVL_NODE<T, B>* node);
    AVL_NODE<T, B>* deleteAVL(T key, AVL_NODE<T, B>* node);
    AVL_NODE<T, B>* getMinNode(AVL_NODE<T, B>* node);
    
    // Funciones de utilidad
    void inorderTraversal(AVL_NODE<T, B>* node);
    void preorderTraversal(AVL_NODE<T, B>* node);
    void postorderTraversal(AVL_NODE<T, B>* node);

public:
    AVLTree();
    ~AVLTree();
    
    // Interfaz pública
    void insert(T key, B val);
    void deleteNode(T key);
    AVL_NODE<T, B>* find(T key);
    
    // Recorridos
    void printInorder();
    void printPreorder();
    void printPostorder();
    
    // Verificación del balance
    bool isBalanced();
    int getTreeHeight();
};

// Especialización para nodeTree
#ifndef USE_LEAF_TREE

template<typename T, typename B>
AVLTree<T, B>::AVLTree() : baseTree() {}

template<typename T, typename B>
AVLTree<T, B>::~AVLTree() {}

template<typename T, typename B>
int AVLTree<T, B>::getHeight(nodeT<T, B>* node) {
    if (node == nullptr) return 0;
    
    // Para nodeTree, calculamos la altura recursivamente
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    return 1 + std::max(leftHeight, rightHeight);
}

template<typename T, typename B>
int AVLTree<T, B>::getBalance(nodeT<T, B>* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

template<typename T, typename B>
void AVLTree<T, B>::setHeight(nodeT<T, B>* node) {
    // Para nodeTree no necesitamos almacenar altura explícitamente
    // se calcula dinámicamente
}

template<typename T, typename B>
nodeT<T, B>* AVLTree<T, B>::rotateRight(nodeT<T, B>* y) {
    nodeT<T, B>* x = y->left;
    nodeT<T, B>* T2 = x->right;
    
    // Realizar rotación
    x->right = y;
    y->left = T2;
    
    return x;
}

template<typename T, typename B>
nodeT<T, B>* AVLTree<T, B>::rotateLeft(nodeT<T, B>* x) {
    nodeT<T, B>* y = x->right;
    nodeT<T, B>* T2 = y->left;
    
    // Realizar rotación
    y->left = x;
    x->right = T2;
    
    return y;
}

template<typename T, typename B>
nodeT<T, B>* AVLTree<T, B>::insertAVL(T key, B val, nodeT<T, B>* node) {
    // 1. Inserción normal de BST
    if (node == nullptr) {
        baseTree.incrementSize();
        return new nodeT<T, B>(key, val);
    }
    
    if (key < node->key) {
        node->left = insertAVL(key, val, node->left);
    } else if (key > node->key) {
        node->right = insertAVL(key, val, node->right);
    } else {
        // Clave duplicada, actualizar valor
        node->val = val;
        return node;
    }
    
    // 2. Obtener el factor de balance
    int balance = getBalance(node);
    
    // 3. Si el nodo se desbalanceó, hay 4 casos
    
    // Caso Left Left
    if (balance > 1 && key < node->left->key) {
        return rotateRight(node);
    }
    
    // Caso Right Right
    if (balance < -1 && key > node->right->key) {
        return rotateLeft(node);
    }
    
    // Caso Left Right
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    // Caso Right Left
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

template<typename T, typename B>
nodeT<T, B>* AVLTree<T, B>::getMinNode(nodeT<T, B>* node) {
    if (node == nullptr) return nullptr;
    
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

template<typename T, typename B>
nodeT<T, B>* AVLTree<T, B>::deleteAVL(T key, nodeT<T, B>* node) {
    // 1. Eliminación normal de BST
    if (node == nullptr) return node;
    
    if (key < node->key) {
        node->left = deleteAVL(key, node->left);
    } else if (key > node->key) {
        node->right = deleteAVL(key, node->right);
    } else {
        // Nodo a eliminar encontrado
        baseTree.decrementSize();
        
        if (node->left == nullptr || node->right == nullptr) {
            nodeT<T, B>* temp = node->left ? node->left : node->right;
            
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
        } else {
            nodeT<T, B>* temp = getMinNode(node->right);
            
            node->key = temp->key;
            node->val = temp->val;
            
            baseTree.incrementSize(); // Compensar porque deleteAVL decrementará de nuevo
            node->right = deleteAVL(temp->key, node->right);
        }
    }
    
    if (node == nullptr) return node;
    
    // 2. Obtener el factor de balance
    int balance = getBalance(node);
    
    // 3. Si el nodo se desbalanceó, hay 4 casos
    
    // Caso Left Left
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }
    
    // Caso Left Right
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    // Caso Right Right
    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }
    
    // Caso Right Left
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

#else

// Especialización para leafTree
template<typename T, typename B>
AVLTree<T, B>::AVLTree() : baseTree() {}

template<typename T, typename B>
AVLTree<T, B>::~AVLTree() {}

template<typename T, typename B>
int AVLTree<T, B>::getHeight(node<T, B>* n) {
    if (n == nullptr) return 0;
    
    // Para leafTree, solo las hojas cuentan para la altura
    if (n->leaf) return 1;
    
    int leftHeight = getHeight(n->left);
    int rightHeight = getHeight(n->right);
    return 1 + std::max(leftHeight, rightHeight);
}

template<typename T, typename B>
int AVLTree<T, B>::getBalance(node<T, B>* n) {
    if (n == nullptr) return 0;
    return getHeight(n->left) - getHeight(n->right);
}

template<typename T, typename B>
void AVLTree<T, B>::setHeight(node<T, B>* n) {
    // Para leafTree no necesitamos almacenar altura explícitamente
}

template<typename T, typename B>
node<T, B>* AVLTree<T, B>::rotateRight(node<T, B>* y) {
    node<T, B>* x = y->left;
    node<T, B>* T2 = x->right;
    
    // Realizar rotación
    x->right = y;
    y->left = T2;
    
    return x;
}

template<typename T, typename B>
node<T, B>* AVLTree<T, B>::rotateLeft(node<T, B>* x) {
    node<T, B>* y = x->right;
    node<T, B>* T2 = y->left;
    
    // Realizar rotación
    y->left = x;
    x->right = T2;
    
    return y;
}

template<typename T, typename B>
node<T, B>* AVLTree<T, B>::insertAVL(T key, B val, node<T, B>* n) {
    // Para leafTree, la inserción es más compleja debido a su estructura especial
    // Necesitamos adaptar la lógica de inserción manteniendo el balance AVL
    
    if (n == nullptr) {
        baseTree.incrementSize();
        return new node<T, B>(key, val, nullptr, nullptr, true);
    }
    
    // Inserción usando la lógica de leafTree pero con balance AVL
    if (n->leaf) {
        // Crear nuevo nodo interno y reorganizar
        node<T, B>* newInternal = new node<T, B>(std::max(key, n->key), B{}, nullptr, nullptr, false);
        node<T, B>* newLeaf = new node<T, B>(key, val, nullptr, nullptr, true);
        
        baseTree.incrementSize();
        
        if (key < n->key) {
            newInternal->left = newLeaf;
            newInternal->right = n;
        } else {
            newInternal->left = n;
            newInternal->right = newLeaf;
        }
        
        return newInternal;
    }
    
    // Navegación en nodo interno
    if (key <= n->key) {
        n->left = insertAVL(key, val, n->left);
    } else {
        n->right = insertAVL(key, val, n->right);
    }
    
    // Actualizar clave del nodo interno si es necesario
    if (n->right && n->right->leaf) {
        n->key = n->right->key;
    }
    
    // Verificar balance y rotar si es necesario
    int balance = getBalance(n);
    
    if (balance > 1 && key <= n->left->key) {
        return rotateRight(n);
    }
    
    if (balance < -1 && key > n->right->key) {
        return rotateLeft(n);
    }
    
    if (balance > 1 && key > n->left->key) {
        n->left = rotateLeft(n->left);
        return rotateRight(n);
    }
    
    if (balance < -1 && key <= n->right->key) {
        n->right = rotateRight(n->right);
        return rotateLeft(n);
    }
    
    return n;
}

template<typename T, typename B>
node<T, B>* AVLTree<T, B>::getMinNode(node<T, B>* n) {
    if (n == nullptr) return nullptr;
    
    while (n->left != nullptr) {
        n = n->left;
    }
    return n;
}

template<typename T, typename B>
node<T, B>* AVLTree<T, B>::deleteAVL(T key, node<T, B>* n) {
    // Implementación de eliminación para leafTree con balance AVL
    if (n == nullptr) return nullptr;
    
    if (n->leaf) {
        if (n->key == key) {
            baseTree.decrementSize();
            delete n;
            return nullptr;
        }
        return n;
    }
    
    // Navegación en nodo interno
    if (key <= n->key) {
        n->left = deleteAVL(key, n->left);
    } else {
        n->right = deleteAVL(key, n->right);
    }
    
    // Si un hijo se vuelve nullptr, colapsar el nodo
    if (n->left == nullptr && n->right != nullptr) {
        node<T, B>* temp = n->right;
        delete n;
        return temp;
    }
    if (n->right == nullptr && n->left != nullptr) {
        node<T, B>* temp = n->left;
        delete n;
        return temp;
    }
    
    // Actualizar clave del nodo interno
    if (n->right && n->right->leaf) {
        n->key = n->right->key;
    }
    
    // Verificar balance
    int balance = getBalance(n);
    
    if (balance > 1 && getBalance(n->left) >= 0) {
        return rotateRight(n);
    }
    
    if (balance > 1 && getBalance(n->left) < 0) {
        n->left = rotateLeft(n->left);
        return rotateRight(n);
    }
    
    if (balance < -1 && getBalance(n->right) <= 0) {
        return rotateLeft(n);
    }
    
    if (balance < -1 && getBalance(n->right) > 0) {
        n->right = rotateRight(n->right);
        return rotateLeft(n);
    }
    
    return n;
}

#endif

// Implementaciones comunes para ambas especializaciones
template<typename T, typename B>
void AVLTree<T, B>::insert(T key, B val) {
    // Usar setRoot en lugar de acceso directo a baseTree.root
    baseTree.setRoot(insertAVL(key, val, baseTree.getRoot()));
}

template<typename T, typename B>
void AVLTree<T, B>::deleteNode(T key) {
    // Usar setRoot en lugar de acceso directo a baseTree.root
    baseTree.setRoot(deleteAVL(key, baseTree.getRoot()));
}

template<typename T, typename B>
AVL_NODE<T, B>* AVLTree<T, B>::find(T key) {
    return baseTree.find(key);
}

template<typename T, typename B>
void AVLTree<T, B>::inorderTraversal(AVL_NODE<T, B>* node) {
    if (node != nullptr) {
#ifdef USE_LEAF_TREE
        if (node->leaf) {
            std::cout << "(" << node->key << ", " << node->val << ") ";
        } else {
            inorderTraversal(node->left);
            inorderTraversal(node->right);
        }
#else
        inorderTraversal(node->left);
        std::cout << "(" << node->key << ", " << node->val << ") ";
        inorderTraversal(node->right);
#endif
    }
}

template<typename T, typename B>
void AVLTree<T, B>::preorderTraversal(AVL_NODE<T, B>* node) {
    if (node != nullptr) {
#ifdef USE_LEAF_TREE
        if (node->leaf) {
            std::cout << "(" << node->key << ", " << node->val << ") ";
        } else {
            preorderTraversal(node->left);
            preorderTraversal(node->right);
        }
#else
        std::cout << "(" << node->key << ", " << node->val << ") ";
        preorderTraversal(node->left);
        preorderTraversal(node->right);
#endif
    }
}

template<typename T, typename B>
void AVLTree<T, B>::postorderTraversal(AVL_NODE<T, B>* node) {
    if (node != nullptr) {
#ifdef USE_LEAF_TREE
        if (node->leaf) {
            std::cout << "(" << node->key << ", " << node->val << ") ";
        } else {
            postorderTraversal(node->left);
            postorderTraversal(node->right);
        }
#else
        postorderTraversal(node->left);
        postorderTraversal(node->right);
        std::cout << "(" << node->key << ", " << node->val << ") ";
#endif
    }
}

template<typename T, typename B>
void AVLTree<T, B>::printInorder() {
    std::cout << "Inorder: ";
    // Usar getRoot() en lugar de acceso directo a baseTree.root
    inorderTraversal(baseTree.getRoot());
    std::cout << std::endl;
}

template<typename T, typename B>
void AVLTree<T, B>::printPreorder() {
    std::cout << "Preorder: ";
    // Usar getRoot() en lugar de acceso directo a baseTree.root
    preorderTraversal(baseTree.getRoot());
    std::cout << std::endl;
}

template<typename T, typename B>
void AVLTree<T, B>::printPostorder() {
    std::cout << "Postorder: ";
    // Usar getRoot() en lugar de acceso directo a baseTree.root
    postorderTraversal(baseTree.getRoot());
    std::cout << std::endl;
}

template<typename T, typename B>
bool AVLTree<T, B>::isBalanced() {
    // Usar getRoot() en lugar de acceso directo a baseTree.root
    return std::abs(getBalance(baseTree.getRoot())) <= 1;
}

template<typename T, typename B>
int AVLTree<T, B>::getTreeHeight() {
    // Usar getRoot() en lugar de acceso directo a baseTree.root
    return getHeight(baseTree.getRoot());
}

#endif // AVL_TREE_H
