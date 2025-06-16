#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include "nodeTree.h"
#include "leafTree.h"

using namespace std;
using namespace std::chrono;

struct TestResult {
  int n;
  double nodeTree_insert_median;
  double leafTree_insert_median;
  double nodeTree_successful_search_median;
  double leafTree_successful_search_median;
  double nodeTree_unsuccessful_search_median;
  double leafTree_unsuccessful_search_median;
  double nodeTree_delete_median;
  double leafTree_delete_median;
};

// Función para calcular la mediana
double calculateMedian(vector<double>& times) {
  sort(times.begin(), times.end());
  size_t size = times.size();
  if (size % 2 == 0) {
    return (times[size/2 - 1] + times[size/2]) / 2.0;
  } else {
    return times[size/2];
  }
}

// Función para generar números aleatorios únicos
vector<int> generateRandomArray(int n, int min_val = 1, int max_val = 1000000000) {
  vector<int> arr;
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(min_val, max_val);

  while (arr.size() < n) {
    int val = dis(gen);
    if (find(arr.begin(), arr.end(), val) == arr.end()) {
      arr.push_back(val);
    }
  }
  return arr;
}

// Función para generar valores que no están en el array
vector<int> generateNonExistentValues(const vector<int>& existing, int count, int min_val = 1, int max_val = 1000000000) {
  vector<int> non_existent;
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(min_val, max_val);

  while (non_existent.size() < count) {
    int val = dis(gen);
    if (find(existing.begin(), existing.end(), val) == existing.end() &&
	find(non_existent.begin(), non_existent.end(), val) == non_existent.end()) {
      non_existent.push_back(val);
    }
  }
  return non_existent;
}

TestResult runExperiment(int n) {
  TestResult result;
  result.n = n;

  cout << "Running experiment for n = " << n << "..." << endl;

  // 1. Crear árboles vacíos
  nodeTree<int, int> nTree;
  leafTree<int, int> lTree;

  // 2. Generar array aleatorio
  vector<int> values = generateRandomArray(n);

  // 3. Test de inserción
  vector<double> nodeInsertTimes, leafInsertTimes;

  for (int val : values) {
    // NodeTree insertion
    auto start = high_resolution_clock::now();
    nTree.insert(val, val);
    auto end = high_resolution_clock::now();
    nodeInsertTimes.push_back(duration_cast<nanoseconds>(end - start).count());

    // LeafTree insertion
    start = high_resolution_clock::now();
    lTree.insert(val, val);
    end = high_resolution_clock::now();
    leafInsertTimes.push_back(duration_cast<nanoseconds>(end - start).count());
  }

  result.nodeTree_insert_median = calculateMedian(nodeInsertTimes);
  result.leafTree_insert_median = calculateMedian(leafInsertTimes);

  // 4. Test de búsqueda exitosa
  vector<double> nodeSuccessfulSearchTimes, leafSuccessfulSearchTimes;

  for (int val : values) {
    // NodeTree search
    auto start = high_resolution_clock::now();
    nTree.find(val);
    auto end = high_resolution_clock::now();
    nodeSuccessfulSearchTimes.push_back(duration_cast<nanoseconds>(end - start).count());

    // LeafTree search
    start = high_resolution_clock::now();
    lTree.find(val);
    end = high_resolution_clock::now();
    leafSuccessfulSearchTimes.push_back(duration_cast<nanoseconds>(end - start).count());
  }

  result.nodeTree_successful_search_median = calculateMedian(nodeSuccessfulSearchTimes);
  result.leafTree_successful_search_median = calculateMedian(leafSuccessfulSearchTimes);

  // 5. Test de búsqueda no exitosa
  vector<int> nonExistent = generateNonExistentValues(values, n + 10);
  vector<double> nodeUnsuccessfulSearchTimes, leafUnsuccessfulSearchTimes;

  for (int val : nonExistent) {
    // NodeTree search
    auto start = high_resolution_clock::now();
    nTree.find(val);
    auto end = high_resolution_clock::now();
    nodeUnsuccessfulSearchTimes.push_back(duration_cast<nanoseconds>(end - start).count());

    // LeafTree search
    start = high_resolution_clock::now();
    lTree.find(val);
    end = high_resolution_clock::now();
    leafUnsuccessfulSearchTimes.push_back(duration_cast<nanoseconds>(end - start).count());
  }

  result.nodeTree_unsuccessful_search_median = calculateMedian(nodeUnsuccessfulSearchTimes);
  result.leafTree_unsuccessful_search_median = calculateMedian(leafUnsuccessfulSearchTimes);

  // 6. Test de eliminación
  vector<int> deleteOrder = values;
  random_device rd;
  mt19937 g(rd());
  shuffle(deleteOrder.begin(), deleteOrder.end(), g);

  vector<double> nodeDeleteTimes, leafDeleteTimes;

  for (int val : deleteOrder) {
    // NodeTree deletion
    auto start = high_resolution_clock::now();
    nTree.deleteNode(val);
    auto end = high_resolution_clock::now();
    nodeDeleteTimes.push_back(duration_cast<nanoseconds>(end - start).count());

    // LeafTree deletion
    start = high_resolution_clock::now();
    lTree.deleteNode(val);
    end = high_resolution_clock::now();
    leafDeleteTimes.push_back(duration_cast<nanoseconds>(end - start).count());
  }

  result.nodeTree_delete_median = calculateMedian(nodeDeleteTimes);
  result.leafTree_delete_median = calculateMedian(leafDeleteTimes);

  return result;
}

int main() {
  vector<TestResult> results;
  vector<int> test_sizes;

  // Generar tamaños de prueba de 1 a 10^5
  for (int i = 1; i <= 10000; i *= 10) {
    test_sizes.push_back(i);
    if (i == 1) {
      test_sizes.push_back(10);
      test_sizes.push_back(100);
    } else if (i == 10) {
      test_sizes.push_back(50);
    } else if (i == 100) {
      test_sizes.push_back(500);
    } else if (i == 1000) {
      test_sizes.push_back(5000);
    } else if (i == 10000) {
      test_sizes.push_back(100000);
    }
  }

  sort(test_sizes.begin(), test_sizes.end());

  cout << "Starting Tree Performance Benchmark..." << endl;
  cout << "Test sizes: ";
  for (int size : test_sizes) {
    cout << size << " ";
  }
  cout << endl << endl;

  for (int n : test_sizes) {
    TestResult result = runExperiment(n);
    results.push_back(result);
  }

  // Escribir resultados a archivo CSV
  ofstream outFile("tree_benchmark_results.csv");
  outFile << "n,nodeTree_insert_median,leafTree_insert_median,";
  outFile << "nodeTree_successful_search_median,leafTree_successful_search_median,";
  outFile << "nodeTree_unsuccessful_search_median,leafTree_unsuccessful_search_median,";
  outFile << "nodeTree_delete_median,leafTree_delete_median" << endl;

  for (const TestResult& result : results) {
    outFile << result.n << ",";
    outFile << result.nodeTree_insert_median << ",";
    outFile << result.leafTree_insert_median << ",";
    outFile << result.nodeTree_successful_search_median << ",";
    outFile << result.leafTree_successful_search_median << ",";
    outFile << result.nodeTree_unsuccessful_search_median << ",";
    outFile << result.leafTree_unsuccessful_search_median << ",";
    outFile << result.nodeTree_delete_median << ",";
    outFile << result.leafTree_delete_median << endl;
  }

  outFile.close();

  // Mostrar tabla de resultados
  cout << "\n" << setw(8) << "n" 
    << setw(15) << "NT Insert" 
    << setw(15) << "LT Insert" 
    << setw(15) << "NT Search+" 
    << setw(15) << "LT Search+" 
    << setw(15) << "NT Search-" 
    << setw(15) << "LT Search-" 
    << setw(15) << "NT Delete" 
    << setw(15) << "LT Delete" << endl;

  cout << string(120, '-') << endl;

  for (const TestResult& result : results) {
    cout << setw(8) << result.n
      << setw(15) << fixed << setprecision(2) << result.nodeTree_insert_median
      << setw(15) << result.leafTree_insert_median
      << setw(15) << result.nodeTree_successful_search_median
      << setw(15) << result.leafTree_successful_search_median
      << setw(15) << result.nodeTree_unsuccessful_search_median
      << setw(15) << result.leafTree_unsuccessful_search_median
      << setw(15) << result.nodeTree_delete_median
      << setw(15) << result.leafTree_delete_median << endl;
  }

  cout << "\nResults saved to 'tree_benchmark_results.csv'" << endl;
  cout << "Use the Python script to generate plots." << endl;

  return 0;
}
