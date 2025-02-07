# Bun URL Shortener

A simple, fast, and scalable URL shortener built using **TypeScript**, **Bun**, and **SQLite**—no external libraries required.

## Features

✅ **Super Fast** – Powered by Bun's high-performance runtime  
✅ **Lightweight** – No external libraries, just Bun and SQLite  
✅ **Persistent Storage** – URLs stored in an SQLite database  
✅ **Short Keys** – 6-character unique IDs  
✅ **Scalable** – Designed to work seamlessly with other databases like PostgreSQL or MySQL in the future

---

## 🚀 Installation

### 1. Install Bun

Ensure you have **Bun** installed. If not, install it using the following command:

```bash
curl -fsSL https://bun.sh/install | bash
```

```powershell
powershell -c "irm bun.sh/install.ps1 | iex"
```

Verify installation:

```sh
bun --version
```

### 2. Set Up the Database

The SQLite database (`shortener.db`) will be automatically created when you run the server. If you'd like to inspect the database, install an SQLite client such as `sqlite3`.

---

## 🕠 Usage

### **Start the Server**

Run the server with Bun:

```sh
bun run index.ts
```

By default, the server will start at `http://localhost:3000`.

---

### **Shorten a URL**

Send a `POST` request to `/shorten` with a JSON payload:

```sh
curl -X POST -H "Content-Type: application/json" -d '{"url": "https://example.com"}' http://localhost:3000/shorten
```

**Response:**

```json
{
  "original_url": "https://example.com",
  "shortened_url": "http://localhost:3000/abc123"
}
```

### **Redirect to the Original URL**

Access the shortened URL in your browser or via curl. For example:

```sh
curl -L http://localhost:3000/abc123
```

This will redirect you to the original URL (`https://example.com`).

---

## 📁 Project Structure

```
🗂 bun-url-shortener
├── index.ts       # main application logic
├── shortener.db   # SQLite database file (auto-generated)
├── README.md      # Project documentation
├── bun.lockb      # Bun lockfile
├── tsconfig.json  # TypeScript configuration
└── package.json   # Bun package configuration
```

---

## 📜 License

This project is open-source under the **MIT License**.

---

## 🔥 Future Enhancements

- ✅ Add support for alternative databases (e.g., PostgreSQL, MySQL)
- ✅ Implement rate limiting and API keys for enhanced security
- ✅ Include analytics for tracking URL visits

🚀 **Happy coding!**
