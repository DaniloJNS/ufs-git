// Package main provides ...
package main

import (
	"fmt"
	"math"
	"strings"
)

func solver_recursive(s1 string, s2 string) float64 {
	s1IndexLastChar := len(s1) - 1
	s2IndexLastChar := len(s2) - 1
	if len(s1) == 0 || len(s2) == 0 {
		return 0
	}
	if strings.Compare(string(s1[len(s1)-1]), string(s2[len(s2)-1])) == 0 {
		return 1 + solver_recursive(s1[:s1IndexLastChar], s2[:s2IndexLastChar])
	}
	return math.Max(solver_recursive(s1[:s1IndexLastChar], s2), solver_recursive(s1, s2[:s2IndexLastChar]))
}

func solver_memorise(s1 string, s2 string, memorise [][]float64) float64 {
	s1IndexLastChar := len(s1) - 1
	s2IndexLastChar := len(s2) - 1
	if len(s1) == 0 || len(s2) == 0 {
		return 0
	}
	if strings.Compare(string(s1[len(s1)-1]), string(s2[len(s2)-1])) == 0 {
		return 1 + solver_recursive(s1[:s1IndexLastChar], s2[:s2IndexLastChar])
	}
}

func main() {
	var s1, s2 string

	fmt.Scanf("%s", &s1)
	fmt.Scanf("%s", &s2)

	fmt.Printf("lcs = %.0f", solver_recursive(s1, s2))
}
