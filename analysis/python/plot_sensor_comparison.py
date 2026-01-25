"""
Generate sensor comparison visualization for Chapter 2
Shows cost vs accuracy trade-offs with MPPT/research classification

Author: Dr. Asitha Kulasekera
"""

import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

OUTPUT_DIR = Path(__file__).parent.parent.parent / "docs" / "figures"
OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
DPI = 300

def plot_sensor_comparison(save=True, show=False):
    """
    Create bubble chart comparing sensors on cost vs accuracy
    Bubble size = importance for MPPT
    """
    
    # Sensor data: (name, cost_usd, accuracy_pct, importance, is_mppt_min, is_research)
    sensors = [
        ('Cup Anemometer', 150, 95, 60, False, True),
        ('Sonic Anemometer', 900, 99, 50, False, True),
        ('Hall RPM', 4, 99.5, 100, True, True),
        ('Optical Encoder', 80, 99.98, 70, False, True),
        ('INA226 Shunt', 10, 99.9, 100, True, True),
        ('Fluxgate Current', 75, 99.5, 40, False, True),
        ('Pitch Sensor', 30, 99, 30, False, True),
        ('BMP280 Baro', 5, 99, 20, False, True),
    ]
    
    # Unpack data
    names = [s[0] for s in sensors]
    costs = np.array([s[1] for s in sensors])
    accuracies = np.array([s[2] for s in sensors])
    importances = np.array([s[3] for s in sensors])
    is_mppt_min = np.array([s[4] for s in sensors])
    
    # Create figure
    fig, ax = plt.subplots(figsize=(10, 6))
    
    # Separate MPPT-minimum sensors
    mppt_mask = is_mppt_min == True
    other_mask = is_mppt_min == False
    
    # Plot non-essential sensors (gray)
    scatter1 = ax.scatter(costs[other_mask], accuracies[other_mask],
                         s=importances[other_mask]*5, alpha=0.5,
                         c='lightgray', edgecolors='gray', linewidth=1.5,
                         label='Research-Grade (Optional)', zorder=2)
    
    # Plot MPPT-minimum sensors (green)
    scatter2 = ax.scatter(costs[mppt_mask], accuracies[mppt_mask],
                         s=importances[mppt_mask]*5, alpha=0.7,
                         c='#4CAF50', edgecolors='darkgreen', linewidth=2,
                         label='MPPT Essential', zorder=3, marker='D')
    
    # Annotate each sensor
    for i, name in enumerate(names):
        offset_x = 15 if costs[i] < 500 else -15
        offset_y = 0.3 if i % 2 == 0 else -0.3
        
        ax.annotate(name, (costs[i], accuracies[i]),
                   xytext=(offset_x, offset_y), textcoords='offset points',
                   fontsize=8, weight='bold' if is_mppt_min[i] else 'normal',
                   bbox=dict(boxstyle='round,pad=0.3',
                           facecolor='yellow' if is_mppt_min[i] else 'white',
                           alpha=0.7, edgecolor='black', linewidth=0.5),
                   arrowprops=dict(arrowstyle='->', lw=0.8, color='black'))
    
    # Cost zones
    ax.axvspan(0, 50, alpha=0.1, color='green', zorder=1)
    ax.axvspan(50, 200, alpha=0.1, color='yellow', zorder=1)
    ax.axvspan(200, 1000, alpha=0.1, color='red', zorder=1)
    
    ax.text(25, 94.5, 'Budget\n(<$50)', fontsize=8, ha='center',
           color='darkgreen', weight='bold')
    ax.text(125, 94.5, 'Standard\n($50-200)', fontsize=8, ha='center',
           color='orange', weight='bold')
    ax.text(600, 94.5, 'Premium\n(>$200)', fontsize=8, ha='center',
           color='darkred', weight='bold')
    
    # Formatting
    ax.set_xlabel('Cost (USD, 2026 prices)', fontsize=11)
    ax.set_ylabel('Measurement Accuracy (%)', fontsize=11)
    ax.set_title('VAWT Sensor Selection: Cost vs Accuracy Trade-offs\n' +
                'Bubble size = importance for control system',
                fontsize=12, weight='bold', pad=15)
    
    ax.set_xscale('log')
    ax.set_xlim(2, 1200)
    ax.set_ylim(94, 100.5)
    ax.grid(True, alpha=0.3, linestyle='--', linewidth=0.5)
    
    # Legend
    legend1 = ax.legend(loc='lower right', fontsize=9, framealpha=0.9)
    ax.add_artist(legend1)
    
    # Add bubble size reference
    for size, label in [(20*5, 'Low'), (60*5, 'Medium'), (100*5, 'High')]:
        ax.scatter([], [], s=size, c='lightblue', alpha=0.6,
                  edgecolors='blue', linewidth=1, label=f'{label} Importance')
    ax.legend(loc='upper left', title='Importance for MPPT',
             fontsize=8, framealpha=0.9)
    ax.add_artist(legend1)  # Re-add first legend
    
    # Annotation for minimal system
    ax.annotate('Minimal Viable System:\nHall RPM + INA226 + BMP280\n' +
               'Total: ~$19 USD',
               xy=(7, 99.5), xytext=(100, 97.5),
               arrowprops=dict(arrowstyle='-|>', lw=2, color='green'),
               fontsize=9, weight='bold', color='darkgreen',
               bbox=dict(boxstyle='round,pad=0.5', facecolor='lightgreen',
                        alpha=0.8, edgecolor='darkgreen', linewidth=2))
    
    plt.tight_layout()
    
    if save:
        output_path = OUTPUT_DIR / "sensor-comparison-chart.png"
        plt.savefig(output_path, dpi=DPI, bbox_inches='tight',
                   facecolor='white')
        print(f"✓ Sensor comparison chart saved to: {output_path}")
    
    if show:
        plt.show()
    else:
        plt.close()
    
    return fig, ax

if __name__ == "__main__":
    print("Generating sensor comparison chart...")
    plot_sensor_comparison(save=True, show=False)
    print("SUCCESS: Sensor comparison chart complete")
