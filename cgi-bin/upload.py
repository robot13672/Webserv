# # # # # # # # # #!/usr/bin/env python3
# # # # # # # # # import os
# # # # # # # # # import sys
# # # # # # # # # import cgi
# # # # # # # # # from pathlib import Path

# # # # # # # # # print("Content-Type: text/plain")
# # # # # # # # # print()  # Empty line after headers

# # # # # # # # # try:
# # # # # # # # #     form = cgi.FieldStorage()
    
# # # # # # # # #     if 'file' not in form or 'newName' not in form:
# # # # # # # # #         print("Error: Missing file or new name")
# # # # # # # # #         sys.exit(1)

# # # # # # # # #     fileitem = form['file']
# # # # # # # # #     new_name = form.getvalue('newName')
    
# # # # # # # # #     if not fileitem.filename:
# # # # # # # # #         print("Error: No file selected")
# # # # # # # # #         sys.exit(1)
    
# # # # # # # # #     # Get original extension
# # # # # # # # #     ext = Path(fileitem.filename).suffix
    
# # # # # # # # #     # Create upload directory if it doesn't exist
# # # # # # # # #     upload_dir = "upload"
# # # # # # # # #     if not os.path.exists(upload_dir):
# # # # # # # # #         os.makedirs(upload_dir)
    
# # # # # # # # #     # Construct new filename with original extension
# # # # # # # # #     new_filename = os.path.join(upload_dir, new_name + ext)
    
# # # # # # # # #     # Save file with new name
# # # # # # # # #     with open(new_filename, 'wb') as f:
# # # # # # # # #         f.write(fileitem.file.read())
    
# # # # # # # # #     print(f"File uploaded successfully as {new_name + ext}")

# # # # # # # # # except Exception as e:
# # # # # # # # #     print(f"Error: {str(e)}")
# # # # # # # # #     sys.exit(1)


# # # # # # # # #!/usr/bin/env python3
# # # # # # # # import os
# # # # # # # # import sys
# # # # # # # # import cgi
# # # # # # # # from pathlib import Path

# # # # # # # # # First print headers
# # # # # # # # print("Content-Type: text/plain")
# # # # # # # # print()  # Empty line after headers

# # # # # # # # try:
# # # # # # # #     # Parse multipart form data
# # # # # # # #     form = cgi.FieldStorage()
    
# # # # # # # #     # Check if required fields are present
# # # # # # # #     if 'file' not in form or 'newName' not in form:
# # # # # # # #         print("Error: Missing file or new name")
# # # # # # # #         sys.exit(1)

# # # # # # # #     fileitem = form['file']
# # # # # # # #     new_name = form.getvalue('newName')
    
# # # # # # # #     if not fileitem.filename:
# # # # # # # #         print("Error: No file was uploaded")
# # # # # # # #         sys.exit(1)
    
# # # # # # # #     # Get original file extension
# # # # # # # #     original_ext = Path(fileitem.filename).suffix
    
# # # # # # # #     # Create upload directory if it doesn't exist
# # # # # # # #     upload_dir = "upload"
# # # # # # # #     if not os.path.exists(upload_dir):
# # # # # # # #         os.makedirs(upload_dir)
    
# # # # # # # #     # Construct new filename with original extension
# # # # # # # #     new_filename = os.path.join(upload_dir, new_name + original_ext)
    
# # # # # # # #     # Check if file already exists
# # # # # # # #     if os.path.exists(new_filename):
# # # # # # # #         print(f"Error: File {new_name + original_ext} already exists")
# # # # # # # #         sys.exit(1)
    
# # # # # # # #     # Save file with new name
# # # # # # # #     try:
# # # # # # # #         with open(new_filename, 'wb') as f:
# # # # # # # #             f.write(fileitem.file.read())
# # # # # # # #         print(f"File uploaded successfully as {new_name + original_ext}")
# # # # # # # #     except IOError as e:
# # # # # # # #         print(f"Error saving file: {str(e)}")
# # # # # # # #         sys.exit(1)

# # # # # # # # except Exception as e:
# # # # # # # #     print(f"Error: {str(e)}")
# # # # # # # #     sys.exit(1)

# # # # # # # #!/usr/bin/env python3
# # # # # # # import os
# # # # # # # import sys
# # # # # # # from pathlib import Path

# # # # # # # # First print headers
# # # # # # # print("Content-Type: text/plain")
# # # # # # # print()  # Empty line after headers

# # # # # # # try:
# # # # # # #     # Read content length
# # # # # # #     content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    
# # # # # # #     # Read POST data
# # # # # # #     post_data = sys.stdin.buffer.read(content_length)
    
# # # # # # #     # Parse multipart form data boundary
# # # # # # #     content_type = os.environ.get('CONTENT_TYPE', '')
# # # # # # #     boundary = content_type.split('boundary=')[1]
    
# # # # # # #     # Parse the multipart form data
# # # # # # #     parts = post_data.split(('--' + boundary).encode())
    
# # # # # # #     # Process form fields
# # # # # # #     filename = None
# # # # # # #     new_name = None
# # # # # # #     file_data = None
    
# # # # # # #     for part in parts:
# # # # # # #         if not part.strip():
# # # # # # #             continue
            
# # # # # # #         # Split headers and content
# # # # # # #         try:
# # # # # # #             headers, content = part.split(b'\r\n\r\n', 1)
# # # # # # #             headers = headers.decode()
            
# # # # # # #             if 'filename=' in headers:
# # # # # # #                 # This is the file part
# # # # # # #                 filename = headers.split('filename=')[1].split('\r\n')[0].strip('"')
# # # # # # #                 file_data = content.rstrip(b'\r\n')
# # # # # # #             elif 'name="newName"' in headers:
# # # # # # #                 # This is the new name field
# # # # # # #                 new_name = content.rstrip(b'\r\n').decode()
# # # # # # #         except:
# # # # # # #             continue
    
# # # # # # #     if not filename or not new_name or not file_data:
# # # # # # #         print("Error: Missing required fields")
# # # # # # #         sys.exit(1)
    
# # # # # # #     # Get original extension
# # # # # # #     original_ext = Path(filename).suffix
    
# # # # # # #     # Create upload directory if it doesn't exist
# # # # # # #     upload_dir = "upload"
# # # # # # #     if not os.path.exists(upload_dir):
# # # # # # #         os.makedirs(upload_dir)
    
# # # # # # #     # Construct new filename with original extension
# # # # # # #     new_filename = os.path.join(upload_dir, new_name + original_ext)
    
# # # # # # #     # Check if file already exists
# # # # # # #     if os.path.exists(new_filename):
# # # # # # #         print(f"Error: File {new_name + original_ext} already exists")
# # # # # # #         sys.exit(1)
    
# # # # # # #     # Save file with new name
# # # # # # #     try:
# # # # # # #         with open(new_filename, 'wb') as f:
# # # # # # #             f.write(file_data)
# # # # # # #         print(f"File uploaded successfully as {new_name + original_ext}")
# # # # # # #     except IOError as e:
# # # # # # #         print(f"Error saving file: {str(e)}")
# # # # # # #         sys.exit(1)

# # # # # # # except Exception as e:
# # # # # # #     print(f"Error: {str(e)}")
# # # # # # #     sys.exit(1)

# # # # # # #!/usr/bin/env python3
# # # # # # import os
# # # # # # import sys
# # # # # # from pathlib import Path

# # # # # # # First print headers
# # # # # # print("Content-Type: text/plain")
# # # # # # print()

# # # # # # try:
# # # # # #     # Get Content-Length
# # # # # #     content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    
# # # # # #     # Get Content-Type and boundary
# # # # # #     content_type = os.environ.get('CONTENT_TYPE', '')
# # # # # #     if not content_type.startswith('multipart/form-data'):
# # # # # #         print("Error: Invalid content type")
# # # # # #         sys.exit(1)
    
# # # # # #     boundary = content_type.split('boundary=')[1].encode()
    
# # # # # #     # Read POST data
# # # # # #     post_data = sys.stdin.buffer.read(content_length)
    
# # # # # #     # Split into parts
# # # # # #     parts = post_data.split(b'--' + boundary)
    
# # # # # #     file_data = None
# # # # # #     new_name = None
# # # # # #     original_filename = None
    
# # # # # #     # Parse each part
# # # # # #     for part in parts:
# # # # # #         if not part.strip():
# # # # # #             continue
        
# # # # # #         try:
# # # # # #             headers, content = part.split(b'\r\n\r\n', 1)
# # # # # #             headers = headers.decode()
            
# # # # # #             if 'filename=' in headers:
# # # # # #                 # This is the file part
# # # # # #                 original_filename = headers.split('filename=')[1].split('\r\n')[0].strip('"')
# # # # # #                 file_data = content.rstrip(b'\r\n--')
# # # # # #             elif 'name="newName"' in headers:
# # # # # #                 # This is the new name field
# # # # # #                 new_name = content.rstrip(b'\r\n--').decode()
# # # # # #         except:
# # # # # #             continue
    
# # # # # #     if not file_data or not new_name or not original_filename:
# # # # # #         print("Error: Missing file or new name")
# # # # # #         sys.exit(1)
    
# # # # # #     # Get original extension
# # # # # #     ext = Path(original_filename).suffix
    
# # # # # #     # Create upload directory if needed
# # # # # #     upload_dir = "upload"
# # # # # #     if not os.path.exists(upload_dir):
# # # # # #         os.makedirs(upload_dir)
    
# # # # # #     # Create new filename with original extension
# # # # # #     new_filename = os.path.join(upload_dir, new_name + ext)
    
# # # # # #     # Save file
# # # # # #     try:
# # # # # #         with open(new_filename, 'wb') as f:
# # # # # #             f.write(file_data)
# # # # # #         print(f"File uploaded successfully as {new_name + ext}")
# # # # # #     except IOError as e:
# # # # # #         print(f"Error saving file: {str(e)}")
# # # # # #         sys.exit(1)

# # # # # # except Exception as e:
# # # # # #     print(f"Error: {str(e)}")
# # # # # #     sys.exit(1)


# # # # # #!/usr/bin/env python3
# # # # # import os
# # # # # import sys
# # # # # import select
# # # # # from pathlib import Path

# # # # # # Debug logging
# # # # # def log_debug(message):
# # # # #     sys.stderr.write(f"DEBUG: {message}\n")

# # # # # # First print headers
# # # # # print("Content-Type: text/plain")
# # # # # print()

# # # # # try:
# # # # #     # Get Content-Length with debug
# # # # #     content_length = int(os.environ.get('CONTENT_LENGTH', 0))
# # # # #     log_debug(f"Content-Length: {content_length}")
    
# # # # #     # Get Content-Type with debug
# # # # #     content_type = os.environ.get('CONTENT_TYPE', '')
# # # # #     log_debug(f"Content-Type: {content_type}")
    
# # # # #     if not content_type.startswith('multipart/form-data'):
# # # # #         print("Error: Invalid content type")
# # # # #         sys.exit(1)
    
# # # # #     # Extract boundary
# # # # #     boundary = content_type.split('boundary=')[1].encode()
# # # # #     log_debug(f"Boundary: {boundary}")
    
# # # # #     # Read POST data with timeout
# # # # #     post_data = b''
# # # # #     timeout = 5  # 5 seconds timeout
    
# # # # #     ready = select.select([sys.stdin.buffer], [], [], timeout)
# # # # #     if ready[0]:
# # # # #         post_data = sys.stdin.buffer.read(content_length)
# # # # #         log_debug(f"Read {len(post_data)} bytes")
# # # # #     else:
# # # # #         print("Error: Timeout reading POST data")
# # # # #         sys.exit(1)
    
# # # # #     # Split into parts
# # # # #     parts = post_data.split(b'--' + boundary)
# # # # #     log_debug(f"Found {len(parts)} parts")
    
# # # # #     file_data = None
# # # # #     new_name = None
# # # # #     original_filename = None
    
# # # # #     # Parse each part
# # # # #     for part in parts:
# # # # #         if not part.strip():
# # # # #             continue
        
# # # # #         try:
# # # # #             headers, content = part.split(b'\r\n\r\n', 1)
# # # # #             headers = headers.decode()
# # # # #             log_debug(f"Processing part with headers: {headers[:100]}")
            
# # # # #             if 'filename=' in headers:
# # # # #                 original_filename = headers.split('filename=')[1].split('\r\n')[0].strip('"')
# # # # #                 file_data = content.rstrip(b'\r\n--')
# # # # #                 log_debug(f"Found file: {original_filename}")
# # # # #             elif 'name="newName"' in headers:
# # # # #                 new_name = content.rstrip(b'\r\n--').decode()
# # # # #                 log_debug(f"Found new name: {new_name}")
# # # # #         except Exception as e:
# # # # #             log_debug(f"Error processing part: {str(e)}")
# # # # #             continue
    
# # # # #     if not file_data or not new_name or not original_filename:
# # # # #         print("Error: Missing file or new name")
# # # # #         sys.exit(1)
    
# # # # #     # Get original extension
# # # # #     ext = Path(original_filename).suffix
# # # # #     log_debug(f"File extension: {ext}")
    
# # # # #     # Create upload directory if needed
# # # # #     upload_dir = "upload"
# # # # #     if not os.path.exists(upload_dir):
# # # # #         os.makedirs(upload_dir)
# # # # #         log_debug(f"Created upload directory: {upload_dir}")
    
# # # # #     # Create new filename
# # # # #     new_filename = os.path.join(upload_dir, new_name + ext)
# # # # #     log_debug(f"New filename: {new_filename}")
    
# # # # #     # Save file
# # # # #     try:
# # # # #         with open(new_filename, 'wb') as f:
# # # # #             f.write(file_data)
# # # # #         print(f"File uploaded successfully as {new_name + ext}")
# # # # #     except IOError as e:
# # # # #         print(f"Error saving file: {str(e)}")
# # # # #         sys.exit(1)

# # # # # except Exception as e:
# # # # #     print(f"Error: {str(e)}")
# # # # #     log_debug(f"Exception: {str(e)}")
# # # # #     sys.exit(1)

# # # # #!/usr/bin/env python3
# # # # import os
# # # # import sys
# # # # import select
# # # # from pathlib import Path

# # # # def log_debug(message):
# # # #     sys.stderr.write(f"DEBUG: {message}\n")
# # # #     sys.stderr.flush()  # Ensure debug messages are written immediately

# # # # print("Content-Type: text/plain")
# # # # print()
# # # # sys.stdout.flush()  # Ensure headers are sent immediately

# # # # try:
# # # #     # Get and validate Content-Length
# # # #     content_length = int(os.environ.get('CONTENT_LENGTH', 0))
# # # #     log_debug(f"Content-Length: {content_length}")
# # # #     if content_length == 0:
# # # #         print("Error: No content received")
# # # #         sys.exit(1)

# # # #     # Get and validate Content-Type and boundary
# # # #     content_type = os.environ.get('CONTENT_TYPE', '')
# # # #     log_debug(f"Content-Type: {content_type}")
# # # #     if not content_type.startswith('multipart/form-data'):
# # # #         print("Error: Invalid content type")
# # # #         sys.exit(1)

# # # #     boundary = content_type.split('boundary=')[1].encode()
# # # #     log_debug(f"Using boundary: {boundary}")

# # # #     # Read POST data
# # # #     post_data = b''
# # # #     bytes_read = 0
# # # #     chunk_size = 8192  # Read in 8KB chunks
    
# # # #     while bytes_read < content_length:
# # # #         ready = select.select([sys.stdin.buffer], [], [], 5)  # 5 second timeout
# # # #         if not ready[0]:
# # # #             print("Error: Timeout reading POST data")
# # # #             sys.exit(1)
            
# # # #         chunk = sys.stdin.buffer.read(min(chunk_size, content_length - bytes_read))
# # # #         if not chunk:
# # # #             break
# # # #         post_data += chunk
# # # #         bytes_read += len(chunk)
# # # #         log_debug(f"Read {bytes_read} of {content_length} bytes")

# # # #     # Validate received data
# # # #     if len(post_data) != content_length:
# # # #         print(f"Error: Expected {content_length} bytes but received {len(post_data)}")
# # # #         sys.exit(1)

# # # #     # Split and process parts
# # # #     parts = post_data.split(b'--' + boundary)
# # # #     log_debug(f"Found {len(parts)} parts")

# # # #     file_data = None
# # # #     new_name = None
# # # #     original_filename = None

# # # #     # Process each part
# # # #     for part in parts:
# # # #         if not part.strip():
# # # #             continue

# # # #         try:
# # # #             # Split headers and content
# # # #             if b'\r\n\r\n' not in part:
# # # #                 continue
# # # #             headers, content = part.split(b'\r\n\r\n', 1)
# # # #             headers = headers.decode('utf-8', errors='ignore')
            
# # # #             if 'Content-Disposition' not in headers:
# # # #                 continue

# # # #             if 'filename=' in headers:
# # # #                 # Extract filename
# # # #                 original_filename = headers.split('filename=')[1].split('\r\n')[0].strip('"')
# # # #                 file_data = content.rstrip(b'\r\n--')
# # # #                 log_debug(f"Found file: {original_filename}, size: {len(file_data)} bytes")
# # # #             elif 'name="newName"' in headers:
# # # #                 new_name = content.rstrip(b'\r\n--').decode('utf-8', errors='ignore')
# # # #                 log_debug(f"Found new name: {new_name}")

# # # #         except Exception as e:
# # # #             log_debug(f"Error processing part: {str(e)}")
# # # #             continue

# # # #     # Validate required data
# # # #     if not all([file_data, new_name, original_filename]):
# # # #         print("Error: Missing required fields")
# # # #         log_debug(f"file_data: {bool(file_data)}, new_name: {bool(new_name)}, original_filename: {bool(original_filename)}")
# # # #         sys.exit(1)

# # # #     # Get extension and create path
# # # #     ext = Path(original_filename).suffix
# # # #     upload_dir = "upload"
# # # #     if not os.path.exists(upload_dir):
# # # #         os.makedirs(upload_dir)
    
# # # #     new_filename = os.path.join(upload_dir, new_name + ext)
# # # #     log_debug(f"Saving to: {new_filename}")

# # # #     # Save file
# # # #     try:
# # # #         with open(new_filename, 'wb') as f:
# # # #             f.write(file_data)
# # # #         print(f"File uploaded successfully as {new_name + ext}")
# # # #         log_debug("File saved successfully")
# # # #     except IOError as e:
# # # #         print(f"Error saving file: {str(e)}")
# # # #         log_debug(f"Save error: {str(e)}")
# # # #         sys.exit(1)

# # # # except Exception as e:
# # # #     print(f"Error: {str(e)}")
# # # #     log_debug(f"Fatal error: {str(e)}")
# # # #     sys.exit(1)


# # # #!/usr/bin/env python3
# # # import os
# # # import sys
# # # import select
# # # from pathlib import Path

# # # def log_debug(message):
# # #     sys.stderr.write(f"DEBUG: {message}\n")
# # #     sys.stderr.flush()

# # # print("Content-Type: text/plain")
# # # print()
# # # sys.stdout.flush()

# # # try:
# # #     # Log environment variables
# # #     log_debug(f"Environment: {dict(os.environ)}")
    
# # #     content_length = int(os.environ.get('CONTENT_LENGTH', 0))
# # #     log_debug(f"Content-Length: {content_length}")
    
# # #     # Read POST data with detailed logging
# # #     post_data = b''
# # #     bytes_read = 0
# # #     chunk_size = 8192
    
# # #     log_debug("Starting to read POST data")
# # #     while bytes_read < content_length:
# # #         ready = select.select([sys.stdin.buffer], [], [], 5)
# # #         if not ready[0]:
# # #             log_debug("Timeout while reading data")
# # #             print("Error: Timeout reading POST data")
# # #             sys.exit(1)
            
# # #         chunk = sys.stdin.buffer.read(min(chunk_size, content_length - bytes_read))
# # #         if not chunk:
# # #             log_debug("No more data to read")
# # #             break
            
# # #         post_data += chunk
# # #         bytes_read += len(chunk)
# # #         log_debug(f"Progress: {bytes_read}/{content_length} bytes")

# # #     log_debug("Finished reading POST data")
# # #     log_debug(f"Total bytes read: {len(post_data)}")

# # #     # Split parts
# # #     parts = post_data.split(b'--' + boundary)
# # #     log_debug(f"Number of parts found: {len(parts)}")

# # #     # Process parts with more logging
# # #     for i, part in enumerate(parts):
# # #         if not part.strip():
# # #             continue

# # #         log_debug(f"Processing part {i}")
# # #         try:
# # #             if b'\r\n\r\n' not in part:
# # #                 log_debug(f"Part {i}: No header-body separator found")
# # #                 continue
                
# # #             headers, content = part.split(b'\r\n\r\n', 1)
# # #             headers = headers.decode('utf-8', errors='ignore')
# # #             log_debug(f"Part {i} headers: {headers[:200]}")

# # #             if 'filename=' in headers:
# # #                 original_filename = headers.split('filename=')[1].split('\r\n')[0].strip('"')
# # #                 file_data = content.rstrip(b'\r\n--')
# # #                 log_debug(f"Found file: {original_filename} ({len(file_data)} bytes)")
# # #             elif 'name="newName"' in headers:
# # #                 new_name = content.rstrip(b'\r\n--').decode('utf-8', errors='ignore')
# # #                 log_debug(f"Found new name: {new_name}")

# # #         except Exception as e:
# # #             log_debug(f"Error processing part {i}: {str(e)}")
# # #             continue

# # #     # Check data
# # #     if not all([file_data, new_name, original_filename]):
# # #         log_debug("Missing required fields:")
# # #         log_debug(f"file_data present: {bool(file_data)}")
# # #         log_debug(f"new_name present: {bool(new_name)}")
# # #         log_debug(f"original_filename present: {bool(original_filename)}")
# # #         print("Error: Missing required fields")
# # #         sys.exit(1)

# # #     # Save file with logging
# # #     try:
# # #         ext = Path(original_filename).suffix
# # #         upload_dir = "upload"
# # #         if not os.path.exists(upload_dir):
# # #             os.makedirs(upload_dir)
# # #             log_debug(f"Created directory: {upload_dir}")

# # #         new_filename = os.path.join(upload_dir, new_name + ext)
# # #         log_debug(f"Writing to: {new_filename}")

# # #         with open(new_filename, 'wb') as f:
# # #             f.write(file_data)
# # #         log_debug(f"Successfully wrote {len(file_data)} bytes")
# # #         print(f"File uploaded successfully as {new_name + ext}")

# # #     except IOError as e:
# # #         log_debug(f"IO Error: {str(e)}")
# # #         print(f"Error saving file: {str(e)}")
# # #         sys.exit(1)

# # # except Exception as e:
# # #     log_debug(f"Fatal error: {str(e)}")
# # #     print(f"Error: {str(e)}")
# # #     sys.exit(1)

# # #!/usr/bin/env python3
# # # import os
# # # import sys
# # # import select
# # # from pathlib import Path

# # # def log_debug(message):
# # #     sys.stderr.write(f"DEBUG: {message}\n")
# # #     sys.stderr.flush()

# # # print("Content-Type: text/plain")
# # # print()
# # # sys.stdout.flush()

# # # try:
# # #     # Get Content-Length and Content-Type
# # #     content_length = int(os.environ.get('CONTENT_LENGTH', 0))
# # #     content_type = os.environ.get('CONTENT_TYPE', '')
# # #     boundary = content_type.split('boundary=')[1].encode()
    
# # #     log_debug(f"Content-Length: {content_length}")
# # #     log_debug(f"Content-Type: {content_type}")
# # #     log_debug(f"Boundary: {boundary}")

# # #     # Read POST data
# # #     post_data = sys.stdin.buffer.read(content_length)
# # #     log_debug(f"Read {len(post_data)} bytes")

# # #     # Split parts
# # #     parts = post_data.split(b'--' + boundary)
# # #     log_debug(f"Found {len(parts)} parts")

# # #     file_data = None
# # #     new_name = None
# # #     original_filename = None

# # #     # Process parts
# # #     for part in parts:
# # #         if not part.strip():
# # #             continue

# # #         try:
# # #             if b'\r\n\r\n' not in part:
# # #                 continue
# # #             headers, content = part.split(b'\r\n\r\n', 1)
# # #             headers = headers.decode('utf-8', errors='ignore')

# # #             if 'filename=' in headers:
# # #                 original_filename = headers.split('filename=')[1].split('\r\n')[0].strip('"')
# # #                 file_data = content.rstrip(b'\r\n--')
# # #                 log_debug(f"Found file: {original_filename} ({len(file_data)} bytes)")
# # #             elif 'name="newName"' in headers:
# # #                 new_name = content.rstrip(b'\r\n--').decode('utf-8', errors='ignore')
# # #                 log_debug(f"Found new name: {new_name}")

# # #         except Exception as e:
# # #             log_debug(f"Error processing part: {str(e)}")
# # #             continue

# # #     # Validate data
# # #     if not all([file_data, new_name, original_filename]):
# # #         log_debug("Missing required fields")
# # #         print("Error: Missing required fields")
# # #         sys.exit(1)

# # #     # Save file
# # #     ext = Path(original_filename).suffix
# # #     upload_dir = "upload"
# # #     if not os.path.exists(upload_dir):
# # #         os.makedirs(upload_dir)

# # #     new_filename = os.path.join(upload_dir, new_name + ext)
# # #     log_debug(f"Saving to: {new_filename}")

# # #     with open(new_filename, 'wb') as f:
# # #         f.write(file_data)
# # #     print(f"File uploaded successfully as {new_name + ext}")

# # # except Exception as e:
# # #     log_debug(f"Error: {str(e)}")
# # #     print(f"Error: {str(e)}")
# # #     sys.exit(1)

# # #!/usr/bin/env python3
# # import os
# # import sys
# # from pathlib import Path

# # print("Content-Type: text/plain")
# # print()

# # try:
# #     content_length = int(os.environ.get('CONTENT_LENGTH', 0))
# #     content_type = os.environ.get('CONTENT_TYPE', '')
# #     boundary = content_type.split('boundary=')[1].encode()

# #     # Read POST data directly
# #     post_data = sys.stdin.buffer.read(content_length)
    
# #     # Split into parts
# #     parts = post_data.split(b'--' + boundary)
    
# #     file_data = None
# #     new_name = None
# #     original_filename = None

# #     # Process each part
# #     for part in parts:
# #         if not part.strip():
# #             continue
            
# #         if b'\r\n\r\n' not in part:
# #             continue
            
# #         headers, content = part.split(b'\r\n\r\n', 1)
# #         headers = headers.decode('utf-8', errors='ignore')
        
# #         if 'filename=' in headers:
# #             original_filename = headers.split('filename=')[1].split('\r\n')[0].strip('"')
# #             file_data = content.rstrip(b'\r\n--')
# #         elif 'name="newName"' in headers:
# #             new_name = content.rstrip(b'\r\n--').decode('utf-8', errors='ignore')

# #     # Save file
# #     if file_data and new_name and original_filename:
# #         ext = Path(original_filename).suffix
# #         upload_dir = "upload"
        
# #         if not os.path.exists(upload_dir):
# #             os.makedirs(upload_dir)
        
# #         new_filename = os.path.join(upload_dir, new_name + ext)
        
# #         with open(new_filename, 'wb') as f:
# #             f.write(file_data)
# #         print(f"File uploaded successfully as {new_name + ext}")
# #     else:
# #         print("Error: Missing required fields")
# #         sys.exit(1)

# # except Exception as e:
# #     print(f"Error: {str(e)}")
# #     sys.exit(1)
# #!/usr/bin/env python3
# import os
# import sys
# from pathlib import Path

# # Send headers
# print("Content-Type: text/plain")
# print()

# try:
#     # Get POST data
#     content_length = int(os.environ.get('CONTENT_LENGTH', 0))
#     content_type = os.environ.get('CONTENT_TYPE', '')
    
#     # Debug output
#     sys.stderr.write(f"DEBUG: Starting CGI script\n")
#     sys.stderr.write(f"DEBUG: Content-Length: {content_length}\n")
#     sys.stderr.write(f"DEBUG: Content-Type: {content_type}\n")
    
#     # Get boundary
#     boundary = content_type.split('boundary=')[1].encode()
    
#     # Read POST data
#     post_data = sys.stdin.buffer.read(content_length)
    
#     # Split into parts using boundary
#     parts = post_data.split(b'--' + boundary)
    
#     file_data = None
#     new_name = None
#     original_filename = None
    
#     # Process each part
#     for part in parts:
#         if not part.strip():
#             continue
            
#         # Get headers and content
#         if b'\r\n\r\n' not in part:
#             continue
            
#         headers, content = part.split(b'\r\n\r\n', 1)
#         headers = headers.decode('utf-8', errors='ignore')
        
#         # Extract file or new name
#         if 'filename=' in headers:
#             original_filename = headers.split('filename=')[1].split('\r\n')[0].strip('"')
#             file_data = content.rstrip(b'\r\n--')
#             sys.stderr.write(f"DEBUG: Found file: {original_filename}\n")
#         elif 'name="newName"' in headers:
#             new_name = content.rstrip(b'\r\n--').decode('utf-8', errors='ignore')
#             sys.stderr.write(f"DEBUG: Found new name: {new_name}\n")

#     # Save file with new name
#     if file_data and new_name and original_filename:
#         # Get original extension
#         ext = Path(original_filename).suffix
#         upload_dir = "upload"
        
#         # Create upload directory if needed
#         if not os.path.exists(upload_dir):
#             os.makedirs(upload_dir)
        
#         # Create new filename
#         new_filename = os.path.join(upload_dir, new_name + ext)
        
#         # Save file
#         with open(new_filename, 'wb') as f:
#             f.write(file_data)
#         print(f"File uploaded successfully as {new_name + ext}")
#     else:
#         print("Error: Missing required fields")
#         sys.exit(1)

# except Exception as e:
#     sys.stderr.write(f"DEBUG: Error: {str(e)}\n")
#     print(f"Error: {str(e)}")
#     sys.exit(1)


#!/usr/bin/env python3
import os
import sys
from pathlib import Path

# First print headers
# print("Content-Type: text/plain")  # Changed from text/html to text/plain
print()  # Empty line after headers
sys.stdout.flush()  # Ensure headers are sent

try:
    # Debug output
# sys.stderr.write("DEBUG: Starting upload script\n")
    sys.stderr.flush()

    # Get content info
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    content_type = os.environ.get('CONTENT_TYPE', '')
    
    if not content_type.startswith('multipart/form-data'):
        print("Error: Invalid content type")
        sys.exit(1)

    boundary = content_type.split('boundary=')[1].encode()
    
    # Read POST data in chunks
    chunk_size = 8192  # 8KB chunks
    post_data = bytearray()
    bytes_read = 0
    
    while bytes_read < content_length:
        remaining = content_length - bytes_read
        chunk = sys.stdin.buffer.read(min(chunk_size, remaining))
        if not chunk:
            break
        post_data.extend(chunk)
        bytes_read += len(chunk)
        # sys.stderr.write(f"DEBUG: Read {bytes_read} of {content_length} bytes\n")
        sys.stderr.flush()

    # Convert to bytes
    post_data = bytes(post_data)
    
    # Split into parts
    parts = post_data.split(b'--' + boundary)
    
    file_data = None
    new_name = None
    original_filename = None
    
    # Process each part
    for part in parts:
        if not part.strip():
            continue
            
        # Split headers and content
        if b'\r\n\r\n' not in part:
            continue
            
        headers, content = part.split(b'\r\n\r\n', 1)
        headers = headers.decode('utf-8', errors='ignore')
        
        if 'filename=' in headers:
            # This is the file part
            for line in headers.split('\r\n'):
                if 'filename=' in line:
                    original_filename = line.split('filename=')[1].strip('"')
                    break
            file_data = content.rstrip(b'\r\n--')
        elif 'name="newName"' in headers:
            # This is the new name field
            new_name = content.rstrip(b'\r\n--').decode('utf-8', errors='ignore')

    # Validate data
    if not all([file_data, new_name, original_filename]):
        print("Error: Missing required fields")
        sys.exit(1)

    # Get file extension and create new filename
    ext = Path(original_filename).suffix
    upload_dir = "upload"
    
    # Create upload directory if needed
    if not os.path.exists(upload_dir):
        os.makedirs(upload_dir)

    # Create new filename with original extension
    new_filename = os.path.join(upload_dir, new_name + ext)
    
    # Save file
    with open(new_filename, 'wb') as f:
        f.write(file_data)
    
    print(f"File uploaded successfully as {new_name + ext}")

except Exception as e:
    sys.stderr.write(f"ERROR: {str(e)}\n")
    print(f"Error: {str(e)}")
    sys.exit(1)