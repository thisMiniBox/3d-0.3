#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
class NeuralNetwork_神经网络 {
public:
    NeuralNetwork_神经网络(std::vector<int> architecture);

    void train(std::vector<std::vector<double>> inputs, std::vector<std::vector<double>> expected_outputs, int epochs, double learning_rate);
    std::vector<double> predict(std::vector<double> input) {
        return forward(input).back();
    }
    std::vector<std::vector<std::vector<double>>> weights;
private:
    std::vector<int> architecture;
    std::vector<std::vector<double>> biases;
    // 定义激活函数
    double sigmoid(double x) {
        return 1 / (1 + exp(-x));
    }

    // 定义激活函数的导数
    double sigmoid_derivative(double x) {
        return sigmoid(x) * (1 - sigmoid(x));
    }
    //初始化权重
    void init_weights();
    //误差分析
    std::vector<std::vector<double>> calculate_errors(std::vector<double> expected_output, std::vector<std::vector<double>> layer_outputs);
    //反向传播
    void backward(std::vector<double> expected_output, std::vector<std::vector<double>> layer_outputs, double learning_rate);
    std::vector<std::vector<double>> forward(std::vector<double> input);

};
