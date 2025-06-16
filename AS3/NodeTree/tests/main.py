import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

# Configurar estilo
plt.style.use('seaborn-v0_8')
sns.set_palette("husl")

def load_and_plot_results(filename='tree_benchmark_results.csv'):
    """
    Carga los resultados del benchmark y genera los gráficos solicitados
    """

    # Cargar datos
    try:
        df = pd.read_csv(filename)
        print(f"Datos cargados exitosamente: {len(df)} experimentos")
        print(f"Tamaños de prueba: {df['n'].tolist()}")
    except FileNotFoundError:
        print(f"Error: No se encontró el archivo {filename}")
        print("Asegúrate de ejecutar primero el programa C++ para generar los datos.")
        return

    # Configurar la figura con subplots
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    fig.suptitle('Comparación de Rendimiento: Node Trees vs Leaf Trees', fontsize=16, fontweight='bold')

    # Convertir tiempos de nanosegundos a microsegundos para mejor legibilidad
    time_cols = [col for col in df.columns if 'median' in col]
    for col in time_cols:
        df[col] = df[col] / 1000  # ns to μs

    # a) Tiempo de inserción
    ax1 = axes[0, 0]
    ax1.plot(df['n'], df['nodeTree_insert_median'], 'o-', label='Node Tree', linewidth=2, markersize=6)
    ax1.plot(df['n'], df['leafTree_insert_median'], 's-', label='Leaf Tree', linewidth=2, markersize=6)
    ax1.set_xlabel('Tamaño del árbol (n)')
    ax1.set_ylabel('Tiempo mediano de inserción (μs)')
    ax1.set_title('a) Tiempo de Inserción')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    ax1.set_xscale('log')
    ax1.set_yscale('log')

    # b) Tiempo de búsqueda exitosa
    ax2 = axes[0, 1]
    ax2.plot(df['n'], df['nodeTree_successful_search_median'], 'o-', label='Node Tree', linewidth=2, markersize=6)
    ax2.plot(df['n'], df['leafTree_successful_search_median'], 's-', label='Leaf Tree', linewidth=2, markersize=6)
    ax2.set_xlabel('Tamaño del árbol (n)')
    ax2.set_ylabel('Tiempo mediano de búsqueda exitosa (μs)')
    ax2.set_title('b) Tiempo de Búsqueda Exitosa')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    ax2.set_xscale('log')
    ax2.set_yscale('log')

load_and_plot_results()
