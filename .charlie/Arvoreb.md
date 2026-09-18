Em uma **B-tree em memória secundária (disco)**, seguindo a implementação atual em `Arvoreb.cppm`, existem **dois tipos principais de registros/blocos gravados no arquivo**:

1. **Cabeçalho da B-tree (`head`)**
2. **Nós da B-tree (`node`)**

---

## 1. Cabeçalho da B-tree (`struct head`)

O cabeçalho fica no início do arquivo e armazena os metadados da estrutura.

```c++
struct head {
  int root = -1;              // Índice do nó raiz atual.
  int quantidadeDeVias = -1;  // Ordem M (K vias = M, e K - 1 chaves).
  int lixeira = -1;           // Índice da Pilha de registros apagados.
};
```

### Para que serve cada campo?

- **root**
    - Indica a posição (RRN/Índice) do nó raiz no arquivo. Inicializado como `-1` se a árvore estiver vazia.
- **quantidadeDeVias**
    - Define a ordem `M` da árvore.
    - Se a ordem é `M`, cada nó pode ter até `M - 1` chaves e `M` vias (filhos).
- **lixeira**
    - Armazena o índice do primeiro nó em uma lista ligada de espaços reaproveitáveis (registros apagados).

---

## 2. Registro de nó da B-tree (`struct node`)

Diferente do modelo tradicional de tamanho fixo estático, a implementação atual utiliza vetores, o que requer atenção na serialização para o disco.

```c++
struct node {
  int chavesEscritas;
  bool isFolha;
  vector<int> chaves; // Numero de Nós - 1
  vector<int> nos;    // K vias (ponteiros para filhos)
  vector<int> dados;  // Índice do registro no arquivo de dados principal.
};
```

### Campos do nó

- **chavesEscritas**
    - Indica a quantidade de chaves atualmente armazenadas no nó. Substitui o antigo `quantidadeChaves`.
- **isFolha**
    - Booleano que indica se o nó é uma folha (`true`) ou um nó interno (`false`).
- **chaves**
    - Vetor que armazena os valores das chaves.
- **nos**
    - Vetor de "vias" ou ponteiros (RRNs) para os nós filhos. Equivale ao campo `filhos` em outras implementações.
- **dados**
    - Vetor que guarda a posição do registro correspondente em um arquivo de dados externo (índice secundário).

---

## Diferenças Importantes e Observações Técnicas

### 1. Dinamicidade e Cálculo de Tamanho
A classe `btree` possui um método `calcularNodeSize()` que sugere que o tamanho do registro é calculado em tempo de execução, permitindo ordens variáveis:
```c++
void calcularNodeSize() {
  // Calcula o tamanho dos nodes em tempo de execução
  registroSize = (sizeof(head) + (sizeof(head.quantidadeDeVias) * 2) + 7);
};
```
*Nota: A fórmula acima no código parece ser um esboço e deve ser validada conforme a lógica de escrita em disco.*

### 2. Uso de `vector` vs Gravação em Bloco
Diferente do aviso no arquivo original, o código utiliza `std::vector`. Para gravar esses nós no disco de forma correta, é necessário serializar os dados manualmente ou garantir que o espaço seja pré-alocado, já que `sizeof(node)` não retornará o tamanho dos dados contidos nos vetores, apenas o tamanho da estrutura de controle do vetor.

### 3. Persistência do Cabeçalho
O cabeçalho é lido no construtor e atualizado automaticamente no disco pelo destruidor da classe `~btree()`:
```c++
~btree() {
  file.write(reinterpret_cast<char *>(&head), sizeof(head));
  file.close();
}
```
Isso garante que informações como a nova raiz ou a lixeira sejam salvas ao fechar o programa.
