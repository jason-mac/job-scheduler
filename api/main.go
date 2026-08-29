package main

import (
	"log"
	"net/http"

	"github.com/jason-mac/job-scheduler/api/internal/handlers"
)

func main() {
	mux := http.NewServeMux()
	mux.HandleFunc("/api/health", handlers.Health)

	log.Fatal(http.ListenAndServe("0.0.0.0:3000", mux))
}
