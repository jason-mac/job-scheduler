package main

import (
	"context"
	"log"
	"net/http"
	"os"

	"github.com/joho/godotenv"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"

	"github.com/jason-mac/job-scheduler/api/internal/db"
	"github.com/jason-mac/job-scheduler/api/internal/handlers"
	"github.com/jason-mac/job-scheduler/api/internal/schedulerpb"
)

func getEnv(key, defaultValue string) string {
	if value, ok := os.LookupEnv(key); ok {
		return value
	}
	return defaultValue
}

func main() {
	godotenv.Load()

	conn, err := grpc.NewClient(getEnv("SUPERVISOR_ADDRESS", "localhost:50051"), grpc.WithTransportCredentials(insecure.NewCredentials()))
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()

	pool, err := db.Connect(context.Background(), getEnv("POSTGRES_DSN", "postgres://localhost:5432/jobscheduler"))
	if err != nil {
		log.Fatal(err)
	}
	defer pool.Close()

	client := schedulerpb.NewSchedulerClient(conn)
	jobHandler := handlers.NewJobHandler(client, pool)

	mux := http.NewServeMux()
	mux.HandleFunc("/api/health", handlers.Health)
	mux.HandleFunc("/api/jobs", jobHandler.SubmitJob)

	log.Fatal(http.ListenAndServe("0.0.0.0:3000", mux))
}
