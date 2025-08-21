#include<iostream>
#include<memory>
#include<thread>
#include<string>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<algorithm>

class Summator {
private:
    std::string name;
    int sum;
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

        sum = localSum;
        
        printf("%s sumó 100 números aleatorios. Total: %d\n", name.c_str(), sum);
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
        int threadIndex = i;  // Capture the current value of i
        threads.emplace_back([&summators, threadIndex]() { 
            summators[threadIndex]->calculateSum(); 
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
    printf("El thread con puntuación más alta es: %s con un total de %d\n", 
           threadPtrs[0]->getName().c_str(), threadPtrs[0]->getSum());
    return 0;
}
