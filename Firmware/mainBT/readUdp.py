import socket

UDP_IP = ""          # vazio = escuta em todas as interfaces
UDP_PORT = 5005      # mesma porta do ESP32

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Escutando UDP na porta {UDP_PORT}...")

while True:
    data, addr = sock.recvfrom(1024)
    print(f"{addr}: {data.decode(errors='ignore')}", end="")