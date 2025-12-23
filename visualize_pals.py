#!/usr/bin/env python3
"""
PALS Comparison Visualization Script

This script generates bar charts comparing baseline lottery scheduler
vs PALS (Peppermint-Aware Adaptive Lottery Scheduling) performance.

Usage:
    python3 visualize_pals.py

The script will prompt you to enter test results from both schedulers.
"""

import matplotlib.pyplot as plt
import numpy as np
from typing import List, Dict

def get_interactive_latencies() -> Dict[str, List[float]]:
    """Get wakeup latency data from pals_int test."""
    print("\n" + "="*70)
    print("PALS_INT: Interactive Latency Test Results")
    print("="*70)
    print("\nEnter wakeup-to-work latency values from pals_int output.")
    print("Format: Enter comma-separated values (e.g., 5,12,8,15,20)")
    print("Or press ENTER to use example data for demonstration.\n")
    
    baseline_input = input("BASELINE latencies: ").strip()
    pals_input = input("PALS latencies: ").strip()
    
    if not baseline_input or not pals_input:
        print("\nUsing example data for demonstration...")
        return {
            'baseline': [5, 12, 8, 15, 20, 7, 18, 10, 14, 9, 16, 11, 13, 8, 19, 6, 17, 12, 10, 15],
            'pals': [3, 4, 5, 4, 6, 5, 7, 5, 4, 6, 5, 4, 6, 5, 7, 4, 5, 6, 5, 4]
        }
    
    baseline = [float(x.strip()) for x in baseline_input.split(',')]
    pals = [float(x.strip()) for x in pals_input.split(',')]
    
    return {'baseline': baseline, 'pals': pals}

def get_aging_times() -> Dict[str, Dict[str, float]]:
    """Get completion times from pals_aging test."""
    print("\n" + "="*70)
    print("PALS_AGING: Starvation Prevention Test Results")
    print("="*70)
    print("\nEnter finish times for dominant and low-priority processes.")
    print("Press ENTER to use example data for demonstration.\n")
    
    baseline_dom = input("BASELINE - Dominant process finish time: ").strip()
    baseline_low = input("BASELINE - Average low-priority finish time: ").strip()
    pals_dom = input("PALS - Dominant process finish time: ").strip()
    pals_low = input("PALS - Average low-priority finish time: ").strip()
    
    if not all([baseline_dom, baseline_low, pals_dom, pals_low]):
        print("\nUsing example data for demonstration...")
        return {
            'baseline': {'dominant': 500, 'low_priority': 7500},
            'pals': {'dominant': 500, 'low_priority': 2800}
        }
    
    return {
        'baseline': {
            'dominant': float(baseline_dom),
            'low_priority': float(baseline_low)
        },
        'pals': {
            'dominant': float(pals_dom),
            'low_priority': float(pals_low)
        }
    }

def get_io_latencies() -> Dict[str, List[float]]:
    """Get I/O average latencies from pals_cmp test."""
    print("\n" + "="*70)
    print("PALS_CMP: I/O Latency Test Results")
    print("="*70)
    print("\nEnter average wakeup latencies for I/O-bound processes.")
    print("Format: Enter comma-separated values for 3 I/O processes")
    print("Press ENTER to use example data for demonstration.\n")
    
    baseline_input = input("BASELINE I/O latencies (3 values): ").strip()
    pals_input = input("PALS I/O latencies (3 values): ").strip()
    
    if not baseline_input or not pals_input:
        print("\nUsing example data for demonstration...")
        return {
            'baseline': [15, 18, 12],
            'pals': [7, 9, 6]
        }
    
    baseline = [float(x.strip()) for x in baseline_input.split(',')]
    pals = [float(x.strip()) for x in pals_input.split(',')]
    
    return {'baseline': baseline, 'pals': pals}

def plot_interactive_latency(data: Dict[str, List[float]], output_dir: str):
    """Create bar chart for interactive latency comparison."""
    baseline = data['baseline']
    pals = data['pals']
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))
    
    # Plot 1: Mean latency comparison
    means = [np.mean(baseline), np.mean(pals)]
    stds = [np.std(baseline), np.std(pals)]
    labels = ['Baseline', 'PALS']
    colors = ['#e74c3c', '#27ae60']
    
    bars = ax1.bar(labels, means, yerr=stds, capsize=10, color=colors, alpha=0.8, edgecolor='black')
    ax1.set_ylabel('Wakeup Latency (ticks)', fontsize=12, fontweight='bold')
    ax1.set_title('Interactive Latency: Mean Wakeup Time', fontsize=14, fontweight='bold')
    ax1.grid(axis='y', alpha=0.3, linestyle='--')
    
    # Add value labels on bars
    for bar in bars:
        height = bar.get_height()
        ax1.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:.1f}',
                ha='center', va='bottom', fontweight='bold')
    
    # Calculate improvement
    improvement = ((means[0] - means[1]) / means[0]) * 100
    ax1.text(0.5, max(means) * 0.9, f'{improvement:.1f}% improvement',
            ha='center', transform=ax1.transData,
            bbox=dict(boxstyle='round', facecolor='yellow', alpha=0.7),
            fontsize=10, fontweight='bold')
    
    # Plot 2: Latency distribution
    positions = np.arange(len(baseline))
    width = 0.35
    
    ax2.bar(positions - width/2, baseline, width, label='Baseline', 
            color='#e74c3c', alpha=0.8, edgecolor='black')
    ax2.bar(positions + width/2, pals, width, label='PALS',
            color='#27ae60', alpha=0.8, edgecolor='black')
    
    ax2.set_xlabel('Burst Number', fontsize=12, fontweight='bold')
    ax2.set_ylabel('Latency (ticks)', fontsize=12, fontweight='bold')
    ax2.set_title('Latency Distribution Across Bursts', fontsize=14, fontweight='bold')
    ax2.set_xticks(positions[::2])
    ax2.set_xticklabels(positions[::2])
    ax2.legend(fontsize=10)
    ax2.grid(axis='y', alpha=0.3, linestyle='--')
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/interactive_latency.png', dpi=300, bbox_inches='tight')
    print(f"✓ Saved: {output_dir}/interactive_latency.png")
    plt.close()

def plot_aging_comparison(data: Dict[str, Dict[str, float]], output_dir: str):
    """Create bar chart for starvation prevention comparison."""
    fig, ax = plt.subplots(figsize=(10, 6))
    
    categories = ['Dominant\n(200 tickets)', 'Low-Priority\n(1 ticket)']
    baseline_times = [data['baseline']['dominant'], data['baseline']['low_priority']]
    pals_times = [data['pals']['dominant'], data['pals']['low_priority']]
    
    x = np.arange(len(categories))
    width = 0.35
    
    bars1 = ax.bar(x - width/2, baseline_times, width, label='Baseline',
                   color='#e74c3c', alpha=0.8, edgecolor='black')
    bars2 = ax.bar(x + width/2, pals_times, width, label='PALS',
                   color='#27ae60', alpha=0.8, edgecolor='black')
    
    ax.set_ylabel('Completion Time (ticks)', fontsize=12, fontweight='bold')
    ax.set_title('Starvation Prevention: Process Completion Times', fontsize=14, fontweight='bold')
    ax.set_xticks(x)
    ax.set_xticklabels(categories, fontsize=11)
    ax.legend(fontsize=11)
    ax.grid(axis='y', alpha=0.3, linestyle='--')
    
    # Add value labels
    for bars in [bars1, bars2]:
        for bar in bars:
            height = bar.get_height()
            ax.text(bar.get_x() + bar.get_width()/2., height,
                   f'{int(height)}',
                   ha='center', va='bottom', fontweight='bold', fontsize=10)
    
    # Calculate slowdown ratios
    baseline_ratio = baseline_times[1] / baseline_times[0]
    pals_ratio = pals_times[1] / pals_times[0]
    
    # Add annotation
    annotation = f'Baseline: Low-priority {baseline_ratio:.1f}x slower\n'
    annotation += f'PALS: Low-priority {pals_ratio:.1f}x slower\n'
    annotation += f'Improvement: {((baseline_ratio - pals_ratio) / baseline_ratio * 100):.1f}% reduction'
    
    ax.text(0.98, 0.97, annotation,
            transform=ax.transAxes,
            verticalalignment='top',
            horizontalalignment='right',
            bbox=dict(boxstyle='round', facecolor='yellow', alpha=0.7),
            fontsize=9, fontweight='bold')
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/aging_comparison.png', dpi=300, bbox_inches='tight')
    print(f"✓ Saved: {output_dir}/aging_comparison.png")
    plt.close()

def plot_io_latency(data: Dict[str, List[float]], output_dir: str):
    """Create bar chart for I/O latency comparison."""
    fig, ax = plt.subplots(figsize=(10, 6))
    
    processes = ['I/O Process 0\n(5 tickets)', 'I/O Process 1\n(10 tickets)', 'I/O Process 2\n(15 tickets)']
    baseline = data['baseline']
    pals = data['pals']
    
    x = np.arange(len(processes))
    width = 0.35
    
    bars1 = ax.bar(x - width/2, baseline, width, label='Baseline',
                   color='#e74c3c', alpha=0.8, edgecolor='black')
    bars2 = ax.bar(x + width/2, pals, width, label='PALS',
                   color='#27ae60', alpha=0.8, edgecolor='black')
    
    ax.set_ylabel('Average Wakeup Latency (ticks)', fontsize=12, fontweight='bold')
    ax.set_title('I/O Responsiveness: Average Wakeup Latency', fontsize=14, fontweight='bold')
    ax.set_xticks(x)
    ax.set_xticklabels(processes, fontsize=10)
    ax.legend(fontsize=11)
    ax.grid(axis='y', alpha=0.3, linestyle='--')
    
    # Add value labels
    for bars in [bars1, bars2]:
        for bar in bars:
            height = bar.get_height()
            ax.text(bar.get_x() + bar.get_width()/2., height,
                   f'{height:.1f}',
                   ha='center', va='bottom', fontweight='bold', fontsize=10)
    
    # Calculate average improvement
    avg_baseline = np.mean(baseline)
    avg_pals = np.mean(pals)
    improvement = ((avg_baseline - avg_pals) / avg_baseline) * 100
    
    ax.text(0.98, 0.97, f'Average Improvement: {improvement:.1f}%',
            transform=ax.transAxes,
            verticalalignment='top',
            horizontalalignment='right',
            bbox=dict(boxstyle='round', facecolor='yellow', alpha=0.7),
            fontsize=11, fontweight='bold')
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/io_latency.png', dpi=300, bbox_inches='tight')
    print(f"✓ Saved: {output_dir}/io_latency.png")
    plt.close()

def plot_summary(latency_data, aging_data, io_data, output_dir: str):
    """Create summary comparison chart."""
    fig, ax = plt.subplots(figsize=(12, 6))
    
    # Calculate metrics
    metrics = [
        'Interactive\nLatency',
        'Starvation\nPrevention',
        'I/O\nResponsiveness'
    ]
    
    baseline_scores = [
        np.mean(latency_data['baseline']),
        aging_data['baseline']['low_priority'] / aging_data['baseline']['dominant'],
        np.mean(io_data['baseline'])
    ]
    
    pals_scores = [
        np.mean(latency_data['pals']),
        aging_data['pals']['low_priority'] / aging_data['pals']['dominant'],
        np.mean(io_data['pals'])
    ]
    
    # Calculate percentage improvements
    improvements = [
        ((baseline_scores[0] - pals_scores[0]) / baseline_scores[0]) * 100,
        ((baseline_scores[1] - pals_scores[1]) / baseline_scores[1]) * 100,
        ((baseline_scores[2] - pals_scores[2]) / baseline_scores[2]) * 100
    ]
    
    x = np.arange(len(metrics))
    colors = ['#3498db', '#e74c3c', '#f39c12']
    
    bars = ax.bar(x, improvements, color=colors, alpha=0.8, edgecolor='black', linewidth=2)
    
    ax.set_ylabel('Improvement (%)', fontsize=14, fontweight='bold')
    ax.set_title('PALS Performance Improvements Over Baseline', fontsize=16, fontweight='bold')
    ax.set_xticks(x)
    ax.set_xticklabels(metrics, fontsize=12, fontweight='bold')
    ax.axhline(y=0, color='black', linestyle='-', linewidth=0.8)
    ax.grid(axis='y', alpha=0.3, linestyle='--')
    
    # Add value labels
    for bar in bars:
        height = bar.get_height()
        ax.text(bar.get_x() + bar.get_width()/2., height,
               f'{height:.1f}%',
               ha='center', va='bottom' if height > 0 else 'top',
               fontweight='bold', fontsize=12)
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/summary_improvements.png', dpi=300, bbox_inches='tight')
    print(f"✓ Saved: {output_dir}/summary_improvements.png")
    plt.close()

def main():
    """Main function to generate all visualizations."""
    print("\n" + "="*70)
    print(" "*15 + "PALS VISUALIZATION GENERATOR")
    print("="*70)
    print("\nThis script will generate comparison charts for:")
    print("  1. Interactive latency (pals_int)")
    print("  2. Starvation prevention (pals_aging)")
    print("  3. I/O responsiveness (pals_cmp)")
    print("  4. Overall summary")
    print("\nYou can enter your actual test results or use example data.")
    print("="*70)
    
    # Get data
    latency_data = get_interactive_latencies()
    aging_data = get_aging_times()
    io_data = get_io_latencies()
    
    # Create output directory
    output_dir = "pals_visualizations"
    import os
    os.makedirs(output_dir, exist_ok=True)
    
    print("\n" + "="*70)
    print("Generating visualizations...")
    print("="*70 + "\n")
    
    # Generate plots
    plot_interactive_latency(latency_data, output_dir)
    plot_aging_comparison(aging_data, output_dir)
    plot_io_latency(io_data, output_dir)
    plot_summary(latency_data, aging_data, io_data, output_dir)
    
    print("\n" + "="*70)
    print("✅ All visualizations generated successfully!")
    print("="*70)
    print(f"\nOutput directory: {output_dir}/")
    print("\nGenerated files:")
    print("  1. interactive_latency.png - Wakeup latency comparison")
    print("  2. aging_comparison.png - Starvation prevention")
    print("  3. io_latency.png - I/O responsiveness")
    print("  4. summary_improvements.png - Overall improvements")
    print("\n" + "="*70)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\nVisualization cancelled by user.")
    except Exception as e:
        print(f"\n❌ Error: {e}")
        import traceback
        traceback.print_exc()
