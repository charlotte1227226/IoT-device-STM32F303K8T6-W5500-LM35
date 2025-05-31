import socket
import time

def get_temperature_once(ip, port, request_payload=b"GET_TEMP\r\n"):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(5.0)
    try:
        sock.connect((ip, port))
        sock.sendall(request_payload)
        response = sock.recv(64)
        return response
    finally:
        sock.close()

if __name__ == "__main__":
    STM32_IP = "192.168.0.20"
    STM32_PORT = 5000

    while True:
        try:
            # 1. 連線、拉資料、馬上關掉
            data = get_temperature_once(STM32_IP, STM32_PORT, b"GET_TEMP\r\n")
            text = data.decode("utf-8", errors="ignore").strip()
            print(f"[{time.strftime('%H:%M:%S')}] 溫度：{text}")
        except Exception as e:
            print(f"[WARN] 此次連線失敗或逾時：{e}")

        # 2. 關掉 socket 之後，暫停一段時間，給 STM32 端回到 LISTEN 的時間
        time.sleep(2)  # 可依實際狀況調整，通常 1~2 秒足矣

        # 3. 等 5 秒之後做下一輪拉溫度
        time.sleep(3)  # 總共 2 + 3 = 5 秒間隔
