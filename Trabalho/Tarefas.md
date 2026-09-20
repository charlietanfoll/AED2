# Tarefas - Trabalho 1: Árvore B (AED II)

## Status Geral do Projeto
- **Árvore B (Núcleo Algorítmico):** 100% implementada em `Arvoreb.cppm` (busca iterativa com pilha e contadores, inserção com split teto, remoção completa com redistribuição/fusão e coleta de lixo).
- **Interface de Usuário (`main.cpp`):** 100% implementada (menu em loop com `switch/case`, tratamento de exceções, parametrização dinâmica de $M$ e exibição dos contadores de acesso ao disco).
- **Integração com Arquivo Principal (`livros.dat`):** Não utilizada conforme orientação.

---

## 1. Estrutura e Paradigma
- [x] Implementar obrigatoriamente usando **classes em C++** (Tipos Abstratos de Dados).
- [x] A árvore deve residir em **memória secundária (disco)**.
- [x] A raiz **não** deve ser variável global e deve ser persistente entre execuções (armazenada no `header` em disco).

---

## 2. Métodos da Árvore B (`Btree`)
- [x] Implementar `mSearch` (Busca):
  - [x] Deve ser **iterativo** (sem recursão).
  - [x] Deve retornar uma estrutura apropriada (`caminhoDaBusca` contendo flag, contadores e pilha de nós), sem imprimir no terminal.
  - [x] Deve realizar o **empilhamento estruturado** dos nós visitados (evita releituras do pai durante split/rebalanceamento).
- [x] Implementar `insertB` (Inserção):
  - [x] Deve ser **iterativo**.
  - [x] Deve obrigatoriamente chamar o `mSearch`.
  - [x] Retorna `std::optional<int>` contendo o total de acessos a disco (ou `std::nullopt` se duplicado).
  - [x] Split (cisão) com elemento mediano baseado no teto $\lceil m/2 \rceil = (m + 1)/2$ e propagação iterativa pela pilha.
- [x] Implementar `deleteB` (Remoção):
  - [x] Deve ser **iterativo**.
  - [x] Deve obrigatoriamente chamar o `mSearch`.
  - [x] Retorna `std::optional<int>` contendo o total de acessos a disco (ou `std::nullopt` se inexistente).
  - [x] Troca pelo menor elemento da subárvore direita (sucessor) quando a chave está em nó interno.
  - [x] Rebalanceamento subindo pela pilha: empréstimo do irmão esquerdo/direito ou fusão (*merge*) com nós descartados na lixeira.
- [x] **Impressão da Árvore B (`imprimirArvore`):**
  - [x] Método implementado para imprimir a estrutura hierárquica por níveis (BFS) com chaves, filhos e folhas no terminal via `cout`.
- [x] **Documentação no código:** Comentários com descrição, parâmetros, funcionalidades, pré e pós-condições (padrão Doxygen).

---

## 3. Gerenciamento de Disco (Eficiência de I/O)
- [x] Manter o arquivo aberto durante as operações (evitar abrir/fechar a cada nó).
- [x] Leitura e escrita de nós feitas em **um único comando para o bloco/struct inteira** (`file.read` / `file.write` de tamanho fixo).
- [x] A ordem da árvore ($m$) deve estar no cabeçalho separado em disco, não dentro de cada nó.
- [x] **Coleta de Lixo em Disco (Garbage Collection):**
  - [x] Reaproveitar nós removidos usando pilha encadeada no disco (`header.pilhaDaLixeira`, `obterNovoId` e `moverParaLixeira`).
- [x] **Estatísticas de Acesso a Disco (Bônus/Avaliação):**
  - [x] Contadores de leituras e escritas em disco para medir acessos em buscas, inserções e remoções.

---

## 4. Interface de Uso (`main.cpp`)
- [x] Menu interativo em loop usando `switch/case`:
  - [x] **Opção 1:** Inserir Chave (insertB com retorno de acessos a disco).
  - [x] **Opção 2:** Remover Chave (deleteB com retorno de acessos a disco).
  - [x] **Opção 3:** Buscar Chave (mSearch exibindo a tupla de retorno, profundidade e leituras a disco).
  - [x] **Opção 4:** Imprimir Índice (Árvore B nível a nível).
  - [x] **Opção 5:** Exibir Metadados e Cabeçalho (RRN da raiz, ordem M, capacidades e topo da lixeira).
  - [x] **Opção 0:** Sair do programa (persiste cabeçalho e fecha com segurança).
- [x] Tratamento robusto de mensagens de erro e exceções (limpeza de buffer `cin`, validação de inteiros).
- [x] **Parametrização Dinâmica:** Permite configurar a ordem $M$ na criação inicial do arquivo em disco.

---

## 5. Entrega e Apresentação
- [ ] Adicionar nomes e Números USP dos integrantes no cabeçalho de todos os arquivos-fonte (`Arvoreb.cppm`, `main.cpp`).
- [ ] Gerar documentação Doxygen (`Doxyfile`).
- [ ] Preparar slides da apresentação (15 a 17 min) com foco em I/O de disco e decisões de projeto.
- [ ] Gerar arquivo `.zip` para envio no e-Disciplinas.
