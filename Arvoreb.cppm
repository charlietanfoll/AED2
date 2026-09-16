#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct head {
  int nodesize;
  int chaves;
  int nivel;
  int altura;
  int root;
};

class node {
public:
  node(int size): chaves(size - 1), nos(size) {};
  int id;
  vector<int> chaves; // Numero de Nós -1
  vector<int> nos; //K vias?
};




class btree {
public:
  bool abrir(string alias) {
    fstream file(alias, ios::in | ios::out | ios::binary);
    this->file = &file;
    if (file.is_open()) {
      return true;
    } else {
      return false;
    }
    this->carregar();
  }

  // A priori, retorna true se achou, e false se não. (ainda nn li
  // os slides do prof, então talvez depois precisaremos mudar
  //  pra retornar uma struct, se nn me engano) att. charlie
  bool msearch() {
    return true;
  };

  // Mesma coisa de msearch(), retorna true se deu certo, e false
  // se não.        att. charlie
  bool insertb() {
    return true;
  };

  // Mesma coisa dos outros métodos...   att.charlie
  bool deleteb() {
    return true;
  };

private:
  void carregar() {
    
  }
  fstream* file = nullptr;
  head head;
};
