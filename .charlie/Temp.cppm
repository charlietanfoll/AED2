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
        int posicao = static_cast<int>(sizeof(header)) + (chave * static_cast<int>(ordem * 2 * sizeof(int)));
        file->seekg(posicao);
        file->read(reinterpret_cast<char *>(buffer.data()), buffer.size() * sizeof(int));

        // Iniciando as views/spans nas posições certas
        chaves = span<int>(buffer.data() + 1, ordem - 1);
        nos = span<int>(buffer.data() + ordem ,ordem);
    };

    //Atributos Fisicos
    int& chavesTotais() { return buffer[0];}
    const int& chavesTotais() const { return buffer[0];}
    vector<int> buffer;
    span<int> chaves;
    span<int> nos;

    //Atributos Temporários
    int chaveInterna;
};

class Btree {
public:
    Btree(string alias, int ordem = 10) {
        // Verifica se o arquivo existe, se não existir, cria & popula o header da Btree
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
    optional<caminhoDaBusca> msearch(int elemento) {
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

            // Se chegou aqui, 'inicio' é exatamente o índice do filho em 'nos'
            indice = inicio;
            int proximoRrn = noAtual->nos[indice];

            caminho.posicoesInternas.push_back(indice);
            caminho.nodesAcessados.push_back(move(noAtual));

            rrnAtual = proximoRrn;
        }

        return nullopt; // Chave não encontrada
    }

    bool insertB(int chave) {
        // CASO 1: A árvore está completamente vazia
        if (header.root == -1) {
            int novoId = obterNovoId();

            // O obterNovoId já cuidou de atualizar o header.pilhaDaLixeira no disco
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

        // CASO 2: A árvore já tem elementos
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

    bool deleteB();

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

    // Função auxiliar para calcular a posição e salvar o nó no disco
    void salvarNoNoDisco(int id, Node* no) {
        int posicao = sizeof(header) + (id * header.ordem * 2 * sizeof(int));
        file.seekp(posicao);
        file.write(reinterpret_cast<char*>(no->buffer.data()), no->buffer.size() * sizeof(int));
    }

    // Função que gerencia de onde virá o espaço para o novo nó
    int obterNovoId() {
        // Verifica se existe alguma posição na lixeira para ser reaproveitada
        if (header.pilhaDaLixeira != -1) {
            int rrnReutilizado = header.pilhaDaLixeira;

            // Calcula a posição no disco onde o nó deletado está
            int posicao = sizeof(header) + (rrnReutilizado * header.ordem * 2 * sizeof(int));
            file.seekg(posicao);

            // Lê o primeiro inteiro desse bloco. Em um nó deletado,
            // este valor guarda o RRN do PRÓXIMO elemento na pilha da lixeira.
            int proximoLixo;
            file.read(reinterpret_cast<char*>(&proximoLixo), sizeof(int));

            // Atualiza o header fazendo a lixeira apontar para o próximo espaço livre (pop)
            header.pilhaDaLixeira = proximoLixo;

            // Salva a alteração do header imediatamente no disco
            file.seekp(0);
            file.write(reinterpret_cast<char*>(&header), sizeof(header));

            return rrnReutilizado; // Retorna o ID reaproveitado
        }

        // Se a lixeira estiver vazia (-1), alocamos no final do arquivo
        file.seekg(0, ios::end);
        int tamanhoArquivo = file.tellg();

        // Se o arquivo só tem o header, o primeiro nó é o ID 0
        if (tamanhoArquivo == sizeof(header)) {
            return 0;
        }

        // Calcula a nova posição no final do arquivo
        return (tamanhoArquivo - sizeof(header)) / (header.ordem * 2 * sizeof(int));
    }
};
