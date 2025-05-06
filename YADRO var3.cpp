#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>

std::vector<double> generateSineWave(double frequency, double samplingRate, double duration) {
  int numSamples = static_cast<int>(samplingRate * duration);
  std::vector<double> sineWave(numSamples);
  for (int i = 0; i < numSamples; ++i) {
    double t = static_cast<double>(i) / samplingRate;
    sineWave[i] = sin(2.0 * M_PI * frequency * t);
  }
  return sineWave;
}

std::vector<double> interpolate(const std::vector<double>& input) {
  std::vector<double> interpolatedSignal;
  interpolatedSignal.reserve(2 * input.size());
  for (size_t i = 0; i < input.size() - 1; ++i) {
    interpolatedSignal.push_back(input[i]);
    double interpolatedValue = (input[i] + input[i + 1]) / 2.0;
    interpolatedSignal.push_back(interpolatedValue);
  }
  if (!input.empty()) {
    interpolatedSignal.push_back(input.back());
  }
  return interpolatedSignal;
}

std::vector<double> decimate(const std::vector<double>& input) {
  std::vector<double> decimatedSignal;

  for (size_t i = 0; i < input.size(); i += 2) {
    decimatedSignal.push_back(input[i]);
  }

  return decimatedSignal;
}

double calculateMSE(const std::vector<double>& original, const std::vector<double>& processed) {
    if (original.size() != processed.size()) {
        std::cerr << "Error: Vectors have different sizes!" << std::endl;
        return -1.0;
    }

    double sumOfSquaredErrors = 0.0;
    for (size_t i = 0; i < original.size(); ++i) {
        double error = original[i] - processed[i];
        sumOfSquaredErrors += error * error;
    }

    return sumOfSquaredErrors / original.size();
}

int main() {
  double samplingRate = 100.0;
  double duration = 1.0;
  for (double frequency = 0.0; frequency <= 50.0; frequency += 5.0) {
    std::cout << "Frequency: " << frequency << " Hz" << std::endl;
    std::vector<double> sineWave = generateSineWave(frequency, samplingRate, duration);
    std::vector<double> decimatedSignal = decimate(sineWave);
    std::vector<double> interpolatedSignal = interpolate(decimatedSignal);
    while (interpolatedSignal.size() < sineWave.size()) {
        interpolatedSignal.push_back(interpolatedSignal.back());
    }
    interpolatedSignal.resize(sineWave.size());
    double mse = calculateMSE(sineWave, interpolatedSignal);
    std::cout << "MSE: " << mse << std::endl;
  }

  return 0;
}