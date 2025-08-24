#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include <random>
#include <thread>
#include <atomic>
#include <algorithm>
#include <numeric>

class CPUStressTest {
private:
    std::atomic<bool> running{false};
    std::atomic<long long> totalOperations{0};
    std::chrono::high_resolution_clock::time_point startTime;
    
public:
    // Prime number calculation - CPU intensive
    long long calculatePrimes(int limit) {
        long long primeSum = 0;
        for (int n = 2; n < limit; n++) {
            bool isPrime = true;
            for (int i = 2; i <= std::sqrt(n); i++) {
                if (n % i == 0) {
                    isPrime = false;
                    break;
                }
            }
            if (isPrime) {
                primeSum += n;
            }
        }
        return primeSum;
    }
    
    // Matrix multiplication - memory and CPU intensive
    std::vector<std::vector<double>> matrixMultiply(
        const std::vector<std::vector<double>>& A,
        const std::vector<std::vector<double>>& B) {
        
        int n = A.size();
        int m = B[0].size();
        int p = B.size();
        
        std::vector<std::vector<double>> C(n, std::vector<double>(m, 0.0));
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int k = 0; k < p; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return C;
    }
    
    // Generate random matrix
    std::vector<std::vector<double>> generateRandomMatrix(int rows, int cols) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        
        std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = dis(gen);
            }
        }
        return matrix;
    }
    
    // Mathematical operations - floating point intensive
    double intensiveMathOperations(int iterations) {
        double result = 0.0;
        for (int i = 1; i <= iterations; i++) {
            result += std::sin(i) * std::cos(i) + std::tan(i * 0.01);
            result += std::sqrt(i * i + 1);
            result += std::log(i + 1);
            result += std::pow(i % 100, 2.5);
            result += std::exp(i * 0.001);
        }
        return result;
    }
    
    // Fibonacci calculation - recursive stress
    long long fibonacci(int n) {
        if (n <= 1) return n;
        
        std::vector<long long> fib(n + 1);
        fib[0] = 0;
        fib[1] = 1;
        
        for (int i = 2; i <= n; i++) {
            fib[i] = fib[i-1] + fib[i-2];
        }
        return fib[n];
    }
    
    // Sorting operations - memory intensive
    void intensiveSorting(int size) {
        std::random_device rd;
        std::mt19937 gen(rd());
        
        for (int iteration = 0; iteration < 10; iteration++) {
            std::vector<int> data(size);
            std::iota(data.begin(), data.end(), 0);
            std::shuffle(data.begin(), data.end(), gen);
            
            // Multiple sorting algorithms
            std::sort(data.begin(), data.end());
            std::reverse(data.begin(), data.end());
            std::sort(data.begin(), data.end(), std::greater<int>());
        }
    }
    
    // Worker thread function
    void workerThread(int threadId) {
        long long operations = 0;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> taskDis(1, 5);
        
        while (running) {
            int task = taskDis(gen);
            
            switch (task) {
                case 1: {
                    // Prime calculation
                    calculatePrimes(100000);
                    operations++;
                    break;
                }
                case 2: {
                    // Matrix multiplication
                    auto matA = generateRandomMatrix(100, 100);
                    auto matB = generateRandomMatrix(100, 100);
                    matrixMultiply(matA, matB);
                    operations++;
                    break;
                }
                case 3: {
                    // Math operations
                    intensiveMathOperations(100000);
                    operations++;
                    break;
                }
                case 4: {
                    // Fibonacci
                    fibonacci(60);
                    operations++;
                    break;
                }
                case 5: {
                    // Sorting
                    intensiveSorting(1000000);
                    operations++;
                    break;
                }
            }
        }
        
        totalOperations += operations;
    }
    
    void runStressTest(int durationSeconds = 5) {
        startTime = std::chrono::high_resolution_clock::now();
        running = true;
        totalOperations = 0;
        
        // Create worker threads (one per CPU core)
        std::vector<std::thread> threads;
        unsigned int numThreads = std::thread::hardware_concurrency();
        
        for (unsigned int i = 0; i < numThreads; i++) {
            threads.emplace_back(&CPUStressTest::workerThread, this, i);
        }
        
        // Wait for specified duration
        std::this_thread::sleep_for(std::chrono::seconds(durationSeconds));
        running = false;
        
        // Wait for all threads to finish
        for (auto& thread : threads) {
            thread.join();
        }
    }
    
    void printResults() {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        double actualSeconds = totalDuration.count() / 1000.0;
        long long finalOperations = totalOperations.load();
        unsigned int numThreads = std::thread::hardware_concurrency();
        
        std::cout << "Duration: " << actualSeconds << "s | Operations: " << finalOperations 
                  << " | Ops/sec: " << static_cast<long long>(finalOperations / actualSeconds) 
                  << " | Threads: " << numThreads 
                  << " | CPU Score: " << static_cast<long long>((finalOperations / actualSeconds) / numThreads) << std::endl;
    }
};

int main() {
    CPUStressTest stressTest;
    stressTest.runStressTest(5);
    stressTest.printResults();
    return 0;
}