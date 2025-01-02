package main

import (
	"fmt"
	"io"
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
		io.WriteString(conn, "\nHey man how are you ! !\n")
		fmt.Fprintln(conn, "welcome to my tcp server .... ")
		fmt.Fprintf(conn, "no of connections: %d\n", count)
		conn.Close()
	}
}
