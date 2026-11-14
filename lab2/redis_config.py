"""Redis connection settings loaded from the repository's local environment file."""

import os
from pathlib import Path

import redis
from dotenv import load_dotenv


load_dotenv(Path(__file__).resolve().parents[1] / ".env.local")


def create_redis_client():
    required = ("REDIS_HOST", "REDIS_PORT", "REDIS_USERNAME", "REDIS_PASSWORD")
    missing = [name for name in required if not os.getenv(name)]
    if missing:
        raise RuntimeError(f"Missing Redis settings in .env.local: {', '.join(missing)}")

    return redis.Redis(
        host=os.environ["REDIS_HOST"],
        port=int(os.environ["REDIS_PORT"]),
        username=os.environ["REDIS_USERNAME"],
        password=os.environ["REDIS_PASSWORD"],
        decode_responses=True,
    )
