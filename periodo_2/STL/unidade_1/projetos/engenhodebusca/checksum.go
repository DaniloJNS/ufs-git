package main

import "fmt"

func main() {
	var input string
	var hash int
	var length int = 3

	fmt.Scanf("%s", &input)

	for _, char := range input {
		// if hash == 0 {
		// 	hash = int(char)
		// } else {
		// 	hash ^= int(char)
		// }
		hash ^= int(char)
		fmt.Println("ASCII = ", int(char), " - hash = ", hash)
	}
	
	fmt.Println("H1(X) = ", 7919 * hash % length, " - H2(X) = ", (104729 * hash + 123 % length))
	check := 7919 * hash
	check += (104729 * hash + 123 % length)
	fmt.Println("check = ", check % length)
	return
}
