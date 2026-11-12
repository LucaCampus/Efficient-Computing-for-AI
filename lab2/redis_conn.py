"""Basic connection example.
"""

import redis
import time

redis_client = redis.Redis(
    host='megasweet-vegetable-cover-83233.db.redis.io',
    port=14483,
    decode_responses=True,
    username="default",
    password="0l55BJiIdeLbzgzz2akDmatGIwIPCf5q",
)


is_connected = redis_client.ping()
print("Redis Connected:", is_connected)


timestamp_ms = int(time.time() * 1000)

redis_client.ts().add(
    f"{mac_address}:temperature", timestamp_ms, temperature
)
redis_client.ts().add(
    f"{mac_address}:humidity", timestamp_ms, humidity
)