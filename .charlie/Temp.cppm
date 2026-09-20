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
struct caminho;
struct resultadoDaBusca;

struct header {
    int root;
    int ordem; // ordem = M e chaves = M-1
    int pilhaDaLixeira;
};

struct caminho {
    vector<int> indicesAcessados;
    vector<unique_ptr<Node>> nosAcessados;
};

struct resultadoDaBusca {
    bool encontrado;
    int  indiceNo;
    int  posicao;     // posicao da chave dentro do no (encontrada OU de insercao)
    caminho resultado;
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

    //Atributos
    int& chavesTotais() { return buffer[0];}
    const int& chavesTotais() const { return buffer[0];}
    vector<int> buffer;
    span<int> chaves;
    span<int> nos;
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
    resultadoDaBusca msearch();
    header header;
    fstream file;
};
