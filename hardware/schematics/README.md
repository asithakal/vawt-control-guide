# Hardware Schematics

## Overview

This folder contains electrical schematics for a complete 500 W VAWT control system.

**File Formats:**

- KiCad 7.0 project files (.kicad_sch, .kicad_pcb)
- PDF exports for viewing without KiCad
- Fritzing diagrams (.fzz) for beginners

## Schematics Included

1. **DC-DC-Converter-Buck-Boost.kicad_sch**  
   500 W synchronous buck-boost converter using LM5176 controller IC.  
   Input: 30-60 VDC (PMSG rectified output)  
   Output: 48 VDC regulated (battery bus)

2. **Dump-Load-Circuit.kicad_sch**  
   1 kW resistive dump load with PWM-controlled MOSFET (IRFP260N).  
   Includes heatsink design notes.

3. **Grid-Tie-Inverter-Interface.kicad_sch**  
   Interface circuit for SMA Sunny Boy 240 or equivalent.  
   Isolation relays, anti-islanding protection.

4. **Protection-TVS-MOV.kicad_sch**  
   Transient voltage suppression, metal-oxide varistors (MOVs), fuses.  
   Lightning/surge protection for tropical deployment.

## Bill of Materials

See `../bom/BOM-Standard-Budget.xlsx` for complete parts list with:

- Sri Lankan suppliers (LKR pricing)
- International alternatives (USD pricing)
- Mouser/DigiKey part numbers
- Colombo electronics market equivalents

## Assembly Notes

1. **PCB fabrication**: 2-layer, 2 oz copper, HASL finish  
   Sri Lankan PCB fab: [PCB Power](https://www.pcbpower.lk/) or [Grintek](https://grintek.lk/)

2. **Critical components**:
   - Use genuine Infineon/Vishay MOSFETs (avoid counterfeit)
   - Conformal coating for tropical humidity
   - Thermal paste on all heatsinks

3. **Testing checklist**: See `../../docs/templates/hardware-test-checklist.md`

## Safety Warnings

⚠️ **HIGH VOLTAGE**: Grid-tie sections operate at 230 VAC. Qualified electricians only.  
⚠️ **HIGH CURRENT**: Dump load can dissipate 1 kW. Proper heatsinking mandatory.  
⚠️ **LIGHTNING**: Install surge arrestors. Bond all grounds to turbine tower.
