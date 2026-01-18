# Example VAWT Control System Datasets

## Dataset: `monsoon-2026-january.csv`

**Description:** 10-minute excerpt from a 6-month field campaign at Galle coastal site during NE monsoon transition period (January 2026).

**Turbine:** 500 W Helical Gorlov VAWT  
**Location:** 6.0535°N, 80.2210°E, 5m elevation  
**Period:** 2026-01-15 14:20:00 to 14:30:00 (UTC+5:30)  
**Weather:** Partly cloudy, gusty winds 6-10 m/s

### Data Dictionary

| Column | Unit | Description | Sensor/Method |
|--------|------|-------------|---------------|
| `timestamp` | ISO 8601 | UTC+05:30 timestamp | NTP-synchronized ESP32 RTC |
| `state` | enum | Control state (IDLE, MPPT, POWER_REGULATION, STALL, FAULT) | State machine |
| `wind_speed_ms` | m/s | Wind speed at hub height (1.5m) | Cup anemometer (Inspeed Vortex) |
| `rotor_rpm` | RPM | Rotor angular velocity | Hall effect sensor, interrupt-counted |
| `duty_cycle` | 0-1 | DC-DC converter duty cycle | PWM setpoint |
| `voltage_dc` | V | DC bus voltage | INA226 I2C sensor |
| `current_dc` | A | DC bus current | INA226 I2C sensor |
| `power_w` | W | Instantaneous electrical power | V × I |
| `cp` | - | Power coefficient (dimensionless) | 0.5 × ρ × A × v³ |
| `lambda` | - | Tip-speed ratio (dimensionless) | (ω × R) / v |
| `temp_c` | °C | Ambient temperature | BME280 |
| `humidity_pct` | % | Relative humidity | BME280 |
| `pressure_hpa` | hPa | Atmospheric pressure (for ρ correction) | BME280 |

### Quality Flags

- **Good data:** All sensors within range, state = MPPT or POWER_REGULATION
- **Flagged:** Wind speed < 2 m/s (below cut-in, Cp/λ not meaningful)
- **Missing:** No missing data in this excerpt

### Citation

```bibtex
@dataset{kulasekera2026monsoon,
  author = {Kulasekera, Asitha},
  title = {Small VAWT Control Dataset - Monsoon Transition 2026},
  year = {2026},
  publisher = {Zenodo},
  doi = {10.5281/zenodo.XXXXXXX}
}
```

### License

CC BY 4.0 - Free to use with attribution

### Contact

Questions? Open an issue on [GitHub](https://github.com/asithakal/vawt-control-guide/issues)
