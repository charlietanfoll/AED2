#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

using namespace std;

struct head {
  int version = -1;
  int quantidade_de_vias = -1;
  int ultimo_id = -1;
  int lixeira = -1;
  int root = -1;
};

struct node {
  node(int size): chaves(size - 1), nos(size) {};
  int id;
  vector<int> chaves; // Numero de Nós -1
  vector<int> nos; //K vias?
};




class btree {
public:
  btree(string alias): file(alias, ios::in | ios::out | ios::binary) {
    if (file.is_open()) {
      //Se o arquivo Abrir, popula o head com as informações da btree.
      file.read(reinterpret_cast<char *>(&head), sizeof(head));
      cout << "Arquivo aberto com sucesso!" << endl;
    } else {
      cout << "Arquivo não pode ser aberto!" << endl;
      exit(1);
    }
  };

  void nova_tree(string alias, int vias) {
    //Verifica se não existe um arquivo com mesmo nome e o criar;
    if (file.is_open()) {
      cout << "A btree já existe! Crie outra instância" << endl;
      exit(1);
    }
    if (std::filesystem::exists(alias)) {
      cout << "Nome de arquivo já usado!" << endl;
      exit(1);
    } else {
      file.open(alias, ios::out | ios::in | ios::binary);
      head.quantidade_de_vias = vias;
    }
  };

  ~btree() {
    // Escreve o novo cabeçalho no arquivo antes de limpar a memória!
    file.write(reinterpret_cast<char *>(&head), sizeof(head));
    file.close();
  }

  void msearch() {};
  bool insertb() {};
  bool deleteb() {};

private:
  //Gerenciar os espaços apagados por aqui. Dps vou implementar.
  void reaproveitar() {};
  head head;
  fstream file;
};
