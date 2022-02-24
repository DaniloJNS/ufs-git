// Package main provides ...
package main

import "fmt"

func fibonacci_recursivo(n uint32) uint32 {
	if n == 0 || n == 1 {
		return n
	}

	return fibonacci_recursivo(n-2) + fibonacci_recursivo(n-1)
}

func fibonacci_iterativo(n uint32) uint32 {
	if n == 0 || n == 1 {
		return n
	}

	const base uint32 = 2
	var th1, th2 uint32 = 1, 1

	for i := base; i < n; i++ {
		th1, th2 = th2, th2+th1
	}

	return th2
}

func main() {
	const n uint32 = 4

	res := fibonacci_recursivo(n)
	res_ite := fibonacci_iterativo(n)

	fmt.Printf("Fibonacci_recursivo(%d) = %d\n", n, res)
	fmt.Printf("Fibonacci_iterativo(%d) = %d\n", n, res_ite)
}
