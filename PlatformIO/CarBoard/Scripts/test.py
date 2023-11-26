import pandas as pd
import json
import numpy as np
import matplotlib.pyplot as plt
from math import radians, sin, cos, sqrt, atan2

def haversine(lat1, lon1, lat2, lon2):
    R = 6371.0  # Earth radius in kilometers
    dlat = radians(lat2 - lat1)
    dlon = radians(lon2 - lon1)
    a = sin(dlat / 2) ** 2 + cos(radians(lat1)) * cos(radians(lat2)) * sin(dlon / 2) ** 2
    c = 2 * atan2(sqrt(a), sqrt(1 - a))
    distance = R * c
    return distance


# Load JSON data from file
with open('test.json', 'r') as file:
    json_data = json.load(file)

# Extracting 'fields' data
fields_data = [item['fields'] for item in json_data]

# Creating a DataFrame
df = pd.DataFrame(fields_data)

# Convert 'timestamp' column to datetime
df['timestamp'] = pd.to_datetime(df['timestamp'])

# Calculate time difference from the first timestamp in seconds
df['seconds_since_first'] = (df['timestamp'] - df['timestamp'].min()).dt.total_seconds()

# Calculate difference between current and previous timestamp in seconds
df['time_diff_prev_sample'] = df['timestamp'].diff().dt.total_seconds()


print(df['time_diff_prev_sample'] .mean())
print(df)


#LATITUD - GRÁFICA DE ERROR ABSOLUTO

# Reference latitude (replace with your desired reference latitude)
reference_latitude = 4874117*10e-6
# Calculate error between reference latitude and 'latitud' column
df['latitude_error'] = abs(df['latitud']*10e-7  - reference_latitude)
df_filtered = df[df['latitude_error'] >= 0] 
df_filtered = df[df['latitude_error'] <= 4] 
# Plotting the geometric error after filtering
plt.figure(figsize=(10, 6))
plt.plot(df_filtered['seconds_since_first'], df_filtered['latitude_error'], marker='o', linestyle='--', markersize=4)
plt.title('Error absoluto entre la latitud de referencia y la latitud experimental')
plt.xlabel('Tiempo (s)')
plt.ylabel('Error en la latitud (°)')
plt.grid(True)
# Calculate and plot the mean line
mean_latitude_error = df_filtered['latitude_error'].mean()
plt.axhline(y=mean_latitude_error, color='red', linestyle='--', label=f'Error absoluto promedio: {mean_latitude_error:.9f}°')
plt.legend()

plt.show()

#LONGITUD - GRÁFICA DE ERROR ABSOLUTO

# Reference longitude (replace with your desired reference longitude)
reference_longitude = -74046444*10e-6
# Calculate error between reference longitude and 'longitud' column
df['longitude_error'] = abs(df['longitud']*10e-7  - reference_longitude)

df_filtered = df[df['longitude_error'] <= 4]
# Plotting the geometric error after filtering
plt.figure(figsize=(10, 6))
plt.plot(df_filtered['seconds_since_first'], df_filtered['longitude_error'], marker='o', linestyle='--', markersize=4)
plt.title('Error absoluto entre la longitud de referencia y la longitud experimental')
plt.xlabel('Tiempo (s)')
plt.ylabel('Error en la longitud (°)')
plt.grid(True)
# Calculate and plot the mean line
mean_longitude_error = df_filtered['longitude_error'].mean()
plt.axhline(y=mean_longitude_error, color='red', linestyle='--', label=f'Error absoluto promedio: {mean_longitude_error:.9f}°')
plt.legend()
plt.show()

#DISTANCIA HARVESINE - GRÁFICA DE ERROR ABSOLUTO
# Calculate Haversine distance error
df['haversine_distance'] = df.apply(lambda row: haversine(row['latitud']*10e-7 , row['longitud']*10e-7 , reference_latitude, reference_longitude), axis=1)
df['haversine_distance'] = df['haversine_distance'][df['haversine_distance'] <= 4]

# Plotting the Haversine distance error
plt.figure(figsize=(10, 6))
plt.plot(df['seconds_since_first'], df['haversine_distance'], marker='o', linestyle='--', markersize=4)
plt.title('Error de distancia Haversine entre la posición de referencia y la posición experimental')
plt.xlabel('Tiempo (s)')
plt.ylabel('Error en la distancia (m)')
plt.grid(True)

# Calculate and plot the mean line for Haversine distance error
mean_haversine_distance = df['haversine_distance'].mean()
plt.axhline(y=mean_haversine_distance, color='red', linestyle='--', label=f'Error promedio Haversine: {mean_haversine_distance:.3f} m')
plt.legend()

est_deviation = df['haversine_distance'].std()
plt.axhline(y=est_deviation, color='blue', linestyle='--', label=f'Desviación estándar: {est_deviation:.3f} m')
plt.legend()

plt.show()





plt.figure(figsize=(10, 6))
plt.plot(df_filtered['seconds_since_first'], df_filtered['time_diff_prev_sample'], marker='o', linestyle='--', markersize=4)
plt.title('Diferencia en segundos desde el último paquete recibido')
plt.xlabel('Tiempo (s)')
plt.ylabel('Diferencia(s)')
plt.grid(True)
# Calculate and plot the mean line
mean_time_diff = df_filtered['time_diff_prev_sample'].mean()
plt.axhline(y=mean_time_diff, color='red', linestyle='--', label=f'Media de diferencia: {mean_time_diff:.2f} s')
plt.legend()



plt.show()