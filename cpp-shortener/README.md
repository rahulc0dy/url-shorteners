# C++ URL Shortener (No External HTTP Libraries)

A simple URL shortener written in **C++**, using **WinSock for HTTP requests** and **SQLite for storage**, without any external HTTP libraries.

## Features

✅ **Pure C++** – No external HTTP libraries (uses WinSock)  
✅ **SQLite for Persistent Storage** – Stores URLs locally  
✅ **Lightweight & Fast** – Runs as a simple HTTP server  
✅ **Short Key Generation** – Generates 6-character short links  
✅ **Portable** – No need for system-wide SQLite installation

> [!IMPORTANT]  
> This project uses windows libraries. For linux
> or macos, please modify the program to use native libraries.

---

## 🚀 Installation

### 1️⃣ Clone the Repository

```sh
git clone https://github.com/rahulc0dy/url-shorteners.git
cd url-shorteners/cpp-shortener
```

### 2️⃣ Install Dependencies (Locally)

SQLite is included in the project folder, so no global installation is needed.

---

## 🛠 Build and Run

### 3️⃣ Compile Using Makefile

```sh
make
```

### 4️⃣ Run the Server

```sh
./url_shortener.exe
```

Server will start on `http://localhost:8080`.

### 5️⃣ Test the API

#### **Shorten a URL**

```sh
curl -X POST "http://localhost:8080/shorten?url=https://example.com"
```

**Response:**

```sh
Shortened URL: http://localhost:8080/aB3dX9
```

#### **Redirect to the Original URL**

Go to: `http://localhost:8080/aB3dX9` → Redirects to `https://example.com`

---

## 📁 Project Structure

```
📦 cpp-url-shortener
├── 📂 sqlite3          # SQLite local installation
│   ├── sqlite3.h       # SQLite header
│   ├── sqlite3.c       # SQLite source code
│   ├── sqlite3.dll     # SQLite runtime DLL
│   ├── sqlite3.lib     # SQLite static library
├── main.cpp            # Main HTTP server logic
├── Makefile            # Build automation
├── shortener.db        # SQLite database file
├── README.md           # Documentation
```

---

## 🛠 Clean Up

To remove compiled files:

```sh
make clean
```

---

## 📜 License

This project is open-source under the **MIT License**.

🚀 Happy coding!
