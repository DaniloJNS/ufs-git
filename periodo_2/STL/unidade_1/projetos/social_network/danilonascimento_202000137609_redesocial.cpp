#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

using namespace std;

class Lista {
  string null;

protected:
  class Node {
  public:
	string x;
	Node *next;
	Node *prev;
	Node(string x0) {
	  x = x0;
	  next = NULL;
	  prev = NULL;
	}
  };

  const string OK =    "[ OK  ] ";
  const string ERROR = "[ERROR] ";
  Node *head;
  Node *tail;
  int n;

public:
  Lista() {
	n = 0;
	head = tail = NULL;
  }

  int size() { return n; }

  string add(string x) {
    string message;
    if (search_node(x) == NULL) {
      Node *u = new Node(x);
      if (n == 0) {
	head = u;
      } else {
	tail->next = u;
	u->prev = tail;
      }
      tail = u;
      n++;
      return log(true, "ADD", x);
    } else {
      return log(false, "ADD", x);
    }
  }

  string remove(string x) {
      Node *u = search_node(x);
      string message;

      if (u == NULL) {
	return log(false, "REMOVE",x);
      } else {
	  if (u == head)
	    head = head->next;

	  if (u == tail)
	    tail = tail->prev;

	  if (!(u->prev == NULL)) 
	      u->prev->next = u->next;
	  
	  if (!(u->next == NULL)) 
	      u->next->prev = u->prev;
	  
	  delete u;
	  n--;
	  return log(true, "REMOVE", x);
      }
  }

  Node* search_node(string x) {
    Node *u = head;

    while (u != NULL && u->x != x)
      u = u->next;


    return u;
  }

  void index() {
    Node *u = head;

    while (u != NULL) {
	    std::cout << u->x << std::endl;
	    u = u->next;
    }

  }
  string show(string x) {
	string node_right = "?";
	string node_left = "?";
	string message;

	Node *u = search_node(x);

	if (!(u == NULL)) {
	  if (!(u->next ==NULL)) {
		  node_right = u->next->x;
	  } else {
		  node_right = head->x;
	  }
	  if (!(u->prev == NULL) ) {
		  node_left = u->prev->x;
	  } else {
		  node_left = tail->x;
	  }
	  message = OK + node_left+ "<-" + x + "->" + node_right;
	} else {
	  message = ERROR + node_left+ "<-" + x + "->" + node_right;
	}
	return message;
  }
  string log(bool option, string action, string name) {
    string msg_type = option ? OK : ERROR;

    return msg_type+action+" "+name;
  }
};

int main(int argc, char* argv[]) {
	printf("#ARGS = %i\n", argc);
	printf("PROGRAMA = %s\n", argv[0]);
	printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);

	FILE* input = fopen(argv[1], "r");
	fstream file_out;
	file_out.open(argv[2], ios_base::out);
	if (input == NULL || !file_out.is_open()) {
	   printf("Problemas com os arquivo\n");
	   return EXIT_FAILURE;
	}

	Lista *lista = new Lista();

	char linha[100];
	char *result;
	string action, name;

	while (!feof(input)) {
	    result = fgets(linha, 100, input);  // o 'fgets' lê até 99 caracteres ou até o '\n'

	    if (result) {
	      action = strtok (linha," ");
	      name = strtok(NULL, "\n");
	      if (action.compare("ADD") == 0)
		file_out << lista->add(name)<<endl;
	      if (action.compare("REMOVE") == 0)
		file_out << lista->remove(name)<<endl;
	      if (action.compare("SHOW") == 0)
		file_out << lista->show(name) <<endl;
	    }
	}

	fclose(input);
	file_out.close();
	return EXIT_SUCCESS; 
}
