// Package main provides ...
package main

import (
	"bufio"
	"fmt"
	"os"
)

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

func main() {
	text, err := read_file(os.Args[1])

	if err != nil {
		println(err)
		os.Exit(1)
	}
	fmt.Print(text)
}
