package main

import (
	"log"
	"os"
	"text/template"
)

type User struct {
	username string
	age      int
}

func main() {
	tpl, err := template.ParseGlob("template.gohtml")
	if err != nil {
		log.Fatalln("error while parsing template", err)
		return
	}
	err = tpl.Execute(os.Stdout, []string{
		"Cat",
		"Dog",
		"Elephant",
	})

	if err != nil {
		log.Fatalln("error while executing template", err)
	}
}
