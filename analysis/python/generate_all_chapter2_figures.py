"""
Master script to generate all Chapter 2 figures in one run
Executes all plotting scripts sequentially

Author: Dr. Asitha Kulasekera
"""

import sys
from pathlib import Path

# Add current directory to path for imports
sys.path.insert(0, str(Path(__file__).parent))

# Import plotting functions
from plot_cp_lambda_helical import plot_cp_lambda_curve, export_data_csv
from plot_state_machine import plot_state_machine_matplotlib
from plot_sensor_comparison import plot_sensor_comparison
from plot_control_hierarchy import plot_control_hierarchy

def main():
    print("="*70)
    print(" CHAPTER 2 FIGURE GENERATION")
    print(" Foundational Concepts for VAWT Control Systems")
    print("="*70)
    print()
    
    figures_generated = []
    
    # Figure 1: Cp-lambda curve
    print("[1/4] Generating Cp-lambda curve...")
    try:
        plot_cp_lambda_curve(save=True, show=False)
        export_data_csv()
        figures_generated.append("✓ cp-lambda-helical.png")
    except Exception as e:
        print(f"  ERROR: {e}")
        figures_generated.append("✗ cp-lambda-helical.png FAILED")
    
    print()
    
    # Figure 2: State machine
    print("[2/4] Generating state machine diagram...")
    try:
        plot_state_machine_matplotlib(save=True, show=False)
        figures_generated.append("✓ state-machine-diagram.png")
    except Exception as e:
        print(f"  ERROR: {e}")
        figures_generated.append("✗ state-machine-diagram.png FAILED")
    
    print()
    
    # Figure 3: Sensor comparison
    print("[3/4] Generating sensor comparison chart...")
    try:
        plot_sensor_comparison(save=True, show=False)
        figures_generated.append("✓ sensor-comparison-chart.png")
    except Exception as e:
        print(f"  ERROR: {e}")
        figures_generated.append("✗ sensor-comparison-chart.png FAILED")
    
    print()
    
    # Figure 4: Control hierarchy
    print("[4/4] Generating control hierarchy diagram...")
    try:
        plot_control_hierarchy(save=True, show=False)
        figures_generated.append("✓ control-hierarchy-diagram.png")
    except Exception as e:
        print(f"  ERROR: {e}")
        figures_generated.append("✗ control-hierarchy-diagram.png FAILED")
    
    print()
    print("="*70)
    print(" GENERATION COMPLETE")
    print("="*70)
    print("\nFigures generated:")
    for fig in figures_generated:
        print(f"  {fig}")
    
    print("\nOutput directory: docs/figures/")
    print("\nNext steps:")
    print("  1. Verify all 4 PNG files in docs/figures/")
    print("  2. Check datasets/validation/cp-curve-jan2026.csv")
    print("  3. Update Chapter 2 Quarto document with figure references")
    print("  4. Render book: quarto render")

if __name__ == "__main__":
    main()
