package main

import (
	"os"
	"text/template"
)

type User struct {
	Username string
	Age      int
}
type data struct {
	List     []string
	Employee User
	Stuff    map[string]string
}

func main() {

	tml := template.Must(template.New("datas").Parse(`
		{{- .Employee.Username }} {{ .Employee.Age }}
{{ range $index, $element := .List }} {{- $index }} {{$element}} 
{{end -}}

{{range $key, $value := .Stuff}} 
{{ $key}} is a {{$value -}}
{{end -}}
`))
	tml.Execute(os.Stdout, data{
		[]string{"Apple", "Mango", "Bananas"},
		User{"Jeremy", 12},
		map[string]string{
			"Ball":    "Toy",
			"Mango":   "Fruit",
			"Keybord": "Computer Accessory",
			"Guitar":  "Musical Instrument",
		},
	})

}
