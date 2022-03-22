package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)
type Book struct {
	ISBN int
	author string
	name string
}
func binary_search(ISBN int, books []Book) (resultado int) {
	resultado = -1
	middle := len(books) / 2
	isbn_middle := books[middle].ISBN
	fmt.Println(isbn_middle)

	if len(books) == 1 && isbn_middle != ISBN {
		return
	} else if isbn_middle == ISBN {
		return ISBN 
	} else if isbn_middle > ISBN {
		resultado = binary_search(ISBN, books[:middle])
	} else {
		resultado = binary_search(ISBN, books[middle+1:])
	}

	return
}
func interpolate_search(ISBN int, books []Book) int {
	var 
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

func raise(err error) {
	if err != nil {
		log.Panic(err.Error())
	}
}
func main() {
	var books []Book
	input, err := read_file(os.Args[1])
	raise(err)

	books_n, _ := strconv.Atoi(input[0])
	var livros []string = input[1:books_n]
	var consultas []string = input[books_n+2:]

	var data [] string
	var isbn int
	for _, livro := range livros {
		data = strings.SplitN(livro, " ", 3)
		isbn, _ = strconv.Atoi(data[0])
		books = append(books, Book{ISBN: isbn, author: data[1], name: data[2]})
		fmt.Println(isbn)
	}
	
	for _, consulta := range consultas {
		fmt.Println(consulta)
	}
	fmt.Println("ISBN = ", 9780130224187)
	fmt.Printf("binary_search(%d) = %d\n", 9780130224187, binary_search(9780130224187, books))
	fmt.Println("ISBN = ", 9780201416077)
	fmt.Printf("binary_search(%d) = %d\n", 9780201416077, binary_search(9780201416077, books))
	fmt.Println("ISBN = ", 9780262033848)
	fmt.Printf("binary_search(%d) = %d\n", 9780262033848, binary_search(9780262033848, books))
	fmt.Println("ISBN = ", 9780321751041)
	fmt.Printf("binary_search(%d) = %d\n", 9780321751041, binary_search(9780321751041, books))
	fmt.Println("ISBN = ", 9781584884354)
	fmt.Printf("binary_search(%d) = %d\n", 9781584884354, binary_search(9781584884354, books))
}
