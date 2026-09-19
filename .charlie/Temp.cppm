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

private:

};
