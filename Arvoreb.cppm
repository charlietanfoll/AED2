#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <memory>

using namespace std;

struct head {
  int root = -1;              // Indice do nó raiz atual.
  int quantidadeDeVias = -1;  //Ordem M, com K vias =M. E K - 1 chaves.
  int lixeira = -1;           //Índice da Pilha de registros apagados.
};
// Ainda estou editando o registro, estava faltando coisas
struct node {
  int chavesEscritas;
  bool isFolha;
  vector<int> chaves; // K - 1 nos
  vector<int> nos; //K vias
  vector<int> dados; //indice do registro que vai estar no arquivo de dados.
};

struct resultadoDaBusca {
  int id;
  int quantidadeDeVias;
  vector<unique_ptr<node>> caminho;
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
  void novaTree(string alias, int vias) {
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
      head.quantidadeDeVias = vias;
    }
  };

  //Destrutor que escreve o cabeçalho atualizado ao fechar o programa no disco.
  ~btree() {
    // Escreve o novo cabeçalho no arquivo antes de limpar a memória!
    if (file.is_open()) {
      file.write(reinterpret_cast<char *>(&head), sizeof(head));
      file.close();
    }
  };

  //Charlie Maracutaias por aqui
  static resultadoDaBusca mSearch(int id) {
    resultadoDaBusca resultado;
    return resultado;
  };

  void insertb() {};
  void deleteb() {};

private:
  //Gerenciar os espaços apagados por aqui. Dps vou implementar.
  void reaproveitar() {};

  void calcularNodeSize() {
    // Calcula o tamanho dos nodes em tempo de execução (Requisito Opt 2)
    //                     header + nodes e chaves + inteiros
    registroSize = (sizeof(head) + (sizeof(head.quantidadeDeVias) * 2) + 7);
  };

  int headSize = sizeof(struct::head);
  int registroSize;
  head head;
  fstream file;
};
