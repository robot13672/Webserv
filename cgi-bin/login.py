#!/usr/bin/env python3

import os
import cgi
import http.cookies

print("Content-Type: text/html")

# Проверяем, есть ли куки
cookie = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE"))
username = cookie.get("username")

if username:
    print("\n")  # Разделение заголовков и тела
    print(f"<h1>Welcome back, {username.value}!</h1>")
else:
    form = cgi.FieldStorage()
    username = form.getvalue("username", "Guest")

    # Создаём куку с логином
    cookie = http.cookies.SimpleCookie()
    cookie["username"] = username
    cookie["username"]["path"] = "/"
    cookie["username"]["max-age"] = 3600  # Кука действует 1 час

    print(f"\n{cookie}")  # Заголовок Set-Cookie
    print("<h1>Login successful</h1>")
    print(f"<p>Welcome, {username}!</p>")