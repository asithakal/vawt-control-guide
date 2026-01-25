"""
Generate hierarchical state machine diagram for VAWT control
Shows state transitions between IDLE, STARTUP, MPPT, POWER_REG, STALL, FAULT

Author: Dr. Asitha Kulasekera
Dependencies: matplotlib, graphviz (install: pip install graphviz)
Alternative: Pure matplotlib version included below
"""

import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.patches import FancyBboxPatch, FancyArrowPatch
from pathlib import Path

# Output configuration
OUTPUT_DIR = Path(__file__).parent.parent.parent / "docs" / "figures"
OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
DPI = 300

def plot_state_machine_matplotlib(save=True, show=False):
    """
    Pure matplotlib version (no Graphviz dependency)
    Draws state machine using boxes and arrows
    """
    fig, ax = plt.subplots(figsize=(10, 8))
    ax.set_xlim(0, 10)
    ax.set_ylim(0, 10)
    ax.axis('off')
    
    # State definitions: (x, y, width, height, name, color)
    states = {
        'IDLE': (1, 8, 1.5, 0.8, 'IDLE\n(System Check)', '#E8F4F8'),
        'STANDBY': (1, 6.5, 1.5, 0.8, 'STANDBY\n(Ready)', '#D0E8F2'),
        'STARTUP': (4, 8, 1.5, 0.8, 'STARTUP\n(n < 50 RPM)', '#FFF4E6'),
        'MPPT': (4, 6, 1.8, 1.0, 'MPPT\n(Hill-Climb Search)\nλ → λopt', '#C8E6C9'),
        'POWER_REG': (7, 6, 1.8, 1.0, 'POWER_REG\n(Soft-Stall)\nP ≈ Prated', '#FFF9C4'),
        'STALL': (7, 4, 1.5, 0.8, 'STALL\n(Dump Load)', '#FFE0B2'),
        'FAULT': (4, 2, 2.0, 1.0, 'FAULT\n(Emergency Brake)\nRPM > 250', '#FFCDD2')
    }
    
    # Draw state boxes
    state_boxes = {}
    for state_id, (x, y, w, h, label, color) in states.items():
        box = FancyBboxPatch((x, y), w, h,
                            boxstyle="round,pad=0.1",
                            edgecolor='black', facecolor=color,
                            linewidth=2, zorder=2)
        ax.add_patch(box)
        ax.text(x + w/2, y + h/2, label,
               ha='center', va='center', fontsize=9,
               weight='bold', zorder=3)
        state_boxes[state_id] = (x + w/2, y + h/2)
    
    # Transition arrows with labels
    transitions = [
        # (from_state, to_state, label, curvature)
        ('IDLE', 'STANDBY', 'Init OK', 0),
        ('STANDBY', 'STARTUP', 'vw > 3 m/s', 0),
        ('STARTUP', 'MPPT', 'n > 50 RPM', 0),
        ('MPPT', 'POWER_REG', 'P > 0.95Prated\n10 s', 0.3),
        ('POWER_REG', 'MPPT', 'P < 0.8Prated', -0.3),
        ('POWER_REG', 'STALL', 'vw > 12 m/s', 0),
        ('MPPT', 'STALL', 'n > 210 RPM', 0.4),
        ('STALL', 'STANDBY', 'vw < 10 m/s\nn < 150 RPM', -0.5),
        ('MPPT', 'FAULT', 'n > 250 RPM\nV > 60 V', 0),
        ('POWER_REG', 'FAULT', 'Sensor fail', 0),
        ('STALL', 'FAULT', 'n > 250 RPM', 0.3),
        ('FAULT', 'IDLE', 'Manual\nReset', -0.7)
    ]
    
    for from_state, to_state, label, curve in transitions:
        x1, y1 = state_boxes[from_state]
        x2, y2 = state_boxes[to_state]
        
        # Calculate control points for curved arrows
        dx = x2 - x1
        dy = y2 - y1
        mx = (x1 + x2) / 2 + curve * dy
        my = (y1 + y2) / 2 - curve * dx
        
        # Draw arrow
        arrow = FancyArrowPatch(
            (x1, y1), (x2, y2),
            connectionstyle=f"arc3,rad={curve}",
            arrowstyle='-|>', mutation_scale=20,
            linewidth=1.5, color='#424242', zorder=1,
            alpha=0.7
        )
        ax.add_patch(arrow)
        
        # Add label at midpoint
        label_x = mx if abs(curve) > 0.1 else (x1 + x2) / 2
        label_y = my if abs(curve) > 0.1 else (y1 + y2) / 2
        ax.text(label_x, label_y, label,
               fontsize=7, ha='center', va='center',
               bbox=dict(boxstyle='round,pad=0.3', 
                        facecolor='white', edgecolor='gray',
                        alpha=0.8), zorder=4)
    
    # Add title and legend
    ax.text(5, 9.5, 'VAWT Hierarchical State Machine',
           fontsize=14, weight='bold', ha='center')
    ax.text(5, 9.2, '500W Helical VAWT (ESP32 Implementation)',
           fontsize=10, ha='center', style='italic', color='gray')
    
    # Legend for state colors
    legend_elements = [
        mpatches.Patch(color='#E8F4F8', label='Initialization'),
        mpatches.Patch(color='#C8E6C9', label='Normal Operation'),
        mpatches.Patch(color='#FFF9C4', label='Power Limiting'),
        mpatches.Patch(color='#FFE0B2', label='High Wind'),
        mpatches.Patch(color='#FFCDD2', label='Emergency')
    ]
    ax.legend(handles=legend_elements, loc='lower left',
             fontsize=8, framealpha=0.9)
    
    # Add footnote
    footnote = ("Transitions include 5 s hysteresis to prevent chattering.\n"
               "See firmware/lib/StateMachine/TurbineStateMachine.cpp")
    ax.text(5, 0.3, footnote, fontsize=7, ha='center',
           style='italic', color='gray')
    
    plt.tight_layout()
    
    if save:
        output_path = OUTPUT_DIR / "state-machine-diagram.png"
        plt.savefig(output_path, dpi=DPI, bbox_inches='tight',
                   facecolor='white', edgecolor='none')
        print(f"✓ State machine diagram saved to: {output_path}")
    
    if show:
        plt.show()
    else:
        plt.close()
    
    return fig, ax

if __name__ == "__main__":
    print("Generating state machine diagram...")
    plot_state_machine_matplotlib(save=True, show=False)
    print("SUCCESS: State machine diagram complete")
