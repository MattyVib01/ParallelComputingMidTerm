# Implementazione K-Means con OpenMP: Sequenziale vs Parallela

Il seguente progetto è un lavoro per il corso di Parallel Computing per i corso di Ingegneria Informatica presso l'Università degli Studi di Firenze.
Questo repository contiene i file necessari ad eseguire gli eseperimenti illustrati nella relazione `main.pdf`


Questo progetto implementa l'algoritmo K-Means, un noto metodo di apprendimento non supervisionato che permette di partizionare un insieme di punti, appartenenti a un determinato spazio dimensionale, in un numero definito di cluster (K). 

Lo scopo principale del repository è valutare e confrontare l'implementazione classica sequenziale dell'algoritmo con le relative versioni parallele sviluppate tramite il framework OpenMP.

## Struttura dell'Algoritmo

L'algoritmo cerca di minimizzare la varianza all'interno di ogni gruppo assegnando a ciascun punto il centroide più vicino. Il processo si sviluppa in tre fasi principali:

* **Inizializzazione:** Vengono selezionati casualmente K punti all'interno del dataset per fungere da centroidi iniziali.
* **Assegnamento:** L'algoritmo individua per ogni punto il centroide più vicino e gli assegna il rispettivo cluster.
* **Aggiornamento:** Si calcola la media delle posizioni dei punti appartenenti a ciascun cluster per aggiornare le coordinate del relativo centroide.

L'algoritmo si arresta in convergenza, ovvero quando nessun punto cambia cluster dopo un'iterazione completa di assegnazione.

## Strategie di Parallelizzazione

Poiché all'aumentare della dimensionalità e del numero di punti i tempi di esecuzione dell'algoritmo sequenziale si dilatano, il codice sfrutta le direttive di OpenMP per parallelizzare i calcoli. 

Sono stati studiati due diversi approcci di gestione della memoria:

* **Array of Structures (AoS):** È l'approccio più fedele alla versione sequenziale, in cui le coordinate del singolo punto sono organizzate spazialmente vicine in memoria. 
* **Structure of Arrays (SoA):** Questo approccio separa le singole componenti dei punti in array distinti. Questa organizzazione garantisce ai thread la possibilità di leggere blocchi contigui di memoria durante la valutazione delle distanze, migliorando l'efficienza.
