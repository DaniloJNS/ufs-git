// Package main provides ...
package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

type Lista struct {
	element Documento
	next *Lista
}

type Fila struct {
	head   *Lista
	tail   *Lista
	length int
}

type Pilha struct {
	head   *Lista
	tail   *Lista
	length int
}

type Documento struct {
	nome    string
	paginas int
}

type Impressora struct {
	nome       string
	documentos Pilha
	impressoes int
}

type Sistema_de_impressao struct {
	impressoras []Impressora
	documentos  Pilha
	impressoes  int
}

// ******* Interface para fila de documentos ******* {{
func (fila *Fila) add(element Documento) {
	if fila.length == 0 {
		fila.head = &Lista{element: element, next: nil}
		fila.tail = fila.head
	} else {
		fila.head = &Lista{element: element, next: fila.head}
	}
	fila.length++
}

func (fila *Fila) pop() (element Documento) {
	current := fila.head
	if fila.length > 0 {
		if fila.length == 1 {
			element = fila.head.element
			fila.head = nil
		} else {
			for current.next.next != nil {
				current = current.next
			}
			element = current.next.element
			current.next = nil
		}
		fila.length--
	}
	return
}

func (fila *Fila) show() {
	var current *Lista = fila.head

	for current != nil {
		fmt.Println(current.element)
		current = current.next
	}
}

// }}

// ******* Interface para pilha de documentos ******* {{
func (pilha *Pilha) add(element Documento) {
	if pilha.length == 0 {
		pilha.head = &Lista{element: element, next: nil}
		pilha.tail = pilha.head
	} else {
		pilha.head = &Lista{element: element, next: pilha.head}
	}
	pilha.length++
}

func (pilha *Pilha) pop() (element Documento) {
	if pilha.length > 0 {
		element = pilha.head.element
		if pilha.length == 1 {
			pilha.head = nil
		} else {
			pilha.head = pilha.head.next
		}
		pilha.length--
	}
	return
}
// }}
/*
	O tempo para imprimir as paginas dos documentos são contantes
	com isso, é definido qual impressora realizara a impressão com base
	na quatidade paginas que elas ja imprimiu, logo a proxima impressora
	a imprimir é a que tiver menos impressões
	impressora. documentos < ...Impressooras
*/
func (sistema_de_impressao *Sistema_de_impressao) imprimir(documento Documento) string {
	proxima := &Impressora{nome: "", impressoes: 999999}

	for i := 0; i < len((*sistema_de_impressao).impressoras); i++ {
		if proxima.impressoes > (*sistema_de_impressao).impressoras[i].impressoes {
			proxima = &(*sistema_de_impressao).impressoras[i]
		}
	}

	if proxima.documentos.head != nil {
		sistema_de_impressao.documentos.add(proxima.documentos.head.element)
	}
	sistema_de_impressao.impressoes += documento.paginas
	proxima.imprimmir(documento)
	return proxima.show()
}

func (impressora *Impressora) imprimmir(documento Documento) {
	impressora.documentos.add(documento)
	impressora.impressoes += documento.paginas
}

func (impressora Impressora) show() (show string) {
	show = fmt.Sprintf("[%s] ", impressora.nome)
	current := impressora.documentos.head
	for current != nil {
		if current.next != nil {
			show += fmt.Sprintf("%s-%dp, ", current.element.nome, current.element.paginas)
		} else {
			show += fmt.Sprintf("%s-%dp", current.element.nome, current.element.paginas)
		}
		current = current.next
	}
	return
}

func read_file(file_name string) ([]string, error) {
	arquivo, err := os.Open(file_name)

	if err != nil {
		return nil, err
	}

	defer arquivo.Close()

	var linhas []string
	scanner := bufio.NewScanner(arquivo)

	for scanner.Scan() {
		linhas = append(linhas, scanner.Text())
	}

	return linhas, scanner.Err()
}

func write_file(file *os.File, message string) {
	file.Seek(0, 2)
	file.WriteString(fmt.Sprintln(message))
	file.Sync()
}

func async_write_file(file *os.File, message string) {
	file.Seek(0, 2)
	file.WriteString(fmt.Sprintln(message))
	go file.Sync()
}

func raise(err error) {
	if err != nil {
		log.Panic(err.Error())
	}
}

func main() {
	start := time.Now()
	var sistema_de_impressao Sistema_de_impressao

	input, err := read_file(os.Args[1])
	output_file, err_2 := os.OpenFile(os.Args[2], os.O_RDWR, 0644)

	raise(err)
	raise(err_2)

	defer output_file.Close()

	n_impressoras, err := strconv.Atoi(input[0])
	documentos := input[n_impressoras+2:]

	// Armazenando as impressoras disponiveis
	for _, value := range input[1 : n_impressoras+1] {
	sistema_de_impressao.impressoras = append(
		sistema_de_impressao.impressoras, Impressora{
											nome:       value,
											documentos: Pilha{},
											impressoes: 0,
		})
	}

	var dados []string
	var nome string
	var paginas int

	// Enfililerando todos os documentos
	for _, documento := range documentos {
		dados = strings.SplitN(documento, " ", 2)
		if len(dados) == 2 {
			nome = dados[0]
			paginas, err = strconv.Atoi(dados[1])
			raise(err)
			async_write_file(output_file, sistema_de_impressao.imprimir(Documento{nome: nome, paginas: paginas}))
		}
	}

	for _, impressora := range sistema_de_impressao.impressoras {
		if impressora.documentos.head != nil {
			sistema_de_impressao.documentos.add(impressora.documentos.head.element)
		}
	}

	async_write_file(output_file, fmt.Sprintf("%dp", sistema_de_impressao.impressoes))

	current := sistema_de_impressao.documentos.head

	for current != nil {
		async_write_file(output_file, fmt.Sprintf("%s-%dp", current.element.nome, current.element.paginas))
		current = current.next
	}
	
	fmt.Println("Time exec: ", time.Since(start))
}