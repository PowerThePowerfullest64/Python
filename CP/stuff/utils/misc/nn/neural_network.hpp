#pragma once

#include <cmath>
#include <vector>

#include "utils/math/random.hpp"

#include "matrix.hpp"

inline double Activation(const double& x) {
    return std::tanh(x);
}

inline double SigmoidDerivative(const double& sigmoidOutput) {
    return sigmoidOutput * (1.0 - sigmoidOutput);
}

struct Layer {
    Matrix weights; // Rows = neuron in this layer, cols = inputs from previous layer.
    std::vector<double> biases; // Just one per neuron in this layer.

    // Needed for backprop aughhhh...
    std::vector<double> lastInput;
    std::vector<double> lastOutput;

    Layer (int numInputs, int numNeurons)
    : weights(numNeurons, numInputs), biases(numNeurons) {
        weights.Randomize();

        for (size_t i = 0; i < numNeurons; ++i)
            biases[i] = Random::Range(-0.5, 0.5);
    }

    std::vector<double> Forward(const std::vector<double>& input) {
        lastInput = input;
        std::vector<double> output(weights.rows);

        for (int r = 0; r < weights.rows; ++r) {
            double sum = biases[r];

            for (int c = 0; c < weights.cols; ++c)
                sum += weights(r, c) * input[c];
            output[r] = Activation(sum);
        }

        lastOutput = output;
        return output;
    }
};

struct NeuralNetwork {
    std::vector<Layer> layers;

    NeuralNetwork(const std::vector<int>& layerSizes) {
        for (size_t i = 1; i < layerSizes.size(); ++i)
            layers.emplace_back(layerSizes[i - 1], layerSizes[i]);
    }

    std::vector<double> Forward(const std::vector<double>& input) {
        std::vector<double> current = input;
        for (Layer& layer : layers) {
            current = layer.Forward(current);
        }

        return current;
    }

    NeuralNetwork Mutate(float mutationAmount) {
        NeuralNetwork nn(*this);

        for (auto& layer : nn.layers) {
            for (auto& weight : layer.weights.data)
                weight += Random::Range(-mutationAmount, mutationAmount);
            for (auto& bias : layer.biases)
                bias += Random::Range(-mutationAmount, mutationAmount);
        }

        return nn;
    }
};