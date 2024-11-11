# Robot Dog Servo Calibration

This repository contains a Python script designed for calibrating servos for a robot dog application. The calibration process uses Google Colab to map each servo's PWM signal to its actual angle, improving accuracy by correcting for each servo's unique error profile. This guide walks through the setup, usage, and the methodology used to achieve accurate servo positioning.

## Repo Content

1. Calibration_Procedure.ipynb: Google collab that will find coefficient for least square regression to convert degrees to PWM with accuracy for each servo

2. calibarte_servos.ino: Arduino code that with help of [Calibration Tool]() will find real PWM to reach known angles

3. save_coefficent: Folder containg both, .csv and .xlxs, used to save and store calibration values and coefficients for each servo in each leg

## Background and Motivation

Servos are typically controlled by PWM (Pulse Width Modulation) signals to achieve specific angles. However, in practice, each servo may have slightly different mechanical and electrical tolerances, causing deviations between the desired angle and the actual angle. This calibration script was developed to:
1. Map each servo's specific response curve (desired angle vs. actual angle) based on a set of PWM signals.
2. Generate a best-fit curve that allows for more accurate angle positioning by correcting these deviations.

The curve follows a linear formula based on datasheet information:
   - **PWM = 7.4 * desired_angle + 500**

For example, if we input `[0, 45, 90, 135, 180]` degrees, this should ideally correspond to `[500, 833, 1166, 1500, 1832]` PWM signals. However, due to hardware inconsistencies, actual angles may vary, so calibration is necessary.

## Process of Calibration

The following process will guide you through setting up and running the calibration:

1. Use cell 1 to retrive expected PWM for a known set of angles (`[0, 45, 90, 135, 180]`). PWM calculation are based on the ideal servo described in the datasheet.

2. Use the [arduino code]() to allign the calibration tool in the desire angle and save the real PWM to reach desire angle.

3. Save PWM signals founded. For example, for my Front Right leg I had this:
   ```python
   real_pwm_SFR = np.array([564, 890, 1219, 1564, 1897])
   real_pwm_FFR = np.array([606, 930, 1265, 1606, 1930])
   real_pwm_TFR = np.array([555, 895, 1230, 1580, 1910])
   ```

4. Run Least Square regression and save a, b, c coefficients to fulfill the equation that best map desire angle in degree to PWM

5. Test new accuracy

### Arduino code
The arduino code, when uploaded, will move one servo to a desire angle.
Upload the code and open serial monitor. You will see the current PWM signal.
1. set it to 500
2. Attach the servo arm with the [calibration tool]()

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

### Cell 2

### Cell 3

## Result
You should end up with some results like this:

[result](https://github.com/botzo-team/our_images_and_videos/blob/main/result_of_one_leg_calibration.png)

Before calibration

![Before calibration](https://github.com/botzo-team/our_images_and_videos/blob/main/before_calibration.gif)

After calibration
![After calibration](https://github.com/botzo-team/our_images_and_videos/blob/main/after_calibration.gif)

## Notes
Ensure you measure the actual angles carefully, as they significantly impact the calibration accuracy.

Run this script separately for each servo on the robot dog, as each servo may have unique parameters.

By following these steps, you can effectively calibrate each servo for optimal performance in your robot dog application. This script provides an essential calibration layer, allowing your robot dog to achieve precise angles based on accurate PWM control.

Once the servo is calibrated and coefficents are saved do not remove the servo arm, or you will need to re-calibrate the servo.