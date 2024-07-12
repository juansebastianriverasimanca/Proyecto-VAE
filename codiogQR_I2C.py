import sensor
import image
import time
import pyb

# Inicializa el bus I2C como maestro
i2c = pyb.I2C(4, pyb.I2C.MASTER, baudrate=100000)

# Dirección del dispositivo ESP32 en el bus I2C
direccion_dispositivo_esp32 = 0x68  # Dirección I2C del ESP32

# Valores proporcionados
x_reference = 83
y_reference = 27
w_reference = 200
h_reference = 194

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(10)
clock = time.clock()

detected_qr = False  # Variable para verificar si se detectó un código QR

while True:
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.6)

    qr_detected = False  # Bandera para verificar si se detectó un código QR en este bucle

    for code in img.find_qrcodes():
        x_detected = code.x()
        y_detected = code.y()
        w_detected = code.w()
        h_detected = code.h()

        # Calcular la correlación
        correlation = (
            (x_detected * x_reference + y_detected * y_reference + w_detected * w_reference + h_detected * h_reference)
            / ((x_detected**2 + y_detected**2 + w_detected**2 + h_detected**2)**0.5
               * (x_reference**2 + y_reference**2 + w_reference**2 + h_reference**2)**0.5)
        )

        # Mostrar el valor de correlación
        print("Correlación:", correlation)

        # Comprobar si la correlación es mayor a 0.8
        if 0.85 < correlation < 0.90:
            #print("Correlación casi perfecta")
            qr_detected = True  # Actualizar la bandera indicando que se detectó un código QR
            # Envía mensaje a ESP32
            i2c.send("1", addr=direccion_dispositivo_esp32)
            # Comprobar si la correlación es mayor a 0.8
        if 0.90 < correlation < 0.95:
           #print("Correlación casi perfecta")
            # Envía mensaje a ESP32
            i2c.send("2", addr=direccion_dispositivo_esp32)
            qr_detected = True  # Actualizar la bandera indicando que se detectó un código QR
        if 0.95 < correlation < 0.98:
           # print("Correlación casi perfecta")
            qr_detected = True  # Actualizar la bandera indicando que se detectó un código QR
            # Envía mensaje a ESP32
            i2c.send("3", addr=direccion_dispositivo_esp32)
            # Comprobar si la correlación es mayor a 0.8
        if 0.98 < correlation < 1:
            #print("Correlación casi perfecta")
            qr_detected = True  # Actualizar la bandera indicando que se detectó un código QR
            # Envía mensaje a ESP32
            i2c.send("4", addr=direccion_dispositivo_esp32)

    if not qr_detected:
        #print("Acércate más")  # Mensaje si no se detecta ningún código QR

        # Envía mensaje a ESP32
        i2c.send("0", addr=direccion_dispositivo_esp32)

    pyb.delay(1000)  # Espera antes de la siguiente iteración
