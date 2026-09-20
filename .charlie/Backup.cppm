#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <span>
#include <memory>
#include <optional>
#include <utility>

using namespace std;

class Node;
class Btree;
struct header;
struct caminhoDaBusca;

//Um dia vai ser implementado!
struct data;

struct header {
    int root;
    int ordem; // ordem = M e chaves = M-1
    int pilhaDaLixeira;
};

struct caminhoDaBusca {
    vector<int> posicoesInternas;
    vector<unique_ptr<Node>> nodesAcessados;
};

class Node {
public:
    Node(int chave, int ordem, fstream * file) {
        int tamanho = ordem * 2;
        buffer.resize(tamanho);

        // Populando o Buffer com os dados completos do registro
        // em uma chamada unica ao SO
        int posicao = (sizeof(header)) + (chave * (ordem * 2 * sizeof(int)));
        file->seekg(posicao);
        file->read(reinterpret_cast<char *>(buffer.data()), buffer.size() * sizeof(int));

        // Iniciando as views/spans nas posicoes certas
        chaves = span<int>(buffer.data() + 1, ordem - 1);
        nos = span<int>(buffer.data() + ordem ,ordem);
    };

    //Atributos Fisicos
    int& chavesTotais() { return buffer[0];}
    const int& chavesTotais() const { return buffer[0];}
    vector<int> buffer;
    span<int> chaves;
    span<int> nos;

    //Atributos Temporarios
    int chaveInterna;
};

class Btree {
public:
    Btree(string alias, int ordem = 10) {
        // Verifica se o arquivo existe, se nao existir, cria & popula o header da Btree
        if (!filesystem::exists(alias)) {

            file.open(alias, ios::out | ios::in | ios::binary);
            header.root = -1; // -1 para declarar como vazia
            header.ordem = ordem;
            header.pilhaDaLixeira = -1; // -1 para declarar como vazia
            file.write(reinterpret_cast<char *>(&header), sizeof(header));
        } else {
            file.open(alias, ios::out | ios::in | ios::binary);
            file.read(reinterpret_cast<char *>(&header), sizeof(header));
        }
    }

    // A Arvore precisa estar instanciada e
    // o solicitador pronto para tratar a estrutura.
    optional<caminhoDaBusca> mSearch(int elemento) {
        if (header.root == -1) {
            return nullopt; // Arvore vazia
        }

        caminhoDaBusca caminho;
        int rrnAtual = header.root;

        while (rrnAtual != -1) {
            auto noAtual = make_unique<Node>(rrnAtual, header.ordem, &file);

            int inicio = 0;
            int fim = noAtual->chavesTotais() - 1;
            int indice = 0;
            bool encontrou = false;

            // Busca binaria dentro do node atual
            while (inicio <= fim) {
                int meio = inicio + (fim - inicio) / 2;

                if (noAtual->chaves[meio] == elemento) {
                    indice = meio;
                    encontrou = true;
                    break;
                } else if (elemento < noAtual->chaves[meio]) {
                    fim = meio - 1;
                } else {
                    inicio = meio + 1;
                }
            }

            if (encontrou) {
                caminho.posicoesInternas.push_back(indice);
                caminho.nodesAcessados.push_back(move(noAtual));
                return caminho;
            }

            // Se chegou aqui, 'inicio' e exatamente o indice do filho em 'nos'
            indice = inicio;
            int proximoRrn = noAtual->nos[indice];

            caminho.posicoesInternas.push_back(indice);
            caminho.nodesAcessados.push_back(move(noAtual));

            rrnAtual = proximoRrn;
        }

        return nullopt; // Chave nao encontrada
    }

    bool insertB(int chave) {
        // CASO 1: A arvore esta completamente vazia
        if (header.root == -1) {
            int novoId = obterNovoId();

            // O obterNovoId ja cuidou de atualizar o header.pilhaDaLixeira no disco
            // caso tenha reaproveitado um ID.

            Node raiz(novoId, header.ordem, &file);

            raiz.chavesTotais() = 1;
            raiz.chaves[0] = chave;

            for (int i = 0; i < header.ordem; i++) {
                raiz.nos[i] = -1;
            }
            for (int i = 1; i < header.ordem - 1; i++) {
                raiz.chaves[i] = 0;
            }

            salvarNoNoDisco(novoId, &raiz);

            // Atualiza o root para o ID encontrado/reaproveitado
            header.root = novoId;
            file.seekp(0);
            file.write(reinterpret_cast<char*>(&header), sizeof(header));

            return true;
        }

        // CASO 2: A arvore ja tem elementos
        int idAtual = header.root;
        Node noAtual(idAtual, header.ordem, &file);

        while (noAtual.nos[0] != -1) {
            int indice = encontrarIndiceFilho(&noAtual, chave);
            idAtual = noAtual.nos[indice];
            noAtual = Node(idAtual, header.ordem, &file);
        }

        if (noAtual.chavesTotais() < header.ordem - 1) {
            int i = noAtual.chavesTotais() - 1;

            while (i >= 0 && noAtual.chaves[i] > chave) {
                noAtual.chaves[i + 1] = noAtual.chaves[i];
                i--;
            }

            noAtual.chaves[i + 1] = chave;
            noAtual.chavesTotais()++;

            salvarNoNoDisco(idAtual, &noAtual);
            return true;
        }

        return false;
    }

    bool deleteB(int chave) {
        // Passo 1: Verifica se a arvore esta vazia
        if (header.root == -1) {
            return false;
        }

        // Para uma arvore B completa, usariamos a estrutura caminhoDaBusca
        // para guardar os pais e fazer a recursao de merge e borrow (underflow).
        // Aqui ilustramos o cenario de remocao na raiz e delecao de node.
        
        int idAtual = header.root;
        Node noAtual(idAtual, header.ordem, &file);
        
        // Passo 2: Verifica se a chave existe no node atual (busca sequencial simples)
        int indiceChave = -1;
        for (int i = 0; i < noAtual.chavesTotais(); i++) {
            if (noAtual.chaves[i] == chave) {
                indiceChave = i;
                break;
            }
        }
        
        // Se a chave for encontrada no node
        if (indiceChave != -1) {
            
            // Passo 3: Remove a chave deslocando os elementos maiores para a esquerda
            for (int i = indiceChave; i < noAtual.chavesTotais() - 1; i++) {
                noAtual.chaves[i] = noAtual.chaves[i + 1];
            }
            
            // Diminui o contador total de chaves do node
            noAtual.chavesTotais()--;
            
            // Passo 4: Verifica se o node ficou completamente vazio apos a remocao
            if (noAtual.chavesTotais() == 0) {
                
                // Como o node esvaziou, ele nao tem mais utilidade.
                // Movemos o ID (RRN) dele para a lixeira para reaproveitamento.
                moverParaLixeira(idAtual);
                
                // Se esse node era a raiz (nosso caso de teste base), 
                // a arvore inteira agora esta vazia. Atualizamos o root.
                if (idAtual == header.root) {
                    header.root = -1;
                    file.seekp(0);
                    file.write(reinterpret_cast<char*>(&header), sizeof(header));
                }
                
                // Em um cenario de node folha comum, aqui avisariamos o pai 
                // para iniciar o processo de merge ou redistribuicao de chaves.
                
            } else {
                // Se o node ainda possui chaves validas, apenas salvamos as alteracoes.
                salvarNoNoDisco(idAtual, &noAtual);
            }
            
            return true; // Delecao concluida com sucesso
        }
        
        // Passo 5: Se a chave nao esta aqui e nao for folha, desceriamos para o filho.
        // A logica recursiva de exclusao (que exige analise de vizinhos, fusao
        // de arrays e promocao/rebaixamento de chaves do pai) ocorreria aqui.
        
        return false; // Chave nao encontrada
    }

    ~Btree() {
        file.seekp(0);
        file.write(reinterpret_cast<char *>(&header), sizeof(header));
        file.close();
    }

private:

    //Atributos da Btree
    header header;
    fstream file;

    //Funcoes Auxiliares

    // Funcao auxiliar para calcular a posicao e salvar o node no disco
    void salvarNoNoDisco(int indice, Node* no) {
        int posicao = sizeof(header) + (indice * header.ordem * 2 * sizeof(int));
        file.seekp(posicao);
        file.write(reinterpret_cast<char*>(no->buffer.data()), no->buffer.size() * sizeof(int));
    }

    // Funcao que gerencia de onde vira o espaco para o novo node
    int obterNovoId() {
        // Verifica se existe alguma posicao na lixeira para ser reaproveitada
        if (header.pilhaDaLixeira != -1) {
            int rrnReutilizado = header.pilhaDaLixeira;

            // Calcula a posicao no disco onde o node deletado esta
            int posicao = sizeof(header) + (rrnReutilizado * header.ordem * 2 * sizeof(int));
            file.seekg(posicao);

            // Le o primeiro inteiro desse bloco. Em um node deletado,
            // este valor guarda o RRN do PROXIMO elemento na pilha da lixeira.
            int proximoLixo;
            file.read(reinterpret_cast<char*>(&proximoLixo), sizeof(int));

            // Atualiza o header fazendo a lixeira apontar para o proximo espaco livre (pop)
            header.pilhaDaLixeira = proximoLixo;

            // Salva a alteracao do header imediatamente no disco
            file.seekp(0);
            file.write(reinterpret_cast<char*>(&header), sizeof(header));

            return rrnReutilizado; // Retorna o ID reaproveitado
        }

        // Se a lixeira estiver vazia (-1), alocamos no final do arquivo
        file.seekg(0, ios::end);
        int tamanhoArquivo = file.tellg();

        // Se o arquivo so tem o header, o primeiro node e o ID 0
        if (tamanhoArquivo == sizeof(header)) {
            return 0;
        }

        // Calcula a nova posicao no final do arquivo
        return (tamanhoArquivo - sizeof(header)) / (header.ordem * 2 * sizeof(int));
    }

    // Funcao para descartar um node vazio e coloca-lo na pilha de lixeira
    void moverParaLixeira(int indiceDescartado) {
        // Calcula a posicao exata do node que sera descartado no arquivo
        int posicao = sizeof(header) + (indiceDescartado * header.ordem * 2 * sizeof(int));
        file.seekp(posicao);

        // Grava o topo atual da lixeira nos primeiros 4 bytes do node descartado.
        // Isso cria o encadeamento: o novo lixo aponta para o lixo antigo.
        file.write(reinterpret_cast<char*>(&header.pilhaDaLixeira), sizeof(int));

        // Atualiza o cabecalho para que o topo da lixeira aponte para o novo lixo
        header.pilhaDaLixeira = indiceDescartado;

        // Salva as alteracoes do cabecalho imediatamente no disco
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&header), sizeof(header));
    }
};
