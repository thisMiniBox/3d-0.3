#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
class NeuralNetwork_������ {
public:
    NeuralNetwork_������(std::vector<int> architecture);

    void train(std::vector<std::vector<double>> inputs, std::vector<std::vector<double>> expected_outputs, int epochs, double learning_rate);
    std::vector<double> predict(std::vector<double> input) {
        return forward(input).back();
    }
    std::vector<std::vector<std::vector<double>>> weights;
private:
    std::vector<int> architecture;
    std::vector<std::vector<double>> biases;
    // ���弤���
    double sigmoid(double x) {
        return 1 / (1 + exp(-x));
    }

    // ���弤����ĵ���
    double sigmoid_derivative(double x) {
        return sigmoid(x) * (1 - sigmoid(x));
    }
    //��ʼ��Ȩ��
    void init_weights();
    //������
    std::vector<std::vector<double>> calculate_errors(std::vector<double> expected_output, std::vector<std::vector<double>> layer_outputs);
    //���򴫲�
    void backward(std::vector<double> expected_output, std::vector<std::vector<double>> layer_outputs, double learning_rate);
    std::vector<std::vector<double>> forward(std::vector<double> input);

};
