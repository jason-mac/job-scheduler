CREATE TABLE IF NOT EXISTS jobs (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    command TEXT NOT NULL,
    status TEXT NOT NULL DEFAULT 'pending',
    exit_code INTEGER,
    created_at TIMESTAMPTZ NOT NULL DEFAULT now()
);
