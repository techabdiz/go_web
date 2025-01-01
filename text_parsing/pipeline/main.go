package main

import (
	"os"
	"strings"
	"text/template"
)

func main() {

	tpl := template.Must(template.New("pipe").Parse(`
	{{define "T1" -}} Template 1{{- end}}
	{{define "T2" -}} Template 2
	{{- end }}
	{{define "T3"}}{{template "T1"}}{{template "T2"}}{{end}}
	{{template "T3"}}`)) //

	tpl.Execute(os.Stdout, "data to the file")

	template.HTMLEscape(os.Stdout, []byte("<html></html>"))
}

func Uppercase(str string) string {
	return strings.ToUpper(str)
}
