# # # #!/usr/bin/env python3
# # # import os
# # # import cgi

# # # # Получаем метод запроса
# # # method = os.environ.get('REQUEST_METHOD', '')

# # # if method == 'POST':
# # #     # Читаем данные формы
# # #     form = cgi.FieldStorage()
# # #     num1 = float(form.getvalue('num1', 0))
# # #     num2 = float(form.getvalue('num2', 0))
# # #     operation = form.getvalue('operation', '+')
    
# # #     # Выполняем вычисление
# # #     result = 0
# # #     if operation == '+':
# # #         result = num1 + num2
# # #     elif operation == '-':
# # #         result = num1 - num2
# # #     elif operation == '*':
# # #         result = num1 * num2
# # #     elif operation == '/' and num2 != 0:
# # #         result = num1 / num2

# # #     # Выводим результат
# # #     print("Content-Type: text/plain\r\n\r\n")
# # #     print(f"{result}")


# # # #!/usr/bin/env python3
# # # import os
# # # import sys
# # # from urllib.parse import parse_qs

# # # print("Content-Type: text/html\r\n\r\n")
# # # print("Access-Control-Allow-Origin: *")
# # # print()

# # # try:
# # #     # Get query parameters
# # #     query_string = os.environ.get('QUERY_STRING', '')
# # #     params = parse_qs(query_string)
    
# # #     # Extract values
# # #     num1 = float(params.get('num1', [0])[0])
# # #     num2 = float(params.get('num2', [0])[0])
# # #     operation = params.get('operation', ['+'])[0]
    
# # #     # Perform calculation
# # #     result = None
# # #     if operation == '+':
# # #         result = num1 + num2
# # #     elif operation == '-':
# # #         result = num1 - num2
# # #     elif operation == '*':
# # #         result = num1 * num2
# # #     elif operation == '/':
# # #         if num2 == 0:
# # #             raise ValueError("Cannot divide by zero")
# # #         result = num1 / num2
    
# # #     # Format result
# # # #     print(f"{num1} {operation} {num2} = {result}")

# # # # except ValueError as e:
# # # #     print(f"Error: {str(e)}")
# # # # except Exception as e:
# # # #     print(f"Error: Invalid input or calculation error")

# # #     print(f"""
# # #     <!DOCTYPE html>
# # #     <html>
# # #     <head>
# # #         <title>Calculator Result</title>
# # #         <style>
# # #             body {
# # #                 display: flex;
# # #                 flex-direction: column;
# # #                 align-items: center;
# # #                 min-height: 100vh;
# # #                 background-color: #1a1a1a;
# # #                 font-family: 'Roboto', sans-serif;
# # #                 padding: 40px 20px;
# # #                 color: #fff;
# # #                 margin: 0;
# # #             }
# # #             .result {
# # #                 font-size: 24px;
# # #                 color: #00ff9d;
# # #                 margin: 20px 0;
# # #             }
# # #             .nav-buttons {
# # #                 position: fixed;
# # #                 top: 20px;
# # #                 left: 20px;
# # #                 right: 20px;
# # #                 display: flex;
# # #                 justify-content: space-between;
# # #             }
# # #             .button {
# # #                 padding: 12px 24px;
# # #                 font-size: 18px;
# # #                 border: none;
# # #                 border-radius: 5px;
# # #                 cursor: pointer;
# # #                 font-weight: bold;
# # #                 transition: all 0.3s ease;
# # #             }
# # #             .home-button {
# # #                 background: #00ff9d;
# # #                 color: #1a1a1a;
# # #             }
# # #             .cgi-button {
# # #                 background: #ff6b6b;
# # #                 color: white;
# # #             }
# # #             .button:hover {
# # #                 transform: translateY(-2px);
# # #             }
# # #         </style>
# # #     </head>
# # #     <body>
# # #         <div class="nav-buttons">
# # #             <button class="button home-button" onclick="location.href='/'">HOME</button>
# # #             <button class="button cgi-button" onclick="location.href='/calculator'">BACK</button>
# # #         </div>
# # #         <div class="result">
# # #             {num1} {operation} {num2} = {result}
# # #         </div>
# # #     </body>
# # #     </html>
# # #     """)

# # # except ValueError as e:
# # #     print(f"<h1>Error: {str(e)}</h1>")
# # # except Exception as e:
# # #     print(f"<h1>Error: Invalid input or calculation error</h1>")
# # #!/usr/bin/env python3
# # import os
# # import sys
# # from urllib.parse import parse_qs

# # # First print the headers with proper line endings
# # print("Content-Type: text/html")
# # print("Access-Control-Allow-Origin: *")
# # print()  # Empty line is required to separate headers from content

# # try:
# #     # Get query parameters
# #     query_string = os.environ.get('QUERY_STRING', '')
# #     params = parse_qs(query_string)
    
# #     # Extract values with error checking
# #     num1 = float(params.get('num1', [0])[0])
# #     num2 = float(params.get('num2', [0])[0])
# #     operation = params.get('operation', ['+'])[0]
    
# #     # Perform calculation
# #     result = None
# #     if operation == '+':
# #         result = num1 + num2
# #     elif operation == '-':
# #         result = num1 - num2
# #     elif operation == '*':
# #         result = num1 * num2
# #     elif operation == '/':
# #         if num2 == 0:
# #             raise ValueError("Cannot divide by zero")
# #         result = num1 / num2
    
# #     # Return HTML page with result
# #     print("""<!DOCTYPE html>
# # <html>
# # <head>
# #     <title>Calculator Result</title>
# #     <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@400;700&display=swap" rel="stylesheet">
# #     <style>
# #         * {
# #             margin: 0;
# #             padding: 0;
# #             box-sizing: border-box;
# #         }
# #         body {
# #             display: flex;
# #             flex-direction: column;
# #             align-items: center;
# #             min-height: 100vh;
# #             background-color: #1a1a1a;
# #             font-family: 'Roboto', sans-serif;
# #             padding: 40px 20px;
# #             color: #fff;
# #             margin: 0;
# #         }
# #         .result {
# #             font-size: 48px;
# #             color: #00ff9d;
# #             margin: 20px 0;
# #             padding: 40px;
# #             background: rgba(255, 255, 255, 0.1);
# #             border-radius: 10px;
# #             text-align: center;
# #             min-width: 300px;
# #         }
# #         .nav-buttons {
# #             position: fixed;
# #             top: 20px;
# #             left: 20px;
# #             right: 20px;
# #             display: flex;
# #             justify-content: space-between;
# #             z-index: 1000;
# #         }
# #         .button {
# #             padding: 12px 24px;
# #             font-size: 18px;
# #             border: none;
# #             border-radius: 5px;
# #             cursor: pointer;
# #             font-weight: bold;
# #             transition: all 0.3s ease;
# #         }
# #         .home-button {
# #             background: #00ff9d;
# #             color: #1a1a1a;
# #         }
# #         .cgi-button {
# #             background: #ff6b6b;
# #             color: white;
# #         }
# #         .button:hover {
# #             transform: translateY(-2px);
# #             opacity: 0.9;
# #         }
# #     </style>
# # </head>
# # <body>
# #     <div class="nav-buttons">
# #         <button class="button home-button" onclick="location.href='/'">HOME</button>
# #         <button class="button cgi-button" onclick="location.href='/cgi'">BACK</button>
# #     </div>
# #     <div class="result">
# #         {} {} {} = {}
# #     </div>
# # </body>
# # </html>""".format(num1, operation, num2, result))

# # except ValueError as e:
# #     print("""<!DOCTYPE html>
# # <html>
# # <head>
# #     <title>Calculator Error</title>
# #     <style>
# #         body {
# #             background-color: #1a1a1a;
# #             color: #ff6b6b;
# #             font-family: Arial, sans-serif;
# #             display: flex;
# #             justify-content: center;
# #             align-items: center;
# #             height: 100vh;
# #             margin: 0;
# #             padding: 20px;
# #         }
# #         .error {
# #             padding: 40px;
# #             background: rgba(255, 107, 107, 0.1);
# #             border-radius: 10px;
# #             text-align: center;
# #         }
# #         h1 { margin: 0; }
# #     </style>
# # </head>
# # <body>
# #     <div class="error">
# #         <h1>Error: {}</h1>
# #     </div>
# # </body>
# # </html>""".format(str(e)))

# # except Exception as e:
# #     print("""<!DOCTYPE html>
# # <html>
# # <head>
# #     <title>Calculator Error</title>
# #     <style>
# #         body {
# #             background-color: #1a1a1a;
# #             color: #ff6b6b;
# #             font-family: Arial, sans-serif;
# #             display: flex;
# #             justify-content: center;
# #             align-items: center;
# #             height: 100vh;
# #             margin: 0;
# #             padding: 20px;
# #         }
# #         .error {
# #             padding: 40px;
# #             background: rgba(255, 107, 107, 0.1);
# #             border-radius: 10px;
# #             text-align: center;
# #         }
# #         h1 { margin: 0; }
# #     </style>
# # </head>
# # <body>
# #     <div class="error">
# #         <h1>Error: Invalid input or calculation error</h1>
# #     </div>
# # </body>
# # </html>""")

# #!/usr/bin/env python3
# import os
# import sys
# from urllib.parse import parse_qs

# # Send headers first
# print("Content-Type: text/html")
# print()  # Empty line between headers and content

# try:
#     # Get query parameters
#     query_string = os.environ.get('QUERY_STRING', '')
#     params = parse_qs(query_string)
    
#     # Debug output
#     sys.stderr.write(f"Query string: {query_string}\n")
#     sys.stderr.write(f"Parsed params: {params}\n")
    
#     # Extract values
#     num1 = float(params.get('num1', [0])[0])
#     num2 = float(params.get('num2', [0])[0])
#     operation = params.get('operation', ['+'])[0]
    
#     # Calculate
#     result = None
#     if operation == '+':
#         result = num1 + num2
#     elif operation == '-':
#         result = num1 - num2
#     elif operation == '*':
#         result = num1 * num2
#     elif operation == '/':
#         if num2 == 0:
#             raise ValueError("Cannot divide by zero")
#         result = num1 / num2

#     # Print HTML response
#     print(f"""
# <!DOCTYPE html>
# <html>
# <head>
#     <title>Calculator Result</title>
#     <style>
#         body {{
#             background-color: #1a1a1a;
#             color: #fff;
#             font-family: Arial, sans-serif;
#             display: flex;
#             flex-direction: column;
#             align-items: center;
#             min-height: 100vh;
#             margin: 0;
#             padding: 20px;
#         }}
#         .nav-buttons {{
#             position: fixed;
#             top: 20px;
#             left: 20px;
#             right: 20px;
#             display: flex;
#             justify-content: space-between;
#         }}
#         .button {{
#             padding: 12px 24px;
#             font-size: 18px;
#             border: none;
#             border-radius: 5px;
#             cursor: pointer;
#             font-weight: bold;
#         }}
#         .home-button {{
#             background: #00ff9d;
#             color: #1a1a1a;
#         }}
#         .cgi-button {{
#             background: #ff6b6b;
#             color: white;
#         }}
#         .result {{
#             font-size: 48px;
#             color: #00ff9d;
#             margin-top: 100px;
#             padding: 40px;
#             background: rgba(255, 255, 255, 0.1);
#             border-radius: 10px;
#         }}
#     </style>
# </head>
# <body>
#     <div class="nav-buttons">
#         <button class="button home-button" onclick="location.href='/'">HOME</button>
#         <button class="button cgi-button" onclick="location.href='/cgi'">BACK</button>
#     </div>
#     <div class="result">
#         {num1} {operation} {num2} = {result}
#     </div>
# </body>
# </html>
#     """)

# except ValueError as e:
#     print(f"""
# <!DOCTYPE html>
# <html>
# <head>
#     <title>Error</title>
#     <style>
#         body {{
#             background-color: #1a1a1a;
#             color: #ff6b6b;
#             font-family: Arial, sans-serif;
#             display: flex;
#             justify-content: center;
#             align-items: center;
#             height: 100vh;
#             margin: 0;
#         }}
#         .error {{
#             padding: 20px;
#             background: rgba(255, 107, 107, 0.1);
#             border-radius: 10px;
#         }}
#     </style>
# </head>
# <body>
#     <div class="error">
#         <h1>Error: {str(e)}</h1>
#     </div>
# </body>
# </html>
#     """)
# except Exception as e:
#     sys.stderr.write(f"Error: {str(e)}\n")
#     print("Content-Type: text/html\n")
#     print("<h1>Internal Server Error</h1>")


# #!/usr/bin/env python3
# import os
# import sys
# from urllib.parse import parse_qs

# # Важно: сначала отправляем заголовки
# print("Content-Type: text/html")
# print("Access-Control-Allow-Origin: *")
# print()  # Пустая строка - разделитель между заголовками и телом

# try:
#     # Get query parameters
#     query_string = os.environ.get('QUERY_STRING', '')
#     params = parse_qs(query_string)
    
#     # Extract values
#     num1 = float(params.get('num1', [0])[0])
#     num2 = float(params.get('num2', [0])[0])
#     operation = params.get('operation', ['+'])[0]
    
#     # Calculate
#     result = None
#     if operation == '+':
#         result = num1 + num2
#     elif operation == '-':
#         result = num1 - num2
#     elif operation == '*':
#         result = num1 * num2
#     elif operation == '/':
#         if num2 == 0:
#             raise ValueError("Cannot divide by zero")
#         result = num1 / num2

#     # Print HTML response
#     print(f"""<!DOCTYPE html>
# <html>
# <head>
#     <title>Result</title>
#     <style>
#         body {{
#             background-color: #1a1a1a;
#             color: #00ff9d;
#             font-family: Arial, sans-serif;
#             display: flex;
#             flex-direction: column;
#             align-items: center;
#             justify-content: center;
#             height: 100vh;
#             margin: 0;
#         }}
#         .result {{
#             font-size: 48px;
#             padding: 20px;
#             background: rgba(255,255,255,0.1);
#             border-radius: 10px;
#         }}
#     </style>
# </head>
# <body>
#     <div class="result">
#         {num1} {operation} {num2} = {result}
#     </div>
# </body>
# </html>""")

# except Exception as e:
#     print(f"""<!DOCTYPE html>
# <html>
# <head>
#     <title>Error</title>
#     <style>
#         body {{
#             background-color: #1a1a1a;
#             color: #ff6b6b;
#             font-family: Arial, sans-serif;
#             display: flex;
#             justify-content: center;
#             align-items: center;
#             height: 100vh;
#             margin: 0;
#         }}
#     </style>
# </head>
# <body>
#     <h1>Error: {str(e)}</h1>
# </body>
# </html>""")

#!/usr/bin/env python3
import os
import sys
from urllib.parse import parse_qs

# Headers must be printed first
# print("Content-Type: text/html")
print()  # Empty line is crucial
# Debug output to stderr
# sys.stderr.write("CGI Script Starting\n")
# sys.stderr.write(f"QUERY_STRING: {os.environ.get('QUERY_STRING', 'None')}\n")


try:
    # Get and parse query parameters
    query_string = os.environ.get('QUERY_STRING', '')
    params = parse_qs(query_string)
    
    sys.stderr.write(f"Parsed params: {params}\n")
    
    # Extract values
    num1 = int(params.get('num1', [0])[0])
    num2 = int(params.get('num2', [0])[0])
    operation = params.get('operation', ['+'])[0]
    
    sys.stderr.write(f"Values: num1={num1}, num2={num2}, operation={operation}\n")
    
    # Calculate
    result = None
    if operation == '+':
        result = num1 + num2
    elif operation == '-':
        result = num1 - num2
    elif operation == '*':
        result = num1 * num2
    elif operation == '/':
        if num2 == 0:
            raise ValueError("Cannot divide by zero")
        result = num1 / num2

    # Print HTML response
    response = f"""<!DOCTYPE html>
<html>
<head>
    <title>Result</title>
    <style>
        body {{
            background-color: #1a1a1a;
            color: #00ff9d;
            font-family: Arial, sans-serif;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
            margin: 0;
            padding: 20px;
            position: relative;
        }}
        .nav-buttons {{
            position: fixed;
            top: 20px;
            left: 20px;
            right: 20px;
            display: flex;
            justify-content: space-between;
            gap: 20px;
            width: calc(100% - 40px);
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
        .calculator-button {{
            background: #00ff9d;
            color: #1a1a1a;
        }}
        .button:hover {{
            transform: translateY(-2px);
            opacity: 0.9;
        }}
        .result {{
            font-size: 48px;
            padding: 40px;
            background: rgba(255,255,255,0.1);
            border-radius: 10px;
            margin-top: 80px;
        }}
    </style>
</head>
<body>
    <div class="nav-buttons">
        <button class="button home-button" onclick="location.href='/'">HOME</button>
        <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
        <button class="button calculator-button" onclick="location.href='/calculator'">CALCULATOR</button>
    </div>
    <div class="result">
        {num1} {operation} {num2} = {result}
    </div>
</body>
</html>"""
    
    sys.stderr.write("Sending response\n")
    print(response)
    sys.stderr.write("Response sent\n")

except Exception as e:
    sys.stderr.write(f"Error occurred: {str(e)}\n")
    print(f"""<!DOCTYPE html>
<html>
<head>
    <title>Error</title>
    <style>
        body {{
            background-color: #1a1a1a;
            color: #ff6b6b;
            font-family: Arial, sans-serif;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
            margin: 0;
        }}
        .nav-buttons {{
            position: fixed;
            top: 20px;
            left: 20px;
            right: 20px;
            display: flex;
            justify-content: space-between;
            gap: 20px;
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
        .error {{
            margin-top: 80px;
            padding: 20px;
            background: rgba(255, 107, 107, 0.1);
            border-radius: 10px;
        }}
    </style>
</head>
<body>
    <div class="nav-buttons">
        <button class="button home-button" onclick="location.href='/'">HOME</button>
        <button class="button cgi-button" onclick="location.href='/cgi'">CGI</button>
        <button class="button calculator-button" onclick="location.href='/calculator'">CALCULATOR</button>
    </div>
    <h1>Error: {str(e)}</h1>
</body>
</html>""")