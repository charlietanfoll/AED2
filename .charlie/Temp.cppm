#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <span>
#include <memory>
#include <optional>

using namespace std;

class Node;
class Btree;
struct header;
struct caminhoDaBusca;

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

    bool insertB();

    bool deleteB();

    ~Btree() {
        file.seekp(0);
        file.write(reinterpret_cast<char *>(&header), sizeof(header));
        file.close();
    }

private:
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
                caminho.nodesAcessados.push_back(std::move(noAtual));
                return caminho;
            }

            // Se chegou aqui, 'inicio' é exatamente o índice do filho em 'nos'
            indice = inicio;
            int proximoRrn = noAtual->nos[indice];

            caminho.posicoesInternas.push_back(indice);
            caminho.nodesAcessados.push_back(std::move(noAtual));

            rrnAtual = proximoRrn;
        }

        return nullopt; // Chave não encontrada
    }

    //Atributos da Btree
    header header;
    fstream file;
};
