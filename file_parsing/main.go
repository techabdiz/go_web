package main

import (
	"log"
	"os"
	"text/template"
)

type User struct {
}

func main() {
	tpl, err := template.ParseGlob("template.gohtml")
	if err != nil {
		log.Fatalln("error while parsing template", err)
		return
	}
	err = tpl.Execute(os.Stdout, 42)

	if err != nil {
		log.Fatalln("error while executing template", err)
	}
}
