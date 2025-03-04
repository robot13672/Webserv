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