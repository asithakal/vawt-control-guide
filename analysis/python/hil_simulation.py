"""
Hardware-in-the-Loop (HIL) Simulation for VAWT MPPT Testing
Simulates turbine plant model with real controller firmware via serial
Author: Dr. Asitha Kulasekera
"""

import numpy as np
import matplotlib.pyplot as plt
import serial
import time

class VAWTPlant:
    """Simplified 1-DOF VAWT dynamics with static Cp(lambda) curve"""
    
    def __init__(self):
        self.J = 2.5  # kg·m² (rotor inertia)
        self.B = 0.5  # N·m·s (damping)
        self.R = 0.6  # m (rotor radius)
        self.H = 1.5  # m (height)
        self.A = 1.8  # m² (swept area)
        self.rho = 1.15  # kg/m³
        
        # Cp-lambda lookup table (from CFD or experiment)
        self.lambda_table = np.array([0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5])
        self.cp_table = np.array([0.05, 0.15, 0.28, 0.35, 0.32, 0.25, 0.18])
        
        self.omega = 0  # rad/s
        self.dt = 0.1  # s (100 ms timestep)
    
    def step(self, wind_speed, torque_elec):
        """Simulate one timestep"""
        lambda_tsr = (self.omega * self.R) / max(wind_speed, 0.5)
        cp = np.interp(lambda_tsr, self.lambda_table, self.cp_table)
        
        P_wind = 0.5 * self.rho * self.A * wind_speed**3
        P_aero = cp * P_wind
        tau_aero = P_aero / max(self.omega, 0.1)
        
        # Dynamics: J·dω/dt = τ_aero - τ_elec - B·ω
        domega_dt = (tau_aero - torque_elec - self.B * self.omega) / self.J
        self.omega += domega_dt * self.dt
        self.omega = max(0, self.omega)  # No negative rotation
        
        rpm = self.omega * 60 / (2 * np.pi)
        power_elec = torque_elec * self.omega
        
        return rpm, power_elec, cp, lambda_tsr

def run_hil_test(wind_profile, duration=60):
    """
    Run HIL test with real ESP32 via serial
    
    Args:
        wind_profile: function that returns wind speed at time t
        duration: test duration in seconds
    """
    
    # Connect to ESP32 (adjust port)
    ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.1)
    time.sleep(2)  # Wait for ESP32 reset
    
    plant = VAWTPlant()
    
    # Data logging
    data = {
        't': [],
        'wind': [],
        'rpm': [],
        'duty': [],
        'power': [],
        'cp': [],
        'lambda': []
    }
    
    t = 0
    while t < duration:
        # Get wind speed from profile
        v_wind = wind_profile(t)
        
        # Read duty cycle from ESP32 (controller output)
        line = ser.readline().decode('utf-8').strip()
        try:
            duty = float(line.split(',')[0])  # Assume "duty,rpm,power" format
        except:
            duty = 0.3  # Default if parse fails
        
        # Convert duty cycle to electrical torque
        # Simplified model: τ_elec = k_torque × duty × ω
        k_torque = 10.0  # N·m per unit duty at 1 rad/s
        torque_elec = k_torque * duty * plant.omega
        
        # Simulate plant
        rpm, power_elec, cp, lambda_tsr = plant.step(v_wind, torque_elec)
        
        # Send sensor feedback to ESP32
        feedback = f"{v_wind:.2f},{rpm:.1f},{power_elec:.1f}\n"
        ser.write(feedback.encode('utf-8'))
        
        # Log data
        data['t'].append(t)
        data['wind'].append(v_wind)
        data['rpm'].append(rpm)
        data['duty'].append(duty)
        data['power'].append(power_elec)
        data['cp'].append(cp)
        data['lambda'].append(lambda_tsr)
        
        print(f"t={t:.1f}s | v={v_wind:.1f} m/s | λ={lambda_tsr:.2f} | Cp={cp:.3f} | P={power_elec:.0f} W")
        
        time.sleep(plant.dt)
        t += plant.dt
    
    ser.close()
    
    # Plot results
    plot_hil_results(data)
    
    return data

def plot_hil_results(data):
    """Generate validation plots"""
    fig, axes = plt.subplots(3, 2, figsize=(12, 10))
    
    axes[0,0].plot(data['t'], data['wind'], 'b-')
    axes[0,0].set_ylabel('Wind Speed (m/s)')
    axes[0,0].grid(True)
    
    axes[0,1].plot(data['t'], data['rpm'], 'r-')
    axes[0,1].set_ylabel('Rotor RPM')
    axes[0,1].grid(True)
    
    axes[1,0].plot(data['t'], data['duty'], 'g-')
    axes[1,0].set_ylabel('Duty Cycle')
    axes[1,0].set_ylim([0, 1])
    axes[1,0].grid(True)
    
    axes[1,1].plot(data['t'], data['power'], 'm-')
    axes[1,1].set_ylabel('Power (W)')
    axes[1,1].grid(True)
    
    axes[2,0].plot(data['lambda'], data['cp'], 'o', markersize=3)
    axes[2,0].set_xlabel('Tip Speed Ratio λ')
    axes[2,0].set_ylabel('Power Coefficient Cp')
    axes[2,0].grid(True)
    
    axes[2,1].plot(data['t'], data['lambda'], 'c-', label='λ')
    axes[2,1].axhline(y=2.0, color='k', linestyle='--', label='λ_opt')
    axes[2,1].set_xlabel('Time (s)')
    axes[2,1].set_ylabel('λ')
    axes[2,1].legend()
    axes[2,1].grid(True)
    
    plt.tight_layout()
    plt.savefig('hil_test_results.png', dpi=300)
    plt.show()

if __name__ == '__main__':
    # Test scenario: Step wind gust
    def wind_step_gust(t):
        if t < 20:
            return 6.0
        elif t < 40:
            return 9.0  # Gust
        else:
            return 7.0
    
    print("Starting HIL test with step-gust wind profile...")
    data = run_hil_test(wind_step_gust, duration=60)
    
    # Calculate MPPT efficiency
    avg_cp = np.mean([cp for cp, lam in zip(data['cp'], data['lambda']) if 1.8 < lam < 2.2])
    mppt_eff = avg_cp / 0.35  # 0.35 = Cp_max
    print(f"\nMPPT Efficiency: {mppt_eff*100:.1f}%")
    print(f"Average Cp near λ_opt: {avg_cp:.3f}")