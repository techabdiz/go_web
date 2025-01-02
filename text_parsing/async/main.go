package main

import (
	"fmt"
	"os"
	"text/template"
	"time"
)

func main() {
	var ch chan string = make(chan string)
	go writeToChan(ch, 10, 1)
	tpl := template.Must(template.New("channel-template").Parse(`
	{{range .}} {{template "basic-template"}} {{.}}
	{{ end}}
	 `))

	template.Must(tpl.New("basic-template").Parse("The value of current iteration: "))

	tpl.ExecuteTemplate(os.Stdout, "channel-template", ch)
	close(ch)
}

func writeToChan(ch chan string, interations int, secInterval int) {
	for i := 0; i < interations; i++ {
		ch <- fmt.Sprintf("%d", i)
		time.Sleep(time.Second * time.Duration(secInterval))
	}
}
