package org.defix;

import java.util.Arrays;
import java.util.Random;
import java.util.function.Function;

public class Main {
    private interface GaussMatrixCalculator {
        double[] calculate(double[][] matrix);
    }

    private static class GaussWithLeadElementCalculator implements GaussMatrixCalculator {
        @Override
        public double[] calculate(double[][] matrix) {
            int n = matrix.length;
            if (n < 2 || matrix[0].length != n + 1)
                throw new IllegalArgumentException("Invalid matrix size.");

            for (int columnId = 0; columnId < n; columnId++) {
                // Выбор ведущего элемента (по модулю)
                int leadRow = columnId;
                for (int rowId = columnId + 1; rowId < n; rowId++) {
                    if (Math.abs(matrix[rowId][columnId]) > Math.abs(matrix[leadRow][columnId])) {
                        leadRow = rowId;
                    }
                }

                // Меняем местами строки, если нужно
                if (leadRow != columnId) {
                    double[] temp = matrix[columnId];
                    matrix[columnId] = matrix[leadRow];
                    matrix[leadRow] = temp;
                }

                // Делим ведущую строку на ведущий элемент
                double leadElement = matrix[columnId][columnId];
                for (int j = 0; j < n + 1; j++) {
                    matrix[columnId][j] /= leadElement;
                }

                // Обнуляем остальные строки
                for (int i = 0; i < n; i++) {
                    if (i != columnId) {  // Не трогаем ведущую строку
                        double coeff = matrix[i][columnId];
                        for (int j = 0; j < n + 1; j++) {
                            matrix[i][j] -= coeff * matrix[columnId][j];
                        }
                    }
                }
            }

            // Записываем результаты (последний столбец)
            double[] answers = new double[n];
            for (int i = 0; i < n; i++) {
                answers[i] = matrix[i][n];
            }
            return answers;
        }
    }

    private static class GaussZeydelyaCalculator implements GaussMatrixCalculator {
        private static final double SOURCE_EPSILON = 0.1;
        private final double epsilon;

        public GaussZeydelyaCalculator(double epsilon) {
            this.epsilon = epsilon;
        }

        public GaussZeydelyaCalculator() {
            this.epsilon = SOURCE_EPSILON;
        }

        @Override
        public double[] calculate(double[][] matrix) {
            int n = matrix.length;
            if (n < 2 || matrix[0].length != n + 1)
                throw new IllegalArgumentException("Invalid matrix size.");

            double[] lastRes = new double[n];
            double[] res = new double[n];
            boolean finish = false;

            while (!finish) {
                finish = true; // Предполагаем, что сходимость достигнута

                for (int i = 0; i < n; i++) {
                    double curNum = matrix[i][n]; // Свободный член b_i

                    for (int j = 0; j < n; j++) {
                        if (j != i) curNum -= matrix[i][j] * res[j]; // Используем уже обновлённые значения
                    }
                    curNum /= matrix[i][i];
                    res[i] = curNum;

                    // Проверка на сходимость
                    if (Math.abs(res[i] - lastRes[i]) >= epsilon) {
                        finish = false;
                    }
                }

                // Копируем новое приближение в lastRes
                System.arraycopy(res, 0, lastRes, 0, n);
            }
            return res;
        }
    }

    public static void main(String[] args) {
        double[][] matrix = new double[][]{
                {3, 0, -1, -4},
                {2, -5, 1, 9},
                {2, -2, 6, 8}
        };

        GaussWithLeadElementCalculator leadCalculator = new GaussWithLeadElementCalculator();
        System.out.println(Arrays.toString(leadCalculator.calculate(matrix)));

        GaussZeydelyaCalculator zeydelyaCalculator = new GaussZeydelyaCalculator();
        System.out.println(Arrays.toString(zeydelyaCalculator.calculate(matrix)));
    }
}