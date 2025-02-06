# URL Shorteners in Multiple Languages

This repository contains implementations of a URL shortener built with different programming languages, including Rust, Go, Python, TypeScript, and C++.

## Overview

Each implementation provides a basic URL shortener that allows users to shorten long URLs and retrieve the original URL using a short identifier. The implementations vary in terms of frameworks, storage mechanisms, and additional features.

## Implementations

### 1. Rust

-   **Framework**: Actix-Web
-   **Storage**: SQLite
-   **Features**:
    -   REST API with JSON support
    -   Persistent storage using SQLite
    -   Simple token-based authentication

### 2. Go

-   **Framework**: STD Library
-   **Storage**: SQLite
-   **Features**:
    -   REST API with net/http
    -   Database integration with SQLite
    -   Rate limiting for API endpoints

### 3. Python

-   **Framework**: FastAPI
-   **Storage**: SQLite
-   **Features**:
    -   Asynchronous API with FastAPI
    -   In-memory storage with Redis for fast lookups
    -   QR code generation for shortened URLs

### 4. TypeScript (Node.js)

-   **Framework**: Bun
-   **Storage**: MongoDB
-   **Features**:
    -   REST API with Express.js
    -   MongoDB integration for URL persistence
    -   JWT-based authentication for API security

### 5. C++

-   **Framework**: Crow (Lightweight Web Framework)
-   **Storage**: SQLite
-   **Features**:
    -   Minimalist REST API
    -   Local SQLite database for URL storage
    -   Simple in-memory caching for performance

## Setup Instructions

Each implementation has its own directory with a README file containing detailed setup instructions. To get started with a specific language, navigate to its folder and follow the instructions.

### General Prerequisites

-   Install the necessary language runtime (Rust, Go, Python, Node.js, C++)
-   Set up the required database (PostgreSQL, SQLite, Redis, MongoDB)
