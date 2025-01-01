package main

import (
	"os"
	"text/template"
)

type Employee struct {
	Name string
}
type Company struct {
	Name      string
	Employees []Employee
}

func main() {
	template.Must(template.New("nesting").
		Parse(`
			{{- range $index, $company := .Companies}}
{{ $company.Name}}: {{ range $company.Employees}} {{.Name}} {{end}}
			{{- end}}
			`)).
		Execute(os.Stdout, struct {
			Companies []Company
		}{
			[]Company{
				{
					"IBM",
					[]Employee{
						{
							"Jerry",
						},
						{
							"Arvind",
						},
						{
							"Raj",
						},
					},
				},
				{
					"Accenture",
					[]Employee{
						{
							"Ramesh",
						},
						{
							"Abdullah",
						},
						{
							"Omar",
						},
					},
				},
			},
		})
}
