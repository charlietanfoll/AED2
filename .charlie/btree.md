Em uma **B-tree em memória secundária (disco)**, normalmente existem **dois tipos principais de registros/blocos gravados no arquivo**:

1. **Cabeçalho do arquivo**
2. **Nós da árvore B**

Além disso, dependendo do trabalho, pode existir também um **arquivo principal de dados**, por exemplo com registros de livros, pacientes etc.

---

## 1. Cabeçalho da B-tree

O cabeçalho fica geralmente no **início do arquivo** e guarda informações globais da árvore.

Ele costuma ter:

```c++
struct Cabecalho {
    int ordem;          // ordem m da árvore B
    int raiz;           // posição/RRN da raiz no arquivo
    int proximoRRN;     // próximo índice livre para novo nó
    int lixeira;        // início da lista de nós removidos, se houver
};
```


### Para que serve cada campo?

- **ordem**
    - Quantidade máxima de filhos de um nó.
    - Se a ordem é `m`, cada nó pode ter até `m - 1` chaves e `m` filhos.

- **raiz**
    - Indica onde está o nó raiz no arquivo.
    - Importante para a árvore continuar funcionando depois que o programa fecha.

- **proximoRRN**
    - Indica qual será a próxima posição disponível para gravar um novo nó.

- **lixeira**
    - Usada para reaproveitar espaço de nós removidos.
    - Se você ainda não implementou remoção física/reuso, pode deixar como `-1`.

---

## 2. Registro de nó da B-tree

Cada nó da B-tree também é gravado no disco como um bloco de tamanho fixo.

Um nó normalmente contém:

```c++
struct NoBTree {
    int quantidadeChaves;      // número atual de chaves no nó
    bool folha;                // indica se o nó é folha
    int chaves[MAX_CHAVES];    // chaves armazenadas no nó
    int filhos[MAX_FILHOS];    // ponteiros/RRNs para os filhos
    int dados[MAX_CHAVES];     // ponteiros/RRNs para os registros no arquivo principal
};
```


---

## Campos essenciais de um nó

### 1. Quantidade de chaves

```c++
int quantidadeChaves;
```


Indica quantas chaves estão realmente ocupadas naquele nó.

Exemplo:

```plain text
quantidadeChaves = 3
chaves = [10, 25, 40, _, _]
```


Mesmo que o vetor tenha espaço para mais chaves, só as três primeiras são válidas.

---

### 2. Indicador de folha

```c++
bool folha;
```


Diz se o nó é uma folha ou não.

- `true`: o nó não possui filhos válidos.
- `false`: o nó possui filhos.

Isso facilita a busca, inserção e remoção.

---

### 3. Chaves

```c++
int chaves[MAX_CHAVES];
```


São os valores usados para navegar na árvore.

Exemplo:

```plain text
[10 | 25 | 40]
```


Essas chaves devem estar sempre **ordenadas dentro do nó**.

---

### 4. Ponteiros para filhos

```c++
int filhos[MAX_FILHOS];
```


Guardam as posições dos filhos no arquivo.

Se a ordem da árvore é `m`, o nó pode ter:

```plain text
m filhos
m - 1 chaves
```


Exemplo:

```plain text
filhos[0] -> chaves menores que 10
filhos[1] -> chaves entre 10 e 25
filhos[2] -> chaves entre 25 e 40
filhos[3] -> chaves maiores que 40
```


Esses ponteiros geralmente são **RRNs** ou offsets no arquivo.

---

### 5. Ponteiros para dados do arquivo principal

```c++
int dados[MAX_CHAVES];
```


Esse campo é muito importante se a B-tree for apenas um **índice**.

A árvore B guarda a chave, por exemplo:

```plain text
ISBN
```


Mas o registro completo do livro fica em outro arquivo.

Então o nó da B-tree guarda:

```plain text
chave -> posição do livro no arquivo principal
```


Exemplo:

```c++
chaves[0] = 12345;
dados[0] = 8;
```


Significa:

> A chave `12345` aponta para o registro de livro na posição `8` do arquivo principal.

---

## Resumo prático

Um nó de B-tree em disco geralmente precisa guardar:

```plain text
quantidade de chaves
se é folha
vetor de chaves
vetor de ponteiros para filhos
vetor de ponteiros para os registros de dados
```


E o cabeçalho do arquivo guarda:

```plain text
ordem da árvore
posição da raiz
próximo nó livre
lixeira / lista de nós removidos
```


---

## Exemplo visual

Para uma árvore de ordem `4`:

```plain text
Máximo de filhos: 4
Máximo de chaves: 3
```


Um nó poderia ser:

```plain text
quantidadeChaves = 2
folha = false

chaves:
[20, 50, _]

dados:
[rrn_dado_20, rrn_dado_50, _]

filhos:
[filho0, filho1, filho2, -1]
```


Interpretação:

```plain text
filho0 -> chaves menores que 20
filho1 -> chaves entre 20 e 50
filho2 -> chaves maiores que 50
```


---

## Atenção importante

Se a exigência é ler/escrever o nó inteiro em **um único comando**, evite usar `vector` dentro da struct que vai para o disco.

Ou seja, isto **não é ideal para gravar direto em binário**:

```c++
struct No {
    int total;
    vector<int> chaves;
    vector<int> filhos;
};
```


Porque `vector` guarda ponteiros internos, tamanho, capacidade etc. O conteúdo real fica em outra região da memória.

Para disco, prefira arrays de tamanho fixo:

```c++
const int ORDEM = 5;

struct No {
    int total;
    bool folha;
    int chaves[ORDEM - 1];
    int filhos[ORDEM];
    int dados[ORDEM - 1];
};
```


Se a ordem for dinâmica, você pode precisar calcular manualmente o tamanho do nó e serializar campo por campo, ou definir um limite máximo de ordem.
