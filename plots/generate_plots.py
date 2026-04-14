import matplotlib.pyplot as plt

# Asse X: numero di threads
threads = [1, 2, 4, 6, 8, 10]

# Asse Y: valori di speedup per ogni N
speedup_1000 = [1, 1.77, 1.88, 1.69, 1.30, 1.24]
speedup_10000 = [1, 2.06 ,2.89, 3.41, 3.59, 3.52]
speedup_100000 = [1, 2.55, 2.93, 3.61, 4.25, 4.09]
speedup_1000000 = [1, 2.86, 3.23, 3.89, 4.52, 4.42]

# Creazione della figura e impostazione delle dimensioni
plt.figure(figsize=(10, 6))


plt.plot(threads, speedup_1000, marker='o', label='N=1000')
plt.plot(threads, speedup_10000, marker='s', label='N=10000')
plt.plot(threads, speedup_100000, marker='^', label='N=100000')
plt.plot(threads, speedup_1000000, marker='d', label='N=1000000')

plt.title('Grafico Speedup: SoA', fontsize=14, fontweight='bold')
plt.xlabel('Threads', fontsize=12)
plt.ylabel('Speedup', fontsize=12)

plt.grid(True, linestyle='--', alpha=0.7)

plt.xticks(threads)

plt.legend(title="Dimensione dati (N)")

plt.tight_layout()

plt.savefig('grafico_speedup.png', dpi=300, bbox_inches='tight')

plt.show()