# Robot Dog Servo Calibration

This repository contains a Python script designed for calibrating servos for a robot dog application. The calibration process uses Google Colab to map each servo's PWM signal to its actual angle, improving accuracy by correcting for each servo's unique error profile. This guide walks through the setup, usage, and the methodology used to achieve accurate servo positioning.

## Background and Motivation

Servos are typically controlled by PWM (Pulse Width Modulation) signals to achieve specific angles. However, in practice, each servo may have slightly different mechanical and electrical tolerances, causing deviations between the desired angle and the actual angle. This calibration script was developed to:
1. Map each servo's specific response curve (desired angle vs. actual angle) based on a set of PWM signals.
2. Generate a best-fit curve that allows for more accurate angle positioning by correcting these deviations.

The curve follows a linear formula based on datasheet information:
   - **PWM = 7.4 * desired_angle + 500**

For example, if we input `[0, 45, 90, 135, 180]` degrees, this should ideally correspond to `[500, 833, 1166, 1500, 1832]` PWM signals. However, due to hardware inconsistencies, actual angles may vary, so calibration is necessary.

## Process of Calibration

The following process will guide you through setting up and running the calibration:

1. **Set Initial Position**:
   - Start by setting the servo to the 0-degree position (500 PWM), and attach the arm in a known starting position (e.g., horizontal).

2. **Record Desired vs. Actual Angles**:
   - Using the script, pass the following PWM signals to the servo: `[500, 833, 1166, 1500, 1832, 2500]`.
   - Measure the actual angle reached for each PWM signal and note these values. For example, if the target angles are `[0, 45, 90, 135, 180, 270]`, the actual angles reached might be `[−4, 40, 86, 130, 175]`.

3. **Generate Correction Curve**:
   - With the measured values, the script fits a curve that corrects for the difference between the desired and actual angles.
   - The script outputs a best-fit equation that allows the servo to reach the desired angle more accurately.

## Code Structure

The code is split into three main cells in a Google Colab environment, each serving a specific role in the calibration process.

### Cell 1: Setup and PWM Conversion

This cell initializes the setup and converts desired angles into PWM signals based on the known formula from the datasheet.

```python
# Libraries
import numpy as np

# Convert desired degrees to PWM based on datasheet formula
def deg2PWM(desire_deg_angles):
    output = []
    for angle in desire_deg_angles:
        pulse = round((7.4074 * angle) + 500, 0)
        output.append(pulse)
        print(f"// {angle} degrees => {pulse} PWM")
    return output

# Desired angles and corresponding PWM (based on formula)
desire_angles = [0, 45, 90, 135, 180, 270]
desired_PWM = deg2PWM(desire_angles)
print("\nDesired PWM values:", desired_PWM)

# Prompt to manually measure actual angles
print("\nPlease run the servo with the above PWM values and record the actual angles reached.")
```

### Cell 2: Input Actual Angles
After running the servo through the specified PWM signals, input the actual angles measured for each corresponding PWM. This cell also provides a quick visualization of the difference between ideal and actual values.

```python
# Input the actual angles measured for each corresponding PWM
# Replace with your own measurements for accuracy
actual_angles = [-4, 40, 86, 130, 175, 260]  # Example measurements
print("Actual angles entered:", actual_angles)

# Plot ideal vs actual for a quick visualization
import matplotlib.pyplot as plt

plt.figure()
plt.plot(desire_angles, desired_PWM, 'bo-', label='Ideal PWM Curve')
plt.plot(desire_angles, actual_angles, 'go-', label='Measured Angles')
plt.xlabel('Desired Angle (degrees)')
plt.ylabel('PWM/Actual Angle')
plt.legend()
plt.show()
```

### Cell 3

This cell calculates the best-fit correction curve based on the actual measurements. It uses a cubic spline and polynomial curve fitting to model the correction and displays the results in a 3D plot, showing the relationship between desired angle, actual angle, and PWM signal.

```python
# Fit polynomial to relate PWM and actual angles to create correction curve
# Using a cubic spline to address non-linearity
from scipy.interpolate import CubicSpline
from scipy.optimize import curve_fit

cs = CubicSpline(desire_angles, actual_angles)

# Use the fitted curve to generate adjusted PWM for a more accurate result
adjusted_PWM = [cs(angle) for angle in desire_angles]

# Define a function to approximate the desired-to-PWM relationship
# General form: PWM = a*angle^2 + b*angle + c
def angle_to_pwm(x, a, b, c):
    return a * x**2 + b * x + c

# Use curve fitting to find optimal parameters for the correction
params, _ = curve_fit(angle_to_pwm, actual_angles, desired_PWM)
a, b, c = params
print(f"\nBest-fit parameters for PWM = {a:.4f} * angle^2 + {b:.4f} * angle + {c:.4f}")

# Plot the 3D fit with data points and correction curve
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Scatter plot of the measured values
ax.scatter(desire_angles, actual_angles, desired_PWM, color='blue', label='Measured Points')

# Generate smooth line for the correction curve
angles_smooth = np.linspace(min(desire_angles), max(desire_angles), 100)
corrected_pwm = angle_to_pwm(angles_smooth, *params)
ax.plot(angles_smooth, cs(angles_smooth), corrected_pwm, color='red', label='Fitted Curve')

# Labels
ax.set_xlabel("Desired Angle (deg)")
ax.set_ylabel("Actual Angle (deg)")
ax.set_zlabel("PWM Signal")
plt.legend()
plt.show()
```

## Notes
Ensure you measure the actual angles carefully, as they significantly impact the calibration accuracy.

Run this script separately for each servo on the robot dog, as each servo may have unique parameters.

By following these steps, you can effectively calibrate each servo for optimal performance in your robot dog application. This script provides an essential calibration layer, allowing your robot dog to achieve precise angles based on accurate PWM control.