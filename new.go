package main

import (
	"fmt"
	"math/rand"
)

func add(x, y int) int {

	fmt.Println(x + y)
	return x + y
}

func swip(x, y string) (string, string) {
	return y, x
}

var o, p = 1, "bitch"

func test() {
	var a, b, c = 1, 2, 3
	fmt.Println(a, b, c)
	fmt.Println(o, p)

}

func main() {
	fmt.Println("test deos this work?")
	fmt.Println("new line???")
	fmt.Println("", rand.Intn(10))

	z := 10
	v := 10

	x, y := swip("hello", "world")
	fmt.Println(x, y)

	add(z, v)

	fmt.Println(add(5, 5))

	test()

	// test
}
