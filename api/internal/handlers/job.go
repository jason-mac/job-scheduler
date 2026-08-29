package handlers

import (
	"encoding/json"
	"net/http"

	"github.com/jackc/pgx/v5/pgxpool"

	"github.com/jason-mac/job-scheduler/api/internal/schedulerpb"
)

type JobHandler struct {
	client schedulerpb.SchedulerClient
	db     *pgxpool.Pool
}

func NewJobHandler(client schedulerpb.SchedulerClient, db *pgxpool.Pool) *JobHandler {
	return &JobHandler{client: client, db: db}
}

type SubmitJobRequest struct {
	Command string `json:"command"`
}

type SubmitJobResponse struct {
	JobID string `json:"job_id"`
}

func (h *JobHandler) SubmitJob(w http.ResponseWriter, r *http.Request) {
	var req SubmitJobRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		http.Error(w, "invalid request body", http.StatusBadRequest)
		return
	}

	var jobID string
	err := h.db.QueryRow(r.Context(), "INSERT INTO jobs (command) VALUES ($1) RETURNING id", req.Command).Scan(&jobID)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	_, err = h.client.SubmitJob(r.Context(), &schedulerpb.SubmitJobRequest{Command: req.Command, JobId: jobID})
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(SubmitJobResponse{JobID: jobID})
}
