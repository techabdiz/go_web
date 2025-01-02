package main

import (
	"os"
	"strings"
	"text/template"
	"time"
)

var fm = template.FuncMap{
	"upperCase": strings.ToUpper,
	"lowerCase": func(str string) string {
		return strings.ToLower(str)
	},
	"atIndex": func(slice []string, index int) string {
		return slice[index]
	},
	"currentDate": func() time.Time {
		return time.Now()
	},
	"formatDate": func(t time.Time) string {
		return t.Format("02-01-2006")
	},
}

func main() {
	template.Must(template.New("template").
		Funcs(fm).
		Parse(`
		{{- atIndex . 2 | lowerCase }}
{{ atIndex . 1 | upperCase }}
{{ currentDate | formatDate}}
`)).Execute(os.Stdout, []string{
		"Apple", "Mango", "Banana",
	})
}
