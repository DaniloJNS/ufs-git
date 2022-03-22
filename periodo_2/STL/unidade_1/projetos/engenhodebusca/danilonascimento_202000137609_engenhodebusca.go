package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

type Lista struct {
    element string

    next *Lista
}

type Fila struct {
    head   *Lista
    tail   *Lista
    length int
}

type Server struct {
    requests Fila
}
type Master struct {
    servers []Server
    length int
    max_resquest int
}

func checksum(input []string) (checksum int) {
	var check_val []int 
	for _, string := range input {
		checksum = 0
		for _, char := range string {
			if checksum == 0 {
				checksum = int(char)
			} else {
				checksum ^= int(char)
			}
		}
		check_val = append(check_val, checksum)
	}
	checksum = 0
	for _, val := range check_val {
		if checksum == 0 {
			checksum += val
		} else {
			checksum ^= val
		}
	}
	
	return 
}

// }

func Hash(length int, input []string, retry int) (hash int) {
	var checksum_val int = checksum(input)
	println("check_val = ", checksum_val)

	hash = 7919 * checksum_val % length
	hash +=  retry * (104729 * checksum_val + 123 % length)
	hash %= length

	return 
}

func (master *Master) checksum(amount int,text []string, retry int) int {
	var hash int = Hash(master.length, text, retry)

	if master.servers[hash].requests.length < master.max_resquest || retry > 20 {
		master.servers[hash].requests.add(strings.Join(text, " "))
	} else {
		hash =  Hash(master.length, text, 1)
		master.servers[hash].requests.add(strings.Join(text, " "))
	}

    	return hash
}

func (master *Master)resquest(body string) {
    
}

func (fila *Fila) add(element string) {
	if fila.length == 0 {
		fila.head = &Lista{element: element, next: nil}
		fila.tail = fila.head
	} else {
		fila.head = &Lista{element: element, next: fila.head}
	}
	fila.length++

}

func read_file(file_name string) ([]string, error) {
	file, err := os.Open(file_name)

	if err != nil {
		return nil, err
	}

	defer file.Close()

	var lines []string
	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}

	return lines, scanner.Err()
}

func raise(err error) {
	if err != nil {
		log.Panic(err.Error())
	}
}

func main() {
	file_parsed, err := read_file(os.Args[1])
	raise(err)

	data_server := strings.SplitN(file_parsed[0], " ", 2)

	amount_server, err := strconv.Atoi(data_server[0])
	raise(err)

	max_resquest, err := strconv.Atoi(data_server[1])
	raise(err)

    	master := Master { 
        	servers: make([]Server, amount_server),
        	length: amount_server,
        	max_resquest: max_resquest,
    	}
    	var data []string
        for _, request := range file_parsed[2:] {
		data = strings.Split(request, " ")
		amount, err := strconv.Atoi(data[0])
		raise(err)
		fmt.Println("hash = ", master.checksum(amount,data[1:], 0))
    	}
}
