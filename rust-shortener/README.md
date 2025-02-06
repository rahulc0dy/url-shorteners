# Rust URL Shortener

A simple, fast, and scalable URL shortener built using **Rust**, **Actix Web**, and **SQLite**.

## Features

✅ Fast & Memory-Safe (Rust’s performance is close to C)  
✅ Persistent Storage (SQLite for saving URLs)  
✅ Concurrency-Safe (Asynchronous SQL queries with `sqlx`)  
✅ Short Keys with Nanoid (6-character unique IDs)  
✅ Scalable (Easily swap SQLite with PostgreSQL/MySQL)

---

## 🚀 Installation

### 1. Install Dependencies

Ensure you have **Rust** and **Cargo** installed.

```sh
cargo build
```

### 2. Set Up the Database

Create the SQLite database and table:

```sh
touch urls.db
sqlite3 urls.db < migrations/init.sql  # (Use PowerShell fix if needed)
```

Or in PowerShell:

```sh
Get-Content migrations/init.sql | sqlite3 urls.db
```

### 3. Run the Server

```sh
cargo run
```

---

## 🛠 Usage

### **Shorten a URL**

Send a `POST` request to `/shorten` with JSON:

```sh
curl -X POST -H "Content-Type: application/json" -d '{"url": "https://example.com"}' http://localhost:8080/shorten
```

**Response:**

```json
{
    "original_url": "https://example.com",
    "shortened_url": "http://localhost:8080/aB3dX9"
}
```

### **Redirect to Original URL**

Go to `http://localhost:8080/aB3dX9` → Redirects to `https://example.com`.

---

## 📁 Project Structure

```
📦 rust-shortener
├── 📂 migrations
│   └── init.sql  # Database schema
├── 📂 src
│   ├── main.rs   # Main application logic
├── urls.db        # SQLite database file
├── Cargo.toml     # Rust dependencies
└── README.md      # Project documentation
```

---

## 📜 License

This project is open-source under the **MIT License**.

---

## 🔥 Future Enhancements

-   ✅ Add PostgreSQL/MySQL support
-   ✅ Rate limiting & API keys
-   ✅ Analytics for URL visits

🚀 **Happy coding!**
