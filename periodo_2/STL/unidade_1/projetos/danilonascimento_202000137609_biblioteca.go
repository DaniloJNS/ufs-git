package main

import (
	"bufio"
	"fmt"
	"log"
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
		fmt.Println(scanner.Text())
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
	_, err := read_file(os.Args[1])
	raise(err)
}
