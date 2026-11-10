#ifndef RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_HPP

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

enum class Color {
    RED,
    BLACK
};

inline std::string colorToString(Color c) {
    return (c == Color::RED) ? "RED" : "BLACK";
}

template <typename T = int>
class RedBlackTree {
public:
    struct Node {
        T key;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(const T& k, Color c, Node* l = nullptr, Node* r = nullptr, Node* p = nullptr)
            : key(k), color(c), left(l), right(r), parent(p) {}
    };

    struct NodeInfo {
        T key;
        Color color;
        int depth;
    };

    struct PropertyCheck {
        int id;
        std::string name;
        std::string formalRule;
        bool passed;
        std::string detail;
    };

    struct VerificationReport {
        bool allValid;
        int blackHeight;
        std::vector<PropertyCheck> checks;

        void print(std::ostream& os = std::cout) const {
            os << "========================================================\n";
            os << " REPORTE DE VERIFICACIÓN FORMAL DE PROPIEDADES RED-BLACK\n";
            os << "========================================================\n";
            for (const auto& c : checks) {
                os << " [" << (c.passed ? "\033[1;32mCORRECTO\033[0m" : "\033[1;31mFALLO\033[0m") << "] "
                   << "Propiedad " << c.id << ": " << c.name << "\n"
                   << "     Definición: " << c.formalRule << "\n"
                   << "     Resultado:  " << c.detail << "\n";
            }
            os << "--------------------------------------------------------\n";
            os << " Estado Global: " 
               << (allValid ? "\033[1;32mÁRBOL RED-BLACK VÁLIDO\033[0m" : "\033[1;31mINVÁLIDO\033[0m") 
               << " (Altura Negra de la raíz = " << blackHeight << ")\n";
            os << "========================================================\n";
        }
    };

private:
    Node* nil;
    Node* root;
    size_t nodeCount;

    // Métodos auxiliares para rotaciones
    void rotateLeft(Node* x) {
        Node* y = x->right;
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

    void rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != nil) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == nil) {
            root = x;
        } else if (y == y->parent->right) {
            y->parent->right = x;
        } else {
            y->parent->left = x;
        }
        x->right = y;
        y->parent = x;
    }

    /**
     * @brief Corrección de violaciones tras inserción (CLRS RB-Insert-Fixup).
     * Casos:
     * 1. Tío es ROJO: Recoloración de padre, tío y abuelo.
     * 2. Tío es NEGRO y z es hijo derecho (triángulo): Rotación izquierda sobre padre.
     * 3. Tío es NEGRO y z es hijo izquierdo (línea): Recoloración de padre y abuelo + rotación derecha sobre abuelo.
     * (Y simétrico si el padre es hijo derecho del abuelo).
     */
    void insertFixup(Node* z) {
        while (z->parent->color == Color::RED) {
            if (z->parent == z->parent->parent->left) {
                Node* uncle = z->parent->parent->right;
                if (uncle->color == Color::RED) {
                    // Caso 1: Tío Rojo -> Recoloración
                    z->parent->color = Color::BLACK;
                    uncle->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    // Tío Negro
                    if (z == z->parent->right) {
                        // Caso 2: Formación en triángulo (zig-zag) -> Rotación Izquierda
                        z = z->parent;
                        rotateLeft(z);
                    }
                    // Caso 3: Formación en línea (zig-zig) -> Recoloración y Rotación Derecha
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotateRight(z->parent->parent);
                }
            } else {
                // Simétrico: El padre es hijo derecho del abuelo
                Node* uncle = z->parent->parent->left;
                if (uncle->color == Color::RED) {
                    // Caso 1: Tío Rojo -> Recoloración
                    z->parent->color = Color::BLACK;
                    uncle->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    // Tío Negro
                    if (z == z->parent->left) {
                        // Caso 2: Formación en triángulo -> Rotación Derecha
                        z = z->parent;
                        rotateRight(z);
                    }
                    // Caso 3: Formación en línea -> Recoloración y Rotación Izquierda
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = Color::BLACK;
    }

    void printTreeHelper(Node* node, const std::string& prefix, bool isLeft, std::ostream& os) const {
        if (node == nil) return;

        os << prefix;
        os << (isLeft ? "├── " : "└── ");
        if (node->color == Color::RED) {
            os << "\033[1;31m" << node->key << " [R]\033[0m\n";
        } else {
            os << "\033[1;37;40m" << node->key << " [B]\033[0m\n";
        }

        bool hasLeft = (node->left != nil);
        bool hasRight = (node->right != nil);

        if (hasLeft || hasRight) {
            if (hasLeft) {
                printTreeHelper(node->left, prefix + (isLeft ? "│   " : "    "), true, os);
            } else {
                os << prefix + (isLeft ? "│   " : "    ") << "├── \033[2mNIL [B]\033[0m\n";
            }
            if (hasRight) {
                printTreeHelper(node->right, prefix + (isLeft ? "│   " : "    "), false, os);
            } else {
                os << prefix + (isLeft ? "│   " : "    ") << "└── \033[2mNIL [B]\033[0m\n";
            }
        }
    }

    // Auxiliar para comprobar altura negra en cada subárbol
    int computeBlackHeight(Node* node, bool& ok, std::vector<std::string>& issues) const {
        if (node == nil) {
            return 1; // NIL cuenta como negro
        }
        int leftBH = computeBlackHeight(node->left, ok, issues);
        int rightBH = computeBlackHeight(node->right, ok, issues);

        if (leftBH != rightBH) {
            ok = false;
            std::ostringstream ss;
            ss << "Nodo " << node->key << " tiene alturas negras distintas: izq=" 
               << leftBH << ", der=" << rightBH;
            issues.push_back(ss.str());
        }

        return (node->color == Color::BLACK ? 1 : 0) + std::max(leftBH, rightBH);
    }

public:
    RedBlackTree() {
        nil = new Node(T(), Color::BLACK);
        nil->left = nil;
        nil->right = nil;
        nil->parent = nil;
        root = nil;
        nodeCount = 0;
    }

    ~RedBlackTree() {
        clear();
        delete nil;
    }

    RedBlackTree(const RedBlackTree&) = delete;
    RedBlackTree& operator=(const RedBlackTree&) = delete;

    RedBlackTree(RedBlackTree&& other) noexcept 
        : nil(other.nil), root(other.root), nodeCount(other.nodeCount) {
        other.nil = nullptr;
        other.root = nullptr;
        other.nodeCount = 0;
    }

    RedBlackTree& operator=(RedBlackTree&& other) noexcept {
        if (this != &other) {
            clear();
            delete nil;
            nil = other.nil;
            root = other.root;
            nodeCount = other.nodeCount;
            other.nil = nullptr;
            other.root = nullptr;
            other.nodeCount = 0;
        }
        return *this;
    }

    void clear() {
        if (root == nil || nil == nullptr) return;

        std::stack<Node*> st;
        std::stack<Node*> toDelete;
        st.push(root);

        while (!st.empty()) {
            Node* curr = st.top();
            st.pop();
            toDelete.push(curr);
            if (curr->left != nil) st.push(curr->left);
            if (curr->right != nil) st.push(curr->right);
        }

        while (!toDelete.empty()) {
            delete toDelete.top();
            toDelete.pop();
        }

        root = nil;
        nodeCount = 0;
    }

    /**
     * @brief Inserción de un nuevo elemento en el Red-Black Tree.
     * Realiza inserción de BST estándar y luego repara las propiedades llamando a insertFixup.
     * @param key Valor a insertar.
     * @return true si se insertó con éxito, false si ya existía duplicado.
     */
    bool insert(const T& key) {
        Node* z = new Node(key, Color::RED, nil, nil, nil);
        Node* y = nil;
        Node* x = root;

        while (x != nil) {
            y = x;
            if (key < x->key) {
                x = x->left;
            } else if (key > x->key) {
                x = x->right;
            } else {
                delete z;
                return false; // No duplicados
            }
        }

        z->parent = y;
        if (y == nil) {
            root = z;
        } else if (z->key < y->key) {
            y->left = z;
        } else {
            y->right = z;
        }

        z->left = nil;
        z->right = nil;
        z->color = Color::RED;
        nodeCount++;

        insertFixup(z);
        return true;
    }

    /**
     * @brief Calcula la altura del árbol de forma iterativa por niveles (BFS usando std::queue).
     * @return Número de niveles en el árbol (árbol vacío = 0, raíz solitaria = 1).
     */
    int getHeight() const {
        if (root == nil) return 0;

        std::queue<Node*> q;
        q.push(root);
        int height = 0;

        while (!q.empty()) {
            size_t levelSize = q.size();
            for (size_t i = 0; i < levelSize; ++i) {
                Node* curr = q.front();
                q.pop();

                if (curr->left != nil) q.push(curr->left);
                if (curr->right != nil) q.push(curr->right);
            }
            height++;
        }

        return height;
    }

    /**
     * @brief Calcula la altura negra (black-height) desde la raíz hasta los nodos NIL.
     * Cuenta cuántos nodos negros se atraviesan en un camino simple (incluyendo o excluyendo la raíz).
     * Convencionalmente excluye la raíz e incluye hojas NIL, o cuenta nodos negros internos.
     */
    int getBlackHeight() const {
        if (root == nil) return 0;
        int bh = 0;
        Node* curr = root->left; // No se cuenta la raíz según la definición formal CLRS
        while (curr != nil) {
            if (curr->color == Color::BLACK) bh++;
            curr = curr->left;
        }
        return bh + 1; // +1 por el nodo NIL
    }

    /**
     * @brief Recorrido Pre-Orden iterativo usando std::stack.
     * Guarda la clave, el color y la profundidad de cada nodo para facilitar graficación.
     */
    std::vector<NodeInfo> preOrder() const {
        std::vector<NodeInfo> result;
        if (root == nil) return result;

        std::stack<std::pair<Node*, int>> st;
        st.push({root, 0});

        while (!st.empty()) {
            auto [curr, depth] = st.top();
            st.pop();

            result.push_back({curr->key, curr->color, depth});

            if (curr->right != nil) st.push({curr->right, depth + 1});
            if (curr->left != nil) st.push({curr->left, depth + 1});
        }

        return result;
    }

    /**
     * @brief Recorrido In-Order iterativo usando std::stack.
     */
    std::vector<NodeInfo> inOrder() const {
        std::vector<NodeInfo> result;
        std::stack<Node*> st;
        Node* curr = root;

        while (curr != nil || !st.empty()) {
            while (curr != nil) {
                st.push(curr);
                curr = curr->left;
            }

            curr = st.top();
            st.pop();
            result.push_back({curr->key, curr->color, 0});

            curr = curr->right;
        }

        return result;
    }

    /**
     * @brief Verificación formal exhaustiva de todas las propiedades de un Red-Black Tree.
     * Reglas evaluadas:
     * 1. Cada nodo es ROJO o NEGRO.
     * 2. La raíz es NEGRA.
     * 3. Ningún nodo ROJO puede tener hijo ROJO (ausencia de nodos rojos consecutivos).
     * 4. Todo camino simple desde cualquier nodo a sus hojas NIL contiene el mismo número de nodos negros.
     * 5. Las hojas nulas (NIL) son NEGRAS.
     * Propiedad adicional: Cumplimiento de orden de Árbol Binario de Búsqueda (BST).
     */
    VerificationReport verifyProperties() const {
        VerificationReport report;
        report.allValid = true;

        if (root == nil) {
            report.blackHeight = 0;
            report.checks.push_back({1, "Colores válidos", "Cada nodo es rojo o negro", true, "Árbol vacío"});
            report.checks.push_back({2, "Raíz negra", "La raíz debe ser negra", true, "Raíz es NIL (negra)"});
            report.checks.push_back({3, "Sin rojos consecutivos", "Hijos de nodo rojo son negros", true, "Árbol vacío"});
            report.checks.push_back({4, "Equilibrio de altura negra", "Todos los caminos a NIL tienen igual número de nodos negros", true, "Árbol vacío"});
            report.checks.push_back({5, "Hojas NIL negras", "Los centinelas NIL deben ser negros", nil->color == Color::BLACK, "nil->color == BLACK"});
            report.checks.push_back({6, "Orden BST", "Subárbol izq < nodo < subárbol der", true, "Árbol vacío"});
            return report;
        }

        // P1: Cada nodo es rojo o negro
        bool p1 = true;
        // P3: Sin rojos consecutivos
        bool p3 = true;
        std::vector<std::string> p3Violations;

        // Recorrido BFS para validar todos los nodos
        std::queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* curr = q.front();
            q.pop();

            if (curr->color != Color::RED && curr->color != Color::BLACK) {
                p1 = false;
            }

            if (curr->color == Color::RED) {
                if (curr->left->color == Color::RED) {
                    p3 = false;
                    p3Violations.push_back("Nodo rojo " + std::to_string(curr->key) + 
                                          " tiene hijo izq rojo " + std::to_string(curr->left->key));
                }
                if (curr->right->color == Color::RED) {
                    p3 = false;
                    p3Violations.push_back("Nodo rojo " + std::to_string(curr->key) + 
                                          " tiene hijo der rojo " + std::to_string(curr->right->key));
                }
            }

            if (curr->left != nil) q.push(curr->left);
            if (curr->right != nil) q.push(curr->right);
        }

        // P2: La raíz es negra
        bool p2 = (root->color == Color::BLACK);

        // P4: Todos los caminos a NIL tienen igual cantidad de negros
        bool p4 = true;
        std::vector<std::string> p4Issues;
        computeBlackHeight(root, p4, p4Issues);
        report.blackHeight = getBlackHeight();

        // P5: Hojas NIL son negras
        bool p5 = (nil->color == Color::BLACK);

        // P6: Propiedad de orden BST
        bool p6 = true;
        std::vector<NodeInfo> ordered = inOrder();
        for (size_t i = 1; i < ordered.size(); ++i) {
            if (ordered[i].key <= ordered[i - 1].key) {
                p6 = false;
                break;
            }
        }

        report.checks.push_back({
            1, "Colores válidos", "Cada nodo es rojo o negro",
            p1, p1 ? "Todos los nodos tienen color RED o BLACK válido" : "Existen nodos con color indefinido"
        });

        report.checks.push_back({
            2, "Raíz negra", "La raíz debe ser negra",
            p2, p2 ? "root->color == BLACK (" + std::to_string(root->key) + ")" : "La raíz es ROJA (violación)"
        });

        std::string p3Detail = p3 ? "Ningún nodo rojo posee hijo rojo en todo el árbol" : "";
        for (const auto& v : p3Violations) p3Detail += v + "; ";
        report.checks.push_back({
            3, "Sin rojos consecutivos", "Si un nodo es rojo, sus hijos son negros",
            p3, p3Detail
        });

        std::string p4Detail = p4 ? "Todos los caminos desde cualquier nodo a NIL tienen idéntico black-height (BH raíz = " + std::to_string(report.blackHeight) + ")" : "";
        for (const auto& issue : p4Issues) p4Detail += issue + "; ";
        report.checks.push_back({
            4, "Equilibrio de altura negra", "Todo camino a hoja NIL tiene el mismo número de nodos negros",
            p4, p4Detail
        });

        report.checks.push_back({
            5, "Hojas NIL negras", "Las hojas centinela NIL son negras",
            p5, p5 ? "nil->color == BLACK verificado" : "nil->color != BLACK"
        });

        report.checks.push_back({
            6, "Propiedad BST", "In-order estrictamente ascendente (izq < clave < der)",
            p6, p6 ? "Cumple rigurosamente el orden de BST (" + std::to_string(ordered.size()) + " elementos en orden)" : "Violación del orden de BST"
        });

        report.allValid = (p1 && p2 && p3 && p4 && p5 && p6);
        return report;
    }

    size_t size() const {
        return nodeCount;
    }

    bool empty() const {
        return root == nil;
    }

    void printTree(std::ostream& os = std::cout) const {
        if (root == nil) {
            os << "(Árbol vacío)\n";
            return;
        }
        printTreeHelper(root, "", false, os);
    }
};

#endif // RED_BLACK_TREE_HPP
