// Package main provides ...
package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"
)

type lista struct {
	nome string
	next *lista
	prev *lista
}

type Lista struct {
	head *lista
	tail *lista
}


func (list *Lista) add(element string) (message string){
	if !list.present(element) {
		if list.head == nil {
			list.head = &lista{nome: element, next: nil}
			list.tail = list.head
		} else {
			list.tail.next = &lista{nome: element, next: nil}
			list.tail.next.prev = list.tail
			list.tail = list.tail.next
		}
		message = output(true, "ADD", element)
	} else {
		message = output(false, "ADD", element)
	}
	return
}

func (list *Lista) remove(element string) (message string){
	var current *lista = list.head

	for current != nil && current.nome != element {
		current = current.next
	}

	if current == nil {
		message = output(false, "REMOVE", element)
	} else {
		if !(current.prev == nil) {
			current.prev.next = current.next
		}
		if !(current.next == nil) {
			current.next.prev = current.prev
		}
		message = output(true, "REMOVE", element)
	}
	return
}

func (list *Lista) show(element string) (message string) {
	var current *lista = list.head
	var friend_right, friend_left string = "?", "?"

	for current != nil && current.nome != element {
		current = current.next
	}
	if !(current == nil) {
		if !(current.next ==nil) {
			friend_right = current.next.nome
		} else {
			friend_right = list.head.nome
		}
		if !(current.prev == nil) {
			friend_left = current.prev.nome
		} else {
			friend_left = list.tail.nome
		}
		message = "[ OK  ] "+friend_left+"<-"+current.nome+"->"+friend_right
	} else {
		message = "[ERROR] "+" ?<-"+element+"->?"
	}
	return
}

func (list *Lista) present(element string) bool {
	var current *lista = list.head
	for current != nil && current.nome != element {
		current = current.next
	}

	if current == nil {
		return false
	}
	return true
}

func output(option bool, action string, element string) string {
	var msg_type string
	if option {
		msg_type = "[ OK  ]"
	} else {
		msg_type = "[ERROR]"
	}
	return msg_type+" "+action+" "+element
}

func write_file(file *os.File, message string) {
	file.Seek(0, 2)
	file.WriteString(fmt.Sprintln(message))
	go file.Sync()
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

func raise(err error) {
	if err != nil {
		log.Panic(err.Error())
	}
}

func main() {
	text, err := read_file(os.Args[1])

	output_file, err_2 := os.OpenFile(os.Args[2], os.O_RDWR, 0644)

	defer output_file.Close()

	raise(err)
	raise(err_2)

	var listing Lista

	for _, value := range text {
		action := strings.SplitN(value, " ", 2)

		switch action[0] {
		case "ADD":
			write_file(output_file, listing.add(action[1]))
		case "REMOVE":
			write_file(output_file, listing.remove(action[1]))
		case "SHOW":
			write_file(output_file, listing.show(action[1]))
		default:

		}
	}
}
