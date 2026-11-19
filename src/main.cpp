#include <iostream>
#include <iomanip>
#include <vector>
#include "../include/BST.hpp"
#include "../include/RedBlackTree.hpp"

void printSeparator(const std::string& title) {
    std::cout << "\n======================================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "======================================================================\n";
}

void printSubSeparator(const std::string& subtitle) {
    std::cout << "\n--- " << subtitle << " ---\n";
}

int main() {
    std::cout << "\n";
    std::cout << "**********************************************************************\n";
    std::cout << "* PROGRAMACIÓN III (CS2013) - TAREA #6: ÁRBOLES Y GRAFOS             *\n";
    std::cout << "* EJERCICIO 1: BST Iterativo y Análisis de Degradación              *\n";
    std::cout << "* PROBLEMA 2:  Red-Black Tree, Propiedades Formales y Comparación    *\n";
    std::cout << "**********************************************************************\n";

    // ======================================================================
    // EJERCICIO 1: BST ITERATIVO
    // ======================================================================
    printSeparator("EJERCICIO 1: BST ITERATIVO");

    BST<int> bst;
    std::vector<int> datos20 = {50, 30, 70, 20, 40, 60, 80, 15, 25, 35, 45, 55, 65, 75, 85, 10, 28, 38, 52, 90};

    std::cout << "1. Inserción iterativa de 20 elementos:\n   [ ";
    for (int v : datos20) {
        bst.insert(v);
        std::cout << v << " ";
    }
    std::cout << "]\n";

    std::cout << "\n2. Altura calculada de forma iterativa (BFS con std::queue): " 
              << bst.getHeight() << " niveles (aristas en camino más largo: " 
              << bst.getHeight() - 1 << ")\n";
    std::cout << "   Total de nodos en el BST: " << bst.size() << "\n";

    printSubSeparator("Recorrido In-Order iterativo (usando std::stack) antes de eliminar");
    std::vector<int> inOrder1 = bst.inOrder();
    std::cout << "In-Order (" << inOrder1.size() << " elementos): ";
    for (int v : inOrder1) std::cout << v << " ";
    std::cout << "\n";

    printSubSeparator("Eliminación iterativa de 2 elementos");
    int elem1 = 10; // Caso 0 o 1 hijo (hoja)
    int elem2 = 30; // Caso 2 hijos (se reemplaza por sucesor in-order)

    std::cout << "-> Eliminando elemento " << elem1 << " (nodo hoja)... " 
              << (bst.remove(elem1) ? "EXITOSO" : "FALLÓ") << "\n";
    std::cout << "-> Eliminando elemento " << elem2 << " (nodo con 2 hijos, sucesor in-order)... " 
              << (bst.remove(elem2) ? "EXITOSO" : "FALLÓ") << "\n";

    std::cout << "\n3. Estado tras las eliminaciones:\n";
    std::cout << "   Nodos restantes: " << bst.size() << "\n";
    std::cout << "   Nueva altura iterativa: " << bst.getHeight() << " niveles\n";

    printSubSeparator("Recorrido In-Order iterativo (std::stack) posterior a la eliminación");
    std::vector<int> inOrder2 = bst.inOrder();
    std::cout << "In-Order (" << inOrder2.size() << " elementos): ";
    for (int v : inOrder2) std::cout << v << " ";
    std::cout << "\n";

    printSubSeparator("Visualización Gráfica ASCII del BST");
    bst.printTree();

    // ======================================================================
    // PROBLEMA 2: RED-BLACK TREE
    // ======================================================================
    printSeparator("PROBLEMA 2: RED-BLACK TREE (PROPIEDADES FORMALES)");

    RedBlackTree<int> rbt;
    std::cout << "1. Inserción en Red-Black Tree con rotaciones (izq/der) y recoloración:\n   [ ";
    for (int v : datos20) {
        rbt.insert(v);
        std::cout << v << " ";
    }
    std::cout << "]\n";

    std::cout << "\n2. Altura del Red-Black Tree: " << rbt.getHeight() << " niveles\n";
    std::cout << "   Altura Negra (Black-Height) de la raíz: " << rbt.getBlackHeight() << "\n";

    printSubSeparator("Recorrido Pre-Orden (para graficar los nodos: clave y color)");
    auto preOrderRBT = rbt.preOrder();
    std::cout << "Pre-Orden (Clave[Color]):\n";
    for (size_t i = 0; i < preOrderRBT.size(); ++i) {
        std::cout << preOrderRBT[i].key 
                  << (preOrderRBT[i].color == Color::RED ? "[R] " : "[B] ");
        if ((i + 1) % 10 == 0) std::cout << "\n";
    }
    std::cout << "\n";

    printSubSeparator("Verificación exhaustiva de las 5 Propiedades Formales (+ BST)");
    auto report = rbt.verifyProperties();
    report.print();

    printSubSeparator("Visualización Gráfica ASCII del Red-Black Tree");
    rbt.printTree();

    // ======================================================================
    // COMPARACIÓN EXPERIMENTAL: BST vs. RED-BLACK TREE
    // ======================================================================
    printSeparator("COMPARACIÓN EXPERIMENTAL: BST vs RED-BLACK TREE (20 ELEMENTOS ASCENDENTES)");
    std::cout << "Se insertan los siguientes 20 elementos estrictamente en orden ascendente:\n";
    std::vector<int> asc20;
    std::cout << "Datos: [ ";
    for (int i = 1; i <= 20; ++i) {
        asc20.push_back(i);
        std::cout << i << " ";
    }
    std::cout << "]\n\n";

    BST<int> bstAsc;
    RedBlackTree<int> rbtAsc;

    for (int x : asc20) {
        bstAsc.insert(x);
        rbtAsc.insert(x);
    }

    int bstHeight = bstAsc.getHeight();
    int rbtHeight = rbtAsc.getHeight();

    std::cout << "+-----------------------+--------------------+--------------------+\n";
    std::cout << "| Métrica               | BST Convencional   | Red-Black Tree     |\n";
    std::cout << "+-----------------------+--------------------+--------------------+\n";
    std::cout << "| Elementos insertados  | " << std::setw(18) << bstAsc.size() 
              << " | " << std::setw(18) << rbtAsc.size() << " |\n";
    std::cout << "| Altura (niveles)      | " << std::setw(18) << bstHeight 
              << " | " << std::setw(18) << rbtHeight << " |\n";
    std::cout << "| Altura (aristas)      | " << std::setw(18) << (bstHeight - 1) 
              << " | " << std::setw(18) << (rbtHeight - 1) << " |\n";
    std::cout << "| Cota teórica O(...)   | " << std::setw(18) << "O(n)" 
              << " | " << std::setw(18) << "O(log n)" << " |\n";
    std::cout << "| Cota superior 2log(n) | " << std::setw(18) << "N/A" 
              << " | " << std::setw(18) << "<= 8.78" << " |\n";
    std::cout << "+-----------------------+--------------------+--------------------+\n";

    printSubSeparator("Recorridos Pre-Orden para Graficación de los árboles resultantes");
    std::cout << "BST Pre-Orden (degenerado a lista): \n";
    for (int v : bstAsc.preOrder()) std::cout << v << " -> ";
    std::cout << "NIL\n\n";

    std::cout << "Red-Black Tree Pre-Orden (balanceado): \n";
    for (const auto& item : rbtAsc.preOrder()) {
        std::cout << item.key << (item.color == Color::RED ? "[R] " : "[B] ");
    }
    std::cout << "\n";

    printSubSeparator("Verificación formal del Red-Black Tree con datos ordenados");
    auto ascReport = rbtAsc.verifyProperties();
    ascReport.print();

    printSubSeparator("Estructura visual del Red-Black Tree (20 elementos ordenados)");
    rbtAsc.printTree();

    printSeparator("PROBLEMA 3: DFS ITERATIVO");

    BST<int> arbolRecorridos;
    int datosRecorridos[] = {10, 5, 15, 3, 7, 12, 18};
    std::cout << "Datos insertados: ";
    for (int valor : datosRecorridos) {
        arbolRecorridos.insert(valor);
        std::cout << valor << " ";
    }
    std::cout << "\n";

    int sumas[] = {18, 37, 15, 100};
    std::stack<int> camino;
    for (int k : sumas) {
        std::cout << "K = " << k << ": ";
        if (arbolRecorridos.findPathSum(k, camino)) {
            // Invertimos la pila para imprimir desde la raiz hasta la hoja.
            std::stack<int> orden;
            while (!camino.empty()) {
                orden.push(camino.top());
                camino.pop();
            }
            while (!orden.empty()) {
                std::cout << orden.top() << " ";
                orden.pop();
            }
            std::cout << "\n";
        } else {
            std::cout << "No existe un camino raiz-hoja con esa suma.\n";
        }
    }

    printSeparator("PROBLEMA 4: BFS ITERATIVO");
    std::cout << "Profundidad en aristas: raiz = 0, arbol vacio = -1.\n";
    std::cout << "Arbol con los 7 datos del problema 3:\n";
    std::cout << "Completo: " << (arbolRecorridos.isComplete() ? "Si" : "No") << "\n";
    std::cout << "Ancho maximo: " << arbolRecorridos.getMaxWidth() << "\n";
    std::cout << "Profundidad minima: " << arbolRecorridos.getMinDepth() << "\n";

    arbolRecorridos.remove(12);
    std::cout << "\nDespues de eliminar 12 (queda un espacio antes de 18):\n";
    std::cout << "Completo: " << (arbolRecorridos.isComplete() ? "Si" : "No") << "\n";
    std::cout << "Ancho maximo: " << arbolRecorridos.getMaxWidth() << "\n";
    std::cout << "Profundidad minima: " << arbolRecorridos.getMinDepth() << "\n";

    BST<int> arbolDesigual;
    int datosDesiguales[] = {10, 5, 15, 3, 2};
    for (int valor : datosDesiguales) arbolDesigual.insert(valor);
    std::cout << "\nArbol con datos 10 5 15 3 2 (hojas a distinta profundidad):\n";
    std::cout << "Completo: " << (arbolDesigual.isComplete() ? "Si" : "No") << "\n";
    std::cout << "Ancho maximo: " << arbolDesigual.getMaxWidth() << "\n";
    std::cout << "Profundidad minima: " << arbolDesigual.getMinDepth() << "\n";

    BST<int> arbolVacio;
    std::cout << "\nArbol vacio:\n";
    std::cout << "Completo: " << (arbolVacio.isComplete() ? "Si" : "No") << "\n";
    std::cout << "Ancho maximo: " << arbolVacio.getMaxWidth() << "\n";
    std::cout << "Profundidad minima: " << arbolVacio.getMinDepth() << "\n";

    printSeparator("FIN DE LA EJECUCIÓN");
    return 0;
}

