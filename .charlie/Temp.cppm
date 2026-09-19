#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <memory>
#include <span>

using namespace std;

struct header {
    int root;
    int ordem; // ordem = M e chaves = M-1
    int pilhaDaLixeira;
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

    void Salvar() {};

    //Atributos
    int& chavesTotais() { return buffer[0];}
    const int& chavesTotais() const { return buffer[0];}
    vector<int> buffer;
    span<int> chaves;
    span<int> nos;
};

class Btree {
public:

private:

};
