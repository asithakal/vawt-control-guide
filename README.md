# Small-Scale VAWT Control Systems: A Practical Guide for Student Researchers

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.18291588.svg)](https://doi.org/10.5281/zenodo.18291588)
[![License: CC BY 4.0](https://img.shields.io/badge/License-CC%20BY%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by/4.0/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-Ready-blue.svg)](https://platformio.org/)
[![Build Status](https://github.com/asithakal/vawt-control-guide/workflows/Firmware%20CI/badge.svg)](https://github.com/asithakal/vawt-control-guide/actions)

---

## 📢 Branch Strategy Notice

**🚧 Important: This repository uses a dual-branch approach**

This repository accompanies the guidebook [**"Small-Scale VAWT Control Systems: A Practical Guide"**](https://github.com/asithakal/vawt-control-book) and maintains two branches:

### 🔵 `main` Branch (This Branch) - **Full Implementation Archive**

- 📦 **Status**: Frozen / Reference Implementation
- 🎯 **Purpose**: Complete scaffold with all planned implementations
- ⚠️ **Warning**: Contains code from **all chapters** (Ch 1-11) including features not yet covered in the book
- 📝 **Use Case**: Reference for developers planning ahead or contributing advanced features
- 📚 **Corresponds To**: Full book completion (target Q2 2026)

### 🟢 `book-aligned` Branch - **Incremental Development** ⭐ **Recommended for Students**

- 📦 **Status**: Active Development
- 🎯 **Purpose**: Code that **matches completed book chapters only**
- ✅ **Current**: Chapter 1 complete (Introduction only)
- 📝 **Use Case**: Students learning step-by-step alongside the book
- 🔗 **Switch to this branch**: 
  ```bash
  git checkout book-aligned
  ```
  Or browse: [book-aligned branch](https://github.com/asithakal/vawt-control-guide/tree/book-aligned)

### Which Branch Should I Use?

| If you want... | Use Branch | Why |
|----------------|------------|-----|
| To **learn incrementally** with the book | `book-aligned` | Only contains tested code for completed chapters |
| To **see the full architecture** before it's finalized | `main` | Complete reference implementation (may be untested) |
| To **contribute** implementations for future chapters | `main` | Submit PRs against full scaffold |
| To **build the system** described in completed chapters | `book-aligned` | Guaranteed alignment with book content |

---

## 🎯 Project Overview

This repository accompanies the guidebook **"Small-Scale VAWT Control Systems: A Practical Guide for Student Researchers"** and provides:

- **Production-ready firmware** for ESP32/STM32 controllers implementing hierarchical state machines and HCS/P&O MPPT
- **Hardware schematics** for DC-DC converters, dump loads, and grid-tie inverter interfaces
- **Example datasets** from Sri Lankan coastal/monsoon deployments
- **Analysis scripts** for Hardware-in-the-Loop (HIL) testing and field validation
- **Templates** for requirements engineering, IEC 61400-2 compliance, and safety documentation

### Companion to DAQ Guide

This control systems guide builds upon the foundational data acquisition architecture in:  
📘 [Wind Turbine Data Acquisition Systems](https://github.com/asithakal/wind-turbine-daq-guide)  
🔗 DOI: [10.5281/zenodo.18093662](https://doi.org/10.5281/zenodo.18093662)

## 📚 Table of Contents

1. **Introduction & Motivation** – Why control systems matter for small VAWTs
2. **Control Objectives by Operating Region** – MPPT, power regulation, safety
3. **Algorithms** – Hill-Climb Search (HCS), Perturb & Observe (P&O), soft-stall
4. **Hardware Architecture** – PMSG, rectifiers, DC-DC converters, dump loads, inverters
5. **Firmware Design** – Hierarchical FSM, ISRs, MPPT loop, protection logic
6. **Case Study: 500 W Helical VAWT** – Gorlov rotor, λ_opt=2.0, Cp_max=0.35
7. **Testing & Validation** – HIL, bench tests, field deployment (monsoon considerations)
8. **IEC 61400-2 & UL 6142 Compliance** – Safety-related control systems (SRCS)
9. **Maintenance, Cybersecurity, & Sustainability**
10. **Appendices** – Derivations, glossary, standards checklists

## 🚀 Quick Start

### Prerequisites

- **Hardware**: ESP32-DevKitC or STM32F4 Nucleo board
- **Software**: [PlatformIO](https://platformio.org/) (VS Code extension) or Arduino IDE
- **Optional**: KiCad 7.0+ for hardware design

### Installation

```bash
# Clone this repository
git clone https://github.com/asithakal/vawt-control-guide.git
cd vawt-control-guide

# For incremental learning (recommended for students):
git checkout book-aligned

# For full reference implementation:
git checkout main

cd firmware\platformio

# Open in PlatformIO
pio run -t upload  # Upload to ESP32

# Or open in Arduino IDE
# File > Open > firmware/arduino/VAWT_Control_Main/VAWT_Control_Main.ino
```

### Hardware Setup

See [`hardware/schematics/README.md`](hardware/schematics/README.md) for wiring diagrams. Minimum system:

- **Sensors**: Wind speed (anemometer), rotor RPM (Hall effect), DC bus voltage/current (INA226)
- **Actuators**: PWM-controlled buck/boost converter, dump load MOSFET, mechanical brake relay
- **Protection**: Overvoltage crowbar, fuses, TVS diodes

**Budget options for Sri Lankan students:**

- Standard: ~250,000 LKR (~$750 USD)
- Constrained: ~150,000 LKR (~$450 USD)

See [`docs/case-studies/budget-comparison.md`](docs/case-studies/budget-comparison.md)

## 🏭️ Repository Structure

```
vawt-control-guide/
├── firmware/               # Embedded control firmware
│   ├── platformio/        # PlatformIO project (ESP32/STM32)
│   │   ├── src/          # Main control loop, state machine
│   │   ├── lib/          # Modular libraries (MPPT, Safety, Logging)
│   │   └── platformio.ini
│   └── arduino/          # Arduino IDE compatible version
├── hardware/             # Electrical design files
│   ├── schematics/       # KiCad schematics (DC-DC, dump load, inverter)
│   ├── fritzing/         # Beginner-friendly Fritzing diagrams
│   └── bom/             # Bills of materials (LKR & USD pricing)
├── docs/                # Documentation and templates
│   ├── templates/       # Requirements, test plans, IEC tables
│   ├── figures/         # Cp-λ plots, state machine diagrams
│   └── case-studies/    # 500W Helical VAWT detailed example
├── datasets/            # Example measurement data
│   ├── example-runs/   # CSV files with timestamp, state, duty, Cp, λ
│   └── README.md       # Metadata and data dictionary
├── analysis/           # Post-processing and HIL simulation
│   ├── python/        # Matplotlib scripts, pandas analysis
│   ├── matlab/        # Simulink HIL models
│   └── jupyter/       # Interactive notebooks
├── appendices/        # Supplementary materials (GitHub Pages PDFs)
│   ├── version-control-guide.pdf
│   ├── mppt-derivations.pdf
│   └── glossary.pdf
└── .github/workflows/ # CI/CD for firmware compilation

```

## 🔬 Case Study Turbine Specifications

The guidebook centers on a realistic small-scale VAWT:

| Parameter | Value | Notes |
|-----------|-------|-------|
| **Type** | Helical Gorlov (Darrieus) | 3 blades, NACA0018, 120° helical twist |
| **Rated Power** | 500 W @ 8-9 m/s | Cut-in: 3 m/s, Cut-out: 15 m/s |
| **Rotor Diameter** | 1.2 m | Swept area A = 2RH = 1.8 m² |
| **Rotor Height** | 1.5 m | |
| **Optimal λ** | 2.0 | Cp_max = 0.35 at λ = 2.0 |
| **Generator** | 3φ PMSG, 48 V nominal | Rectified to DC, battery-coupled |
| **Control Platform** | ESP32 (primary) or STM32F4 | Shared with DAQ system |
| **Deployment** | Coastal Sri Lanka | Monsoon, high humidity, salt spray |

See full characterization in [`docs/case-studies/helical-vawt-500W.md`](docs/case-studies/helical-vawt-500W.md)

## 🧠 Control Architecture

### Hierarchical State Machine

```
INIT → STANDBY → START-UP → MPPT → POWER_REGULATION → SHUTDOWN
                              ↓            ↓
                           FAULT    HIGH_WIND_BRAKE
```

**Key States:**

- **IDLE**: System check, sensor validation
- **MPPT**: Hill-Climb Search with adaptive Δduty based on wind turbulence (σ_v)
- **POWER_REGULATION**: Soft-stall via torque scheduling when P > P_rated
- **STALL/BRAKE**: Dump load + mechanical brake on overspeed (RPM > 1.4 × RPM_rated)

See state transition logic in [`firmware/platformio/lib/StateMachine/TurbineStateMachine.cpp`](firmware/platformio/lib/StateMachine/)

### MPPT Algorithm

Implements **turbulence-adaptive Hill-Climb Search**:

```cpp
// Adaptive step size based on wind variability
float Δduty = Δduty_base × (1 + k_turb × σ_v);

if (P_new > P_prev) {
    duty += Δduty;  // Keep direction
} else {
    duty -= Δduty;  // Reverse search
}
```

**Advantages over fixed-step P&O:**

- Faster convergence in steady winds (large steps)
- Reduced oscillation in gusty conditions (small steps)
- No wind speed sensor required

See full derivation in [`appendices/mppt-derivations.pdf`](appendices/mppt-derivations.pdf)

## 🔌 Hardware Compatibility

**Supported Microcontrollers:**

- ESP32 (recommended): Built-in WiFi, dual-core, 12-bit ADC
- STM32F4 Nucleo: Higher-precision ADC (16-bit external), real-time capable
- Arduino Mega 2560: Budget option (limited to basic MPPT, no advanced features)

**Power Electronics:**

- **DC-DC Converter**: Buck/boost topology, 500 W continuous, 48 V nominal
- **Dump Load**: 1 kW resistive bank, PWM-controlled MOSFET (IRFP260N or equivalent)
- **Inverter (optional)**: 230 V grid-tie, SMA Sunny Boy or DIY H-bridge

See Bill of Materials: [`hardware/bom/BOM-Standard-Budget.xlsx`](hardware/bom/)

**Sri Lankan Suppliers:**

- ESP32 boards: ~LKR 2,500-3,500 (Colombo electronics markets, Daraz.lk)
- Power MOSFETs: ~LKR 800-1,200 each
- Complete standard-budget system: ~LKR 250,000

## 📊 Example Dataset

Sample data structure (CSV format):

```csv
timestamp,state,wind_speed_ms,rotor_rpm,duty_cycle,power_w,cp,lambda,temp_c,humidity_pct
2026-01-15T14:23:01+0530,MPPT,7.2,135,0.42,285,0.31,1.98,28.5,78
2026-01-15T14:23:02+0530,MPPT,7.5,140,0.44,310,0.33,2.03,28.5,78
2026-01-15T14:23:03+0530,MPPT,7.8,142,0.44,325,0.34,2.01,28.6,77
2026-01-15T14:23:04+0530,STALL,12.1,210,0.15,500,0.28,1.88,28.7,77
```

**Metadata compliant with**:

- FAIR principles (Findable, Accessible, Interoperable, Reusable)
- ISO 8601 timestamps (UTC+5:30 for Sri Lanka)
- Column units in header or separate data dictionary

Access datasets: [`datasets/example-runs/`](datasets/example-runs/)

## 🧪 Testing Workflow

1. **Bench Testing**: Static loads, oscilloscope verification (Week 1-2)
2. **Hardware-in-the-Loop (HIL)**: FreeRTOS simulator, Python-based plant model (Week 3-4)
3. **Field Deployment**: Coastal site, monsoon conditions (Month 2-6)
4. **Validation**: Compare measured Cp-λ with CFD predictions (tolerance ±10%)

HIL setup: [`analysis/python/hil_simulation.py`](analysis/python/)

## 🛡️ Safety & Standards Compliance

This system is designed with **IEC 61400-2** (small wind turbines) and **UL 6142** (electrical safety) principles:

- ✅ Redundant overspeed protection (electrical + mechanical)
- ✅ Fail-safe behavior on control power loss
- ✅ Documented safety-related control system (SRCS)
- ✅ Lightning/surge protection (TVS diodes, MOVs)
- ✅ Environmental ratings: IP65 enclosure, conformal coating

**Not certified**, but architecture supports future certification.

See compliance checklist: [`docs/standards/IEC-61400-2-checklist.md`](docs/standards/)

## 🌧️ Tropical Climate Adaptations

**Monsoon-specific considerations for Sri Lanka:**

- **High humidity (70-95% RH)**: Conformal coating on PCBs, silica gel in enclosures
- **Salt spray (coastal sites)**: Stainless steel 316 hardware, IP65+ enclosures
- **Lightning (SW monsoon season, May-Sep)**: Grounding, surge arrestors, fiber-optic isolation
- **Extreme gusts (up to 30 m/s transients)**: Adaptive MPPT step size, conservative overspeed thresholds

Detailed guidelines: [`docs/case-studies/monsoon-deployment-guide.md`](docs/case-studies/)

## 📖 How to Cite

If you use this repository in your research, please cite:

```bibtex
@software{kulasekera2026vawt_control,
  author = {Kulasekera, Asitha},
  title = {Small-Scale VAWT Control Systems: A Practical Guide for Student Researchers},
  year = {2026},
  publisher = {GitHub},
  journal = {GitHub repository},
  howpublished = {\url{https://github.com/asithakal/vawt-control-guide}},
  doi = {10.5281/zenodo.18291588}
}
```

Also cite the companion DAQ guide:

```bibtex
@techreport{kulasekera2025daq,
  author = {Kulasekera, Asitha},
  title = {Wind Turbine Data Acquisition Systems: A Practical Guide for Student Researchers},
  year = {2025},
  institution = {University of Moratuwa},
  doi = {10.5281/zenodo.18093662}
}
```

## 🤝 Contributing

We welcome contributions! Please see [`CONTRIBUTING.md`](CONTRIBUTING.md) for guidelines.

**Priority areas:**

- Additional MPPT algorithms (fuzzy logic, MPC, neural network)
- Advanced pitch control examples
- Regional adaptations (Middle East, Africa, Latin America)
- Translation (Sinhala, Tamil)

## 📜 License

This work is licensed under a [Creative Commons Attribution 4.0 International License](https://creativecommons.org/licenses/by/4.0/).

**Code components** (firmware, scripts) are additionally available under MIT License for maximum reusability.

## 🙏 Acknowledgments

- **University of Moratuwa** – Department of Mechanical Engineering
- **Lund University** – VAWT experimental platform inspiration
- **Sabanci University** – MPPT algorithm validation
- **Sri Lankan student researchers** – Field testing and feedback

## 📧 Contact

**Dr. Asitha Kulasekera**  
University of Moratuwa, Sri Lanka  
📧 asitha@uom.lk  
🌐 [GitHub](https://github.com/asithakal)

---

**Last Updated**: February 6, 2026  
**Status**: 🔵 Reference Implementation (Frozen) | 🟢 Active Development in `book-aligned` branch
