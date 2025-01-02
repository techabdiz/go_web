package main

import (
	"bufio"
	"fmt"
	"net"
)

func main() {
	li, err := net.Listen("tcp", ":8080")
	if err != nil {
		panic(err)
	}
	defer li.Close()
	count := 0
	for {
		conn, err := li.Accept()
		fmt.Println("serving... ", conn.RemoteAddr())
		if err != nil {
			panic(err)
		}
		count += 1

		go handle(conn)
	}
}

func handle(con net.Conn) {
	scanner := bufio.NewScanner(con)

	for scanner.Scan() {
		fmt.Println(scanner.Text())
	}
	defer con.Close()

	fmt.Println("why ?")

}