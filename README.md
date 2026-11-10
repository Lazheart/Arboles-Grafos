# Tarea #6: Árboles y Grafos

**Curso:** Programación III (CS2013) — UTEC 2026-2  
**Profesor:** José A. Chávez Álvarez  
**Tema:** Árboles balanceados y recorridos iterativos  

---

## Estructura del Proyecto

```text
arboles-grafos/
├── include/
│   ├── BST.hpp            # Implementación iterativa del BST (inserción, eliminación, in-order, altura)
│   └── RedBlackTree.hpp   # Implementación del Red-Black Tree (rotaciones, fixup, verificación formal, altura)
├── src/
│   └── main.cpp           # Demostración experimental, comparativas y reportes de validación
├── Makefile               # Automatización de compilación con flags estrictos (-Wall -Wextra -O2)
└── README.md              # Documentación, análisis matemático formal y resultados experimentales
```

### Compilación y Ejecución

```bash
# Compilar y ejecutar
make run

# Solo compilar
make

# Limpiar binarios
make clean
```

---

## Ejercicio 1: BST Iterativo y Análisis de Degradación

Implementar un Árbol Binario de Búsqueda con:

- [x] Inserción iterativa (`BST::insert`).
- [x] Eliminación iterativa (`BST::remove` contemplando 0, 1 y 2 hijos).
- [x] Recorrido in-order usando `std::stack` (`BST::inOrder`).
- [x] Método para calcular la altura del árbol de forma iterativa (`BST::getHeight` usando BFS con `std::queue`).

Luego:

- [x] Insertar 20 elementos, eliminar 2, recorrer con in-order y calcular la altura.
- [x] Explicar matemáticamente (sin intuiciones) por qué la complejidad puede degradarse a O(n) al insertar n elementos.

### Análisis Matemático Formal: Degradación de Complejidad a $\mathcal{O}(n)$

#### 1. Definición Formal del Árbol Binario de Búsqueda (BST)
Sea $T = (V, E)$ un árbol binario donde cada nodo $u \in V$ almacena una clave $key(u)$ perteneciente a un conjunto totalmente ordenado $(S, \le)$.  
La propiedad invariante del BST establece que para todo nodo $u \in V$:
- Si $v \in Subtree(u.left)$, entonces $key(v) < key(u)$.
- Si $w \in Subtree(u.right)$, entonces $key(w) > key(u)$.

#### 2. Modelo de Costo de Inserción
El algoritmo de inserción iterativa para una clave $x$ inicia en la raíz $r$ y desciende por el árbol comparando $x$ con el nodo actual. En cada paso de profundidad $d$, se ejecuta exactamente una comparación y una asignación de puntero. Si el nodo se inserta a profundidad $d(x)$, el número de operaciones elementales es:
$$C(x) = d(x) + 1$$
El costo total para insertar una secuencia de $n$ claves $S = \langle x_1, x_2, \dots, x_n \rangle$ es:
$$T(n) = \sum_{i=1}^{n} C(x_i) = \sum_{i=1}^{n} (d(x_i) + 1)$$

#### 3. Teorema de Degradación en Secuencias Monótonas
**Teorema:** Sea $S = \langle x_1, x_2, \dots, x_n \rangle$ una secuencia estrictamente creciente, es decir, $x_1 < x_2 < \dots < x_n$. La inserción de $S$ en un BST inicialmente vacío genera un árbol degenerado de altura $h = n - 1$ con un costo total de inserción $T(n) = \Theta(n^2)$, lo que implica un costo individual en el peor caso de $\Theta(n) = \mathcal{O}(n)$ por inserción.

**Demostración por Inducción Matemática sobre el número de elementos insertados $k$:**
- **Base Inductiva ($k = 1$):**  
  Se inserta $x_1$. La raíz es $r = x_1$ a profundidad $d(x_1) = 0$. El árbol tiene altura $h = 0$. La proposición se cumple trivialmente.
- **Hipótesis Inductiva ($k = m$):**  
  Supóngase que tras insertar los primeros $m$ elementos ($m \ge 1$), el árbol consiste en una cadena lineal derecha única:
  $$x_1.right = x_2, \; x_2.right = x_3, \; \dots, \; x_{m-1}.right = x_m$$
  donde $x_j.left = \text{NIL}$ para todo $1 \le j \le m$. La profundidad del nodo $x_j$ es $d(x_j) = j - 1$, la altura del árbol es $h = m - 1$, y la hoja es $x_m$.
- **Paso Inductivo ($k = m + 1$):**  
  Se inserta el elemento $x_{m+1}$.  
  Por hipótesis de monotonicidad, $x_{m+1} > x_j$ para todo $1 \le j \le m$.  
  El algoritmo de inserción evalúa:
  1. Comparación con $x_1$: como $x_{m+1} > x_1$, se avanza a $x_1.right = x_2$.
  2. Comparación con $x_2$: como $x_{m+1} > x_2$, se avanza a $x_2.right = x_3$.
  3. $\dots$
  4. Comparación con $x_m$: como $x_{m+1} > x_m$, se inspecciona $x_m.right$.  
  Dado que $x_m.right = \text{NIL}$, la búsqueda termina y se asigna $x_m.right = x_{m+1}$.  
  La profundidad del nuevo nodo es:
  $$d(x_{m+1}) = d(x_m) + 1 = (m - 1) + 1 = m$$
  El número de comparaciones realizadas durante esta inserción es exactamente $m + 1$.  
  El nuevo árbol resultante conserva todos los punteros izquierdos nulos ($x_j.left = \text{NIL}$) y una cadena derecha de longitud $m + 1$, con nueva altura $h = m$. Queda demostrada la topología lineal.

#### 4. Cálculo de la Complejidad Acumulada
Sumando el costo de inserción para los $n$ elementos:
$$T(n) = \sum_{k=1}^{n} (d(x_k) + 1) = \sum_{k=1}^{n} k = \frac{n(n + 1)}{2} = \frac{1}{2}n^2 + \frac{1}{2}n$$

Por definición de notación asintótica:
$$\lim_{n \to \infty} \frac{T(n)}{n^2} = \frac{1}{2} \implies T(n) = \Theta(n^2)$$

Por lo tanto:
- El costo promedio por inserción es $\frac{T(n)}{n} = \frac{n+1}{2} = \Theta(n)$.
- El costo de la $n$-ésima inserción es $C(x_n) = n = \Theta(n) = \mathcal{O}(n)$.
- La altura resultante es $h = n - 1 = \Theta(n)$.  
El árbol es estructuralmente isomorfo a una **lista enlazada simple**, degradando la complejidad de búsqueda, inserción y eliminación de $\mathcal{O}(\log n)$ a $\mathcal{O}(n)$.

---

## Problema 2: Red-Black Tree

Propiedades formales que cumple la implementación:

1. Cada nodo es **rojo** o **negro**.
2. La **raíz** es negra.
3. Ningún nodo rojo puede tener un hijo rojo (**propiedad de rojo**).
4. Todo camino simple desde cualquier nodo hasta sus hojas nulas ($\text{NIL}$) contiene el mismo número de nodos negros (**propiedad de altura negra**).
5. Las hojas nulas ($\text{NIL}$) son **negras**.

Implementación realizada:

- [x] Rotación izquierda (`RedBlackTree::rotateLeft`).
- [x] Rotación derecha (`RedBlackTree::rotateRight`).
- [x] Recoloración (`insertFixup` Caso 1).
- [x] Corrección de incumplimientos de reglas (`insertFixup` Casos 1, 2 y 3).
- [x] Método que verifique las propiedades formales (`RedBlackTree::verifyProperties`).
- [x] Método que calcule la altura (`RedBlackTree::getHeight` y `RedBlackTree::getBlackHeight`).
- [x] Insertar 20 elementos en orden ascendente y comparar la altura del BST del Ejercicio 1 con la del Red-Black Tree usando los mismos datos.
- [x] Imprimir el recorrido Pre-Orden para graficar los árboles (`RedBlackTree::preOrder`).
- [x] Explicar matemáticamente por qué la altura está acotada por $\mathcal{O}(\log n)$.

---

### Análisis Matemático Formal: Cota de Altura $\mathcal{O}(\log n)$ en Red-Black Trees

#### 1. Definición de Altura Negra ($bh$)
Para cualquier nodo $x$ en un Red-Black Tree, la **altura negra** $bh(x)$ se define formalmente como el número de nodos negros en cualquier camino simple desde $x$ (sin incluir a $x$) hasta una hoja centinela $\text{NIL}$.  
Por la **Propiedad 4**, todos los caminos simples descendientes desde $x$ poseen idéntica cantidad de nodos negros, por lo que $bh(x)$ es una función bien definida en todos los nodos.

---

#### 2. Lema 1: Cota Inferior del Número de Nodos Internos
**Lema 1:** Para todo nodo $x$, el subárbol con raíz en $x$ contiene al menos $2^{bh(x)} - 1$ nodos internos.

**Demostración por Inducción Estructural sobre la altura $h(x)$ de $x$:**
- **Caso Base ($h(x) = 0$):**  
  Si $h(x) = 0$, $x$ es una hoja nula centinela $\text{NIL}$.  
  Por definición, un centinela no contiene claves internas, por lo que su número de nodos internos es $size(x) = 0$.  
  Su altura negra es $bh(x) = 0$.  
  Evaluando la fórmula:
  $$2^{bh(x)} - 1 = 2^0 - 1 = 1 - 1 = 0$$
  La igualdad se cumple de manera exacta.
- **Paso Inductivo ($h(x) > 0$):**  
  Sea $x$ un nodo interno con altura $h(x) > 0$. Sus dos hijos son $x.left$ y $x.right$.  
  Dado que $h(x.left) < h(x)$ y $h(x.right) < h(x)$, por Hipótesis Inductiva el número de nodos internos en cada subárbol cumple:
  $$size(x.left) \ge 2^{bh(x.left)} - 1$$
  $$size(x.right) \ge 2^{bh(x.right)} - 1$$
  
  Determinemos $bh(x.left)$ y $bh(x.right)$ en función de $bh(x)$:
  - Si el hijo $c \in \{x.left, x.right\}$ es de color **negro**, entonces $bh(c) = bh(x) - 1$.
  - Si el hijo $c \in \{x.left, x.right\}$ es de color **rojo**, entonces $bh(c) = bh(x)$.  
  
  En cualquier caso:
  $$bh(c) \ge bh(x) - 1$$

  El número total de nodos internos en el subárbol de raíz $x$ es la suma del propio nodo $x$ más los nodos de sus dos subárboles:
  $$size(x) = 1 + size(x.left) + size(x.right)$$
  Aplicando la hipótesis inductiva:
  $$size(x) \ge 1 + (2^{bh(x.left)} - 1) + (2^{bh(x.right)} - 1)$$
  $$size(x) \ge 1 + (2^{bh(x)-1} - 1) + (2^{bh(x)-1} - 1)$$
  $$size(x) \ge 1 + 2 \cdot 2^{bh(x)-1} - 2$$
  $$size(x) \ge 2^{bh(x)} - 1$$

Queda rigurosamente demostrado el Lema 1 por inducción estructural. $\blacksquare$

---

#### 3. Lema 2: Relación entre Altura Total y Altura Negra
**Lema 2:** En un Red-Black Tree de altura $h$ con raíz $r$, la altura negra de la raíz satisface:
$$bh(r) \ge \frac{h}{2}$$

**Demostración:**  
Sea $P = \langle v_0, v_1, v_2, \dots, v_h \rangle$ el camino simple más largo desde la raíz $v_0 = r$ hasta una hoja centinela $v_h = \text{NIL}$, cuya longitud en aristas es $h$.  
Por la **Propiedad 3**, ningún nodo rojo puede tener un hijo rojo. Por consiguiente:
$$\forall i \in \{0, \dots, h-1\}: \; color(v_i) = \text{RED} \implies color(v_{i+1}) = \text{BLACK}$$
Esto implica que no pueden existir dos nodos rojos consecutivos en ningún camino. En consecuencia, a lo sumo la mitad de los nodos en cualquier camino simple pueden ser de color rojo.  
Por lo tanto, al menos la mitad de los nodos en el camino deben ser de color negro:
$$bh(r) \ge \frac{h}{2}$$
$\blacksquare$

---

#### 4. Teorema Principal: Cota Superior Logarítmica
**Teorema:** Un Red-Black Tree con $n$ nodos internos tiene una altura máxima acotada por:
$$h \le 2 \log_2(n + 1)$$

**Demostración:**  
Aplicando el **Lema 1** a la raíz $r$ del árbol con $n$ nodos internos:
$$n = size(r) \ge 2^{bh(r)} - 1$$
Sumando $1$ en ambos lados:
$$n + 1 \ge 2^{bh(r)}$$
Tomando el logaritmo en base 2 (función monótona estrictamente creciente):
$$\log_2(n + 1) \ge bh(r)$$
Sustituyendo la desigualdad del **Lema 2** ($bh(r) \ge \frac{h}{2}$):
$$\log_2(n + 1) \ge \frac{h}{2}$$
Multiplicando por 2:
$$h \le 2 \log_2(n + 1)$$

Como $\lim_{n \to \infty} \frac{2 \log_2(n + 1)}{\log_2(n)} = 2$, se concluye formalmente:
$$h = \mathcal{O}(\log n)$$

Dado que las operaciones de inserción, búsqueda y eliminación descienden como máximo por un camino de longitud $h$, y cada rotación o recoloración toma $\mathcal{O}(1)$ tiempo en cada nivel, **todas las operaciones del Red-Black Tree quedan garantizadas en tiempo $\mathcal{O}(\log n)$ en el peor caso**. $\blacksquare$

---

## Resultados Experimentales de la Ejecución

Al ejecutar `make run`, el programa `main.cpp` realiza las pruebas obligatorias descritas en la rúbrica:

### 1. Prueba de BST Iterativo (Ejercicio 1)
- **Inserción:** 20 elementos arbitrarios:  
  `[ 50 30 70 20 40 60 80 15 25 35 45 55 65 75 85 10 28 38 52 90 ]`
- **Altura inicial:** 5 niveles (4 aristas en el camino más largo).
- **Eliminación iterativa de 2 elementos:**
  - Elemento `10` (nodo hoja, 0 hijos).
  - Elemento `30` (nodo interno con 2 hijos, sustituido por su sucesor in-order `35`).
- **Recorrido In-Order con `std::stack` tras eliminación:**  
  `15 20 25 28 35 38 40 45 50 52 55 60 65 70 75 80 85 90` (estrictamente ordenado).
- **Altura tras eliminación:** 5 niveles.

---

### 2. Comparación Experimental: BST vs. Red-Black Tree (Problema 2)
Inserción de 20 elementos en orden estrictamente ascendente:  
`[ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 ]`

| Métrica / Estructura | BST Convencional (Ejercicio 1) | Red-Black Tree (Problema 2) | Explicación Formal |
| :--- | :---: | :---: | :--- |
| **Elementos insertados** | 20 | 20 | Mismo dataset de prueba |
| **Altura (niveles)** | **20** | **6** | Reducción de 70% en altura |
| **Altura (aristas)** | **19** | **5** | Máxima longitud de camino simple |
| **Cota Asintótica** | $\Theta(n)$ | $\mathcal{O}(\log n)$ | BST degenera; RBT se autobalancea |
| **Cota Superior Teórica** | $n$ | $\le 2 \log_2(21) \approx 8.78$ | Altura $6 \le 8.78$ se cumple con holgura |
| **Topología** | Lista enlazada unilateral | Árbol balanceado | Rotaciones izq/der y recoloración |

#### Recorridos Pre-Orden para Graficación

- **BST Pre-Orden (Degenerado):**
  ```text
  1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> 11 -> 12 -> 13 -> 14 -> 15 -> 16 -> 17 -> 18 -> 19 -> 20 -> NIL
  ```

- **Red-Black Tree Pre-Orden (Balanceado):**
  ```text
  8[B] 4[R] 2[B] 1[B] 3[B] 6[B] 5[B] 7[B] 12[R] 10[B] 9[B] 11[B] 16[B] 14[R] 13[B] 15[B] 18[R] 17[B] 19[B] 20[R]
  ```

#### Árbol Gráfico Red-Black Resultante (20 elementos ordenados)
```text
└── 8 [B]
    ├── 4 [R]
    │   ├── 2 [B]
    │   │   ├── 1 [B]
    │   │   └── 3 [B]
    │   └── 6 [B]
    │       ├── 5 [B]
    │       └── 7 [B]
    └── 12 [R]
        ├── 10 [B]
        │   ├── 9 [B]
        │   └── 11 [B]
        └── 16 [B]
            ├── 14 [R]
            │   ├── 13 [B]
            │   └── 15 [B]
            └── 18 [R]
                ├── 17 [B]
                └── 19 [B]
                    ├── NIL [B]
                    └── 20 [R]
```

---

## Problema 3: DFS Iterativo Avanzado

Usando exclusivamente `std::stack` (sin recursión):

- [ ] Determinar si existe un camino raíz-hoja cuya suma sea igual a K.
- [ ] Devolver dicho camino.
- [ ] Explicar por qué DFS es adecuado para este problema.

---

## Problema 4: BFS Iterativo Avanzado

Usando exclusivamente `std::queue` (sin recursión):

- [ ] Determinar si un árbol es completo.
- [ ] Calcular el ancho máximo del árbol.
- [ ] Determinar la profundidad mínima.
- [ ] Justificar formalmente por qué BFS es el enfoque natural para estas métricas.

---

## Criterios de Evaluación

- Correctitud estructural.
- Implementación adecuada de rotaciones.
- Verificación formal de propiedades.
- Uso correcto de `stack` y `queue`.
- Análisis de complejidad.
- Comparación experimental entre estructuras (usar la función `main` para comprobar las operaciones).
