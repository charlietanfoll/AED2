# Árvore B em Memória Secundária (AED II)

Projeto de implementação de uma **Árvore B de ordem $M$ residente em disco (memória secundária)** com as operações iterativas de busca (`mSearch`), inserção (`insertB`), remoção (`deleteB`), gerenciamento de espaço (*garbage collection*) e métricas de acessos a disco (I/O).

---

## 📖 Como Gerar e Ler a Documentação com o Doxyfile

O projeto conta com documentação completa em padrão **Doxygen** diretamente no código-fonte ([Arvoreb.cppm](file:///C:/Users/charl/Documents/Projetos/AED1/AED2/Arvoreb.cppm) e [main.cpp](file:///C:/Users/charl/Documents/Projetos/AED1/AED2/main.cpp)), detalhando funcionalidades, parâmetros, pré-condições e pós-condições.

Todas as configurações necessárias já estão definidas no arquivo [`Doxyfile`](file:///C:/Users/charl/Documents/Projetos/AED1/AED2/Doxyfile) na raiz do repositório.

---

### 1. Pré-requisito: Instalação do Doxygen

Se você ainda não possui o Doxygen instalado no Windows, pode instalá-lo rapidamente via terminal:

#### Opção A: Usando o Gerenciador de Pacotes do Windows (`winget` - Recomendado)
Abra o PowerShell ou Prompt de Comando e execute:
```powershell
winget install -e --id DimitrivanHeesch.Doxygen
```

#### Opção B: Download Manual
Baixe o instalador oficial no site: [https://www.doxygen.nl/download.html](https://www.doxygen.nl/download.html) e siga o assistente de instalação.

---

### 2. Gerando a Documentação

Com o Doxygen instalado, abra o terminal na pasta raiz do projeto e execute:

```powershell
doxygen Doxyfile
```

O Doxygen irá varrer os arquivos-fonte do projeto (`.cppm`, `.cpp`, `.h`, `.md`) e compilar a documentação no diretório configurado (`doc/html`).

---

### 3. Abrindo e Lendo a Documentação

Após a execução do comando, a documentação estará gerada em formato HTML interativo.

Para abrir no seu navegador padrão:

#### Pelo Terminal (PowerShell):
```powershell
Start-Process "doc/html/index.html"
```
*(ou simplesmente `start doc/html/index.html`)*

#### Pelo Explorador de Arquivos:
1. Acesse a pasta `doc/html/` dentro do projeto.
2. Dê um duplo clique no arquivo **`index.html`**.

---

### 4. O que você encontrará na Documentação

Na interface web gerada pelo Doxygen, você pode navegar pelas seguintes seções:

1. **Classes e Estruturas:**
   - [`Btree`](file:///C:/Users/charl/Documents/Projetos/AED1/AED2/Arvoreb.cppm): Classe principal com os métodos `mSearch`, `insertB`, `deleteB` e `imprimirArvore`.
   - [`Node`](file:///C:/Users/charl/Documents/Projetos/AED1/AED2/Arvoreb.cppm): Encapsulamento dos blocos lidos e gravados em disco via buffer contíguo e `std::span`.
   - [`header`](file:///C:/Users/charl/Documents/Projetos/AED1/AED2/Arvoreb.cppm): Metadados persistentes em disco (RRN da raiz, ordem $M$, topo da lixeira).
   - [`caminhoDaBusca`](file:///C:/Users/charl/Documents/Projetos/AED1/AED2/Arvoreb.cppm): Estrutura retornada por `mSearch` com a pilha de nós visitados e o contador de leituras a disco.

2. **Detalhes dos Métodos:**
   - **`@brief`**: Descrição resumida da funcionalidade.
   - **`@param`**: Descrição detalhada de cada parâmetro de entrada.
   - **`@return`**: Descrição do retorno (incluindo o uso de `std::optional<int>` para métricas de I/O de disco).
   - **`@pre`**: Pré-condições necessárias antes da execução.
   - **`@post`**: Garantias e pós-condições do estado da Árvore B e do disco após a execução.

3. **Código-fonte com Navegação Cruzada:**
   - O Doxygen cria links cruzados permitindo inspecionar onde cada estrutura é chamada e referenciada.

---

## ⚙️ Como Compilar e Executar o Projeto

O projeto utiliza C++23 e pode ser compilado tanto diretamente pelo **CLion** quanto via linha de comando com o compilador MinGW/GCC:

```powershell
# Compilação via g++ (C++23)
g++ -std=c++23 -Wall -Wextra main.cpp -o AED2.exe

# Execução
.\AED2.exe
```
