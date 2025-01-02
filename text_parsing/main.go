package main

import (
	"os"
	"text/template"
)

type User struct {
	username string
	age      int
}

func (u User) GetUsername() string {
	return u.username
}
func (u User) GetAge() int {
	return u.age
}

func main() {

	tmp, err := template.New("deadspider").Parse(`User,                  
	

			{{- .GetUsername}} is of age {{.GetAge -}}         .`)
	if err != nil {
		panic(err)
	}
	user := User{
		"Jerry",
		40,
	}
	err = tmp.Execute(os.Stdout, user)
	if err != nil {
		panic(err)
	}

}
