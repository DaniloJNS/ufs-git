package main

import "fmt"

// Descrição: n * (n-1) * (n -2) * (n -3) * ... * 1
// Analise de complextidade: O(n) = n
// Executa n chamadas recursivas

func fatorial(n int) int {
	if n == 0 {
		return 1
	}
	return n * fatorial(n-1)
}

// Descrição: n * (n-1) * (n -2) * (n -3) * ... * 1
// Analise de complextidade: O(n) = n
// Executa n interações

func fatorial_for(n int) (fatorial int) {
	fatorial = 1
	for i := n; i > 1; i-- {
		fatorial = i * fatorial
	}
	return
}

func main() {
	fmt.Printf("fatorial(%d)= %d\n", 5, fatorial(5))
	fmt.Printf("fatorial_for(%d)= %d\n", 5, fatorial_for(5))
}
