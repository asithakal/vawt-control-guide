"""
Generate three-layer hierarchical control architecture diagram
Shows timing relationships between outer/middle/inner loops

Author: Dr. Asitha Kulasekera
"""

import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.patches import FancyBboxPatch, Rectangle
from pathlib import Path
import numpy as np

OUTPUT_DIR = Path(__file__).parent.parent.parent / "docs" / "figures"
OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
DPI = 300

def plot_control_hierarchy(save=True, show=False):
    """
    Visual representation of nested control loops with timing
    """
    fig, ax = plt.subplots(figsize=(11, 7))
    ax.set_xlim(0, 11)
    ax.set_ylim(0, 9)
    ax.axis('off')
    
    # Outer loop (1 Hz)
    outer_box = FancyBboxPatch((0.5, 0.5), 10, 8,
                              boxstyle="round,pad=0.2",
                              edgecolor='#1565C0', facecolor='#E3F2FD',
                              linewidth=3, zorder=1)
    ax.add_patch(outer_box)
    ax.text(5.5, 8.3, 'OUTER LOOP (1 Hz): State Machine + Safety',
           fontsize=12, weight='bold', ha='center', color='#1565C0')
    
    # State machine block
    state_box = FancyBboxPatch((1, 6.5), 4, 1.2,
                              boxstyle="round,pad=0.1",
                              edgecolor='black', facecolor='#BBDEFB',
                              linewidth=2, zorder=2)
    ax.add_patch(state_box)
    ax.text(3, 7.3, 'TurbineStateMachine.cpp', fontsize=10, ha='center', weight='bold')
    ax.text(3, 6.85, 'IDLE → STARTUP → MPPT\n→ POWER_REG → STALL → FAULT',
           fontsize=7, ha='center', style='italic')
    
    # Safety monitor block
    safety_box = FancyBboxPatch((6, 6.5), 4, 1.2,
                               boxstyle="round,pad=0.1",
                               edgecolor='black', facecolor='#FFCDD2',
                               linewidth=2, zorder=2)
    ax.add_patch(safety_box)
    ax.text(8, 7.3, 'SafetyMonitor.cpp', fontsize=10, ha='center', weight='bold')
    ax.text(8, 6.85, 'Overspeed (n > 250 RPM)\nOvervoltage (V > 60 V)\nFault Logging',
           fontsize=7, ha='center')
    
    # Middle loop (10 Hz)
    middle_box = FancyBboxPatch((1, 2.5), 9, 3.5,
                               boxstyle="round,pad=0.2",
                               edgecolor='#2E7D32', facecolor='#E8F5E9',
                               linewidth=2.5, zorder=1)
    ax.add_patch(middle_box)
    ax.text(5.5, 5.8, 'MIDDLE LOOP (10 Hz): MPPT Controller',
           fontsize=11, weight='bold', ha='center', color='#2E7D32')
    
    # MPPT inputs/outputs
    ax.text(2, 5.2, 'Inputs:', fontsize=9, weight='bold')
    ax.text(2, 4.85, '• P = V × I (INA226)\n• n (Hall RPM)\n• Optional: vw (anemometer)',
           fontsize=8, va='top')
    
    ax.text(7, 5.2, 'Algorithm:', fontsize=9, weight='bold')
    ax.text(7, 4.85, 'Hill-Climb Search (HCS)\nAdaptive Δd based on σv\nEMA filtering (α=0.3)',
           fontsize=8, va='top')
    
    ax.text(5.5, 3.5, 'Output: d_setpoint (duty cycle 0.1–0.9)',
           fontsize=9, ha='center', weight='bold',
           bbox=dict(boxstyle='round,pad=0.3', facecolor='yellow', alpha=0.7))
    
    # Inner loop (100 Hz)
    inner_box = FancyBboxPatch((1.5, 0.8), 8, 1.5,
                              boxstyle="round,pad=0.15",
                              edgecolor='#E65100', facecolor='#FFF3E0',
                              linewidth=2.5, zorder=1)
    ax.add_patch(inner_box)
    ax.text(5.5, 2.15, 'INNER LOOP (100 Hz): PWM Current Regulator',
           fontsize=11, weight='bold', ha='center', color='#E65100')
    
    ax.text(5.5, 1.55, 'Inputs: d_setpoint, I_inductor (INA226)',
           fontsize=8, ha='center')
    ax.text(5.5, 1.25, 'Output: MOSFET gate drive (20 kHz PWM)',
           fontsize=8, ha='center')
    ax.text(5.5, 0.95, 'Prevents shoot-through, ensures stable power transfer',
           fontsize=7, ha='center', style='italic', color='gray')
    
    # Data flow arrows
    # Outer → Middle
    ax.annotate('', xy=(5.5, 5.6), xytext=(5.5, 6.3),
               arrowprops=dict(arrowstyle='-|>', lw=2, color='#1565C0'))
    ax.text(6.2, 6.0, 'State\nCommand', fontsize=7, ha='center', color='#1565C0')
    
    # Middle → Inner
    ax.annotate('', xy=(5.5, 2.4), xytext=(5.5, 3.3),
               arrowprops=dict(arrowstyle='-|>', lw=2, color='#2E7D32'))
    ax.text(6.2, 2.85, 'd_setpoint', fontsize=7, ha='center', color='#2E7D32')
    
    # Feedback arrows
    # Inner → Middle (current feedback)
    ax.annotate('', xy=(1.2, 3.5), xytext=(1.2, 2.3),
               arrowprops=dict(arrowstyle='-|>', lw=1.5, color='red', linestyle='dashed'))
    ax.text(0.7, 2.9, 'I', fontsize=7, ha='center', color='red', weight='bold')
    
    # Middle → Outer (power/state feedback)
    ax.annotate('', xy=(9.8, 6.5), xytext=(9.8, 5.9),
               arrowprops=dict(arrowstyle='-|>', lw=1.5, color='red', linestyle='dashed'))
    ax.text(10.3, 6.2, 'P, n, V', fontsize=7, ha='center', color='red', weight='bold')
    
    # Timing diagram (right side)
    time_x = 10.5
    ax.text(time_x, 8.3, 'Timing', fontsize=9, weight='bold', ha='right', color='purple')
    
    # Time bars
    timing_data = [
        (7.8, '1 s', '#1565C0'),
        (5.5, '100 ms', '#2E7D32'),
        (1.6, '10 ms', '#E65100')
    ]
    
    for y_pos, label, color in timing_data:
        ax.plot([10.2, 10.5], [y_pos, y_pos], lw=3, color=color)
        ax.text(10.55, y_pos, label, fontsize=7, va='center', color=color)
    
    # Add Nyquist criterion note
    footnote = ("Sampling rates follow Nyquist criterion with 10× margin:\n" +
               "PWM @ 20 kHz → Inner loop @ 100 Hz (200× slower)\n" +
               "Rotor τ ≈ 5 s → MPPT @ 10 Hz (50× faster)\n" +
               "ESP32 dual-core @ 240 MHz leaves >80% CPU headroom")
    ax.text(5.5, 0.2, footnote, fontsize=7, ha='center',
           style='italic', color='gray',
           bbox=dict(boxstyle='round,pad=0.4', facecolor='lightyellow', alpha=0.6))
    
    plt.tight_layout()
    
    if save:
        output_path = OUTPUT_DIR / "control-hierarchy-diagram.png"
        plt.savefig(output_path, dpi=DPI, bbox_inches='tight',
                   facecolor='white')
        print(f"✓ Control hierarchy diagram saved to: {output_path}")
    
    if show:
        plt.show()
    else:
        plt.close()
    
    return fig, ax

if __name__ == "__main__":
    print("Generating control hierarchy diagram...")
    plot_control_hierarchy(save=True, show=False)
    print("SUCCESS: Control hierarchy diagram complete")
