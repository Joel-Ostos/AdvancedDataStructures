import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('./datos.csv')
df = df.sort_values('n')

def plot_series(x, y1, y2, title, ylabel):
    plt.figure()
    plt.plot(x, y1, label='Node-Tree', alpha=0.5)
    plt.plot(x, y2, label='Leaf-Tree', alpha=0.5)
    plt.title(title)
    plt.xlabel('n (número de claves)')
    plt.ylabel(ylabel)
    plt.legend()
    plt.grid(True)
    plt.show()

# a) Inserción
plot_series(df['n'], df['ins_node'], df['ins_leaf'], 'Mediana de inserción vs n', 'tiempo (ns)')

# b) Búsqueda exitosa
plot_series(df['n'], df['find_ok_node'], df['find_ok_leaf'], 'Mediana de búsqueda exitosa vs n', 'tiempo (ns)')

# c) Búsqueda fallida
plot_series(df['n'], df['find_fail_node'], df['find_fail_leaf'], 'Mediana de búsqueda fallida vs n', 'tiempo (ns)')

# d) Borrado
plot_series(df['n'], df['del_node'], df['del_leaf'], 'Mediana de borrado vs n', 'tiempo (ns)')

