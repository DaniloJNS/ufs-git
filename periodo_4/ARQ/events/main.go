package main

import (
	"fmt"
	"time"
)

type InsterruptionControl struct {
	data string
}

func handle_ZD(value uint32)  {
	select {
		case data := <- zd_ch:
		if data {
			fmt.Println("tem ZD", value)
		} else {
			fmt.Println("nao tem ZD", value)
		}
		case end_ch <- false:
	}
	time.Sleep(time.Duration(1))
}

var (
	zd_ch = make(chan bool)
	end_ch = make(chan bool, 10)
)

func div(zd uint32)  {
	for i := 0; i < 5; i++ {
		println("Enviando", i)
		time.Sleep(time.Second * 3)
		end_ch <- true
		println("Enviado com sucesso", i)
	}
	zd_ch <- true
}
	
func handle_end()  {
	
}
func main() {
	go div(1)
	<- zd_ch
	println("terminou")
}
