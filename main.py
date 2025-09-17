#!/usr/bin/env python3
import platform, socket, datetime, psutil
import requests
import os

def print_file_colored(filename):
    colors = [31, 32, 33, 34, 35, 36]
    with open(filename, "r") as f:
        for i, line in enumerate(f):
            color = colors[i % len(colors)]
            print(f"\033[{color}m{line.rstrip()}\033[0m")

print_file_colored("ascii.txt")

val = os.environ.get("API_KEY_WEATHER")

print("Name:", socket.gethostname())
print("Arch:", platform.machine())
print("Kernel:", platform.version())

now = datetime.datetime.now()
print("Local time:", now.strftime("%Y-%m-%d %H:%M:%S"))

boot_time = datetime.datetime.fromtimestamp(psutil.boot_time())
uptime = datetime.datetime.now() - boot_time
print("Uptime:", str(uptime).split('.')[0])

res = requests.get("http://ip-api.com/json/")
js = res.json()

weather_res = requests.get(f"https://api.openweathermap.org/data/2.5/weather?lat={js['lat']}&lon={js['lon']}&appid={val}")
data = weather_res.json()

humidity = data["main"]["humidity"]
feels_like = data["main"]["feels_like"] - 273.15
min_temp = data["main"]["temp_min"] - 273.15
max_temp = data["main"]["temp_max"] - 273.15
pressure = data["main"]["pressure"] * 100
visibility = data["visibility"]
weather_description = data["weather"][0]["description"]
wind_speed = data["wind"]["speed"]
name = data["name"]

print(f"Name: {name}")
print(f"Humidity: {humidity} %")
print(f"Feels like: {feels_like:.2f} °C")
print(f"Min temp: {min_temp:.2f} °C")
print(f"Max temp: {max_temp:.2f} °C")
print(f"Pressure: {pressure} Pa")
print(f"Visibility: {visibility} m")
print(f"Weather: {weather_description}")
print(f"Wind speed: {wind_speed} m/s")

