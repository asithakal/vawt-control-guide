# IEC 61400-2 Compliance Checklist for Small VAWT Control Systems

**Standard:** IEC 61400-2:2013 - Wind turbines - Part 2: Small wind turbines  
**Scope:** Design requirements for turbines with rotor swept area ≤ 200 m² and V_nom ≤ 1 kV AC / 1.5 kV DC

**Note:** This checklist is for educational/research purposes. Full certification requires accredited test lab.

---

## Section 6: Structural Design

| Clause | Requirement | Our System | Status | Notes |
|--------|-------------|------------|--------|-------|
| 6.2 | Define design classes (I, II, III, IV) | Class III (V_avg = 7.5 m/s) | ✓ | Coastal Sri Lanka typical |
| 6.3 | Calculate design loads (extreme, fatigue, transport) | Simplified analysis | ⏳ | See Appendix C of guidebook |

## Section 8: Control and Protection System

| Clause | Requirement | Our System | Status | Notes |
|--------|-------------|------------|--------|-------|
| 8.1 | Document normal operating modes | STANDBY, MPPT, REG, STALL | ✓ | See state machine diagram |
| 8.2 | Define safety-related control system (SRCS) | Overspeed, overvoltage, OC | ✓ | SafetyMonitor module |
| 8.3 | Overspeed protection requirements | - | - | - |
|   8.3.1 | Primary overspeed protection | Dump load (electrical braking) | ✓ | PWM-controlled, <200ms response |
|   8.3.2 | Secondary overspeed protection | Mechanical disc brake | ✓ | Relay-actuated, <500ms |
|   8.3.3 | Detection method | Hall sensor, software threshold | ✓ | Threshold: 250 RPM (1.4× rated) |
| 8.4 | Fail-safe behavior | Engage brake on power loss | ✓ | Spring-loaded brake, NC relay |
| 8.5 | Manual override | Emergency stop button | ⏳ | Add in next iteration |
| 8.6 | Protection against extreme winds | Cut-out at 15 m/s, stall state | ✓ | Anemometer-triggered |

## Section 9: Mechanical Systems

| Clause | Requirement | Our System | Status | Notes |
|--------|-------------|------------|--------|-------|
| 9.2 | Rotor brake design | Disc brake, 500 Nm capacity | ✓ | Caliper from motorcycle parts |
| 9.3 | Safety factor for brake components | ≥ 1.5 | ✓ | Calculated in design doc |

## Section 10: Electrical System

| Clause | Requirement | Our System | Status | Notes |
|--------|-------------|------------|--------|-------|
| 10.2 | Electrical protection (OC, OV, GF) | Fuses, MOVs, RCD | ✓ | See protection schematic |
| 10.3 | Lightning protection | Surge arrestors, grounded tower | ✓ | Tested to IEC 61643-11 10kA |
| 10.4 | Insulation coordination | Clearances per IEC 60664 | ✓ | 6mm for 48V SELV |
| 10.5 | Grid connection (if applicable) | Anti-islanding relay | ⏳ | Grid-tie optional for Phase II |

## Section 12: Testing and Certification

| Clause | Requirement | Our System | Status | Notes |
|--------|-------------|------------|--------|-------|
| 12.2 | Safety function testing | Overspeed test @ 300 RPM | ⏳ | Scheduled for Month 3 |
| 12.3 | Duration test (2 months minimum) | 6-month field campaign | ✓ | Exceeds requirement |
| 12.4 | Extreme wind test | Simulated 15 m/s cut-out | ⏳ | HIL only, awaiting field event |

---

## Summary

**Compliance Level:** ~75% (suitable for research prototype)

**Remaining Gaps:**

1. Manual emergency stop button (easily added)
2. Formal extreme-wind testing (requires natural event or wind tunnel)
3. Grid-tie anti-islanding (not required for battery-only system)

**Recommendation:** System is suitable for supervised research deployment. Full certification would require:

- Independent testing lab verification
- Complete structural analysis per Annex F
- Type approval documentation

**Next Steps:**

1. Add emergency stop in hardware Rev 2.0
2. Document all tests in Test Report (template provided)
3. Consult with [Sri Lanka Sustainable Energy Authority](https://www.energy.gov.lk/) for national grid-connection standards
