import os
import time
import pathlib

pipe_name = "my_pipe" 

try:
    os.mkfifo(pipe_name)
except FileExistsError:
    pass  # If the pipe already exists, it's okay

last_modification_time = 0

while True:
    current_modification_time = os.path.getmtime(pipe_name)
    
    if current_modification_time != last_modification_time:
        with open(pipe_name, "r") as pipe:
            while True:
                message = pipe.readline().strip()
                if message:
                    print("Received:", message)
        
        last_modification_time = current_modification_time

    # Add a delay to avoid busy-waiting
    time.sleep(1)  # You can adjust the sleep duration as needed
