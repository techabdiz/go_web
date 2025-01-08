package main

import (
	"bufio"
	"fmt"
	"net"
	"time"
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
	con.SetDeadline(time.Now().Add(time.Second * 10))
	scanner := bufio.NewScanner(con)

	for scanner.Scan() {
		line := scanner.Text()
		fmt.Println(line)
		fmt.Fprintf(con, "You said, %s\n", line)
	}
	defer con.Close()

	fmt.Println("why ?")

}
