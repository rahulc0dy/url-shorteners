# Go URL Shortener

A simple, fast, and scalable URL shortener built using **Go** and **SQLite**.

## Features

✅ **Fast & Memory-Safe** – Leveraging Go’s performance and built-in concurrency  
✅ **Persistent Storage** – Using SQLite to store URLs  
✅ **Concurrency-Safe** – Efficient handling of multiple requests  
✅ **Short Keys** – 6-character unique IDs generated from a robust charset  
✅ **Scalable** – Easily replace SQLite with other databases if needed

---

## 🚀 Installation

### 1. Install Dependencies

Ensure you have **Go** installed on your system. You can download it from [golang.org](https://golang.org/dl/).

### 2. Clone and Build the Project

Build the application:

```sh
go build
```

Alternatively, you can run directly without building:

```sh
go run main.go
```

### 3. Set Up the Database

The application automatically creates the SQLite database (`shortener.db`) and the necessary table when it starts.  
If you need to inspect or manage the database manually, you can use any SQLite client.

---

## 🛠 Usage

### **Shorten a URL**

Send a `POST` request to `/shorten` with JSON payload:

```sh
curl -X POST -H "Content-Type: application/json" -d '{"url": "https://example.com"}' http://localhost:8080/shorten
```

**Response:**

```json
{
    "original_url": "https://example.com",
    "shortened_url": "http://localhost:8080/abc123"
}
```

### **Redirect to the Original URL**

Access the shortened URL in your browser or via curl. For example:

```sh
curl -L http://localhost:8080/abc123
```

This request will redirect you to the original URL (e.g., `https://example.com`).

---

## 📁 Project Structure

```
📂 go-shortener
├── go.mod       # Go module definition
├── go.sum       # Dependency checksum file
├── main.go      # Main application code
├── README.md    # Project documentation
└── shortener.db # SQLite database file

```

---

## 📜 License

This project is open-source under the **MIT License**.

---

## 🔥 Future Enhancements

-   ✅ Add support for alternative databases (e.g., PostgreSQL, MySQL)
-   ✅ Implement rate limiting and API keys for enhanced security
-   ✅ Include analytics for tracking URL visits

🚀 **Happy coding!**
