#!/usr/bin/env python3
import os
import sys
import cgi

# Получаем код ошибки и сообщение из query string
form = cgi.FieldStorage()
error_code = form.getvalue('code', '500')
error_message = form.getvalue('message', 'Internal Server Error')

# Определяем цвет в зависимости от кода ошибки
colors = {
    '400': '#f0ad4e',  # Orange
    '403': '#d9534f',  # Red
    '404': '#5bc0de',  # Light Blue
    '405': '#ff6b6b',  # Red
    '413': '#9370db',  # Purple
    '500': '#d9534f',  # Red
    '505': '#292b2c'   # Dark
}

color = colors.get(error_code, '#d9534f')  # Default to red

# Формируем HTML-страницу
print("Content-Type: text/html\r")
print("\r")
print(f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Error {error_code} - {error_message}</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            background: #1a1a1a;
            color: #fff;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }}
        .error-container {{
            text-align: center;
            padding: 40px;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 10px;
        }}
        h1 {{
            font-size: 48px;
            margin-bottom: 20px;
            color: {color};
        }}
        p {{
            font-size: 18px;
            margin-bottom: 20px;
        }}
        a {{
            color: #00ff9d;
            text-decoration: none;
        }}
        a:hover {{
            text-decoration: underline;
        }}
    </style>
</head>
<body>
    <div class="error-container">
        <h1>{error_code}</h1>
        <p>{error_message}</p>
        <a href="/">Return to Home</a>
    </div>
</body>
</html>
""")