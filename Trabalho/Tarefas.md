# Tarefas - Trabalho 1: Árvore B (AED II)

## Requisitos Obrigatórios

### 1. Estrutura e Paradigma
- [ ] Implementar obrigatoriamente usando **classes em C++** (Tipos Abstratos de Dados).
- [ ] A árvore deve residir em **memória secundária (disco)**.
- [ ] A raiz **não** deve ser variável global e deve ser persistente entre execuções.

### 2. Métodos da Árvore B
- [ ] Implementar `mSearch` (Busca):
    - [ ] Deve ser **iterativo** (sem recursão).
    - [ ] Deve retornar uma estrutura apropriada (ex: tupla), sem imprimir no terminal.
    - [ ] Deve realizar o **empilhamento estruturado** dos nós visitados (para evitar releituras do pai).
- [ ] Implementar `insertB` (Inserção):
    - [ ] Deve ser **iterativo**.
    - [ ] Deve obrigatoriamente chamar o `mSearch`.
- [ ] Implementar `deleteB` (Remoção):
    - [ ] Deve ser **iterativo**.
    - [ ] Deve obrigatoriamente chamar o `mSearch`.
- [ ] **Documentação:** Cada método deve ter comentários com parâmetros, funcionalidades, pré e pós-condições.

### 3. Gerenciamento de Disco (Eficiência)
- [ ] Manter o arquivo aberto durante as operações (evitar abrir/fechar a cada nó).
- [ ] Leitura e escrita de nós devem ser feitas em **um único comando para a struct inteira** (tamanho fixo).
- [ ] A **ordem da árvore (m)** deve estar em um cabeçalho separado, não dentro de cada nó.

### 4. Interface de Uso
- [ ] Menu interativo em loop.
- [ ] Funcionalidade: Imprimir o índice (Árvore B).
- [ ] Funcionalidade: Imprimir conteúdo do arquivo principal.
- [ ] Funcionalidade: Buscar elemento (exibindo a tupla de retorno).
- [ ] Funcionalidade: Inserir elemento.
- [ ] Funcionalidade: Remover elemento.
- [ ] Tratar mensagens de erro e exceções.

---

## Requisitos Opcionais / Bônus

- [ ] **Arquivo Principal:** Indexar um campo chave de uma aplicação prática (ex: pacientes, inventário).
- [ ] **Parametrização Dinâmica:** Permitir definir a ordem `m` no início da execução.
- [ ] **Estatísticas de Acesso:** Contagem de leituras e escritas em disco.
- [ ] **Coleta de Lixo em Disco (Garbage Collection):** Reaproveitar espaço de nós removidos.
- [ ] **Análise Empírica:** Relatório de eficiência variando a ordem `m`.

---

## Entrega e Apresentação
- [ ] Grupo de até 3 alunos.
- [ ] Nomes e Números USP em todos os arquivos-fonte.
- [ ] Preparar slides para apresentação (15-17 min).
- [ ] Entrega em arquivo .ZIP no e-Disciplinas.
