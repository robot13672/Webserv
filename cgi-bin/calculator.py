# import os
# import sys
# from urllib.parse import parse_qs

# # Headers must be printed first
# # print("Content-Type: text/html")
# print()  # Empty line is crucial
# # Debug output to stderr
# # sys.stderr.write("CGI Script Starting\n")
# # sys.stderr.write(f"QUERY_STRING: {os.environ.get('QUERY_STRING', 'None')}\n")


# try:
#     # Get and parse query parameters
#     query_string = os.environ.get('QUERY_STRING', '')
#     params = parse_qs(query_string)
    
#     sys.stderr.write(f"Parsed params: {params}\n")
    
#     # Extract values
#     num1 = int(params.get('num1', [0])[0])
#     num2 = int(params.get('num2', [0])[0])
#     operation = params.get('operation', ['+'])[0]
    
#     sys.stderr.write(f"Values: num1={num1}, num2={num2}, operation={operation}\n")
    
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

import os
import sys
from urllib.parse import parse_qs

# Headers must be printed first
print("Content-Type: text/html")  # Uncomment this line
print()  # Empty line after headers

try:
    # Get and parse query parameters
    query_string = os.environ.get('QUERY_STRING', '')
    params = parse_qs(query_string)
    
    # sys.stderr.write(f"Parsed params: {params}\n")
    
    # Extract values
    num1 = int(params.get('num1', [0])[0])
    num2 = int(params.get('num2', [0])[0])
    operation = params.get('operation', ['+'])[0]
    
    # sys.stderr.write(f"Values: num1={num1}, num2={num2}, operation={operation}\n")
    
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
    
#     sys.stderr.write("Sending response\n")
#     print(response)
#     sys.stderr.write("Response sent\n")

# except Exception as e:
#     sys.stderr.write(f"Error occurred: {str(e)}\n")
#     print(f"""<!DOCTYPE html>
# <html>
# <head>

    print(response)

except Exception as e:
    sys.stderr.write(f"Error occurred: {str(e)}\n")
    print(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
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