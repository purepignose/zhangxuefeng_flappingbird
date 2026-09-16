"""Generate the small original sound effects used by the game."""

import math
import random
import struct
import wave
from pathlib import Path


SAMPLE_RATE = 44100
OUTPUT_DIR = Path(__file__).resolve().parents[1] / "assets" / "sounds"


def envelope(t: float, duration: float, attack: float = 0.015) -> float:
    fade_in = min(1.0, t / attack)
    fade_out = max(0.0, (duration - t) / max(attack, duration * 0.28))
    return fade_in * min(1.0, fade_out)


def save(name: str, duration: float, sample_fn) -> None:
    frames = bytearray()
    for index in range(int(SAMPLE_RATE * duration)):
        t = index / SAMPLE_RATE
        value = max(-1.0, min(1.0, sample_fn(t, duration)))
        frames.extend(struct.pack("<h", int(value * 32767)))

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    with wave.open(str(OUTPUT_DIR / name), "wb") as wav:
        wav.setnchannels(1)
        wav.setsampwidth(2)
        wav.setframerate(SAMPLE_RATE)
        wav.writeframes(frames)


def score(t: float, duration: float) -> float:
    frequency = 660.0 if t < 0.085 else 990.0
    tone = math.sin(2.0 * math.pi * frequency * t)
    sparkle = 0.28 * math.sin(2.0 * math.pi * frequency * 2.0 * t)
    return (tone + sparkle) * 0.38 * envelope(t, duration, 0.006)


random_source = random.Random(20260916)
hurt_noise = [random_source.uniform(-1.0, 1.0) for _ in range(int(SAMPLE_RATE * 0.28))]


def hurt(t: float, duration: float) -> float:
    index = min(len(hurt_noise) - 1, int(t * SAMPLE_RATE))
    frequency = 190.0 - 115.0 * (t / duration)
    square = 1.0 if math.sin(2.0 * math.pi * frequency * t) >= 0 else -1.0
    return (square * 0.58 + hurt_noise[index] * 0.27) * 0.48 * envelope(t, duration, 0.004)


def heal(t: float, duration: float) -> float:
    notes = (523.25, 659.25, 783.99)
    section = min(2, int(t / (duration / 3.0)))
    local_t = t - section * duration / 3.0
    ping = math.sin(2.0 * math.pi * notes[section] * local_t)
    shimmer = 0.3 * math.sin(2.0 * math.pi * notes[section] * 2.0 * local_t)
    return (ping + shimmer) * 0.38 * envelope(local_t, duration / 3.0, 0.008)


dash_random = random.Random(1200)
dash_noise = [dash_random.uniform(-1.0, 1.0) for _ in range(int(SAMPLE_RATE * 0.48))]


def dash(t: float, duration: float) -> float:
    progress = t / duration
    index = min(len(dash_noise) - 1, int(t * SAMPLE_RATE))
    frequency = 170.0 + 750.0 * progress
    sweep = math.sin(2.0 * math.pi * frequency * t)
    noise = dash_noise[index] * (0.45 + 0.55 * progress)
    return (sweep * 0.62 + noise * 0.28) * 0.45 * envelope(t, duration, 0.012)


def death(t: float, duration: float) -> float:
    notes = (392.00, 493.88, 587.33, 783.99, 987.77, 1174.66)
    note_length = 0.22
    note_index = min(len(notes) - 1, int(max(0.0, t - 0.12) / note_length))
    local_t = max(0.0, t - 0.12 - note_index * note_length)
    tone = math.sin(2.0 * math.pi * notes[note_index] * local_t)
    angelic = 0.32 * math.sin(2.0 * math.pi * notes[note_index] * 2.0 * local_t)
    note_env = envelope(local_t, note_length, 0.008)

    final_ding = 0.0
    if t > 1.43:
        ding_t = t - 1.43
        ding_env = math.exp(-5.5 * ding_t)
        final_ding = (
            math.sin(2.0 * math.pi * 1567.98 * ding_t)
            + 0.45 * math.sin(2.0 * math.pi * 2351.97 * ding_t)
        ) * ding_env

    return (tone + angelic) * 0.27 * note_env + final_ding * 0.28


def flap(t: float, duration: float) -> float:
    progress = t / duration
    frequency = 310.0 + 260.0 * progress
    soft_pop = math.sin(2.0 * math.pi * frequency * t)
    airy = math.sin(2.0 * math.pi * 92.0 * t) * (1.0 - progress)
    return (soft_pop * 0.55 + airy * 0.25) * 0.48 * envelope(t, duration, 0.004)


save("score.wav", 0.19, score)
save("hurt.wav", 0.28, hurt)
save("heal.wav", 0.48, heal)
save("dash.wav", 0.48, dash)
save("death.wav", 1.85, death)
save("flap.wav", 0.13, flap)
