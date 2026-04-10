import pandas as pd
from sklearn.datasets import make_blobs

# --- 1. Configurazione dei parametri ---
numero_punti = 1000000      
numero_dimensioni = 2    
numero_cluster = 10       
dispersione = 1.2        

# --- 2. Generazione del dataset ---
# Ora estraiamo solo X (le coordinate). 
# Usiamo '_' per indicare a Python di ignorare le etichette dei cluster originali.
X, _ = make_blobs(
    n_samples=numero_punti, 
    n_features=numero_dimensioni, 
    centers=numero_cluster, 
    cluster_std=dispersione, 
    random_state=42 
)

# --- 3. Strutturazione dei dati ---
# Creiamo il DataFrame contenente esclusivamente le coordinate
colonne = [f'X{i+1}' for i in range(numero_dimensioni)]
df = pd.DataFrame(X, columns=colonne)

# --- 4. Esportazione in CSV ---
nome_file = "n10000002.csv"

# Salviamo il file senza gli indici di riga
df.to_csv(nome_file, index=False)

print(f"Fatto! Dataset generato con successo.")
print(f"I dati grezzi sono stati salvati nel file: {nome_file}")
print("\nEcco un'anteprima delle prime 5 righe (solo coordinate):")
print(df.head())