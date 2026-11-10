#ifndef BST_HPP
#define BST_HPP

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <string>

/**
 * @brief Binary Search Tree with iterative operations.
 *
 * Ejercicio 1:
 * - Inserción iterativa.
 * - Eliminación iterativa.
 * - Recorrido in-order usando std::stack.
 * - Método para calcular la altura del árbol (iterativo por niveles con std::queue).
 */
template <typename T = int>
class BST {
public:
    struct Node {
        T key;
        Node* left;
        Node* right;

        explicit Node(const T& val) : key(val), left(nullptr), right(nullptr) {}
    };

private:
    Node* root;
    size_t nodeCount;

    void printTreeHelper(Node* node, const std::string& prefix, bool isLeft, std::ostream& os) const {
        if (node == nullptr) return;

        os << prefix;
        os << (isLeft ? "├── " : "└── ");
        os << node->key << "\n";

        bool hasLeft = (node->left != nullptr);
        bool hasRight = (node->right != nullptr);

        if (hasLeft || hasRight) {
            if (hasLeft) {
                printTreeHelper(node->left, prefix + (isLeft ? "│   " : "    "), true, os);
            } else {
                os << prefix + (isLeft ? "│   " : "    ") << "├── (null)\n";
            }
            if (hasRight) {
                printTreeHelper(node->right, prefix + (isLeft ? "│   " : "    "), false, os);
            } else {
                os << prefix + (isLeft ? "│   " : "    ") << "└── (null)\n";
            }
        }
    }

public:
    BST() : root(nullptr), nodeCount(0) {}

    ~BST() {
        clear();
    }

    // Disable copy for simplicity and safety, enable move
    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    BST(BST&& other) noexcept : root(other.root), nodeCount(other.nodeCount) {
        other.root = nullptr;
        other.nodeCount = 0;
    }

    BST& operator=(BST&& other) noexcept {
        if (this != &other) {
            clear();
            root = other.root;
            nodeCount = other.nodeCount;
            other.root = nullptr;
            other.nodeCount = 0;
        }
        return *this;
    }

    /**
     * @brief Limpia todos los nodos del árbol de forma iterativa.
     */
    void clear() {
        if (!root) return;
        std::stack<Node*> st;
        std::stack<Node*> toDelete;
        st.push(root);

        while (!st.empty()) {
            Node* curr = st.top();
            st.pop();
            toDelete.push(curr);
            if (curr->left) st.push(curr->left);
            if (curr->right) st.push(curr->right);
        }

        while (!toDelete.empty()) {
            delete toDelete.top();
            toDelete.pop();
        }

        root = nullptr;
        nodeCount = 0;
    }

    /**
     * @brief Inserción iterativa en el BST.
     * @param key Valor a insertar.
     * @return true si se insertó con éxito, false si ya existía duplicado.
     */
    bool insert(const T& key) {
        Node* newNode = new Node(key);
        if (root == nullptr) {
            root = newNode;
            nodeCount++;
            return true;
        }

        Node* curr = root;
        Node* parent = nullptr;

        while (curr != nullptr) {
            parent = curr;
            if (key < curr->key) {
                curr = curr->left;
            } else if (key > curr->key) {
                curr = curr->right;
            } else {
                // Duplicado no permitido
                delete newNode;
                return false;
            }
        }

        if (key < parent->key) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        nodeCount++;
        return true;
    }

    /**
     * @brief Eliminación iterativa en el BST.
     * Contempla los 3 casos:
     * 1. Nodo hoja (0 hijos).
     * 2. Nodo con 1 hijo (izquierdo o derecho).
     * 3. Nodo con 2 hijos (se busca el sucesor in-order, se copia su clave y se elimina el sucesor).
     * @param key Valor a eliminar.
     * @return true si se encontró y eliminó, false si no se encontró.
     */
    bool remove(const T& key) {
        Node* curr = root;
        Node* parent = nullptr;

        // 1. Búsqueda iterativa del nodo a eliminar
        while (curr != nullptr && curr->key != key) {
            parent = curr;
            if (key < curr->key) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }

        if (curr == nullptr) {
            return false; // Clave no encontrada
        }

        // 2. Caso con 2 hijos: encontrar sucesor in-order
        if (curr->left != nullptr && curr->right != nullptr) {
            Node* succParent = curr;
            Node* succ = curr->right;

            // El sucesor in-order es el nodo más a la izquierda del subárbol derecho
            while (succ->left != nullptr) {
                succParent = succ;
                succ = succ->left;
            }

            // Copiar la clave del sucesor al nodo actual
            curr->key = succ->key;

            // Ahora el nodo a desacoplar físicamente es el sucesor
            curr = succ;
            parent = succParent;
        }

        // 3. Caso con 0 o 1 hijo (curr tiene como máximo un hijo)
        Node* child = (curr->left != nullptr) ? curr->left : curr->right;

        if (parent == nullptr) {
            // Eliminando la raíz
            root = child;
        } else if (parent->left == curr) {
            parent->left = child;
        } else {
            parent->right = child;
        }

        delete curr;
        nodeCount--;
        return true;
    }

    /**
     * @brief Búsqueda iterativa de un elemento.
     */
    bool search(const T& key) const {
        Node* curr = root;
        while (curr != nullptr) {
            if (key == curr->key) return true;
            if (key < curr->key) curr = curr->left;
            else curr = curr->right;
        }
        return false;
    }

    /**
     * @brief Recorrido in-order iterativo usando std::stack.
     * Los elementos visitados en un BST siempre están en orden ascendente.
     * @return std::vector con los elementos en orden.
     */
    std::vector<T> inOrder() const {
        std::vector<T> result;
        std::stack<Node*> st;
        Node* curr = root;

        while (curr != nullptr || !st.empty()) {
            // Descender todo lo posible por la rama izquierda
            while (curr != nullptr) {
                st.push(curr);
                curr = curr->left;
            }

            // Tomar el nodo de la cima
            curr = st.top();
            st.pop();
            result.push_back(curr->key);

            // Pasar al subárbol derecho
            curr = curr->right;
        }

        return result;
    }

    /**
     * @brief Recorrido pre-order iterativo usando std::stack.
     * @return std::vector con los elementos en pre-order.
     */
    std::vector<T> preOrder() const {
        std::vector<T> result;
        if (root == nullptr) return result;

        std::stack<Node*> st;
        st.push(root);

        while (!st.empty()) {
            Node* curr = st.top();
            st.pop();
            result.push_back(curr->key);

            // Primero se empuja el hijo derecho para que el izquierdo sea procesado primero
            if (curr->right != nullptr) st.push(curr->right);
            if (curr->left != nullptr) st.push(curr->left);
        }

        return result;
    }

    /**
     * @brief Calcula la altura del árbol de forma iterativa por niveles (BFS usando std::queue).
     * Definición: Número de niveles en el árbol (árbol vacío = 0, raíz solitaria = 1).
     * @return Altura en niveles.
     */
    int getHeight() const {
        if (root == nullptr) return 0;

        std::queue<Node*> q;
        q.push(root);
        int height = 0;

        while (!q.empty()) {
            size_t levelSize = q.size();
            for (size_t i = 0; i < levelSize; ++i) {
                Node* curr = q.front();
                q.pop();

                if (curr->left != nullptr) q.push(curr->left);
                if (curr->right != nullptr) q.push(curr->right);
            }
            height++;
        }

        return height;
    }

    /**
     * @brief Cantidad de nodos almacenados.
     */
    size_t size() const {
        return nodeCount;
    }

    /**
     * @brief Indica si el árbol está vacío.
     */
    bool empty() const {
        return root == nullptr;
    }

    /**
     * @brief Imprime la estructura visual del árbol en formato ASCII.
     */
    void printTree(std::ostream& os = std::cout) const {
        if (root == nullptr) {
            os << "(Árbol vacío)\n";
            return;
        }
        printTreeHelper(root, "", false, os);
    }
};

#endif // BST_HPP

