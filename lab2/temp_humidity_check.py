import adafruit_dht
import uuid
import time
from datetime import datetime
from board import D4
import redis

redis_client = redis.Redis(
    host='megasweet-vegetable-cover-83233.db.redis.io',
    port=14483,
    decode_responses=True,
    username="default",
    password="0l55BJiIdeLbzgzz2akDmatGIwIPCf5q",
)


is_connected = redis_client.ping()
print("Redis Connected:", is_connected)


dht_device = adafruit_dht.DHT11(D4)
mac = uuid.getnode()
mac_address = ':'.join(f'{(mac >> i) & 0xff:02x}' for i in range(40, -1, -8))

check = input("Vai?")

try:

    while(True):
        timestamp = time.time()
        formatted_datetime = datetime.fromtimestamp(timestamp).strftime('%Y-%m-%d %H:%M:%S.%f')
        temperature = dht_device.temperature
        humidity = dht_device.humidity
        # print(formatted_datetime, "\t", temperature, "\t", humidity, "\n")

        timestamp_ms = int(time.time() * 1000)

        redis_client.ts().add(f"{mac_address}:temperature", timestamp_ms, temperature)
        redis_client.ts().add(f"{mac_address}:humidity", timestamp_ms, humidity)
        key = f"{mac_address}:temperature"
        print("Ultimo campione:", redis_client.ts().get(key))
        print("Numero campioni:", redis_client.ts().info(key).total_samples)

        time.sleep(2)

except:
    print("\nSistema spento\n")
    dht_device.exit()
    dht_device = adafruit_dht.DHT11(D4)