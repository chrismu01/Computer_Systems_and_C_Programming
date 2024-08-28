# Monte Carlo Pi Approximation

## Author
Christian Mu

---

## Project Overview
This project utilizes the Monte Carlo method to approximate the value of π. The Monte Carlo method is a statistical technique that uses random sampling to obtain numerical results. In this case, we use it to estimate the value of π by simulating random points in a unit square and calculating the ratio of points that fall within a quarter-circle inscribed within that square.

As the number of iterations (random points) increases, the estimate for π becomes more accurate. This project includes a C program that performs the Monte Carlo simulation and a bash script to plot the results using GNUplot.

---

## Files

- **`monte_carlo.c`**: The main C program that performs the Monte Carlo simulation to approximate π. It generates random points within the unit square and calculates the ratio of points inside the quarter-circle to estimate π.

- **`plot.sh`**: A bash script that uses GNUplot to generate two graphs based on the Monte Carlo simulation data. These graphs visually represent the accuracy of the π approximation as the number of iterations increases.

- **`Makefile`**: A build script that compiles the `monte_carlo.c` program and prepares the necessary files to run the project. It also includes a `clean` command to remove unnecessary files generated during the build process.

---

## How to Build and Run the Project

### Prerequisites
- Ensure that you have a C compiler (e.g., `gcc`) and GNUplot installed on your system.

### Steps to Build and Run
1. **Clone the repository:**
   ```bash
   git clone <repository_url>