import pandas as pd
from sklearn.datasets import make_blobs

numero_punti = 20000     
numero_dimensioni = 2    
numero_cluster = 50       
dispersione = 1.2        

X, _ = make_blobs(
    n_samples=numero_punti, 
    n_features=numero_dimensioni, 
    centers=numero_cluster, 
    cluster_std=dispersione, 
    random_state=42 
)

colonne = [f'X{i+1}' for i in range(numero_dimensioni)]
df = pd.DataFrame(X, columns=colonne)

nome_file = "k50.csv"

df.to_csv(nome_file, index=False)

print(f"Fatto! Dataset generato con successo.")
print(f"I dati grezzi sono stati salvati nel file: {nome_file}")
print("\nEcco un'anteprima delle prime 5 righe (solo coordinate):")
print(df.head())