import socket
import json
import time
import threading
import pygame

ESP_IP = "192.168.0.169"  # Die IP-Adresse, die auf dem ESP32-Display steht
ESP_PORT = 4210

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", ESP_PORT))
sock.settimeout(0.5)

pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("Kein Controller gefunden!")
    raise SystemExit

js = pygame.joystick.Joystick(0)
js.init()
print(f"Controller verbunden: {js.get_name()}")

running = True

# Empfangs-Thread für Telemetriedaten vom ESP32
def receive_telemetry():
    while running:
        try:
            data, addr = sock.recvfrom(1024)
            t = json.loads(data.decode("utf-8"))
            
            status = "RUNNING" if t.get("run") else "STOPPED"
            ax, ay, az = t.get("ax", 0), t.get("ay", 0), t.get("az", 0)
            m1, m2, m3, m4 = t.get("m1", 0), t.get("m2", 0), t.get("m3", 0), t.get("m4", 0)
            
            # Zeigt MPU-Sensordaten und Motoren live in 1 Zeile im PC-Terminal an
            print(f"[{status}] MPU: X={ax:4.1f} Y={ay:4.1f} Z={az:4.1f} | PWM: M1={m1:3d} M2={m2:3d} M3={m3:3d} M4={m4:3d}    ", end="\r")
        except socket.timeout:
            pass
        except Exception:
            pass

recv_thread = threading.Thread(target=receive_telemetry, daemon=True)
recv_thread.start()

print("\n--- STEUERUNG BEREIT ---")
print("START-Taste : Motoren einschalten")
print("BACK-Taste  : NOT-AUS (Motoren stoppen)")
print("RT-Trigger  : Schub / Gas geben (0 - 100%)\n")

try:
    while True:
        pygame.event.pump()

        # Analog-Sticks & Trigger auslesen
        lx = js.get_axis(0)
        ly = js.get_axis(1)
        rx = js.get_axis(2)
        ry = js.get_axis(3)

        lt_raw = js.get_axis(4) if js.get_numaxes() > 4 else -1.0
        rt_raw = js.get_axis(5) if js.get_numaxes() > 5 else -1.0
        lt = round((lt_raw + 1) / 2.0, 2)
        rt = round((rt_raw + 1) / 2.0, 2)

        # D-Pad (Hat)
        hat_x, hat_y = 0, 0
        if js.get_numhats() > 0:
            hat_x, hat_y = js.get_hat(0)

        # JSON Paket zusammenbauen
        payload = {
            "lx": round(lx, 2), "ly": round(ly, 2),
            "rx": round(rx, 2), "ry": round(ry, 2),
            "lt": lt, "rt": rt,
            "A": js.get_button(0), "B": js.get_button(1),
            "X": js.get_button(2), "Y": js.get_button(3),
            "LB": js.get_button(4), "RB": js.get_button(5),
            "BACK": js.get_button(6), "START": js.get_button(7),
            "hat_x": hat_x, "hat_y": hat_y
        }

        # UDP senden
        sock.sendto(json.dumps(payload).encode("utf-8"), (ESP_IP, ESP_PORT))
        time.sleep(0.02)  # 50 Hz

except KeyboardInterrupt:
    print("\nSteuerung beendet.")
finally:
    running = False
    pygame.quit()
    sock.close()