#!/usr/bin/env python3
import os
import sys
from http import cookies
from urllib.parse import parse_qs

# First print headers
# print("Content-Type: text/html")

try:
    # Get query parameters
    query_string = os.environ.get('QUERY_STRING', '')
    params = parse_qs(query_string)
    
    # Get username from query parameters
    username = params.get('username', [''])[0]
    
    if username:
        # Create cookie
        cookie = cookies.SimpleCookie()
        cookie['username'] = username
        cookie['username']['path'] = '/'
        cookie['username']['max-age'] = 3600  # 1 hour
        
        # Print cookie header
        # print(cookie)
    
    # Print empty line to separate headers from body
    print()
    
    # Get existing cookie if any
    stored_cookie = cookies.SimpleCookie(os.environ.get("HTTP_COOKIE", ""))
    stored_username = stored_cookie.get("username")
    
    if username:
        # Show success page with cookie info
        print(f"""
<!DOCTYPE html>
<html>
<head>
    <title>Cookie Set</title>
    <style>
        * {{
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }}
        body {{
            background-color: #1a1a1a;
            color: #00ff9d;
            font-family: Arial, sans-serif;
            display: flex;
            flex-direction: column;
            align-items: center;
            min-height: 100vh;
            padding: 40px 20px;
        }}
        .nav-buttons {{
            position: fixed;
            top: 20px;
            left: 20px;
            right: 20px;
            display: flex;
            justify-content: space-between;
            gap: 20px;
            z-index: 1000;
        }}
        .button {{
            padding: 12px 24px;
            font-size: 18px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-weight: bold;
            transition: all 0.3s ease;
        }}
        .home-button {{
            background: #00ff9d;
            color: #1a1a1a;
        }}
        .cgi-button {{
            background: #ff6b6b;
            color: white;
        }}
        .cookie-button {{
            background: #ff6b6b;
            color: white;
        }}
        .button:hover {{
            transform: translateY(-2px);
            opacity: 0.9;
        }}
        .container {{
            background: rgba(255, 255, 255, 0.1);
            padding: 40px;
            border-radius: 10px;
            text-align: center;
            margin-top: 80px;
            max-width: 600px;
            width: 100%;
        }}
        h1 {{
            margin-bottom: 20px;
            color: #00ff9d;
            font-size: 32px;
        }}
        .info {{
            margin: 20px 0;
            padding: 20px;
            background: rgba(0, 0, 0, 0.2);
            border-radius: 5px;
            text-align: left;
        }}
        .info p {{
            margin: 10px 0;
        }}
    </style>
</head>
<body>
    <div class="nav-buttons">
        <button class="button home-button" onclick="location.href='/'">HOME</button>
        <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
        <button class="button cookie-button" onclick="location.href='/cookie'">COOKIE</button>
    </div>
    <div class="container">
        <h1>Cookie Set Successfully!</h1>
        <div class="info">
            <p><strong>Username:</strong> {username}</p>
            <p><strong>Cookie Expiry:</strong> 1 hour</p>
        </div>
    </div>
</body>
</html>
""")

except Exception as e:
    print(f"Error: {str(e)}")
    sys.exit(1)