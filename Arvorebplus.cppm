#pragma once

#include <fstream>
#include <iostream>

using namespace std;

const int KEYS_COUNT = 50;

struct node {};

class btree {
public:
  // A priori, retorna true se achou, e false se não. (ainda nn li
  // os slides do prof, então talvez depois precisaremos mudar
  //  pra retornar uma struct, se nn me entano) att. charlie
  bool msearch() { return true; };

  // Mesma coisa de msearch(), retorna true se deu certo, e false
  // se não.        att. charlie
  bool insertb() { return true; };

  // Mesma coisa dos outros métodos...   att.charlie
  bool deleteb() { return true; };

private:
};
