#include "smoothing.hpp"

namespace math {

std::valarray<std::complex<float>>
octaveSmoothing(int n, std::valarray<std::complex<float>> &sequence) {
  int N = int(sequence.size());

  std::valarray<std::complex<float>> smoothed(N);

  float aCoef = pow(2, -0.5f / n);
  float bCoef = 1 / aCoef;

  int prevA = 0;
  int prevB = 0;

  smoothed[0] = sequence[0] * std::conj(sequence[0]);

  for (int k = 1; k < N; ++k) {
    int a = int(k * aCoef);
    int b = std::min(N - 1, int(k * bCoef));

    std::complex<float> aSum(0.0f, 0.0f);
    for (int i = prevA; i <= a - 1; ++i) {
      aSum += sequence[i] * std::conj(sequence[i]);
    }

    std::complex<float> bSum(0.0f, 0.0f);
    for (int i = prevB + 1; i <= b; ++i) {
      bSum += sequence[i] * std::conj(sequence[i]);
    }

    smoothed[k] = float(prevB - prevA + 1) * smoothed[k - 1] + bSum - aSum;
    smoothed[k] /= b - a + 1;

    prevA = a;
    prevB = b;
  }

  for (int k = 0; k < N; ++k) {
    smoothed[k] = std::complex<float>(std::sqrt(smoothed[k].real()), 0.0f);
  }

  return smoothed;
}

} // namespace math
