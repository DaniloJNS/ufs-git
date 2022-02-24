// Package main provides ...
package main

import "fmt"

type lista struct {
	nome string

	next *lista
}

type Lista struct {
	head *lista
	tail *lista
}

func (list *Lista) insertion(element string) {
	if list.head == nil {
		list.head = &lista{nome: element, next: nil}
		list.tail = list.head
	} else {
		list.tail.next = &lista{nome: element, next: nil}
		list.tail = list.tail.next
	}
}

func (list *Lista) remove(element string) {
	var prev, current *lista = nil, list.head

	for current.nome != element || current == nil {
		prev = current
		current = current.next
	}

	if current.next == nil {
		prev.next = current.next
	} else {
		*current = *current.next
	}
}

func (list *Lista) show() {
	var current *lista = list.head

	for current != nil {
		fmt.Println(current.nome)
		current = current.next
	}
}

func main() {
	var (
		my_list Lista
	)
	my_list.insertion("danilo")
	my_list.insertion("carro")
	my_list.insertion("casa")
	my_list.insertion("tomate")
	my_list.remove("casa")

	my_list.show()
}
