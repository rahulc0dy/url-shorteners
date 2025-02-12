#include <winsock2.h>
#include <windows.h>
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 4096

// Generate a random 6-character short key
std::string generateShortKey()
{
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string key;
    srand(time(0));

    for (int i = 0; i < 6; i++)
    {
        key += charset[rand() % charset.length()];
    }
    return key;
}

// Initialize SQLite database
sqlite3 *initDatabase()
{
    sqlite3 *db;
    if (sqlite3_open("shortener.db", &db))
    {
        std::cerr << "Error opening SQLite database.\n";
        return nullptr;
    }

    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS urls (id INTEGER PRIMARY KEY, short_key TEXT UNIQUE, original_url TEXT);";
    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        std::cerr << "Error creating table.\n";
    }
    return db;
}

// Store URL in SQLite
bool storeURL(sqlite3 *db, const std::string &shortKey, const std::string &originalURL)
{
    std::ostringstream query;
    query << "INSERT INTO urls (short_key, original_url) VALUES ('" << shortKey << "', '" << originalURL << "');";

    return sqlite3_exec(db, query.str().c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
}

// Retrieve original URL from SQLite
std::string getOriginalURL(sqlite3 *db, const std::string &shortKey)
{
    std::ostringstream query;
    query << "SELECT original_url FROM urls WHERE short_key = '" << shortKey << "' LIMIT 1;";

    sqlite3_stmt *stmt;
    std::string url;

    if (sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            url = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        }
    }
    sqlite3_finalize(stmt);
    return url;
}

// Handle HTTP requests
void handleRequest(SOCKET clientSocket, sqlite3 *db)
{
    char buffer[BUFFER_SIZE] = {0};
    recv(clientSocket, buffer, BUFFER_SIZE, 0);

    std::string request(buffer);
    std::stringstream response;

    // Handle URL shortening (POST /shorten?url=...)
    if (request.find("POST /shorten") != std::string::npos)
    {
        size_t urlPos = request.find("url=");
        if (urlPos != std::string::npos)
        {
            std::string originalURL = request.substr(urlPos + 4);
            originalURL = originalURL.substr(0, originalURL.find(" "));

            std::string shortKey = generateShortKey();
            if (storeURL(db, shortKey, originalURL))
            {
                response << "HTTP/1.1 200 OK\r\n"
                         << "Content-Type: text/plain\r\n"
                         << "Content-Length: " << shortKey.length() + 23 << "\r\n\r\n"
                         << "Shortened URL: http://localhost:" << PORT << "/" << shortKey;
            }
            else
            {
                response << "HTTP/1.1 500 Internal Server Error\r\n\r\nFailed to store URL.";
            }
        }
    }
    // Handle URL redirection (GET /{shortKey})
    else if (request.find("GET /") != std::string::npos)
    {
        size_t startPos = request.find("GET /") + 5;
        size_t endPos = request.find(" ", startPos);
        std::string shortKey = request.substr(startPos, endPos - startPos);

        std::string originalURL = getOriginalURL(db, shortKey);
        if (!originalURL.empty())
        {
            response << "HTTP/1.1 302 Found\r\n"
                     << "Location: " << originalURL << "\r\n\r\n";
        }
        else
        {
            response << "HTTP/1.1 404 Not Found\r\n\r\nURL not found.";
        }
    }

    send(clientSocket, response.str().c_str(), response.str().length(), 0);
    closesocket(clientSocket);
}

// Start HTTP Server (No Libraries)
void startServer(sqlite3 *db)
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr{};

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    std::cout << "Server running at http://localhost:" << PORT << "\n";

    while (true)
    {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        handleRequest(clientSocket, db);
    }

    closesocket(serverSocket);
    WSACleanup();
}

int main()
{
    sqlite3 *db = initDatabase();
    if (!db)
        return 1;

    startServer(db);
    sqlite3_close(db);
    return 0;
}
