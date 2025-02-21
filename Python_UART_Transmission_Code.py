import serial
import time

# Configure serial port
ser = serial.Serial('COM6', 2400, timeout=0.1)
time.sleep(2)  # Allow device to reset

message = """Finance Minister Arun Jaitley Tuesday hit out at former RBI governor Raghuram Rajan for predicting that the next banking crisis would be triggered by MSME lending, saying postmortem is easier than taking action when it was required. Rajan, who had as the chief economist at IMF warned of impending financial crisis of 2008, in a note to a parliamentary committee warned against ambitious credit targets and loan waivers, saying that they could be the sources of next banking crisis. Government should focus on sources of the next crisis, not just the last one. In particular, government should refrain from setting ambitious credit targets or waiving loans. Credit targets are sometimes achieved by abandoning appropriate due diligence, creating the environment for future NPAs, Rajan said in the note. Both MUDRA loans as well as the Kisan Credit Card, while popular, have to be examined more closely for potential credit risk. Rajan, who was RBI governor for three years till September 2016, is currently."""

def send_data():
    total_bits_sent = 0
    start_time = time.time()

    for char in message:
        ser.write(char.encode('utf-8'))

        # Track bits sent
        total_bits_sent += 8

        # Display live speed
        elapsed_time = time.time() - start_time
        if elapsed_time > 0:
            speed = total_bits_sent / elapsed_time
            print(f"Sent: {char} | Speed: {speed:.2f} bits/sec")

    # Give time for all data to be processed
    time.sleep(1)

def receive_data():
    print("\nReceiving Data:")
    start_time = time.time()
    received_data = ""

    while len(received_data) < len(message) or ser.in_waiting > 0:
        char = ser.read(1).decode(errors='ignore')
        if char:
            received_data += char
            elapsed_time = time.time() - start_time
            print(f"Received: {char} | Speed: {8 * len(received_data) / elapsed_time:.2f} bits/sec", flush=True)

    # Wait for any remaining bytes
    time.sleep(1)

send_data()
receive_data()
input("\nPress Enter to exit...")
ser.close()
