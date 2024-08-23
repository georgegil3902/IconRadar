# ICON Radar coding assessment

This document describes a coding assessment for new employees of the ICON Radar team to be send during the interview times. The main attempt is to challenge future employees in their experience on software development utilizing common tools.

## Objective

Create an interactive image processing application that uses OpenCV for image manipulation and ImGui for the graphical user interface. The project should be set up using CMake, supporting cross-platform builds (Windows, macOS and Linux). Only one platform must be implemented (Preferred for Windows).

## Project Configuration

- Use a private git repository to submit results.
- Use CMake to configure the project.
- Ensure that the project supports out-of-source builds.
- CMake configuration should generate appropriate build files for Visual Studio.
- Provide an organized folder structure.
- Input Radar detection data available in CSV container file

  - 1st column: Position in x ∈ ℝ (positive to up)
  - 2nd column: Position in y ∈ ℝ (positive to right)
  - 3rd column: Confidence in c ∈ [0,1] (from blue(low confidence), over green and yellow, to red (high confidence))
  - e.g. OGM.csv

    ```csv
    -2.91705666e+01,  -9.26803131e+01,   1.54000014e-01
    -3.09707851e+01,  -9.04702911e+01,   1.79000005e-01
    -2.79715462e+01,  -9.04027405e+01,   1.70000002e-01
    -3.19817905e+01,  -8.99929352e+01,   1.89000010e-01
    -3.07433662e+01,  -8.94649124e+01,   1.93000004e-01
    ...
    ```

## Functional Requirements

The application must enable the loading of a CSV file containing Radar detection data. Plot the csv data as an image. Apply an edge detection filter (free of choice) to the image to identify road edges and display as overlay in the image. The user must be able to adjust the edge detection thresholds in real-time, with the image automatically re-rendering to reflect these changes.

## Schedule and Deliverables

- The receive of the mail holding this file is creating the start of the assessment.
- Provide a link to a public Git Repository containing
  - Source code of the application and build scripts.
  - A README file explaining dependencies, build process, etc.
- Submit your project as soon as it is functional.
- Send your project anyways after 3 days in case you couldn't achieved functionality.
- Send repository Link to <Sebastian.Pliefke@magna.com>.
