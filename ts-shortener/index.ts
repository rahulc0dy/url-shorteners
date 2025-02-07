import { Database } from "bun:sqlite";

// Constants
const PORT = 3000;
const DB_FILE = "./urls.db";
const BASE_URL = `http://localhost:${PORT}`;
const CHARSET =
  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const KEY_LENGTH = 6;

// Initialize SQLite database
const db = new Database(DB_FILE);
db.run(`
  CREATE TABLE IF NOT EXISTS urls (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    short_key TEXT UNIQUE,
    original_url TEXT
  );
`);

// Generate a unique short key
function generateShortKey(): string {
  let key = "";
  for (let i = 0; i < KEY_LENGTH; i++) {
    key += CHARSET.charAt(Math.floor(Math.random() * CHARSET.length));
  }
  return key;
}

// Check if a short key exists
function isShortKeyExists(shortKey: string): boolean {
  const result = db
    .query("SELECT 1 FROM urls WHERE short_key = ? LIMIT 1")
    .get(shortKey);
  return !!result;
}

// Save URL to the database
function saveURL(shortKey: string, originalURL: string): void {
  db.query("INSERT INTO urls (short_key, original_url) VALUES (?1, ?2)").run(
    shortKey,
    originalURL
  );
}

// Get the original URL from the database
function getOriginalURL(shortKey: string): string | null {
  const result = db
    .query<{ original_url: string }, any>(
      "SELECT original_url FROM urls WHERE short_key = $shortKey"
    )
    .get({ $shortKey: shortKey });

  return result ? result.original_url : null;
}

async function handleShorten(request: Request): Promise<Response> {
  try {
    const { url } = await request.json();
    if (!url) {
      return new Response(JSON.stringify({ error: "URL is required" }), {
        status: 400,
      });
    }

    let shortKey = generateShortKey();
    while (isShortKeyExists(shortKey)) {
      shortKey = generateShortKey();
    }

    saveURL(shortKey, url);

    return new Response(
      JSON.stringify({
        original_url: url,
        shortened_url: `${BASE_URL}/${shortKey}`,
      }),
      {
        headers: { "Content-Type": "application/json" },
      }
    );
  } catch (error) {
    return new Response(JSON.stringify({ error: "Invalid request body" }), {
      status: 400,
    });
  }
}

function handleRedirect(request: Request): Response {
  const shortKey = new URL(request.url).pathname.substring(1); // Extract shortKey from the path
  if (!shortKey) {
    return new Response(JSON.stringify({ error: "Short key is missing" }), {
      status: 400,
    });
  }

  const originalURL = getOriginalURL(shortKey);
  if (!originalURL) {
    return new Response(JSON.stringify({ error: "Shortened key not found" }), {
      status: 404,
    });
  }

  return new Response(null, {
    status: 302,
    headers: { Location: originalURL },
  });
}

const server = Bun.serve({
  port: PORT,
  async fetch(request: Request): Promise<Response> {
    const { pathname } = new URL(request.url);

    if (request.method === "POST" && pathname === "/shorten") {
      return handleShorten(request);
    } else if (request.method === "GET" && pathname !== "/") {
      return handleRedirect(request);
    } else {
      return new Response(JSON.stringify({ message: "Not found" }), {
        status: 404,
      });
    }
  },
  error(error) {
    return new Response(JSON.stringify({ message: error.message }), {
      status: 500,
      headers: { "Content-Type": "application/json" },
    });
  },
});

console.log(`Server is running on ${BASE_URL}`);
