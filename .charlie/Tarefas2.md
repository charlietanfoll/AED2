# Tarefas - Trabalho 1: Árvore B (AED II) (Versão 2)

## Status Geral do Projeto
- **Árvore B (Núcleo Algorítmico):** ~85% implementada em `Arvoreb.cppm` (busca iterativa com pilha, inserção com split, remoção com redistribuição/fusão e coleta de lixo).
- **Integração com Arquivo Principal (`livros.dat`):** Pendente.
- **Interface de Usuário (`main.cpp`):** Pendente.

---

## 1. Estrutura e Paradigma
- [x] Implementar obrigatoriamente usando **classes em C++** (Tipos Abstratos de Dados).
- [x] A árvore deve residir em **memória secundária (disco)**.
- [x] A raiz **não** deve ser variável global e deve ser persistente entre execuções (armazenada no `header` em disco).
- [ ] **Adaptação para Arquivo Principal ($B_i$):**
  - O nó atual possui $K_i$ e $A_i$. Para associar o ISBN ao livro no arquivo principal, incluir o RRN/offset do registro principal ($B_i$) para cada chave no buffer do nó.

---

## 2. Métodos da Árvore B (`Btree`)
- [x] Implementar `mSearch` (Busca):
  - [x] Deve ser **iterativo** (sem recursão).
  - [x] Deve retornar uma estrutura apropriada (`caminhoDaBusca` contendo flag e pilha de nós), sem imprimir no terminal.
  - [x] Deve realizar o **empilhamento estruturado** dos nós visitados (evita releituras do pai durante split/rebalanceamento).
- [x] Implementar `insertB` (Inserção):
  - [x] Deve ser **iterativo**.
  - [x] Deve obrigatoriamente chamar o `mSearch`.
  - [x] Split (cisão) e propagação iterativa pela pilha de nós visitados.
  - [ ] *(Revisão)* Garantir cálculo do elemento mediano com teto $\lceil m/2 \rceil = (m + 1)/2$ de acordo com os slides de aula.
- [x] Implementar `deleteB` (Remoção):
  - [x] Deve ser **iterativo**.
  - [x] Deve obrigatoriamente chamar o `mSearch`.
  - [x] Troca pelo menor elemento da subárvore direita (sucessor) quando a chave está em nó interno.
  - [x] Rebalanceamento subindo pela pilha: empréstimo do irmão esquerdo/direito ou fusão (*merge*) de nós.
- [ ] **Impressão da Árvore B (`printTree`):**
  - [ ] Implementar método para imprimir o índice (Árvore B) de forma legível/hierárquica no terminal (por níveis ou RRNs).
- [x] **Documentação no código:** Comentários com descrição, parâmetros, funcionalidades, pré e pós-condições (padrão Doxygen).

---

## 3. Gerenciamento de Disco (Eficiência de I/O)
- [x] Manter o arquivo aberto durante as operações (evitar abrir/fechar a cada nó).
- [x] Leitura e escrita de nós feitas em **um único comando para o bloco/struct inteira** (`file.read` / `file.write` de tamanho fixo).
- [x] A ordem da árvore ($m$) deve estar no cabeçalho separado em disco, não dentro de cada nó.
- [x] **Coleta de Lixo em Disco (Garbage Collection):**
  - [x] Reaproveitar nós removidos usando pilha encadeada no disco (`header.pilhaDaLixeira`, `obterNovoId` e `moverParaLixeira`).
- [ ] **Estatísticas de Acesso a Disco (Bônus/Avaliação):**
  - [ ] Contadores de leituras e escritas em disco para medir acessos em buscas, inserções e remoções.

---

## 4. Arquivo Principal de Dados (Aplicação Prática: Catálogo de Livros)
- [ ] **Estrutura `Livro`:**
  - Campo chave: `int isbn` (ISBN numérico inteiro, conforme orientação).
  - Demais campos de tamanho fixo: título, autor, ano de publicação, editora, flag de registro ativo.
- [ ] **Gerenciador de Dados (`livros.dat`):**
  - Funções para adicionar livro, consultar por RRN, listar todos os livros cadastrados e marcar livro como excluído.
- [ ] **Sincronização:**
  - Inserção: grava livro em `livros.dat`, obtém seu RRN e insere `(isbn, rrnLivro)` no índice `livros.idx`.
  - Remoção: remove chave do índice e marca/desvincula no arquivo de dados.

---

## 5. Interface de Uso (`main.cpp`)
- [ ] Menu interativo em loop usando `switch/case`:
  - [ ] **Opção 1:** Inserir Livro (lê dados com ISBN inteiro, salva no arquivo principal e indexa na Árvore B).
  - [ ] **Opção 2:** Remover Livro (solicita ISBN, remove do índice e do arquivo de dados).
  - [ ] **Opção 3:** Buscar Livro por ISBN (executa `mSearch`, exibe a tupla/caminho e mostra os dados do livro encontrado).
  - [ ] **Opção 4:** Imprimir Índice (Árvore B).
  - [ ] **Opção 5:** Imprimir Arquivo Principal (`livros.dat`).
  - [ ] **Opção 6:** Exibir Estatísticas de I/O (leituras vs escritas em disco).
  - [ ] **Opção 0:** Sair do programa (persiste cabeçalho e fecha com segurança).
- [ ] Tratamento de mensagens de erro e exceções (limpeza de buffer de entrada, validação de duplicidade, chave não encontrada, etc.).

---

## 6. Entrega e Apresentação
- [ ] Adicionar nomes e Números USP de todos os integrantes no cabeçalho de todos os arquivos-fonte (`Arvoreb.cppm`, `main.cpp`, etc.).
- [ ] Gerar documentação Doxygen (`Doxyfile`).
- [ ] Preparar slides da apresentação (15 a 17 min) com foco em I/O de disco e decisões de projeto.
- [ ] Gerar arquivo `.zip` para envio no e-Disciplinas.
