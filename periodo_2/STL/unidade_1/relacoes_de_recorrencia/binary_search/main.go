// Package main provides ...
package main

import "fmt"

func binary_search(x int, arr []int) int {
	if len(arr) == 1 {
		return -1
	}
	middle := len(arr) / 2
	if arr[middle] == x {
		return x
	} else if arr[middle] > x {
		return binary_search(x, arr[:middle])
	} else {
		return binary_search(x, arr[middle:])
	}
}

func main() {

	arr := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	x := 1

	fmt.Printf("Acho %d", binary_search(x, arr))
}
