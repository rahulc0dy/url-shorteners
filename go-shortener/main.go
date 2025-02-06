package main

import (
	"database/sql"
	"encoding/json"
	"fmt"
	"log"
	"math/rand"
	"net/http"
	"strings"
	"time"

	_ "modernc.org/sqlite"
)

const (
	keyLength = 6
	charset   = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
	baseURL   = "http://localhost:8080"
	dbFile    = "shortener.db"
)

// URLShortener struct holds the database connection
type URLShortener struct {
	db *sql.DB
}

// Initialize database and create table if it doesn't exist
func (us *URLShortener) InitDB() error {
	var err error
	us.db, err = sql.Open("sqlite", dbFile)
	if err != nil {
		return err
	}

	query := `CREATE TABLE IF NOT EXISTS urls (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		short_key TEXT UNIQUE,
		original_url TEXT
	);`
	_, err = us.db.Exec(query)
	return err
}

// Generate a unique short key
func (us *URLShortener) generateUniqueShortKey() string {
	rand.Seed(time.Now().UnixNano())

	for {
		shortKey := make([]byte, keyLength)
		for i := range shortKey {
			shortKey[i] = charset[rand.Intn(len(charset))]
		}

		if !us.isShortKeyExists(string(shortKey)) {
			return string(shortKey)
		}
	}
}

// Check if the short key already exists in the database
func (us *URLShortener) isShortKeyExists(shortKey string) bool {
	var exists bool
	query := `SELECT EXISTS(SELECT 1 FROM urls WHERE short_key = ? LIMIT 1)`
	us.db.QueryRow(query, shortKey).Scan(&exists)
	return exists
}

// Save a URL in the database
func (us *URLShortener) saveURL(shortKey, originalURL string) error {
	query := `INSERT INTO urls (short_key, original_url) VALUES (?, ?)`
	_, err := us.db.Exec(query, shortKey, originalURL)
	return err
}

// Retrieve original URL from the database
func (us *URLShortener) getOriginalURL(shortKey string) (string, bool) {
	var originalURL string
	query := `SELECT original_url FROM urls WHERE short_key = ?`
	err := us.db.QueryRow(query, shortKey).Scan(&originalURL)
	if err != nil {
		return "", false
	}
	return originalURL, true
}

// HandleShorten processes URL shortening requests
func (us *URLShortener) HandleShorten(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Invalid request method", http.StatusMethodNotAllowed)
		return
	}

	var req struct {
		URL string `json:"url"`
	}
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil || req.URL == "" {
		http.Error(w, "Invalid JSON or missing URL", http.StatusBadRequest)
		return
	}

	shortKey := us.generateUniqueShortKey()
	err := us.saveURL(shortKey, req.URL)
	if err != nil {
		http.Error(w, "Failed to save URL", http.StatusInternalServerError)
		return
	}

	response := map[string]string{
		"original_url":  req.URL,
		"shortened_url": fmt.Sprintf("%s/%s", baseURL, shortKey),
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(response)
}

// HandleRedirect redirects short URLs to original URLs
func (us *URLShortener) HandleRedirect(w http.ResponseWriter, r *http.Request) {
	shortKey := strings.TrimPrefix(r.URL.Path, "/")
	if shortKey == "" {
		http.Error(w, "Shortened key is missing", http.StatusBadRequest)
		return
	}

	originalURL, found := us.getOriginalURL(shortKey)
	if !found {
		http.Error(w, "Shortened key not found", http.StatusNotFound)
		return
	}

	http.Redirect(w, r, originalURL, http.StatusMovedPermanently)
}

func main() {
	shortener := &URLShortener{}
	if err := shortener.InitDB(); err != nil {
		log.Fatalf("Failed to initialize database: %v", err)
	}
	defer shortener.db.Close()

	http.HandleFunc("/shorten", shortener.HandleShorten)
	http.HandleFunc("/", shortener.HandleRedirect)

	fmt.Println("URL Shortener is running on :8080")
	log.Fatal(http.ListenAndServe(":8080", nil))
}
