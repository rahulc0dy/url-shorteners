# Python URL Shortener

A simple URL shortener built using **FastAPI** and **SQLite**.

## Features

✅ FastAPI for high-performance async API  
✅ SQLite for lightweight database storage  
✅ URL key generation using custom logic  
✅ CRUD operations for managing short URLs  
✅ Dependency Injection for database handling

---

## 🚀 Installation

### 1. Set Up a Virtual Environment

```sh
python -m venv venv
source venv/bin/activate  # macOS/Linux
venv\Scripts\activate     # Windows
```

### 2. Install Dependencies

```sh
pip install -r requirements.txt
```

### 3. Set Up the Database

```sh
python shortener/database.py
```

### 4. Run the Server

```sh
uvicorn shortener.main:app --reload
```

---

## 🛠 Usage

### **Shorten a URL**

Send a `POST` request to `/shorten` with JSON:

```sh
curl -X POST -H "Content-Type: application/json" -d '{"url": "https://example.com"}' http://localhost:8000/shorten
```

**Response:**

```json
{
    "original_url": "https://example.com",
    "shortened_url": "http://localhost:8000/aB3dX9"
}
```

### **Redirect to Original URL**

Go to `http://localhost:8000/aB3dX9` → Redirects to `https://example.com`.

---

## 📁 Project Structure

```
📦 py-shortener
├── 📂 shortener
│   ├── __init__.py
│   ├── config.py       # Configuration settings
│   ├── crud.py         # Database CRUD functions
│   ├── database.py     # Database connection logic
│   ├── keygen.py       # Key generation logic
│   ├── main.py         # Main FastAPI application
│   ├── models.py       # SQLAlchemy models
│   ├── schemas.py      # Pydantic schemas
├── venv                # Virtual environment
├── .env                # Environment variables
├── shortener.db        # SQLite database file
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
