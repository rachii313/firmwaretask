
import time
import serial
#port configuration
serialport = serial.Serial('COM4    ', 2400, timeout=1)  
#given data to send 
datatosend = """Finance Minister Arun Jaitley Tuesday hit out at former RBI governor Raghuram Rajan for predicting that the next banking crisis would be triggered by MSME lending, saying postmortem is easier than taking action when it was required. Rajan, who had as the chief economist at IMF warned of impending financial crisis of 2008, in a note to a parliamentary committee warned against ambitious credit targets and loan waivers, saying that they could be the sources of next banking crisis. Government should focus on sources of the next crisis, not just the last one.

In particular, government should refrain from setting ambitious credit targets or waiving loans. Credit targets are sometimes achieved by abandoning appropriate due diligence, creating the environment for future NPAs," Rajan said in the note." Both MUDRA loans as well as the Kisan Credit Card, while popular, have to be examined more closely for potential credit risk. Rajan, who was RBI governor for three years till September 2016, is currently."""
#function to calculate bits per second and print the speed
def bitcalculation(start_time, bytes_transferred):
    end_time = time.time()
    duration = end_time - start_time  # Time in seconds
    speed = (bytes_transferred * 8) / duration  # Speed in bits per second
    return speed
# Function to send data from PC to MCU and print speed
def datatomcu():
    bytes_sent = 0
    start_time = time.time()
    for byte in datatosend:
        serialport.write(byte.encode())  # Send each byte
        bytes_sent += 1
        if bytes_sent % 10 == 0:  # Print every 10 bytes for speed
            speed = bitcalculation(start_time, bytes_sent)
            print(f"Sent {bytes_sent} bytes at {speed:.2f} bps")
    # Final speed print after transmission
    final_speed = bitcalculation(start_time, bytes_sent)
    print(f"Final transmission speed: {final_speed:.2f} bps")
# Function to receive data from MCU and print speed
def receive_data_from_mcu():
    bytes_received = 0
    start_time = time.time()
    received_data = ""
    while True:
        byte = serialport.read()  # Read byte from MCU
        if byte:
            received_data += byte.decode()
            bytes_received += 1
            if bytes_received % 10 == 0:  # Print every 10 bytes for speed
                speed = bitcalculation(start_time, bytes_received)
                print(f"Received {bytes_received} bytes at {speed:.2f} bps")
        if bytes_received >= len(datatosend):
            break
 # Final speed print after receiving data
    final_speed = bitcalculation(start_time, bytes_received)
    print(f"Final reception speed: {final_speed:.2f} bps")
    print(f"Received Data: {received_data[:200]}...")  # Print part of the received data
# Main function to perform the UART communication
def main():
    datatomcu()
    receive_data_from_mcu()
if __name__ == "__main__":
    main()
