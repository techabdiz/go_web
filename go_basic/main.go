package main

import (
	"bufio"
	"fmt"
	"net/http"
)

func main() {
	run_bufio_example()
}

func run_bufio_example() {
	res, _ := http.Get("https://www.rfc-editor.org/rfc/rfc7230.txt")
	scan := bufio.NewScanner(res.Body)

	for scan.Scan() {
		line := scan.Text()
		fmt.Println(line)
	}
}
