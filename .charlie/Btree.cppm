#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <memory>

using namespace std;

struct header {
    int root;
    int ordem; // ordem = M e chaves = M-1
    int pilhaDaLixeira;
};

class Node {
public:
    Node(int chave, int ordem, fstream * file) {
        chaves.resize(ordem - 1);
        nos.resize(ordem);

        //Preencher quantidade de chaves totais
        int posicao = static_cast<int>(sizeof(header)) + (chave * static_cast<int>(ordem * 2));
        file->seekg(posicao);
        file->read(reinterpret_cast<char*>(&chavesTotais), sizeof(chavesTotais));

        //Preencher chaves
        posicao += sizeof(chavesTotais);
        file->seekg(posicao);
        file->read(reinterpret_cast<char*>(chaves.data()), sizeof(chaves));

        //Preencher nos
        posicao += chaves.size() * sizeof(int);
        file->seekg(posicao);
        file->read(reinterpret_cast<char*>(nos.data()), sizeof(nos));
    };

    //Atributos
    int chavesTotais = 0;
    vector<int> chaves;
    vector<int> nos;
};

class Btree {
public:

private:

};
