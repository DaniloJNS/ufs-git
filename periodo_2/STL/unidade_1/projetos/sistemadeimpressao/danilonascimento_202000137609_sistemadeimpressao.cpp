#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <system_error>

using namespace std;

typedef struct Documento{
    string name;
    int paginas;
} Documento;


template <class T> class Node {
public:
  T x;
  Node *next;
  Node(T x0) : x(x0){
    next = NULL;
  }
};

template <class T> class Fila {
  string null;

protected:
  int n;

public:
  Fila() {
    n = 0;
    head = NULL;
  }

  Node<T> *head;
  Node<T> *tail;

  int size() { return n; }

  void push(T x) {
    Node<T> *u = new Node<T>(x);
    if (size() == 0) 
      head = u;
    else 
      tail->next = u;
    tail = u;
    n++;
  }

  void pop() {
      if (n > 0) {
        Node<T> *u = head;
        head = head->next;
        delete u;
        n--;
      }
  }

  void index() {
    Node<T> *u = head;

    while (u != NULL) {
        std::cout << u->x.name << std::endl;
        u = u->next;
    }

  }
};
template <class T> class Pilha {
  string null;

protected:
  int n;

public:
  Pilha() {
    n = 0;
    head = NULL;
  }

  Node<T> *head;

  int size() { return n; }

  void add(T x) {
    Node<T> *u = new Node<T>(x);
    if (size() > 0) 
      u->next = head;
    head = u;
    n++;
  }

  void pop() {
      if (n > 0) {
        Node<T> *u = head;
        head = head->next;
        delete u;
        n--;
      }
  }

  void index() {
    Node<T> *u = head;

    while (u != NULL) {
        std::cout << u->x.name << std::endl;
        u = u->next;
    }

  }
};
class Impressora {
  public:
    string name;
    int impressoes;
    Pilha<Documento> *documentos;
    Impressora(string Name) : name(Name){
      documentos = new Pilha<Documento>();
      impressoes = 0;
    };

    void imprimir(Documento documento) {
      documentos->add(documento);
      impressoes += documento.paginas;
    }

    string show() {
      string show;
      show += "["+name +"] ";
      Node<Documento> *documento = documentos->head;
      while (documento != NULL) {
        show += documento->x.name + "-" + to_string(documento->x.paginas) + "p";
        if (documento->next != NULL)
          show += ", ";
        documento = documento->next;
      }
      return show;
    } 
};

class SistemaDeImpressao 
{
public:
  SistemaDeImpressao() {
    impressoras = Fila<Impressora>();
  };
  Fila<Impressora> impressoras;
  Pilha<Documento> documentos;
  int impressoes;

  string imprimir(Documento documento) {
    Node<Impressora> *impressora = impressoras.head;
    Node<Impressora> *next = impressoras.head;

    while(impressora != NULL) {
      if (next->x.impressoes > impressora->x.impressoes)
        next = impressora;
      impressora = impressora->next;
    }

    if (next->x.documentos->head != NULL)
      documentos.add(next->x.documentos->head->x);

    impressoes += documento.paginas;
    next->x.imprimir(documento);
    return next->x.show();
  }
  Node<Impressora>* next() {
    Node<Impressora> *impressora = impressoras.head;
    Node<Impressora> *next = impressoras.head;

    while(impressora != NULL) {
      if (next->x.impressoes > impressora->x.impressoes)
        next = impressora;
      impressora = impressora->next;
    }

    return next;
  }
};

char* read_file(FILE* arg) {
  char *linha = new char[100], *result;
  result = fgets(linha, 100, arg);  // o 'fgets' lê até 99 caracteres ou até o '\n'
  if(result) 
    return linha;
  return linha;
}
int main(int argc, char* argv[]) {
  printf("#ARGS = %i\n", argc);
  printf("PROGRAMA = %s\n", argv[0]);
  printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);

  SistemaDeImpressao sistema;
  char *linha;
  FILE* input = fopen(argv[1], "r");
  fstream file_out;
  file_out.open(argv[2], ios_base::out);

  if (input == NULL || !file_out.is_open()) {
     printf("Problemas com os arquivo\n");
     return EXIT_FAILURE;
  }

  linha = read_file(input);
  int n_impressoras = atoi(linha);
  delete linha;

  std::cout << n_impressoras << std::endl;
  
  for (int i = 0; i < n_impressoras; ++i) {
    linha = read_file(input);
    sistema.impressoras.push(Impressora(strtok(linha, "\n")));
    delete linha;
  }

  linha = read_file(input);
  int n_documentos = atoi(linha);
  std::cout << n_documentos << std::endl;
  delete linha;
  string name; int paginas;

  for (int i = 0; i < n_documentos; ++i) {
    linha = read_file(input);
    name = strtok (linha," ");
    paginas = atoi(strtok (NULL," "));
    delete linha;
    file_out << sistema.imprimir(Documento{name, paginas}) << std::endl;
  }

  Node<Impressora> *impressora = sistema.impressoras.head;
  
  for (int i = 0; i < n_impressoras; ++i) {
    impressora = sistema.next();
    sistema.documentos.add(impressora->x.documentos->head->x);
    impressora->x.impressoes = INT32_MAX;
  }

  Node<Documento> *documento = sistema.documentos.head;

  file_out << sistema.impressoes << "p" << endl;

  while(documento != NULL) {
    file_out << documento->x.name << "-" << documento->x.paginas << "p" << endl;
    documento = documento->next;
  }
  return EXIT_SUCCESS;
}
