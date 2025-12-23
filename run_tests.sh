#!/bin/bash

# PALS Testing Script
# This script helps automate the testing process for baseline vs PALS comparison

echo "================================================================================"
echo "  PALS Testing Helper Script"
echo "================================================================================"
echo ""
echo "This script will guide you through testing both schedulers."
echo ""
echo "IMPORTANT: You'll need to manually run commands inside xv6."
echo "           This script just helps with the build process."
echo ""

# Function to wait for user
wait_for_user() {
    echo ""
    read -p "Press ENTER when ready to continue..."
    echo ""
}

# Test baseline
echo "Step 1: Testing BASELINE Lottery Scheduler"
echo "-------------------------------------------"
echo "Building baseline xv6..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1

if [ $? -eq 0 ]; then
    echo "✓ Baseline build successful"
    echo ""
    echo "Now run: make qemu"
    echo ""
    echo "Inside xv6, run these commands and save the output:"
    echo "  $ pals_int"
    echo "  $ pals_aging"
    echo "  $ pals_cmp"
    echo ""
    echo "Exit xv6 with: Ctrl-A then x"
    echo ""
    wait_for_user
else
    echo "✗ Baseline build failed"
    exit 1
fi

# Test PALS
echo "Step 2: Testing PALS Scheduler"
echo "-------------------------------"
echo "Building PALS xv6..."
make clean > /dev/null 2>&1
CFLAGS=-DPALS make > /dev/null 2>&1

if [ $? -eq 0 ]; then
    echo "✓ PALS build successful"
    echo ""
    echo "Now run: make qemu"
    echo ""
    echo "Inside xv6, run the SAME commands and save the output:"
    echo "  $ pals_int"
    echo "  $ pals_aging"
    echo "  $ pals_cmp"
    echo ""
    echo "Exit xv6 with: Ctrl-A then x"
    echo ""
    echo "Compare the outputs to see the differences!"
    echo ""
else
    echo "✗ PALS build failed"
    exit 1
fi

echo "================================================================================"
echo "Testing complete!"
echo ""
echo "Compare the outputs from both runs to see:"
echo "  - Lower wakeup latency in pals_int (PALS)"
echo "  - Bounded wait times in pals_aging (PALS)"
echo "  - Better I/O responsiveness in pals_cmp (PALS)"
echo ""
echo "See PALS_COMPARISON_REPORT.txt for detailed analysis."
echo "================================================================================"
