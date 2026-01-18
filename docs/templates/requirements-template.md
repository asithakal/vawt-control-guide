# VAWT Control System Requirements Template

## Project Information

- **Project Name**: [Your turbine project name]
- **Student Name**: [Name]
- **Supervisor**: [Name]
- **Institution**: [University]
- **Date**: [YYYY-MM-DD]

## 1. Functional Requirements

| ID | Requirement | Acceptance Criteria | Priority |
|----|-------------|---------------------|----------|
| FR-1 | System shall implement MPPT using HCS algorithm | Cp > 90% of Cp_max in steady winds (σ_v < 1 m/s) | HIGH |
| FR-2 | System shall measure rotor RPM with ±1 RPM accuracy | Validated against tachometer | HIGH |
| FR-3 | System shall log data at 1 Hz to SD card | 99% data completeness over 24h | HIGH |
| FR-4 | System shall engage dump load when P > 1.05×P_rated | Response time < 200 ms | MEDIUM |
| FR-5 | System shall activate mechanical brake if RPM > RPM_overspeed | Response time < 500 ms | HIGH |

## 2. Non-Functional Requirements

| ID | Requirement | Acceptance Criteria | Priority |
|----|-------------|---------------------|----------|
| NFR-1 | System shall operate in 5-45°C ambient | Validated by thermal chamber test | HIGH |
| NFR-2 | Enclosure shall meet IP65 rating | Spray test per IEC 60529 | HIGH |
| NFR-3 | System shall survive 70-95% RH without condensation | 1-month field deployment without failure | HIGH |
| NFR-4 | Total hardware cost shall not exceed LKR 250,000 | Documented BOM with receipts | MEDIUM |

## 3. Safety Requirements (IEC 61400-2 Aligned)

| ID | Requirement | Acceptance Criteria | Priority |
|----|-------------|---------------------|----------|
| SR-1 | System shall detect overspeed (RPM > 1.4×rated) within 100 ms | HIL simulation + field test | HIGH |
| SR-2 | Overspeed protection shall be dual-channel (electrical + mechanical) | Independent hardware watchdog | HIGH |
| SR-3 | System shall fail-safe on control power loss | Manual inspection of brake state | HIGH |
| SR-4 | Lightning protection shall withstand 10 kA surge | Per IEC 61643-11 | MEDIUM |

## 4. Performance Requirements

| Parameter | Target | Measurement Method |
|-----------|--------|-------------------|
| MPPT efficiency | > 95% | Compare P_actual vs P_theoretical at steady wind |
| Response time to gust | < 2 seconds | Step wind input in HIL |
| Data completeness | > 98% | 6-month campaign |
| Uptime | > 95% | Exclude scheduled maintenance |

## 5. Traceability Matrix

| Requirement ID | Design Element | Verification Method | Status |
|----------------|----------------|---------------------|--------|
| FR-1 | `MPPTController.cpp` | Bench test with variable load | ✓ Verified |
| SR-1 | `SafetyMonitor.cpp` + hall sensor | HIL + field test | ⏳ In Progress |
| NFR-2 | IP65 enclosure (Polycase WC-42) | Spray test | ✗ Not Started |

---

**Template Usage:**

1. Copy this template to your project folder
2. Fill in all sections with your specific values
3. Update traceability matrix weekly
4. Include in thesis Appendix A
