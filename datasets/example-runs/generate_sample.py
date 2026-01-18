"""Generate synthetic 10-minute VAWT dataset for documentation"""

import pandas as pd
import numpy as np
from datetime import datetime, timedelta

# Turbine parameters
ROTOR_RADIUS = 0.6  # m
ROTOR_HEIGHT = 1.5  # m
SWEPT_AREA = 1.8    # m²
LAMBDA_OPT = 2.0
CP_MAX = 0.35
RHO = 1.15          # kg/m³

# Simulation parameters
duration_seconds = 600  # 10 minutes
sample_rate = 1         # Hz

# Generate time series
start_time = datetime(2026, 1, 15, 14, 20, 0)
timestamps = [start_time + timedelta(seconds=i) for i in range(duration_seconds)]

# Generate realistic wind speed (with gusts)
base_wind = 7.0
wind_noise = np.random.normal(0, 0.5, duration_seconds)
gust = np.zeros(duration_seconds)
gust[300:350] = 2.0 * np.sin(np.linspace(0, np.pi, 50))  # Gust at 5 min
wind_speed = base_wind + wind_noise + gust
wind_speed = np.clip(wind_speed, 3.0, 15.0)

# Calculate derived quantities
data = []
for i, (ts, v_wind) in enumerate(zip(timestamps, wind_speed)):
    # MPPT controller tracks optimal lambda
    lambda_actual = LAMBDA_OPT + np.random.normal(0, 0.1)
    omega = (lambda_actual * v_wind) / ROTOR_RADIUS  # rad/s
    rpm = omega * 60 / (2 * np.pi)
    
    # Cp from lookup (simplified)
    lambda_error = abs(lambda_actual - LAMBDA_OPT)
    cp = CP_MAX * np.exp(-lambda_error**2 / 0.1)
    
    # Power calculation
    wind_power = 0.5 * RHO * SWEPT_AREA * v_wind**3
    power_elec = cp * wind_power
    
    # Voltage and current (simplified)
    voltage = 48.5 + np.random.normal(0, 0.5)
    current = power_elec / voltage
    
    # Duty cycle (MPPT output)
    duty_cycle = 0.4 + 0.1 * (lambda_actual - LAMBDA_OPT)
    duty_cycle = np.clip(duty_cycle, 0.1, 0.9)
    
    # State machine logic
    if v_wind < 3.0:
        state = "STANDBY"
    elif v_wind > 12.0:
        state = "STALL"
    elif power_elec > 475:  # Near rated
        state = "POWER_REGULATION"
    else:
        state = "MPPT"
    
    # Environmental
    temp = 28.5 + np.random.normal(0, 0.2)
    humidity = 77 + np.random.randint(-2, 3)
    pressure = 1012
    
    data.append({
        'timestamp': ts.strftime('%Y-%m-%dT%H:%M:%S+0530'),
        'state': state,
        'wind_speed_ms': round(v_wind, 1),
        'rotor_rpm': round(rpm, 0),
        'duty_cycle': round(duty_cycle, 2),
        'voltage_dc': round(voltage, 1),
        'current_dc': round(current, 2),
        'power_w': round(power_elec, 1),
        'cp': round(cp, 3),
        'lambda': round(lambda_actual, 2),
        'temp_c': round(temp, 1),
        'humidity_pct': humidity,
        'pressure_hpa': pressure
    })

# Create DataFrame and save
df = pd.DataFrame(data)
output_path = 'sample-10min.csv'
df.to_csv(output_path, index=False)
print(f"Generated {len(df)} records")
print(f"Saved to {output_path}")
print(f"\nSample statistics:")
print(f"  Avg Cp: {df['cp'].mean():.3f}")
print(f"  Avg λ: {df['lambda'].mean():.2f}")
print(f"  Avg Power: {df['power_w'].mean():.1f} W")
"""Generate synthetic 10-minute VAWT dataset for documentation"""
