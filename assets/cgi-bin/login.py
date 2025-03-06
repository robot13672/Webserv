# # !/usr/bin/env python3

# # import os
# # import cgi
# # import http.cookies

# # print("Content-Type: text/html")

# # # Проверяем, есть ли куки
# # cookie = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE"))
# # username = cookie.get("username")

# # if username:
# #     print("\n")  # Разделение заголовков и тела
# #     print(f"<h1>Welcome back, {username.value}!</h1>")
# # else:
# #     form = cgi.FieldStorage()
# #     username = form.getvalue("username", "Guest")

# #     # Создаём куку с логином
# #     cookie = http.cookies.SimpleCookie()
# #     cookie["username"] = username
# #     cookie["username"]["path"] = "/"
# #     cookie["username"]["max-age"] = 3600  # Кука действует 1 час

# #     print(f"\n{cookie}")  # Заголовок Set-Cookie
# #     print("<h1>Login successful</h1>")
# #     print(f"<p>Welcome, {username}!</p>")

# #!/usr/bin/env python3
# # print("Content-Type: text/html\r\n\r\n")
# # print("""
# # <!DOCTYPE html>
# # <html>
# # <head>
# #     <title>CGI Test</title>
# #     <style>
# #         body {
# #             font-family: Arial, sans-serif;
# #             padding: 20px;
# #             background: #1a1a1a;
# #             color: #00ff9d;
# #         }
# #     </style>
# # </head>
# # <body>
# #     <h1>CGI Script Executed Successfully</h1>
# #     <p>This is a response from the Python CGI script.</p>
# # </body>
# # </html>
# # """)

# #!/usr/bin/env python3
# # filepath: /mnt/c/Users/Rostyslav/Desktop/42/web_serv/cgi-bin/login.py

# # print("Content-Type: text/html\r\n\r\n")
# # print("""
# # <!DOCTYPE html>
# # <html>
# # <head>
# #     <title>Login Page</title>
# #     <style>
# #         body {
# #             background-color: #1a1a1a;
# #             color: #00ff9d;
# #             font-family: 'Source Code Pro', monospace;
# #             display: flex;
# #             justify-content: center;
# #             align-items: center;
# #             height: 100vh;
# #             margin: 0;
# #         }
# #         .login-form {
# #             background: rgba(255, 255, 255, 0.05);
# #             padding: 40px;
# #             border-radius: 10px;
# #             text-align: center;
# #         }
# #         h1 {
# #             margin-bottom: 30px;
# #             text-shadow: 0 0 10px rgba(0, 255, 157, 0.3);
# #         }
# #         .env-info {
# #             margin-top: 20px;
# #             text-align: left;
# #         }
# #     </style>
# # </head>
# # <body>
# #     <div class="login-form">
# #         <h1>CGI Test Page</h1>
# #         <p>Python CGI script executed successfully!</p>
# #         <div class="env-info">
# #             <h2>Environment Info:</h2>
# #             <pre>
# # Server Name: %s
# # Server Protocol: %s
# # HTTP Method: %s
# #             </pre>
# #         </div>
# #     </div>
# # </body>
# # </html>
# # """ % (
# #     os.environ.get('SERVER_NAME', 'Unknown'),
# #     os.environ.get('SERVER_PROTOCOL', 'Unknown'),
# #     os.environ.get('REQUEST_METHOD', 'Unknown')
# # ))


# # print("Content-Type: text/html\r\n\r\n")
# # print("""
# # <!DOCTYPE html>
# # <html>
# # <head>
# #     <title>CGI Login Test</title>
# #     <style>
# #         body {
# #             font-family: Arial, sans-serif;
# #             padding: 20px;
# #             background: #1a1a1a;
# #             color: #00ff9d;
# #         }
# #         .container {
# #             max-width: 600px;
# #             margin: 0 auto;
# #             background: rgba(255,255,255,0.1);
# #             padding: 20px;
# #             border-radius: 10px;
# #         }
# #     </style>
# # </head>
# # <body>
# #     <div class="container">
# #         <h1>CGI Script Executed Successfully</h1>
# #         <p>This is a response from the Python CGI script.</p>
# #         <button onclick="window.location.href='/'">Return Home</button>
# #     </div>
# # </body>
# # </html>
# # """)

# print("Content-Type: text/html\r\n\r\n")
# print("""
# <!DOCTYPE html>
# <html>
# <head>
#     <title>CGI Login Test</title>
#     <style>
#         body {
#             font-family: Arial, sans-serif;
#             padding: 20px;
#             background: #1a1a1a;
#             color: #00ff9d;
#             display: flex;
#             justify-content: center;
#             align-items: center;
#             height: 100vh;
#             margin: 0;
#         }
#         .container {
#             background: rgba(255,255,255,0.1);
#             padding: 40px;
#             border-radius: 10px;
#             text-align: center;
#             max-width: 600px;
#         }
#         h1 {
#             color: #00ff9d;
#             margin-bottom: 20px;
#         }
#         .env-info {
#             text-align: left;
#             margin-top: 20px;
#             background: rgba(0,0,0,0.2);
#             padding: 15px;
#             border-radius: 5px;
#         }
#         button {
#             background: #00ff9d;
#             color: #1a1a1a;
#             border: none;
#             padding: 10px 20px;
#             border-radius: 5px;
#             cursor: pointer;
#             margin-top: 20px;
#             font-weight: bold;
#         }
#         button:hover {
#             background: #00cc7d;
#         }
#     </style>
# </head>
# <body>
#     <div class="container">
#         <h1>CGI Script Executed Successfully!</h1>
#         <div class="env-info">
#             <p><strong>Server Software:</strong> %s</p>
#             <p><strong>Server Protocol:</strong> %s</p>
#             <p><strong>Request Method:</strong> %s</p>
#             <p><strong>Script Name:</strong> %s</p>
#         </div>
#         <button onclick="window.location.href='/'">Return Home</button>
#     </div>
# </body>
# </html>
# """ % (
#     os.environ.get('SERVER_SOFTWARE', 'Unknown'),
#     os.environ.get('SERVER_PROTOCOL', 'Unknown'),
#     os.environ.get('REQUEST_METHOD', 'Unknown'),
#     os.environ.get('SCRIPT_NAME', 'Unknown')
# ))

#!/usr/bin/env python3
import os
import sys

# First, print the headers
print("Content-Type: text/html\r")
print("Access-Control-Allow-Origin: *\r")
print("\r")  # Empty line to separate headers from body

# Then print the HTML content
print("""
<!DOCTYPE html>
<html>
<head>
    <title>CGI Login Test</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            padding: 20px;
            background: #1a1a1a;
            color: #00ff9d;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }
        .container {
            background: rgba(255,255,255,0.1);
            padding: 40px;
            border-radius: 10px;
            text-align: center;
            max-width: 600px;
        }
        h1 {
            color: #00ff9d;
            margin-bottom: 20px;
        }
        .info {
            text-align: left;
            margin-top: 20px;
            background: rgba(0,0,0,0.2);
            padding: 15px;
            border-radius: 5px;
        }
        button {
            background: #00ff9d;
            color: #1a1a1a;
            border: none;
            padding: 10px 20px;
            border-radius: 5px;
            cursor: pointer;
            margin-top: 20px;
            font-weight: bold;
        }
        button:hover {
            background: #00cc7d;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>CGI Test Successful!</h1>
        <div class="info">
            <p><strong>Server Software:</strong> {}</p>
            <p><strong>Server Protocol:</strong> {}</p>
            <p><strong>Request Method:</strong> {}</p>
            <p><strong>Script Name:</strong> {}</p>
        </div>
        <button onclick="window.location.href='/'">Return Home</button>
    </div>
</body>
</html>
""".format(
    os.environ.get('SERVER_SOFTWARE', 'Unknown'),
    os.environ.get('SERVER_PROTOCOL', 'Unknown'),
    os.environ.get('REQUEST_METHOD', 'Unknown'),
    os.environ.get('SCRIPT_NAME', 'Unknown')
))