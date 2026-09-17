#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <memory>

using namespace std;

struct head {
  int version = -1;
  int quantidade_de_vias = -1;
  int maior_id = -1;
  int lixeira = -1;
  int root = -1;
};
// Ainda estou editando o registro, estava faltando coisas
struct registro {
  int total;
  vector<int> chaves; // Numero de Nós -1
  vector<int> nos; //K vias?
};




class btree {
public:
  //Abre uma árvore já existente.
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

  //Cria uma nova árvore e um novo arquivo.
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

  //Destrutor que escreve o cabeçalho atualizado ao fechar o programa no disco.
  ~btree() {
    // Escreve o novo cabeçalho no arquivo antes de limpar a memória!
    file.write(reinterpret_cast<char *>(&head), sizeof(head));
    file.close();
  }

  //Charlie Maracutaias por aqui
  vector<unique_ptr<registro>> msearch() {
    vector<unique_ptr<registro>> caminho;
    return caminho;
  };

  void insertb() {};
  void deleteb() {};

private:
  //Gerenciar os espaços apagados por aqui. Dps vou implementar.
  void reaproveitar() {};
  int head_size = sizeof(struct::head);
  int node_size;
  head head;
  fstream file;
};
