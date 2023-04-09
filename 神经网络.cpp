#include "神经网络.h"
NeuralNetwork_神经网络::NeuralNetwork_神经网络(std::vector<int> architecture) {
    this->architecture = architecture;
    for (int i = 0; i < architecture.size() - 1; i++) {
        weights.push_back(std::vector<std::vector<double>>(architecture[i], std::vector<double>(architecture[i + 1])));
        biases.push_back(std::vector<double>(architecture[i + 1]));
    }
    init_weights();
}
void NeuralNetwork_神经网络::train(std::vector<std::vector<double>> inputs, std::vector<std::vector<double>> expected_outputs, int epochs, double learning_rate) {
    for (int i = 0; i < epochs; i++) {
        for (int j = 0; j < inputs.size(); j++) {
            std::vector<std::vector<double>> layer_outputs = forward(inputs[j]);
            backward(expected_outputs[j], layer_outputs, learning_rate);
        }
    }
}
void NeuralNetwork_神经网络::init_weights() {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);
    for (int i = 0; i < weights.size(); i++) {
        for (int j = 0; j < weights[i].size(); j++) {
            for (int k = 0; k < weights[i][j].size(); k++) {
                weights[i][j][k] = distribution(generator);
            }
        }
    }
}
std::vector<std::vector<double>> NeuralNetwork_神经网络::calculate_errors(std::vector<double> expected_output, std::vector<std::vector<double>> layer_outputs) {
    std::vector<std::vector<double>> errors;
    int num_layers = architecture.size();
    for (int i = num_layers - 1; i >= 0; i--) {
        std::vector<double> layer_error;
        if (i == num_layers - 1) {
            for (int j = 0; j < architecture[i]; j++) {
                double error = expected_output[j] - layer_outputs[i][j];
                layer_error.push_back(error);
            }
        }
        else {
            for (int j = 0; j < architecture[i]; j++) {
                double error = 0;
                for (int k = 0; k < architecture[i + 1]; k++) {
                    error += weights[i][j][k] * errors[0][k];
                }
                layer_error.push_back(error);
            }
        }
        errors.insert(errors.begin(), layer_error);
    }
    return errors;
}
void NeuralNetwork_神经网络::backward(std::vector<double> expected_output, std::vector<std::vector<double>> layer_outputs, double learning_rate) {
    std::vector<std::vector<double>> errors = calculate_errors(expected_output, layer_outputs);
    int num_layers = architecture.size();
    for (int i = num_layers - 1; i >= 1; i--) {
        for (int j = 0; j < architecture[i]; j++) {
            double delta = errors[i][j] * sigmoid_derivative(layer_outputs[i][j]);
            biases[i - 1][j] += learning_rate * delta;
            for (int k = 0; k < architecture[i - 1]; k++) {
                weights[i - 1][k][j] += learning_rate * delta * layer_outputs[i - 1][k];
            }
        }
    }
}
std::vector<std::vector<double>> NeuralNetwork_神经网络::forward(std::vector<double> input) {
    std::vector<std::vector<double>> activations = { input };
    for (int i = 0; i < weights.size(); i++) {
        std::vector<double> layer_activations;
        for (int j = 0; j < weights[i][0].size(); j++) {
            double activation = biases[i][j];
            for (int k = 0; k < weights[i].size(); k++) {
                activation += activations.back()[k] * weights[i][k][j];
            }
            layer_activations.push_back(sigmoid(activation));
        }
        activations.push_back(layer_activations);
    }
    return activations;
}
