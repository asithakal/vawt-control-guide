# Contributing to VAWT Control Guide

Thank you for your interest in contributing! This repository supports student researchers worldwide.

## How to Contribute

### 1. Code Contributions

**Areas we welcome:**

- Additional MPPT algorithms (fuzzy logic, MPC, neural networks)
- Hardware designs for different turbine scales
- Regional climate adaptations
- Bug fixes and optimizations

**Process:**

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature-name`
3. Make changes and test thoroughly
4. Commit with clear messages: `git commit -m "Add fuzzy MPPT controller"`
5. Push to your fork: `git push origin feature/your-feature-name`
6. Open a Pull Request with description

### 2. Documentation Improvements

- Fix typos or unclear explanations
- Add regional deployment guides (Middle East, Africa, etc.)
- Translate templates (Sinhala, Tamil, other languages)
- Add photos/videos of hardware builds

### 3. Datasets

**Sharing your field data:**

- Use the template in `datasets/example-runs/README.md`
- Include complete metadata (location, turbine specs, weather)
- Ensure FAIR principles compliance
- Submit via Pull Request or email for Zenodo inclusion

### 4. Bug Reports

**Use GitHub Issues:**

- Clear title: "ESP32 MPPT oscillation at low wind speeds"
- Steps to reproduce
- Expected vs actual behavior
- Hardware/software versions
- Serial monitor output or logs

### 5. Testing and Validation

**High-priority test cases:**

- [ ] HIL simulation with different wind profiles
- [ ] Long-term stability (>1 month continuous)
- [ ] Extreme weather survival (monsoon, lightning)
- [ ] Different microcontroller platforms (STM32, Arduino)

## Code Standards

### Firmware (C++)

```cpp
// Use descriptive names, comment complex logic
float calculateAdaptiveStep(float windSpeed) {
    // Calculate turbulence-adaptive MPPT step size
    // Reduces oscillation in gusty conditions
    float sigma = calculateWindStdDev();
    return BASE_STEP / (1.0 + K_TURB * sigma);
}
```

**Style:**

- 4-space indentation
- Opening braces on same line
- Clear variable names (no `x`, `tmp`, `foo`)
- Constants in UPPER_CASE

### Python

```python
# Follow PEP 8, use type hints
def calculate_power_coefficient(power_w: float, wind_speed_ms: float, 
                                swept_area_m2: float) -> float:
    """Calculate turbine power coefficient (Cp).
    
    Args:
        power_w: Electrical power output (W)
        wind_speed_ms: Wind speed (m/s)
        swept_area_m2: Rotor swept area (m²)
    
    Returns:
        Dimensionless Cp value (0-0.593)
    """
    rho = 1.15  # kg/m³, tropical sea level
    wind_power = 0.5 * rho * swept_area_m2 * wind_speed_ms**3
    return power_w / wind_power if wind_power > 0 else 0
```

### Documentation

- Markdown for all docs
- Use relative links: `[Hardware Guide](../hardware/schematics/README.md)`
- Include units in tables: "Wind Speed (m/s)"
- Add citations where appropriate

## Git Workflow

### Branch Naming

- `feature/adaptive-mppt` - New features
- `bugfix/rpm-overflow` - Bug fixes
- `docs/sri-lanka-suppliers` - Documentation
- `hardware/v2-pcb` - Hardware revisions

### Commit Messages

```markdown
Short summary (50 chars max)
- Detailed point 1
- Detailed point 2
- Reference to issue: Fixes #42
Tested on: ESP32-DevKitC, STM32F4 Nucleo
```

### Pull Request Template

```markdown
## Description
Brief explanation of changes

## Motivation
Why is this needed?

## Testing
- [ ] Compiled successfully for ESP32
- [ ] Bench tested with variable load
- [ ] HIL simulation passes
- [ ] Documentation updated

## Screenshots/Data
(If applicable)

## Checklist
- [ ] Code follows style guidelines
- [ ] Self-review completed
- [ ] Comments added for complex logic
- [ ] No compiler warnings
- [ ] Tests pass
```

## Regional Adaptations

**We especially welcome contributions for:**

- **Middle East**: Sandstorm protection, high temperature (>50°C) testing
- **Sub-Saharan Africa**: Low-cost (<$300) implementations, solar hybrid
- **Southeast Asia**: Typhoon-resistant designs, monsoon strategies
- **Latin America**: High-altitude adaptations (thin air density corrections)

## Translation Guidelines

**Priority languages:**

- Sinhala (සිංහල) - Sri Lankan students
- Tamil (தமிழ்) - Sri Lankan/Indian students
- Spanish - Latin American regions
- Arabic - Middle Eastern deployments

**What to translate:**

1. README.md (keep technical terms in English: "MPPT", "Cp", "λ")
2. Hardware assembly guides
3. Quick-start tutorials
4. Safety warnings (critical!)

**Tools:** We recommend [Weblate](https://weblate.org/) for collaborative translation.

## Code of Conduct

### Our Pledge

We are committed to providing a welcoming, inclusive environment for all contributors regardless of background, identity, or experience level.

### Expected Behavior

- Be respectful and constructive
- Focus on technical merit
- Acknowledge contributions
- Help newcomers

### Unacceptable Behavior

- Harassment, discrimination, or personal attacks
- Spam or off-topic content
- Plagiarism or uncredited work

**Enforcement:** Violations should be reported to [your email]. Serious violations may result in ban from repository.

## Recognition

Contributors will be acknowledged in:

- `README.md` Acknowledgments section
- Publication author lists (for major contributions)
- Zenodo dataset citations

## Questions?

- **GitHub Discussions**: For general questions
- **Issues**: For bugs or feature requests
- **Email**: [asitha@uom.lk] for private inquiries

## License

By contributing, you agree that your contributions will be licensed under:

- **Code**: MIT License
- **Documentation**: CC BY 4.0

---

Thank you for helping make wind energy research more accessible!
