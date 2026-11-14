"""Basic connection example.
"""

import time
from redis_config import create_redis_client

redis_client = create_redis_client()


is_connected = redis_client.ping()
print("Redis Connected:", is_connected)


timestamp_ms = int(time.time() * 1000)

redis_client.ts().add(
    f"{mac_address}:temperature", timestamp_ms, temperature
)
redis_client.ts().add(
    f"{mac_address}:humidity", timestamp_ms, humidity
)
