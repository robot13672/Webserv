# # # # # # # # # # # # # #!/usr/bin/env python3
# # # # # # # # # # # # # import os
# # # # # # # # # # # # # import sys
# # # # # # # # # # # # # from http import cookies
# # # # # # # # # # # # # from urllib.parse import parse_qs

# # # # # # # # # # # # # # First print headers
# # # # # # # # # # # # # # print("Content-Type: text/html")

# # # # # # # # # # # # # try:
# # # # # # # # # # # # #     # Get query parameters
# # # # # # # # # # # # #     query_string = os.environ.get('QUERY_STRING', '')
# # # # # # # # # # # # #     params = parse_qs(query_string)
    
# # # # # # # # # # # # #     # Get username from query parameters
# # # # # # # # # # # # #     username = params.get('username', [''])[0]
    
# # # # # # # # # # # # #     if username:
# # # # # # # # # # # # #         # Create cookie
# # # # # # # # # # # # #         cookie = cookies.SimpleCookie()
# # # # # # # # # # # # #         cookie['username'] = username
# # # # # # # # # # # # #         cookie['username']['path'] = '/'
# # # # # # # # # # # # #         cookie['username']['max-age'] = 3600  # 1 hour
        
# # # # # # # # # # # # #         # Print cookie header
# # # # # # # # # # # # #         # print(cookie)
    
# # # # # # # # # # # # #     # Print empty line to separate headers from body
# # # # # # # # # # # # #     print()
    
# # # # # # # # # # # # #     # Get existing cookie if any
# # # # # # # # # # # # #     stored_cookie = cookies.SimpleCookie(os.environ.get("HTTP_COOKIE", ""))
# # # # # # # # # # # # #     stored_username = stored_cookie.get("username")
    
# # # # # # # # # # # # #     if username:
# # # # # # # # # # # # #         # Show success page with cookie info
# # # # # # # # # # # # #         print(f"""
# # # # # # # # # # # # # <!DOCTYPE html>
# # # # # # # # # # # # # <html>
# # # # # # # # # # # # # <head>
# # # # # # # # # # # # #     <title>Cookie Set</title>
# # # # # # # # # # # # #     <style>
# # # # # # # # # # # # #         * {{
# # # # # # # # # # # # #             margin: 0;
# # # # # # # # # # # # #             padding: 0;
# # # # # # # # # # # # #             box-sizing: border-box;
# # # # # # # # # # # # #         }}
# # # # # # # # # # # # #         body {{
# # # # # # # # # # # # #             background-color: #1a1a1a;
# # # # # # # # # # # # #             color: #00ff9d;
# # # # # # # # # # # # #             font-family: Arial, sans-serif;
# # # # # # # # # # # # #             display: flex;
# # # # # # # # # # # # #             flex-direction: column;
# # # # # # # # # # # # #             align-items: center;
# # # # # # # # # # # # #             min-height: 100vh;
# # # # # # # # # # # # #             padding: 40px 20px;
# # # # # # # # # # # # #         }}
# # # # # # # # # # # # #         .nav-buttons {{
# # # # # # # # # # # # #             position: fixed;
# # # # # # # # # # # # #             top: 20px;
# # # # # # # # # # # # #             left: 20px;
# # # # # # # # # # # # #             right: 20px;
# # # # # # # # # # # # #             display: flex;
# # # # # # # # # # # # #             justify-content: space-between;
# # # # # # # # # # # # #             gap: 20px;
# # # # # # # # # # # # #             z-index: 1000;
# # # # # # # # # # # # #         }}
# # # # # # # # # # # # #         .button {{
# # # # # # # # # # # # #             padding: 12px 24px;
# # # # # # # # # # # # #             font-size: 18px;
# # # # # # # # # # # # #             border: none;
# # # # # # # # # # # # #             border-radius: 5px;
# # # # # # # # # # # # #             cursor: pointer;
# # # # # # # # # # # # #             font-weight: bold;
# # # # # # # # # # # # #             transition: all 0.3s ease;
# # # # # # # # # # # # #         }}
# # # # # # # # # # # # #         .home-button {{
# # # # # # # # # # # # #             background: #00ff9d;
# # # # # # # # # # # # #             color: #1a1a1a;
# # # # # # # # # # # # #         }}
# # # # # # # # # # # # #         .cgi-button {{
# # # # # # # # # # # # #             background: #ff6b6b;
# # # # # # # # # # # # #             color: white;
# # # # # # # # # # # # #         }}
# # # # # # # # # # # # #         .cookie-button {{
# # # # # # # # # # # # #             background: #ff6b6b;
# # # # # # # # # # # # #             color: white;
# # # # # # # # # # # # #         }}
# # # # # # # # # # # # #         .button:hover {{
# # # # # # # # # # # # #             transform: translateY(-2px);
# # # # # # # # # # # # #             opacity: 0.9;
# # # # # # # # # # # # #         }}
# # # # # # # # # # # # #         .container {{
# # # # # # # # # # # # #             background: rgba(255, 255, 255, 0.1);
# # # # # # # # # # # # #             padding: 40px;
# # # # # # # # # # # # #             border-radius: 10px;
# # # # # # # # # # # # #             text-align: center;
# # # # # # # # # # # # #             margin-top: 80px;
# # # # # # # # # # # # #             max-width: 600px;
# # # # # # # # # # # # #             width: 100%;
# # # # # # # # # # # # #         }}
# # # # # # # # # # # # #         h1 {{
# # # # # # # # # # # # #             margin-bottom: 20px;
# # # # # # # # # # # # #             color: #00ff9d;
# # # # # # # # # # # # #             font-size: 32px;
# # # # # # # # # # # # #         }}
# # # # # # # # # # # # #         .info {{
# # # # # # # # # # # # #             margin: 20px 0;
# # # # # # # # # # # # #             padding: 20px;
# # # # # # # # # # # # #             background: rgba(0, 0, 0, 0.2);
# # # # # # # # # # # # #             border-radius: 5px;
# # # # # # # # # # # # #             text-align: left;
# # # # # # # # # # # # #         }}
# # # # # # # # # # # # #         .info p {{
# # # # # # # # # # # # #             margin: 10px 0;
# # # # # # # # # # # # #         }}
# # # # # # # # # # # # #     </style>
# # # # # # # # # # # # # </head>
# # # # # # # # # # # # # <body>
# # # # # # # # # # # # #     <div class="nav-buttons">
# # # # # # # # # # # # #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# # # # # # # # # # # # #         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# # # # # # # # # # # # #         <button class="button cookie-button" onclick="location.href='/cookie'">COOKIE</button>
# # # # # # # # # # # # #     </div>
# # # # # # # # # # # # #     <div class="container">
# # # # # # # # # # # # #         <h1>Cookie Set Successfully!</h1>
# # # # # # # # # # # # #         <div class="info">
# # # # # # # # # # # # #             <p><strong>Username:</strong> {username}</p>
# # # # # # # # # # # # #             <p><strong>Cookie Expiry:</strong> 1 hour</p>
# # # # # # # # # # # # #         </div>
# # # # # # # # # # # # #     </div>
# # # # # # # # # # # # # </body>
# # # # # # # # # # # # # </html>
# # # # # # # # # # # # # """)

# # # # # # # # # # # # # except Exception as e:
# # # # # # # # # # # # #     print(f"Error: {str(e)}")
# # # # # # # # # # # # #     sys.exit(1)


# # # # # # # # # # # # #!/usr/bin/env python3
# # # # # # # # # # # # import os
# # # # # # # # # # # # import sys
# # # # # # # # # # # # from http import cookies
# # # # # # # # # # # # from urllib.parse import parse_qs

# # # # # # # # # # # # try:
# # # # # # # # # # # #     # Get query parameters
# # # # # # # # # # # #     query_string = os.environ.get('QUERY_STRING', '')
# # # # # # # # # # # #     params = parse_qs(query_string)
    
# # # # # # # # # # # #     # Get username from query parameters
# # # # # # # # # # # #     username = params.get('username', [''])[0]
    
# # # # # # # # # # # #     if username:
# # # # # # # # # # # #         # Create cookie
# # # # # # # # # # # #         cookie = cookies.SimpleCookie()
# # # # # # # # # # # #         cookie['username'] = username
# # # # # # # # # # # #         cookie['username']['path'] = '/'
# # # # # # # # # # # #         cookie['username']['max-age'] = 3600  # 1 hour
        
# # # # # # # # # # # #         # Print cookie header FIRST
# # # # # # # # # # # #         print(cookie.output())
        
# # # # # # # # # # # #     # Then print content type header
# # # # # # # # # # # #     print("Content-Type: text/html")
# # # # # # # # # # # #     print()  # Empty line to separate headers from content
    
# # # # # # # # # # # #     # Get existing cookie if any
# # # # # # # # # # # #     stored_cookie = cookies.SimpleCookie(os.environ.get("HTTP_COOKIE", ""))
# # # # # # # # # # # #     stored_username = stored_cookie.get("username")
    
# # # # # # # # # # # #     if username:
# # # # # # # # # # # #         # Show success page with cookie info
# # # # # # # # # # # #         print(f"""
# # # # # # # # # # # # <!DOCTYPE html>
# # # # # # # # # # # # <html>
# # # # # # # # # # # # <head>
# # # # # # # # # # # #     <title>Cookie Set</title>
# # # # # # # # # # # #     <style>
# # # # # # # # # # # #         * {{
# # # # # # # # # # # #              margin: 0;
# # # # # # # # # # # #              padding: 0;
# # # # # # # # # # # #              box-sizing: border-box;
# # # # # # # # # # # #          }}
# # # # # # # # # # # #          body {{
# # # # # # # # # # # #              background-color: 1a1a1a;
# # # # # # # # # # # #              color: 00ff9d;
# # # # # # # # # # # #              font-family: Arial, sans-serif;
# # # # # # # # # # # #              display: flex;
# # # # # # # # # # # #              flex-direction: column;
# # # # # # # # # # # #              align-items: center;
# # # # # # # # # # # #              min-height: 100vh;
# # # # # # # # # # # #              padding: 40px 20px;
# # # # # # # # # # # #          }}
# # # # # # # # # # # #          .nav-buttons {{
# # # # # # # # # # # #              position: fixed;
# # # # # # # # # # # #              top: 20px;
# # # # # # # # # # # #              left: 20px;
# # # # # # # # # # # #              right: 20px;
# # # # # # # # # # # #              display: flex;
# # # # # # # # # # # #              justify-content: space-between;
# # # # # # # # # # # #              gap: 20px;
# # # # # # # # # # # #              z-index: 1000;
# # # # # # # # # # # #          }}
# # # # # # # # # # # #          .button {{
# # # # # # # # # # # #              padding: 12px 24px;
# # # # # # # # # # # #              font-size: 18px;
# # # # # # # # # # # #              border: none;
# # # # # # # # # # # #              border-radius: 5px;
# # # # # # # # # # # #              cursor: pointer;
# # # # # # # # # # # #              font-weight: bold;
# # # # # # # # # # # #              transition: all 0.3s ease;
# # # # # # # # # # # #          }}
# # # # # # # # # # # #          .home-button {{
# # # # # # # # # # # #              background: 00ff9d;
# # # # # # # # # # # #              color: 1a1a1a;
# # # # # # # # # # # #          }}
# # # # # # # # # # # #          .cgi-button {{
# # # # # # # # # # # #              background: ff6b6b;
# # # # # # # # # # # #              color: white;
# # # # # # # # # # # #          }}
# # # # # # # # # # # #          .cookie-button {{
# # # # # # # # # # # #              background: ff6b6b;
# # # # # # # # # # # #              color: white;
# # # # # # # # # # # #          }}
# # # # # # # # # # # #          .button:hover {{
# # # # # # # # # # # #              transform: translateY(-2px);
# # # # # # # # # # # #              opacity: 0.9;
# # # # # # # # # # # #          }}
# # # # # # # # # # # #          .container {{
# # # # # # # # # # # #              background: rgba(255, 255, 255, 0.1);
# # # # # # # # # # # #              padding: 40px;
# # # # # # # # # # # #              border-radius: 10px;
# # # # # # # # # # # #              text-align: center;
# # # # # # # # # # # #              margin-top: 80px;
# # # # # # # # # # # #              max-width: 600px;
# # # # # # # # # # # #              width: 100%;
# # # # # # # # # # # #          }}
# # # # # # # # # # # #          h1 {{
# # # # # # # # # # # #              margin-bottom: 20px;
# # # # # # # # # # # #              color: 00ff9d;
# # # # # # # # # # # #              font-size: 32px;
# # # # # # # # # # # #          }}
# # # # # # # # # # # #          .info {{
# # # # # # # # # # # #              margin: 20px 0;
# # # # # # # # # # # #              padding: 20px;
# # # # # # # # # # # #              background: rgba(0, 0, 0, 0.2);
# # # # # # # # # # # #              border-radius: 5px;
# # # # # # # # # # # #              text-align: left;
# # # # # # # # # # # #          }}
# # # # # # # # # # # #          .info p {{
# # # # # # # # # # # #              margin: 10px 0;
# # # # # # # # # # # #          }}
# # # # # # # # # # # #     </style>
# # # # # # # # # # # # </head>
# # # # # # # # # # # # <body>
# # # # # # # # # # # #     <div class="nav-buttons">
# # # # # # # # # # # #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# # # # # # # # # # # #         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# # # # # # # # # # # #     </div>
# # # # # # # # # # # #     <div class="container">
# # # # # # # # # # # #         <h1>Cookie Set Successfully!</h1>
# # # # # # # # # # # #         <div class="info">
# # # # # # # # # # # #             <p><strong>Username:</strong> {username}</p>
# # # # # # # # # # # #             <p><strong>Cookie Expiry:</strong> 1 hour</p>
# # # # # # # # # # # #         </div>
# # # # # # # # # # # #     </div>
# # # # # # # # # # # # </body>
# # # # # # # # # # # # </html>
# # # # # # # # # # # # """)

# # # # # # # # # # # # except Exception as e:
# # # # # # # # # # # #     print("Content-Type: text/plain\n")
# # # # # # # # # # # #     print(f"Error: {str(e)}")
# # # # # # # # # # # #     sys.exit(1)


# # # # # # # # # # # #!/usr/bin/env python3
# # # # # # # # # # # # import os
# # # # # # # # # # # # import sys
# # # # # # # # # # # # from http import cookies
# # # # # # # # # # # # from urllib.parse import parse_qs

# # # # # # # # # # # # try:
# # # # # # # # # # # #     # Get query parameters
# # # # # # # # # # # #     query_string = os.environ.get('QUERY_STRING', '')
# # # # # # # # # # # #     params = parse_qs(query_string)
    
# # # # # # # # # # # #     # Get username from query parameters
# # # # # # # # # # # #     username = params.get('username', [''])[0]
    
# # # # # # # # # # # #     if username:
# # # # # # # # # # # #         # Create cookie
# # # # # # # # # # # #         cookie = cookies.SimpleCookie()
# # # # # # # # # # # #         cookie['username'] = username
# # # # # # # # # # # #         cookie['username']['path'] = '/'
# # # # # # # # # # # #         cookie['username']['max-age'] = 3600  # 1 hour
        
# # # # # # # # # # # #         # Print cookie header FIRST
# # # # # # # # # # # #         print(cookie.output())
        
# # # # # # # # # # # #     # Then print content type header
# # # # # # # # # # # #     print("Content-Type: text/html")
# # # # # # # # # # # #     print()  # Empty line to separate headers from content
    
# # # # # # # # # # # #     if username:
# # # # # # # # # # # #         # Show success page with cookie info
# # # # # # # # # # # #         print(f"""
# # # # # # # # # # # # <!DOCTYPE html>
# # # # # # # # # # # # <html>
# # # # # # # # # # # # <head>
# # # # # # # # # # # #     <title>Cookie Set</title>
# # # # # # # # # # # #     <style>
# # # # # # # # # # #         # * {{
# # # # # # # # # # #         #     margin: 0;
# # # # # # # # # # #         #     padding: 0;
# # # # # # # # # # #         #     box-sizing: border-box;
# # # # # # # # # # #         # }}
# # # # # # # # # # #         # body {{
# # # # # # # # # # #         #     background-color: #1a1a1a;
# # # # # # # # # # #         #     color: #00ff9d;
# # # # # # # # # # #         #     font-family: Arial, sans-serif;
# # # # # # # # # # #         #     display: flex;
# # # # # # # # # # #         #     flex-direction: column;
# # # # # # # # # # #         #     align-items: center;
# # # # # # # # # # #         #     min-height: 100vh;
# # # # # # # # # # #         #     padding: 40px 20px;
# # # # # # # # # # #         # }}
# # # # # # # # # # #         # .nav-buttons {{
# # # # # # # # # # #         #     position: fixed;
# # # # # # # # # # #         #     top: 20px;
# # # # # # # # # # #         #     left: 20px;
# # # # # # # # # # #         #     right: 20px;
# # # # # # # # # # #         #     display: flex;
# # # # # # # # # # #         #     justify-content: space-between;
# # # # # # # # # # #         #     gap: 20px;
# # # # # # # # # # #         #     z-index: 1000;
# # # # # # # # # # #         # }}
# # # # # # # # # # #         # .button {{
# # # # # # # # # # #         #     padding: 12px 24px;
# # # # # # # # # # #         #     font-size: 18px;
# # # # # # # # # # #         #     border: none;
# # # # # # # # # # #         #     border-radius: 5px;
# # # # # # # # # # #         #     cursor: pointer;
# # # # # # # # # # #         #     font-weight: bold;
# # # # # # # # # # #         #     transition: all 0.3s ease;
# # # # # # # # # # #         # }}
# # # # # # # # # # #         # .home-button {{
# # # # # # # # # # #         #     background: #00ff9d;
# # # # # # # # # # #         #     color: #1a1a1a;
# # # # # # # # # # #         # }}
# # # # # # # # # # #         # .cgi-button {{
# # # # # # # # # # #         #     background: #ff6b6b;
# # # # # # # # # # #         #     color: white;
# # # # # # # # # # #         # }}
# # # # # # # # # # #         # .button:hover {{
# # # # # # # # # # #         #     transform: translateY(-2px);
# # # # # # # # # # #         #     opacity: 0.9;
# # # # # # # # # # #         # }}
# # # # # # # # # # #         # .container {{
# # # # # # # # # # #         #     background: rgba(255, 255, 255, 0.1);
# # # # # # # # # # #         #     padding: 40px;
# # # # # # # # # # #         #     border-radius: 10px;
# # # # # # # # # # #         #     text-align: center;
# # # # # # # # # # #         #     margin-top: 80px;
# # # # # # # # # # #         #     max-width: 600px;
# # # # # # # # # # #         #     width: 100%;
# # # # # # # # # # #         # }}
# # # # # # # # # # #         # h1 {{
# # # # # # # # # # #         #     margin-bottom: 20px;
# # # # # # # # # # #         #     color: #00ff9d;
# # # # # # # # # # #         #     font-size: 32px;
# # # # # # # # # # #         # }}
# # # # # # # # # # #         # .info {{
# # # # # # # # # # #         #     margin: 20px 0;
# # # # # # # # # # #         #     padding: 20px;
# # # # # # # # # # #         #     background: rgba(0, 0, 0, 0.2);
# # # # # # # # # # #         #     border-radius: 5px;
# # # # # # # # # # #         #     text-align: left;
# # # # # # # # # # #         # }}
# # # # # # # # # # #         # .info p {{
# # # # # # # # # # #         #     margin: 10px 0;
# # # # # # # # # # #         # }}
# # # # # # # # # # # #     </style>
# # # # # # # # # # # # </head>
# # # # # # # # # # # # <body>
# # # # # # # # # # # #     <div class="nav-buttons">
# # # # # # # # # # # #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# # # # # # # # # # # #         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# # # # # # # # # # # #     </div>
# # # # # # # # # # # #     <div class="container">
# # # # # # # # # # # #         <h1>Cookie Set Successfully!</h1>
# # # # # # # # # # # #         <div class="info">
# # # # # # # # # # # #             <p><strong>Username:</strong> {username}</p>
# # # # # # # # # # # #             <p><strong>Cookie Expiry:</strong> 1 hour</p>
# # # # # # # # # # # #         </div>
# # # # # # # # # # # #     </div>
# # # # # # # # # # # # </body>
# # # # # # # # # # # # </html>
# # # # # # # # # # # # """)

# # # # # # # # # # # # except Exception as e:
# # # # # # # # # # # #     print("Content-Type: text/plain\n")
# # # # # # # # # # # #     print(f"Error: {str(e)}")
# # # # # # # # # # # #     sys.exit(1)

# # # # # # # # # # # #!/usr/bin/env python3
# # # # # # # # # # # import os
# # # # # # # # # # # import sys
# # # # # # # # # # # from http import cookies
# # # # # # # # # # # from urllib.parse import parse_qs

# # # # # # # # # # # print("Content-Type: text/html")

# # # # # # # # # # # try:
# # # # # # # # # # #     # Get query parameters
# # # # # # # # # # #     query_string = os.environ.get('QUERY_STRING', '')
# # # # # # # # # # #     params = parse_qs(query_string)
    
# # # # # # # # # # #     # Get username from query parameters
# # # # # # # # # # #     username = params.get('username', [''])[0]
    
# # # # # # # # # # #     if username:
# # # # # # # # # # #         # Create cookie - IMPORTANT: Print cookie before content type
# # # # # # # # # # #         cookie = cookies.SimpleCookie()
# # # # # # # # # # #         cookie['username'] = username
# # # # # # # # # # #         cookie['username']['path'] = '/'
# # # # # # # # # # #         cookie['username']['max-age'] = 3600  # 1 hour
# # # # # # # # # # #         print(cookie.output())  # This prints Set-Cookie header
        
# # # # # # # # # # #     # Print blank line to separate headers from body
# # # # # # # # # # #     print()
    
# # # # # # # # # # #     # Get existing cookie if any
# # # # # # # # # # #     stored_cookie = cookies.SimpleCookie(os.environ.get("HTTP_COOKIE", ""))
# # # # # # # # # # #     stored_username = stored_cookie.get("username")
    
# # # # # # # # # # #     if username:
# # # # # # # # # # #         print("""
# # # # # # # # # # # <!DOCTYPE html>
# # # # # # # # # # # <html>
# # # # # # # # # # # <head>
# # # # # # # # # # #     <title>Cookie Set</title>
# # # # # # # # # # #     <style>
# # # # # # # # # # #                  * {{
# # # # # # # # # # #             margin: 0;
# # # # # # # # # # #             padding: 0;
# # # # # # # # # # #             box-sizing: border-box;
# # # # # # # # # # #         }}
# # # # # # # # # # #         body {{
# # # # # # # # # # #             background-color: #1a1a1a;
# # # # # # # # # # #             color: #00ff9d;
# # # # # # # # # # #             font-family: Arial, sans-serif;
# # # # # # # # # # #             display: flex;
# # # # # # # # # # #             flex-direction: column;
# # # # # # # # # # #             align-items: center;
# # # # # # # # # # #             min-height: 100vh;
# # # # # # # # # # #             padding: 40px 20px;
# # # # # # # # # # #         }}
# # # # # # # # # # #         .nav-buttons {{
# # # # # # # # # # #             position: fixed;
# # # # # # # # # # #             top: 20px;
# # # # # # # # # # #             left: 20px;
# # # # # # # # # # #             right: 20px;
# # # # # # # # # # #             display: flex;
# # # # # # # # # # #             justify-content: space-between;
# # # # # # # # # # #             gap: 20px;
# # # # # # # # # # #             z-index: 1000;
# # # # # # # # # # #         }}
# # # # # # # # # # #         .button {{
# # # # # # # # # # #             padding: 12px 24px;
# # # # # # # # # # #             font-size: 18px;
# # # # # # # # # # #             border: none;
# # # # # # # # # # #             border-radius: 5px;
# # # # # # # # # # #             cursor: pointer;
# # # # # # # # # # #             font-weight: bold;
# # # # # # # # # # #             transition: all 0.3s ease;
# # # # # # # # # # #         }}
# # # # # # # # # # #         .home-button {{
# # # # # # # # # # #             background: #00ff9d;
# # # # # # # # # # #             color: #1a1a1a;
# # # # # # # # # # #         }}
# # # # # # # # # # #         .cgi-button {{
# # # # # # # # # # #             background: #ff6b6b;
# # # # # # # # # # #             color: white;
# # # # # # # # # # #         }}
# # # # # # # # # # #         .button:hover {{
# # # # # # # # # # #             transform: translateY(-2px);
# # # # # # # # # # #             opacity: 0.9;
# # # # # # # # # # #         }}
# # # # # # # # # # #         .container {{
# # # # # # # # # # #             background: rgba(255, 255, 255, 0.1);
# # # # # # # # # # #             padding: 40px;
# # # # # # # # # # #             border-radius: 10px;
# # # # # # # # # # #             text-align: center;
# # # # # # # # # # #             margin-top: 80px;
# # # # # # # # # # #             max-width: 600px;
# # # # # # # # # # #             width: 100%;
# # # # # # # # # # #         }}
# # # # # # # # # # #         h1 {{
# # # # # # # # # # #             margin-bottom: 20px;
# # # # # # # # # # #             color: #00ff9d;
# # # # # # # # # # #             font-size: 32px;
# # # # # # # # # # #         }}
# # # # # # # # # # #         .info {{
# # # # # # # # # # #             margin: 20px 0;
# # # # # # # # # # #             padding: 20px;
# # # # # # # # # # #             background: rgba(0, 0, 0, 0.2);
# # # # # # # # # # #             border-radius: 5px;
# # # # # # # # # # #             text-align: left;
# # # # # # # # # # #         }}
# # # # # # # # # # #         .info p {{
# # # # # # # # # # #             margin: 10px 0;
# # # # # # # # # # #         }}
# # # # # # # # # # #      </style>
# # # # # # # # # # # </head>
# # # # # # # # # # # <body>
# # # # # # # # # # #     <div class="container">
# # # # # # # # # # #         <h1>Cookie Set Successfully!</h1>
# # # # # # # # # # #         <div class="info">
# # # # # # # # # # #             <p><strong>Username:</strong> {username}</p>
# # # # # # # # # # #             <p><strong>Cookie Expiry:</strong> 1 hour</p>
# # # # # # # # # # #         </div>
# # # # # # # # # # #     </div>
# # # # # # # # # # # </body>
# # # # # # # # # # # </html>
# # # # # # # # # # # """.format(username=username))

# # # # # # # # # # # except Exception as e:
# # # # # # # # # # #     print("Content-Type: text/plain\n")
# # # # # # # # # # #     print(f"Error: {str(e)}")
# # # # # # # # # # #     sys.exit(1)


# # # # # # # # # # #!/usr/bin/env python3
# # # # # # # # # # import os
# # # # # # # # # # import sys
# # # # # # # # # # from http import cookies
# # # # # # # # # # from urllib.parse import parse_qs

# # # # # # # # # # try:
# # # # # # # # # #     # Get query parameters
# # # # # # # # # #     query_string = os.environ.get('QUERY_STRING', '')
# # # # # # # # # #     params = parse_qs(query_string)
    
# # # # # # # # # #     # Get username from query parameters
# # # # # # # # # #     username = params.get('username', [''])[0]
    
# # # # # # # # # #     if username:
# # # # # # # # # #         # Create cookie - IMPORTANT: Print cookie before content type
# # # # # # # # # #         cookie = cookies.SimpleCookie()
# # # # # # # # # #         cookie['username'] = username
# # # # # # # # # #         cookie['username']['path'] = '/'
# # # # # # # # # #         cookie['username']['max-age'] = 3600  # 1 hour
# # # # # # # # # #         print(cookie.output())  # This prints Set-Cookie header
        
# # # # # # # # # #     # Print content type AFTER cookie headers
# # # # # # # # # #     print("Content-Type: text/html")
# # # # # # # # # #     print()  # Empty line to separate headers from body
    
# # # # # # # # # #     if username:
# # # # # # # # # #         print("""<!DOCTYPE html>
# # # # # # # # # # <html>
# # # # # # # # # # <head>
# # # # # # # # # #     <meta charset="UTF-8">
# # # # # # # # # #     <title>Cookie Set</title>
# # # # # # # # # #     <style>
# # # # # # # # # #         * {
# # # # # # # # # #             margin: 0;
# # # # # # # # # #             padding: 0;
# # # # # # # # # #             box-sizing: border-box;
# # # # # # # # # #         }
# # # # # # # # # #         body {
# # # # # # # # # #             background-color: #1a1a1a;
# # # # # # # # # #             color: #00ff9d;
# # # # # # # # # #             font-family: Arial, sans-serif;
# # # # # # # # # #             display: flex;
# # # # # # # # # #             flex-direction: column;
# # # # # # # # # #             align-items: center;
# # # # # # # # # #             min-height: 100vh;
# # # # # # # # # #             padding: 40px 20px;
# # # # # # # # # #         }
# # # # # # # # # #         .nav-buttons {
# # # # # # # # # #             position: fixed;
# # # # # # # # # #             top: 20px;
# # # # # # # # # #             left: 20px;
# # # # # # # # # #             right: 20px;
# # # # # # # # # #             display: flex;
# # # # # # # # # #             justify-content: space-between;
# # # # # # # # # #             gap: 20px;
# # # # # # # # # #             z-index: 1000;
# # # # # # # # # #         }
# # # # # # # # # #         .button {
# # # # # # # # # #             padding: 12px 24px;
# # # # # # # # # #             font-size: 18px;
# # # # # # # # # #             border: none;
# # # # # # # # # #             border-radius: 5px;
# # # # # # # # # #             cursor: pointer;
# # # # # # # # # #             font-weight: bold;
# # # # # # # # # #             transition: all 0.3s ease;
# # # # # # # # # #         }
# # # # # # # # # #         .home-button {
# # # # # # # # # #             background: #00ff9d;
# # # # # # # # # #             color: #1a1a1a;
# # # # # # # # # #         }
# # # # # # # # # #         .cgi-button {
# # # # # # # # # #             background: #ff6b6b;
# # # # # # # # # #             color: white;
# # # # # # # # # #         }
# # # # # # # # # #         .button:hover {
# # # # # # # # # #             transform: translateY(-2px);
# # # # # # # # # #             opacity: 0.9;
# # # # # # # # # #         }
# # # # # # # # # #         .container {
# # # # # # # # # #             background: rgba(255, 255, 255, 0.1);
# # # # # # # # # #             padding: 40px;
# # # # # # # # # #             border-radius: 10px;
# # # # # # # # # #             text-align: center;
# # # # # # # # # #             margin-top: 80px;
# # # # # # # # # #             max-width: 600px;
# # # # # # # # # #             width: 100%;
# # # # # # # # # #         }
# # # # # # # # # #         h1 {
# # # # # # # # # #             margin-bottom: 20px;
# # # # # # # # # #             color: #00ff9d;
# # # # # # # # # #             font-size: 32px;
# # # # # # # # # #         }
# # # # # # # # # #         .info {
# # # # # # # # # #             margin: 20px 0;
# # # # # # # # # #             padding: 20px;
# # # # # # # # # #             background: rgba(0, 0, 0, 0.2);
# # # # # # # # # #             border-radius: 5px;
# # # # # # # # # #             text-align: left;
# # # # # # # # # #         }
# # # # # # # # # #         .info p {
# # # # # # # # # #             margin: 10px 0;
# # # # # # # # # #         }
# # # # # # # # # #     </style>
# # # # # # # # # # </head>
# # # # # # # # # # <body>
# # # # # # # # # #     <div class="nav-buttons">
# # # # # # # # # #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# # # # # # # # # #         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# # # # # # # # # #     </div>
# # # # # # # # # #     <div class="container">
# # # # # # # # # #         <h1>Cookie Set Successfully!</h1>
# # # # # # # # # #         <div class="info">
# # # # # # # # # #             <p><strong>Username:</strong> {username}</p>
# # # # # # # # # #             <p><strong>Cookie Expiry:</strong> 1 hour</p>
# # # # # # # # # #         </div>
# # # # # # # # # #     </div>
# # # # # # # # # # </body>
# # # # # # # # # # </html>""".format(username=username))

# # # # # # # # # # except Exception as e:
# # # # # # # # # #     print("Content-Type: text/plain\n")
# # # # # # # # # #     print(f"Error: {str(e)}")
# # # # # # # # # #     sys.exit(1)



# # # # # # # # # #!/usr/bin/env python3
# # # # # # # # # import os
# # # # # # # # # import sys
# # # # # # # # # from http import cookies
# # # # # # # # # from urllib.parse import parse_qs

# # # # # # # # # try:
# # # # # # # # #     # Get query parameters
# # # # # # # # #     query_string = os.environ.get('QUERY_STRING', '')
# # # # # # # # #     params = parse_qs(query_string)
    
# # # # # # # # #     # Get username from query parameters
# # # # # # # # #     username = params.get('username', [''])[0]
    
# # # # # # # # #     if username:
# # # # # # # # #         # Create cookie
# # # # # # # # #         cookie = cookies.SimpleCookie()
# # # # # # # # #         cookie['username'] = username
# # # # # # # # #         cookie['username']['path'] = '/'
# # # # # # # # #         cookie['username']['max-age'] = 3600  # 1 hour
# # # # # # # # #         # Print cookie header FIRST
# # # # # # # # #         print(cookie.output())
    
# # # # # # # # #     # Print content type and charset
# # # # # # # # #     print("Content-Type: text/html; charset=utf-8")
# # # # # # # # #     # Empty line to separate headers from content
# # # # # # # # #     print()
    
# # # # # # # # #     if username:
# # # # # # # # #         print("""<!DOCTYPE html>
# # # # # # # # # <html lang="en">
# # # # # # # # # <head>
# # # # # # # # #     <meta charset="UTF-8">
# # # # # # # # #     <meta name="viewport" content="width=device-width, initial-scale=1.0">
# # # # # # # # #     <title>Cookie Set</title>
# # # # # # # # #     <style>
# # # # # # # # #         * {
# # # # # # # # #             margin: 0;
# # # # # # # # #             padding: 0;
# # # # # # # # #             box-sizing: border-box;
# # # # # # # # #         }
# # # # # # # # #         body {
# # # # # # # # #             background-color: #1a1a1a;
# # # # # # # # #             color: #00ff9d;
# # # # # # # # #             font-family: Arial, sans-serif;
# # # # # # # # #             display: flex;
# # # # # # # # #             flex-direction: column;
# # # # # # # # #             align-items: center;
# # # # # # # # #             min-height: 100vh;
# # # # # # # # #             padding: 40px 20px;
# # # # # # # # #         }
# # # # # # # # #         .nav-buttons {
# # # # # # # # #             position: fixed;
# # # # # # # # #             top: 20px;
# # # # # # # # #             left: 20px;
# # # # # # # # #             right: 20px;
# # # # # # # # #             display: flex;
# # # # # # # # #             justify-content: space-between;
# # # # # # # # #             gap: 20px;
# # # # # # # # #             z-index: 1000;
# # # # # # # # #         }
# # # # # # # # #         .button {
# # # # # # # # #             padding: 12px 24px;
# # # # # # # # #             font-size: 18px;
# # # # # # # # #             border: none;
# # # # # # # # #             border-radius: 5px;
# # # # # # # # #             cursor: pointer;
# # # # # # # # #             font-weight: bold;
# # # # # # # # #             transition: all 0.3s ease;
# # # # # # # # #         }
# # # # # # # # #         .home-button {
# # # # # # # # #             background: #00ff9d;
# # # # # # # # #             color: #1a1a1a;
# # # # # # # # #         }
# # # # # # # # #         .cgi-button {
# # # # # # # # #             background: #ff6b6b;
# # # # # # # # #             color: white;
# # # # # # # # #         }
# # # # # # # # #         .button:hover {
# # # # # # # # #             transform: translateY(-2px);
# # # # # # # # #             opacity: 0.9;
# # # # # # # # #         }
# # # # # # # # #         .container {
# # # # # # # # #             background: rgba(255, 255, 255, 0.1);
# # # # # # # # #             padding: 40px;
# # # # # # # # #             border-radius: 10px;
# # # # # # # # #             text-align: center;
# # # # # # # # #             margin-top: 80px;
# # # # # # # # #             max-width: 600px;
# # # # # # # # #             width: 100%;
# # # # # # # # #         }
# # # # # # # # #         h1 {
# # # # # # # # #             margin-bottom: 20px;
# # # # # # # # #             color: #00ff9d;
# # # # # # # # #             font-size: 32px;
# # # # # # # # #         }
# # # # # # # # #         .info {
# # # # # # # # #             margin: 20px 0;
# # # # # # # # #             padding: 20px;
# # # # # # # # #             background: rgba(0, 0, 0, 0.2);
# # # # # # # # #             border-radius: 5px;
# # # # # # # # #             text-align: left;
# # # # # # # # #         }
# # # # # # # # #         .info p {
# # # # # # # # #             margin: 10px 0;
# # # # # # # # #         }
# # # # # # # # #     </style>
# # # # # # # # # </head>
# # # # # # # # # <body>
# # # # # # # # #     <div class="nav-buttons">
# # # # # # # # #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# # # # # # # # #         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# # # # # # # # #     </div>
# # # # # # # # #     <div class="container">
# # # # # # # # #         <h1>Cookie Set Successfully!</h1>
# # # # # # # # #         <div class="info">
# # # # # # # # #             <p><strong>Username:</strong> {username}</p>
# # # # # # # # #             <p><strong>Cookie Expiry:</strong> 1 hour</p>
# # # # # # # # #         </div>
# # # # # # # # #     </div>
# # # # # # # # # </body>
# # # # # # # # # </html>""".format(username=username))

# # # # # # # # # except Exception as e:
# # # # # # # # #     print("Content-Type: text/plain\n")
# # # # # # # # #     print(f"Error: {str(e)}")
# # # # # # # # #     sys.exit(1)

# # # # # # # # # #!/usr/bin/env python3
# # # # # # # # # import os
# # # # # # # # # import sys
# # # # # # # # # from http import cookies
# # # # # # # # # from urllib.parse import parse_qs

# # # # # # # # # try:
# # # # # # # # #     # Get query parameters
# # # # # # # # #     query_string = os.environ.get('QUERY_STRING', '')
# # # # # # # # #     params = parse_qs(query_string)
    
# # # # # # # # #     # Get username from query parameters
# # # # # # # # #     username = params.get('username', [''])[0]
    
# # # # # # # # #     if username:
# # # # # # # # #         # Create and print cookie header FIRST
# # # # # # # # #         cookie = cookies.SimpleCookie()
# # # # # # # # #         cookie['username'] = username
# # # # # # # # #         cookie['username']['path'] = '/'
# # # # # # # # #         cookie['username']['max-age'] = 3600  # 1 hour
# # # # # # # # #         print(cookie.output())
# # # # # # # # #         # Print content type header
# # # # # # # # #         print("Content-Type: text/html; charset=utf-8")
# # # # # # # # #         # Empty line to separate headers from content
# # # # # # # # #         print("\n")
        
# # # # # # # # #         # Print HTML content
# # # # # # # # #         print("""<!DOCTYPE html>
# # # # # # # # # <html lang="en">
# # # # # # # # # <head>
# # # # # # # # #     <meta charset="UTF-8">
# # # # # # # # #     <meta name="viewport" content="width=device-width, initial-scale=1.0">
# # # # # # # # #     <title>Cookie Set</title>
# # # # # # # # #     <style>
# # # # # # # # #         * {
# # # # # # # # #             margin: 0;
# # # # # # # # #             padding: 0;
# # # # # # # # #             box-sizing: border-box;
# # # # # # # # #         }
# # # # # # # # #         body {
# # # # # # # # #             background-color: #1a1a1a;
# # # # # # # # #             color: #00ff9d;
# # # # # # # # #             font-family: Arial, sans-serif;
# # # # # # # # #             display: flex;
# # # # # # # # #             flex-direction: column;
# # # # # # # # #             align-items: center;
# # # # # # # # #             min-height: 100vh;
# # # # # # # # #             padding: 40px 20px;
# # # # # # # # #         }
# # # # # # # # #         .nav-buttons {
# # # # # # # # #             position: fixed;
# # # # # # # # #             top: 20px;
# # # # # # # # #             left: 20px;
# # # # # # # # #             right: 20px;
# # # # # # # # #             display: flex;
# # # # # # # # #             justify-content: space-between;
# # # # # # # # #             gap: 20px;
# # # # # # # # #             z-index: 1000;
# # # # # # # # #         }
# # # # # # # # #         .button {
# # # # # # # # #             padding: 12px 24px;
# # # # # # # # #             font-size: 18px;
# # # # # # # # #             border: none;
# # # # # # # # #             border-radius: 5px;
# # # # # # # # #             cursor: pointer;
# # # # # # # # #             font-weight: bold;
# # # # # # # # #             transition: all 0.3s ease;
# # # # # # # # #         }
# # # # # # # # #         .home-button {
# # # # # # # # #             background: #00ff9d;
# # # # # # # # #             color: #1a1a1a;
# # # # # # # # #         }
# # # # # # # # #         .cgi-button {
# # # # # # # # #             background: #ff6b6b;
# # # # # # # # #             color: white;
# # # # # # # # #         }
# # # # # # # # #         .button:hover {
# # # # # # # # #             transform: translateY(-2px);
# # # # # # # # #             opacity: 0.9;
# # # # # # # # #         }
# # # # # # # # #         .container {
# # # # # # # # #             background: rgba(255, 255, 255, 0.1);
# # # # # # # # #             padding: 40px;
# # # # # # # # #             border-radius: 10px;
# # # # # # # # #             text-align: center;
# # # # # # # # #             margin-top: 80px;
# # # # # # # # #             max-width: 600px;
# # # # # # # # #             width: 100%;
# # # # # # # # #         }
# # # # # # # # #         h1 {
# # # # # # # # #             margin-bottom: 20px;
# # # # # # # # #             color: #00ff9d;
# # # # # # # # #             font-size: 32px;
# # # # # # # # #         }
# # # # # # # # #         .info {
# # # # # # # # #             margin: 20px 0;
# # # # # # # # #             padding: 20px;
# # # # # # # # #             background: rgba(0, 0, 0, 0.2);
# # # # # # # # #             border-radius: 5px;
# # # # # # # # #             text-align: left;
# # # # # # # # #         }
# # # # # # # # #         .info p {
# # # # # # # # #             margin: 10px 0;
# # # # # # # # #         }
# # # # # # # # #     </style>
# # # # # # # # # </head>
# # # # # # # # # <body>
# # # # # # # # #     <div class="nav-buttons">
# # # # # # # # #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# # # # # # # # #         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# # # # # # # # #     </div>
# # # # # # # # #     <div class="container">
# # # # # # # # #         <h1>Cookie Set Successfully!</h1>
# # # # # # # # #         <div class="info">
# # # # # # # # #             <p><strong>Username:</strong> {username}</p>
# # # # # # # # #             <p><strong>Cookie Expiry:</strong> 1 hour</p>
# # # # # # # # #         </div>
# # # # # # # # #     </div>
# # # # # # # # # </body>
# # # # # # # # # </html>""".format(username=username))

# # # # # # # # # except Exception as e:
# # # # # # # # #     print("Content-Type: text/plain\n")
# # # # # # # # #     print(f"Error: {str(e)}")
# # # # # # # # #     sys.exit(1)



# # # # # # # # #!/usr/bin/env python3
# # # # # # # # import os
# # # # # # # # import sys
# # # # # # # # from http import cookies
# # # # # # # # from urllib.parse import parse_qs

# # # # # # # # try:
# # # # # # # #     # Get query parameters
# # # # # # # #     query_string = os.environ.get('QUERY_STRING', '')
# # # # # # # #     params = parse_qs(query_string)
    
# # # # # # # #     # Get username from query parameters
# # # # # # # #     username = params.get('username', [''])[0]
    
# # # # # # # #     if username:
# # # # # # # #         # Create cookie and print headers in correct order
# # # # # # # #         cookie = cookies.SimpleCookie()
# # # # # # # #         cookie['username'] = username
# # # # # # # #         cookie['username']['path'] = '/'
# # # # # # # #         cookie['username']['max-age'] = 3600
# # # # # # # #         print(cookie.output())  # Set-Cookie header
# # # # # # # #         print("Content-Type: text/html; charset=utf-8")
# # # # # # # #         print()  # Empty line after headers
        
# # # # # # # #         # Then print HTML content
# # # # # # # #         print("""<!DOCTYPE html>
# # # # # # # # <html lang="en">
# # # # # # # #  <html>
# # # # # # # #  <head>
# # # # # # # #      <title>Cookie Set</title>
# # # # # # # #      <style>
# # # # # # # #          * {{
# # # # # # # #               margin: 0;
# # # # # # # #               padding: 0;
# # # # # # # #               box-sizing: border-box;
# # # # # # # #           }}
# # # # # # # #           body {{
# # # # # # # #               background-color: 1a1a1a;
# # # # # # # #               color: 00ff9d;
# # # # # # # #               font-family: Arial, sans-serif;
# # # # # # # #               display: flex;
# # # # # # # #               flex-direction: column;
# # # # # # # #               align-items: center;
# # # # # # # #               min-height: 100vh;
# # # # # # # #               padding: 40px 20px;
# # # # # # # #           }}
# # # # # # # #           .nav-buttons {{
# # # # # # # #               position: fixed;
# # # # # # # #               top: 20px;
# # # # # # # #               left: 20px;
# # # # # # # #               right: 20px;
# # # # # # # #               display: flex;
# # # # # # # #               justify-content: space-between;
# # # # # # # #               gap: 20px;
# # # # # # # #               z-index: 1000;
# # # # # # # #           }}
# # # # # # # #           .button {{
# # # # # # # #               padding: 12px 24px;
# # # # # # # #               font-size: 18px;
# # # # # # # #               border: none;
# # # # # # # #               border-radius: 5px;
# # # # # # # #               cursor: pointer;
# # # # # # # #               font-weight: bold;
# # # # # # # #               transition: all 0.3s ease;
# # # # # # # #           }}
# # # # # # # #           .home-button {{
# # # # # # # #               background: 00ff9d;
# # # # # # # #               color: 1a1a1a;
# # # # # # # #           }}
# # # # # # # #           .cgi-button {{
# # # # # # # #               background: ff6b6b;
# # # # # # # #               color: white;
# # # # # # # #           }}
# # # # # # # #           .cookie-button {{
# # # # # # # #               background: ff6b6b;
# # # # # # # #               color: white;
# # # # # # # #           }}
# # # # # # # #           .button:hover {{
# # # # # # # #               transform: translateY(-2px);
# # # # # # # #               opacity: 0.9;
# # # # # # # #           }}
# # # # # # # #           .container {{
# # # # # # # #               background: rgba(255, 255, 255, 0.1);
# # # # # # # #               padding: 40px;
# # # # # # # #               border-radius: 10px;
# # # # # # # #               text-align: center;
# # # # # # # #               margin-top: 80px;
# # # # # # # #               max-width: 600px;
# # # # # # # #               width: 100%;
# # # # # # # #           }}
# # # # # # # #           h1 {{
# # # # # # # #               margin-bottom: 20px;
# # # # # # # #               color: 00ff9d;
# # # # # # # #               font-size: 32px;
# # # # # # # #           }}
# # # # # # # #           .info {{
# # # # # # # #               margin: 20px 0;
# # # # # # # #               padding: 20px;
# # # # # # # #               background: rgba(0, 0, 0, 0.2);
# # # # # # # #               border-radius: 5px;
# # # # # # # #               text-align: left;
# # # # # # # #           }}
# # # # # # # #           .info p {{
# # # # # # # #               margin: 10px 0;
# # # # # # # #           }}
# # # # # # # #      </style>
# # # # # # # #  </head>
# # # # # # # #  <body>
# # # # # # # #      <div class="nav-buttons">
# # # # # # # #          <button class="button home-button" onclick="location.href='/'">HOME</button>
# # # # # # # #          <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# # # # # # # #      </div>
# # # # # # # #      <div class="container">
# # # # # # # #          <h1>Cookie Set Successfully!</h1>
# # # # # # # #          <div class="info">
# # # # # # # #              <p><strong>Username:</strong> {username}</p>
# # # # # # # #              <p><strong>Cookie Expiry:</strong> 1 hour</p>
# # # # # # # #          </div>
# # # # # # # #      </div>
# # # # # # # #  </body>
# # # # # # # #  </html>
# # # # # # # #  """)
        
        
# # # # # # # # except Exception as e:
# # # # # # # #     print("Content-Type: text/plain")
# # # # # # # #     print()  # Empty line after headers
# # # # # # # #     print(f"Error: {str(e)}")
# # # # # # # #     sys.exit(1)

# # # # # # # #!/usr/bin/env python3
# # # # # # # import os
# # # # # # # import sys
# # # # # # # from http import cookies
# # # # # # # from urllib.parse import parse_qs

# # # # # # # try:
# # # # # # #     # Get query parameters
# # # # # # #     query_string = os.environ.get('QUERY_STRING', '')
# # # # # # #     params = parse_qs(query_string)
    
# # # # # # #     # Get username from query parameters
# # # # # # #     username = params.get('username', [''])[0]
    
# # # # # # #     if username:
# # # # # # #         # Set cookie header FIRST
# # # # # # #         cookie = cookies.SimpleCookie()
# # # # # # #         cookie['username'] = username
# # # # # # #         cookie['username']['path'] = '/'
# # # # # # #         cookie['username']['max-age'] = 3600
# # # # # # #         print(cookie.output())
# # # # # # #         # Then content type
# # # # # # #         print("Content-Type: text/html; charset=utf-8")
# # # # # # #         # Empty line after headers
# # # # # # #         print()
        
# # # # # # #         # HTML content
# # # # # # #         print("""<!DOCTYPE html>
# # # # # # # <html lang="en">
# # # # # # # <head>
# # # # # # #     <meta charset="UTF-8">
# # # # # # #     <title>Cookie Set</title>
# # # # # # #     <style>
# # # # # # #         * { margin: 0; padding: 0; box-sizing: border-box; }
# # # # # # #         body {
# # # # # # #             background-color: #1a1a1a;
# # # # # # #             color: #00ff9d;
# # # # # # #             font-family: Arial, sans-serif;
# # # # # # #             display: flex;
# # # # # # #             flex-direction: column;
# # # # # # #             align-items: center;
# # # # # # #             min-height: 100vh;
# # # # # # #             padding: 40px 20px;
# # # # # # #         }
# # # # # # #         .nav-buttons {
# # # # # # #             position: fixed;
# # # # # # #             top: 20px;
# # # # # # #             left: 20px;
# # # # # # #             right: 20px;
# # # # # # #             display: flex;
# # # # # # #             justify-content: space-between;
# # # # # # #             gap: 20px;
# # # # # # #         }
# # # # # # #         .button {
# # # # # # #             padding: 12px 24px;
# # # # # # #             font-size: 18px;
# # # # # # #             border: none;
# # # # # # #             border-radius: 5px;
# # # # # # #             cursor: pointer;
# # # # # # #             font-weight: bold;
# # # # # # #             transition: all 0.3s ease;
# # # # # # #         }
# # # # # # #         .home-button {
# # # # # # #             background: #00ff9d;
# # # # # # #             color: #1a1a1a;
# # # # # # #         }
# # # # # # #         .cgi-button {
# # # # # # #             background: #ff6b6b;
# # # # # # #             color: white;
# # # # # # #         }
# # # # # # #         .container {
# # # # # # #             background: rgba(255, 255, 255, 0.1);
# # # # # # #             padding: 40px;
# # # # # # #             border-radius: 10px;
# # # # # # #             text-align: center;
# # # # # # #             margin-top: 80px;
# # # # # # #             max-width: 600px;
# # # # # # #             width: 100%;
# # # # # # #         }
# # # # # # #         h1 {
# # # # # # #             margin-bottom: 20px;
# # # # # # #             color: #00ff9d;
# # # # # # #             font-size: 32px;
# # # # # # #         }
# # # # # # #         .info {
# # # # # # #             margin: 20px 0;
# # # # # # #             padding: 20px;
# # # # # # #             background: rgba(0, 0, 0, 0.2);
# # # # # # #             border-radius: 5px;
# # # # # # #             text-align: left;
# # # # # # #         }
# # # # # # #         .info p {
# # # # # # #             margin: 10px 0;
# # # # # # #         }
# # # # # # #     </style>
# # # # # # # </head>
# # # # # # # <body>
# # # # # # #     <div class="nav-buttons">
# # # # # # #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# # # # # # #         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# # # # # # #     </div>
# # # # # # #     <div class="container">
# # # # # # #         <h1>Cookie Set Successfully!</h1>
# # # # # # #         <div class="info">
# # # # # # #             <p><strong>Username:</strong> {username}</p>
# # # # # # #             <p><strong>Cookie Expiry:</strong> 1 hour</p>
# # # # # # #         </div>
# # # # # # #     </div>
# # # # # # # </body>
# # # # # # # </html>""".format(username=username))

# # # # # # # except Exception as e:
# # # # # # #     print("Content-Type: text/plain")
# # # # # # #     print()
# # # # # # #     print(f"Error: {str(e)}")
# # # # # # #     sys.exit(1)


# # # # # # #!/usr/bin/env python3
# # # # # # import os
# # # # # # import sys
# # # # # # from http import cookies
# # # # # # from urllib.parse import parse_qs

# # # # # # try:
# # # # # #     # Get query parameters
# # # # # #     query_string = os.environ.get('QUERY_STRING', '')
# # # # # #     params = parse_qs(query_string)
    
# # # # # #     # Get username from query parameters
# # # # # #     username = params.get('username', [''])[0]
    
# # # # # #     if username:
# # # # # #         # Create and set cookie
# # # # # #         cookie = cookies.SimpleCookie()
# # # # # #         cookie['username'] = username
# # # # # #         cookie['username']['path'] = '/'
# # # # # #         cookie['username']['max-age'] = 3600
        
# # # # # #         # Print headers in correct order
# # # # # #         print(f"Set-Cookie: username={username}; Max-Age=3600; Path=/")
# # # # # #         print("Content-Type: text/html; charset=utf-8")
# # # # # #         print()  # Empty line after headers
        
# # # # # #         # Print HTML content
# # # # # #         print("""<!DOCTYPE html>
# # # # # # <html lang="en">
# # # # # # <head>
# # # # # #     <meta charset="UTF-8">
# # # # # #     <title>Cookie Set</title>
# # # # # #     <style>
# # # # # #         * {
# # # # # #             margin: 0;
# # # # # #             padding: 0;
# # # # # #             box-sizing: border-box;
# # # # # #         }
# # # # # #         body {
# # # # # #             background-color: #1a1a1a;
# # # # # #             color: #00ff9d;
# # # # # #             font-family: Arial, sans-serif;
# # # # # #             display: flex;
# # # # # #             flex-direction: column;
# # # # # #             align-items: center;
# # # # # #             min-height: 100vh;
# # # # # #             padding: 40px 20px;
# # # # # #         }
# # # # # #         .nav-buttons {
# # # # # #             position: fixed;
# # # # # #             top: 20px;
# # # # # #             left: 20px;
# # # # # #             right: 20px;
# # # # # #             display: flex;
# # # # # #             justify-content: space-between;
# # # # # #             gap: 20px;
# # # # # #         }
# # # # # #         .button {
# # # # # #             padding: 12px 24px;
# # # # # #             font-size: 18px;
# # # # # #             border: none;
# # # # # #             border-radius: 5px;
# # # # # #             cursor: pointer;
# # # # # #             font-weight: bold;
# # # # # #             transition: all 0.3s ease;
# # # # # #         }
# # # # # #         .home-button {
# # # # # #             background: #00ff9d;
# # # # # #             color: #1a1a1a;
# # # # # #         }
# # # # # #         .cgi-button {
# # # # # #             background: #ff6b6b;
# # # # # #             color: white;
# # # # # #         }
# # # # # #         .container {
# # # # # #             background: rgba(255, 255, 255, 0.1);
# # # # # #             padding: 40px;
# # # # # #             border-radius: 10px;
# # # # # #             text-align: center;
# # # # # #             margin-top: 80px;
# # # # # #             max-width: 600px;
# # # # # #             width: 100%;
# # # # # #         }
# # # # # #         h1 {
# # # # # #             margin-bottom: 20px;
# # # # # #             color: #00ff9d;
# # # # # #             font-size: 32px;
# # # # # #         }
# # # # # #         .info {
# # # # # #             margin: 20px 0;
# # # # # #             padding: 20px;
# # # # # #             background: rgba(0, 0, 0, 0.2);
# # # # # #             border-radius: 5px;
# # # # # #             text-align: left;
# # # # # #         }
# # # # # #         .info p {
# # # # # #             margin: 10px 0;
# # # # # #         }
# # # # # #     </style>
# # # # # # </head>
# # # # # # <body>
# # # # # #     <div class="nav-buttons">
# # # # # #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# # # # # #         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# # # # # #     </div>
# # # # # #     <div class="container">
# # # # # #         <h1>Cookie Set Successfully!</h1>
# # # # # #         <div class="info">
# # # # # #             <p><strong>Username:</strong> {username}</p>
# # # # # #             <p><strong>Cookie Expiry:</strong> 1 hour</p>
# # # # # #         </div>
# # # # # #     </div>
# # # # # # </body>
# # # # # # </html>""".format(username=username))

# # # # # # except Exception as e:
# # # # # #     print("Content-Type: text/plain")
# # # # # #     print()
# # # # # #     print(f"Error: {str(e)}")
# # # # # #     sys.exit(1)


# # # # # #!/usr/bin/env python3
# # # # # import os
# # # # # import sys
# # # # # from urllib.parse import parse_qs

# # # # # try:
# # # # #     # Get query parameters
# # # # #     query_string = os.environ.get('QUERY_STRING', '')
# # # # #     params = parse_qs(query_string)
# # # # #     username = params.get('username', [''])[0]
    
# # # # #     if username:
# # # # #         # First print Set-Cookie header
# # # # #         print(f"Set-Cookie: username={username}; Path=/; Max-Age=3600")
        
# # # # #     # Then print Content-Type header
# # # # #     print("Content-Type: text/html; charset=utf-8")
# # # # #     # Empty line AFTER headers
# # # # #     print()
    
# # # # #     if username:
# # # # #         # Then print the HTML content
# # # # #         print("<!DOCTYPE html>")
# # # # #         print('<html lang="en">')
# # # # #         print("""<head>
# # # # #     <meta charset="UTF-8">
# # # # #     <title>Cookie Set</title>
# # # # #     <style>
# # # # #         * {
# # # # #             margin: 0;
# # # # #             padding: 0;
# # # # #             box-sizing: border-box;
# # # # #         }
# # # # #         body {
# # # # #             background-color: #1a1a1a;
# # # # #             color: #00ff9d;
# # # # #             font-family: Arial, sans-serif;
# # # # #             display: flex;
# # # # #             flex-direction: column;
# # # # #             align-items: center;
# # # # #             min-height: 100vh;
# # # # #             padding: 40px 20px;
# # # # #         }
# # # # #         .nav-buttons {
# # # # #             position: fixed;
# # # # #             top: 20px;
# # # # #             left: 20px;
# # # # #             right: 20px;
# # # # #             display: flex;
# # # # #             justify-content: space-between;
# # # # #             gap: 20px;
# # # # #         }
# # # # #         .button {
# # # # #             padding: 12px 24px;
# # # # #             font-size: 18px;
# # # # #             border: none;
# # # # #             border-radius: 5px;
# # # # #             cursor: pointer;
# # # # #             font-weight: bold;
# # # # #             transition: all 0.3s ease;
# # # # #         }
# # # # #         .home-button {
# # # # #             background: #00ff9d;
# # # # #             color: #1a1a1a;
# # # # #         }
# # # # #         .cgi-button {
# # # # #             background: #ff6b6b;
# # # # #             color: white;
# # # # #         }
# # # # #         .container {
# # # # #             background: rgba(255, 255, 255, 0.1);
# # # # #             padding: 40px;
# # # # #             border-radius: 10px;
# # # # #             text-align: center;
# # # # #             margin-top: 80px;
# # # # #             max-width: 600px;
# # # # #             width: 100%;
# # # # #         }
# # # # #         h1 {
# # # # #             margin-bottom: 20px;
# # # # #             color: #00ff9d;
# # # # #             font-size: 32px;
# # # # #         }
# # # # #         .info {
# # # # #             margin: 20px 0;
# # # # #             padding: 20px;
# # # # #             background: rgba(0, 0, 0, 0.2);
# # # # #             border-radius: 5px;
# # # # #             text-align: left;
# # # # #         }
# # # # #         .info p {
# # # # #             margin: 10px 0;
# # # # #         }
# # # # #     </style>
# # # # # </head>""")
# # # # #         print(f"""<body>
# # # # #     <div class="nav-buttons">
# # # # #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# # # # #         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# # # # #     </div>
# # # # #     <div class="container">
# # # # #         <h1>Cookie Set Successfully!</h1>
# # # # #         <div class="info">
# # # # #             <p><strong>Username:</strong> {username}</p>
# # # # #             <p><strong>Cookie Expiry:</strong> 1 hour</p>
# # # # #         </div>
# # # # #     </div>
# # # # # </body>
# # # # # </html>""")

# # # # # except Exception as e:
# # # # #     print("Content-Type: text/plain")
# # # # #     print()
# # # # #     print(f"Error: {str(e)}")
# # # # #     sys.exit(1)


# # # # #!/usr/bin/env python3
# # # # import os
# # # # import sys
# # # # import cgi
# # # # from http import cookies

# # # # # Print headers first
# # # # print("Content-Type: text/html")

# # # # try:
# # # #     # Get form data
# # # #     form = cgi.FieldStorage()
# # # #     username = form.getvalue('username', '')

# # # #     if username:
# # # #         # Create and set cookie
# # # #         cookie = cookies.SimpleCookie()
# # # #         cookie['username'] = username
# # # #         cookie['username']['path'] = '/'
# # # #         cookie['username']['max-age'] = 3600  # 1 hour
        
# # # #         # Print cookie header
# # # #         print(cookie.output())
# # # #         print()  # Empty line after headers
        
# # # #         # Get stored cookie to display
# # # #         stored_cookie = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
# # # #         stored_username = stored_cookie.get('username', '')
# # # #         if stored_username:
# # # #             username = stored_username.value

# # # #         # Print HTML
# # # #         print("""<!DOCTYPE html>
# # # # <html lang="en">
# # # # <head>
# # # #     <meta charset="UTF-8">
# # # #     <title>Cookie Status</title>
# # # #     <style>
# # # #         * {
# # # #             margin: 0;
# # # #             padding: 0;
# # # #             box-sizing: border-box;
# # # #         }
# # # #         body {
# # # #             background-color: #1a1a1a;
# # # #             color: #00ff9d;
# # # #             font-family: Arial, sans-serif;
# # # #             display: flex;
# # # #             flex-direction: column;
# # # #             align-items: center;
# # # #             min-height: 100vh;
# # # #             padding: 40px 20px;
# # # #         }
# # # #         .nav-buttons {
# # # #             position: fixed;
# # # #             top: 20px;
# # # #             left: 20px;
# # # #             right: 20px;
# # # #             display: flex;
# # # #             justify-content: space-between;
# # # #             gap: 20px;
# # # #         }
# # # #         .button {
# # # #             padding: 12px 24px;
# # # #             font-size: 18px;
# # # #             border: none;
# # # #             border-radius: 5px;
# # # #             cursor: pointer;
# # # #             font-weight: bold;
# # # #             transition: all 0.3s ease;
# # # #         }
# # # #         .home-button {
# # # #             background: #00ff9d;
# # # #             color: #1a1a1a;
# # # #         }
# # # #         .cgi-button {
# # # #             background: #ff6b6b;
# # # #             color: white;
# # # #         }
# # # #         .container {
# # # #             background: rgba(255, 255, 255, 0.1);
# # # #             padding: 40px;
# # # #             border-radius: 10px;
# # # #             text-align: center;
# # # #             margin-top: 80px;
# # # #             max-width: 600px;
# # # #             width: 100%;
# # # #         }
# # # #         h1 {
# # # #             margin-bottom: 20px;
# # # #             color: #00ff9d;
# # # #             font-size: 32px;
# # # #         }
# # # #         .info {
# # # #             margin: 20px 0;
# # # #             padding: 20px;
# # # #             background: rgba(0, 0, 0, 0.2);
# # # #             border-radius: 5px;
# # # #             text-align: left;
# # # #         }
# # # #         .info p {
# # # #             margin: 10px 0;
# # # #         }
# # # #         .cookie-value {
# # # #             color: #ff6b6b;
# # # #             font-weight: bold;
# # # #         }
# # # #     </style>
# # # # </head>
# # # # <body>
# # # #     <div class="nav-buttons">
# # # #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# # # #         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# # # #     </div>
# # # #     <div class="container">
# # # #         <h1>Cookie Information</h1>
# # # #         <div class="info">
# # # #             <p><strong>Stored Username:</strong> <span class="cookie-value">{username}</span></p>
# # # #             <p><strong>Cookie Expiry:</strong> 1 hour</p>
# # # #             <p><strong>Cookie Path:</strong> /</p>
# # # #         </div>
# # # #     </div>
# # # # </body>
# # # # </html>""".format(username=username))
# # # #     else:
# # # #         # If no username provided, show error
# # # #         print()  # Empty line after header
# # # #         print("""<!DOCTYPE html>
# # # # <html lang="en">
# # # # <head>
# # # #     <meta charset="UTF-8">
# # # #     <title>Error</title>
# # # #     <style>
# # # #         body {
# # # #             background-color: #1a1a1a;
# # # #             color: #ff6b6b;
# # # #             font-family: Arial, sans-serif;
# # # #             display: flex;
# # # #             justify-content: center;
# # # #             align-items: center;
# # # #             height: 100vh;
# # # #             margin: 0;
# # # #         }
# # # #         .error {
# # # #             background: rgba(255, 107, 107, 0.1);
# # # #             padding: 40px;
# # # #             border-radius: 10px;
# # # #             text-align: center;
# # # #         }
# # # #     </style>
# # # # </head>
# # # # <body>
# # # #     <div class="error">
# # # #         <h1>Error: Username is required</h1>
# # # #     </div>
# # # # </body>
# # # # </html>""")

# # # # except Exception as e:
# # # #     print()  # Empty line after header
# # # #     print(f"Error: {str(e)}")
# # # #     sys.exit(1)

# # # #!/usr/bin/env python3
# # # import os
# # # import sys
# # # import cgi
# # # from http import cookies
# # # from urllib.parse import parse_qs

# # # try:
# # #     # Get query parameters
# # #     query_string = os.environ.get('QUERY_STRING', '')
# # #     params = parse_qs(query_string)
# # #     username = params.get('username', [''])[0]

# # #     # Print content type first
# # #     print("Content-Type: text/html")
    
# # #     if username:
# # #         # Create and set cookie
# # #         cookie = cookies.SimpleCookie()
# # #         cookie['username'] = username
# # #         cookie['username']['path'] = '/'
# # #         cookie['username']['max-age'] = 3600  # 1 hour
# # #         print(cookie.output())  # Print cookie header
    
# # #     # Empty line to separate headers from content
# # #     print()
# # #     print("<!DOCTYPE html>")
# # #     print('<html lang="en">')
# # #     print("""<head>
# # #     <meta charset="UTF-8">
# # #     <title>Cookie Status</title>
# # #     <style>
# # #         * {
# # #             margin: 0;
# # #             padding: 0;
# # #             box-sizing: border-box;
# # #         }
# # #         body {
# # #             background-color: #1a1a1a;
# # #             color: #00ff9d;
# # #             font-family: Arial, sans-serif;
# # #             display: flex;
# # #             flex-direction: column;
# # #             align-items: center;
# # #             min-height: 100vh;
# # #             padding: 40px 20px;
# # #         }
# # #         .nav-buttons {
# # #             position: fixed;
# # #             top: 20px;
# # #             left: 20px;
# # #             right: 20px;
# # #             display: flex;
# # #             justify-content: space-between;
# # #             gap: 20px;
# # #         }
# # #         .button {
# # #             padding: 12px 24px;
# # #             font-size: 18px;
# # #             border: none;
# # #             border-radius: 5px;
# # #             cursor: pointer;
# # #             font-weight: bold;
# # #             transition: all 0.3s ease;
# # #         }
# # #         .home-button {
# # #             background: #00ff9d;
# # #             color: #1a1a1a;
# # #         }
# # #         .cgi-button {
# # #             background: #ff6b6b;
# # #             color: white;
# # #         }
# # #         .container {
# # #             background: rgba(255, 255, 255, 0.1);
# # #             padding: 40px;
# # #             border-radius: 10px;
# # #             text-align: center;
# # #             margin-top: 80px;
# # #             max-width: 600px;
# # #             width: 100%;
# # #         }
# # #         h1 {
# # #             margin-bottom: 20px;
# # #             color: #00ff9d;
# # #             font-size: 32px;
# # #         }
# # #         .info {
# # #             margin: 20px 0;
# # #             padding: 20px;
# # #             background: rgba(0, 0, 0, 0.2);
# # #             border-radius: 5px;
# # #             text-align: left;
# # #         }
# # #         .info p {
# # #             margin: 10px 0;
# # #         }
# # #     </style>
# # # </head>
# # # <body>
# # #     <div class="nav-buttons">
# # #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# # #         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# # #     </div>
# # #     <div class="container">
# # #         <h1>Cookie Status</h1>
# # #         <div class="info">""")

# # #     # Get stored cookie
# # #     stored_cookie = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
# # #     stored_username = stored_cookie.get('username')
    
# # #     if stored_username:
# # #         print(f'<p><strong>Current Cookie Value:</strong> {stored_username.value}</p>')
# # #         print(f'<p><strong>Cookie Path:</strong> {stored_username["path"]}</p>')
# # #         print(f'<p><strong>Cookie Max-Age:</strong> {stored_username["max-age"]} seconds</p>')
# # #     else:
# # #         print('<p>No cookie currently set</p>')

# # #     if username:
# # #         print(f'<p><strong>New Username Set:</strong> {username}</p>')

# # #     print("""
# # #         </div>
# # #     </div>
# # # </body>
# # # </html>""")

# # # except Exception as e:
# # #     print("Content-Type: text/plain")
# # #     print()
# # #     print(f"Error: {str(e)}")
# # #     sys.exit(1)


# # #!/usr/bin/env python3
# # import os
# # import sys
# # from http import cookies
# # from urllib.parse import parse_qs

# # try:
# #     # Get existing cookies first
# #     stored_cookies = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
    
# #     # Get query parameters
# #     query_string = os.environ.get('QUERY_STRING', '')
# #     params = parse_qs(query_string)
# #     username = params.get('username', [''])[0]

# #     # Print content type first
# #     print("Content-Type: text/html")
    
# #     if username:
# #         # Create new cookie while preserving existing ones
# #         cookie = stored_cookies
# #         cookie['username'] = username
# #         cookie['username']['path'] = '/'
# #         cookie['username']['max-age'] = 3600  # 1 hour
        
# #         # Print all cookies
# #         for morsel in cookie.values():
# #             print(f"Set-Cookie: {morsel.output().split(': ')[1]}")
    
# #     # Empty line to separate headers from content
# #     print()
# #     print("<!DOCTYPE html>")
# #     print('<html lang="en">')
# #     print("""<head>
# #     <meta charset="UTF-8">
# #     <title>Cookie Status</title>
# #     <style>
# #         /* ... existing styles ... */
# #     </style>
# # </head>
# # <body>
# #     <div class="nav-buttons">
# #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# #         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
# #     </div>
# #     <div class="container">
# #         <h1>Cookie Status</h1>
# #         <div class="info">""")

# #     # Display all cookies
# #     stored_cookies = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
# #     if stored_cookies:
# #         for key, morsel in stored_cookies.items():
# #             print(f'<p><strong>Cookie {key}:</strong> {morsel.value}</p>')
# #             print(f'<p><strong>Path:</strong> {morsel["path"]}</p>')
# #             print(f'<p><strong>Max-Age:</strong> {morsel["max-age"]} seconds</p>')
# #             print('<hr>')
# #     else:
# #         print('<p>No cookies currently set</p>')

# #     if username:
# #         print(f'<p><strong>New Username Set:</strong> {username}</p>')

# #     print("""
# #         </div>
# #     </div>
# # </body>
# # </html>""")

# # except Exception as e:
# #     print("Content-Type: text/plain")
# #     print()
# #     print(f"Error: {str(e)}")
# #     sys.exit(1)

# #!/usr/bin/env python3
# import os
# import sys
# import time
# from http import cookies
# from urllib.parse import parse_qs

# try:
#     # Get existing cookies first
#     stored_cookies = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
    
#     # Get query parameters
#     query_string = os.environ.get('QUERY_STRING', '')
#     params = parse_qs(query_string)
#     username = params.get('username', [''])[0]

#     # Print content type first
#     print("Content-Type: text/html")
    
#     if username:
#         # Create new cookie with timestamp to make it unique
#         cookie = stored_cookies
#         timestamp = int(time.time())
#         cookie_name = f'username_{timestamp}'
#         cookie[cookie_name] = username
#         cookie[cookie_name]['path'] = '/'
#         cookie[cookie_name]['max-age'] = 3600  # 1 hour
        
#         # Print all cookies including the new one
#         for morsel in cookie.values():
#             print(f"Set-Cookie: {morsel.output().split(': ')[1]}")
    
#     # Empty line to separate headers from content
#     print()
#     print("<!DOCTYPE html>")
#     print('<html lang="en">')
#     print("""<head>
#     <meta charset="UTF-8">
#     <title>Cookie Status</title>
#     <style>
#         * {
#             margin: 0;
#             padding: 0;
#             box-sizing: border-box;
#         }
#         body {
#             background-color: #1a1a1a;
#             color: #00ff9d;
#             font-family: Arial, sans-serif;
#             display: flex;
#             flex-direction: column;
#             align-items: center;
#             min-height: 100vh;
#             padding: 40px 20px;
#         }
#         .nav-buttons {
#             position: fixed;
#             top: 20px;
#             left: 20px;
#             right: 20px;
#             display: flex;
#             justify-content: space-between;
#             gap: 20px;
#         }
#         .button {
#             padding: 12px 24px;
#             font-size: 18px;
#             border: none;
#             border-radius: 5px;
#             cursor: pointer;
#             font-weight: bold;
#             transition: all 0.3s ease;
#         }
#         .home-button {
#             background: #00ff9d;
#             color: #1a1a1a;
#         }
#         .cgi-button {
#             background: #ff6b6b;
#             color: white;
#         }
#         .container {
#             background: rgba(255, 255, 255, 0.1);
#             padding: 40px;
#             border-radius: 10px;
#             text-align: center;
#             margin-top: 80px;
#             max-width: 600px;
#             width: 100%;
#         }
#         h1 {
#             margin-bottom: 20px;
#             color: #00ff9d;
#             font-size: 32px;
#         }
#         .info {
#             margin: 20px 0;
#             padding: 20px;
#             background: rgba(0, 0, 0, 0.2);
#             border-radius: 5px;
#             text-align: left;
#         }
#         .info p {
#             margin: 10px 0;
#         }
#     </style>
# </head>
# <body>
#     <div class="nav-buttons">
#         <button class="button home-button" onclick="location.href='/'">HOME</button>
#         <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
#     </div>
#     <div class="container">
#         <h1>Cookie Status</h1>
#         <div class="info">""")

#     # Display all stored cookies
#     stored_cookies = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE', ''))
#     if stored_cookies:
#         for key, morsel in stored_cookies.items():
#             print(f'<p><strong>Cookie {key}:</strong> {morsel.value}</p>')
#             print(f'<p><strong>Path:</strong> {morsel["path"]}</p>')
#             print(f'<p><strong>Max-Age:</strong> {morsel["max-age"]} seconds</p>')
#             print('<hr>')
#     else:
#         print('<p>No cookies currently set</p>')

#     if username:
#         print(f'<p><strong>New Cookie Set:</strong> {username}</p>')

#     print("""
#         </div>
#     </div>
# </body>
# </html>""")

# except Exception as e:
#     print("Content-Type: text/plain")
#     print()
#     print(f"Error: {str(e)}")
#     sys.exit(1)

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