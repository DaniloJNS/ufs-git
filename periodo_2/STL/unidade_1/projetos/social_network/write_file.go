// Package main provides ...
package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

func write_file(element string, file_name string) (string, error) {
	file, err := os.OpenFile(
		file_name,
		os.O_WRONLY|os.O_TRUNC|os.O_CREATE,
		0666,
	)
	if err != nil {
		print(err.Error())
		os.Exit(1)
	}

	defer file.Close()

	byteSlice := []byte("nada!\n")
	bytesWritten, err := file.Write(byteSlice)
	if err != nil {
		log.Fatal(err)
	}
	log.Printf("Wrote %d bytes.\n", bytesWritten)

	return element, err
}

func buffer_write_file(element string, file_name string) error {
	// Cria o arquivo de texto
	arquivo, err := os.Create(file_name)
	// Caso tenha encontrado algum erro retornar ele
	if err != nil {
		return err
	}
	// Garante que o arquivo sera fechado apos o uso
	defer arquivo.Close()

	// Cria um escritor responsavel por escrever cada linha do slice no arquivo de texto
	escritor := bufio.NewWriter(arquivo)

	fmt.Fprintln(escritor, element)
	// Caso a funcao flush retorne um erro ele sera retornado aqui tambem
	return escritor.Flush()
}

func main() {
	file_name := os.Args[1]

	text := "alguma coisa"

	err := buffer_write_file(text, file_name)

	if err != nil {
		log.Panic(err.Error())
	}
}
