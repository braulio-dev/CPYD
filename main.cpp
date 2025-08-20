#include<iostream>
#include<memory>
#include<thread>
#include<string>
#include<mutex>
#include<condition_variable>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<algorithm>

class Summator {
private:
    std::string name;
    int sum;
    std::mutex sumMutex;
public:
    Summator(std::string threadName) : name(threadName), sum(0) {
        
    }

    void calculateSum() {
        // generar numero aleatorio entre 1 a 1000
        int localSum = 0;
        for (int i = 0; i < 100; i++) {
            int randomNum = (rand() % 1000) + 1;
            localSum += randomNum;
        }
        
        // asignar el resultado al miembro sum
        {
            std::lock_guard<std::mutex> lock(sumMutex);
            sum = localSum;
        }
        std::cout << name << " sumó 100 números aleatorios. Total: " << sum << std::endl;
    }

    int getSum() const {
        return sum;
    }

    std::string getName() const {
        return name;
    }
};

int main() {
    // inicializar semilla para rand()
    srand(time(nullptr));
    
    // inicializar threads usando for loop
    std::vector<std::unique_ptr<Summator>> summators;
    std::vector<std::thread> threads;
    
    for (int i = 1; i <= 10; i++) {
        std::string threadName = "Thread-" + std::to_string(i);
        summators.push_back(std::make_unique<Summator>(threadName));
    }
    
    // crear threads usando for loop
    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&summators, i]() { 
            summators[i]->calculateSum(); 
        });
    }
    
    // esperar todos los threads usando for loop
    for (auto& thread : threads) {
        thread.join();
    }
    
    // crear vector de punteros para ordenar
    std::vector<Summator*> threadPtrs;
    for (auto& summator : summators) {
        threadPtrs.push_back(summator.get());
    }
    
    // ordenar por puntuación (mayor a menor)
    std::sort(threadPtrs.begin(), threadPtrs.end(), 
              [](const Summator* a, const Summator* b) {
                  return a->getSum() > b->getSum();
              });
    
    // mostrar solo el primero (el de mayor puntuación)
    std::cout << "El thread con puntuación más alta es: " 
              << threadPtrs[0]->getName() << " con un total de " << threadPtrs[0]->getSum() << std::endl;

    return 0;
}
