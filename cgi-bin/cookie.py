#!/usr/bin/env python3
import os
import sys
from http import cookies
from urllib.parse import parse_qs

try:
    # Get query parameters
    query_string = os.environ.get('QUERY_STRING', '')
    params = parse_qs(query_string)
    username = params.get('username', [''])[0]
    value = params.get('value', [''])[0]  # Add value parameter

    # Print headers first
    print("Content-Type: text/html")
    
    if username and value:
        # Create and set cookie
        cookie = cookies.SimpleCookie()
        cookie_name = f'{username}'  # Make unique cookie name
        cookie[cookie_name] = value       # Set value for cookie
        cookie[cookie_name]['path'] = '/'
        cookie[cookie_name]['max-age'] = 3600
        print(cookie.output())
    
    print()  # Empty line after headers

    print("""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Cookie Status</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            background-color: #1a1a1a;
            color: #00ff9d;
            font-family: Arial, sans-serif;
            display: flex;
            flex-direction: column;
            align-items: center;
            min-height: 100vh;
            padding: 40px 20px;
        }
        .nav-buttons {
            position: fixed;
            top: 20px;
            left: 20px;
            right: 20px;
            display: flex;
            justify-content: space-between;
            gap: 20px;
        }
        .button {
            padding: 12px 24px;
            font-size: 18px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-weight: bold;
            transition: all 0.3s ease;
        }
        .home-button {
            background: #00ff9d;
            color: #1a1a1a;
        }
        .cgi-button, .cookie-button {
            background: #ff6b6b;
            color: white;
        }
        .button:hover {
            transform: translateY(-2px);
            opacity: 0.9;
        }
        .container {
            background: rgba(255, 255, 255, 0.1);
            padding: 40px;
            border-radius: 10px;
            text-align: center;
            margin-top: 80px;
            max-width: 600px;
            width: 100%;
        }
        h1 {
            margin-bottom: 20px;
            color: #00ff9d;
            font-size: 32px;
        }
        .info {
            margin: 20px 0;
            padding: 20px;
            background: rgba(0, 0, 0, 0.2);
            border-radius: 5px;
            text-align: left;
        }
        .info p {
            margin: 10px 0;
        }
    </style>
</head>
<body>
    <div class="nav-buttons">
        <button class="button home-button" onclick="location.href='/'">HOME</button>
        <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
        <button class="button cookie-button" onclick="location.href='/cookie'">COOKIE</button>
    </div>
    <div class="container">
        <h1>Cookie Status</h1>
        <div class="info">""")

    # Display all stored cookies
    stored_cookies = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
    if stored_cookies:
        for key, morsel in stored_cookies.items():
            print(f'<p><strong>Cookie Name:</strong> {key}</p>')
            print(f'<p><strong>Cookie Value:</strong> {morsel.value}</p>')
            # print(f'<p><strong>Cookie Path:</strong> {morsel["path"]}</p>')
            print('<hr>')
    else:
        print('<p>No cookies currently set</p>')

    if username and value:
        print(f'<p><strong>New Cookie Set:</strong></p>')
        print(f'<p>Username: {username}</p>')
        print(f'<p>Value: {value}</p>')

    print("""
        </div>
    </div>
</body>
</html>""")

except Exception as e:
    print("Content-Type: text/plain")
    print()
    print(f"Error: {str(e)}")
    sys.exit(1)