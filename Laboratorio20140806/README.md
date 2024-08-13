Esame di Laboratorio di Sistemi di Elaborazione Multimediali 06/08/2014 

***Esercizio*** 

L’algoritmo Adam7 è una tecnica per il caricamento progressivo di immagini utilizzata nel formato PNG.  Questa consiste nello spezzare l’immagine in 7 sotto parti, applicando il seguente pattern su tutti i pixel di  una immagine: 

| 1  | 6  | 4  | 6  | 2  | 6  | 4  | 6 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 7  | 7  | 7  | 7  | 7  | 7  | 7  | 7 |
| 5  | 6  | 5  | 6  | 5  | 6  | 5  | 6 |
| 7  | 7  | 7  | 7  | 7  | 7  | 7  | 7 |
| 3  | 6  | 4  | 6  | 3  | 6  | 4  | 6 |
| 7  | 7  | 7  | 7  | 7  | 7  | 7  | 7 |
| 5  | 6  | 5  | 6  | 5  | 6  | 5  | 6 |
| 7  | 7  | 7  | 7  | 7  | 7  | 7  | 7 |

In questo modo è possibile ricostruire (in modo grezzo) l’immagine utilizzando solo i valori fino ad un  certo livello, visualizzando quindi un’anteprima man mano che avviene il caricamento, oppure disporre di  una versione ridotta dell’immagine scaricando solo la parte che interessa. 

Si scriva un programma a linea di comando che accetti le seguenti opzioni: 

multires c \<input file .PGM\> \<output file .MLT\> 

multires d \<input file .MLT\> \<output prefix\> 

Quando viene specificata l’opzione “c” il programma apre il file .PGM e crea il file in formato .MLT  strutturato come segue: 

| Campo  | Dimensione  | Descrizione |
| :---- | :---- | :---- |
| Magic Number  | 8 byte  | “MULTIRES” |
| Width  | intero senza segno a 32 bit little  endian | Larghezza in pixel |
| Height  | intero senza segno a 32 bit little  endian | Altezza in pixel |
| Level 1  | dipende dal Width e Height  | tutti i pixel per righe che corrispondono al  valore 1 nel pattern Adam7  |
| Level 2  | dipende dal Width e Height  | tutti i pixel per righe che corrispondono al  valore 2 nel pattern Adam7 |
| …  | …  | … |
| Level 7  | dipende dal Width e Height  | tutti i pixel per righe che corrispondono al  valore 7 nel pattern Adam7 |

Quando viene specificata l’opzione “d”, il programma apre il file .MLT e crea 7 file .PGM corrispondenti  all’immagine ricostruita utilizzando solo il livello 1, poi i livelli 1 e 2, poi i livelli 1, 2 e 3 e così via. Se il  prefisso fosse “rec”, il programma produrrebbe “rec\_1.pgm”, “rec\_2.pgm” e così via fino a “rec\_7.pgm”. 

Per la ricostruzione utilizzare una tecnica senza interpolazione (in grassetto il valore realmente  disponibile):  

∙ se si dispone solo del livello 1, l’intero blocco deve essere riempito con quel valore:

| a  | a  | a  | a  | a  | a  | a  | a |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| a  | a  | a  | a  | a  | a  | a  | a |
| a  | a  | a  | a  | a  | a  | a  | a |

| a  | a  | a  | a  | a  | a  | a  | a |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| a  | a  | a  | a  | a  | a  | a  | a |
| a  | a  | a  | a  | a  | a  | a  | a |
| a  | a  | a  | a  | a  | a  | a  | a |
| a  | a  | a  | a  | a  | a  | a  | a |

∙ se è disponibile il livello 2, le prime quattro colonne di ogni blocco sono riempite con il valore del  pixel 1, mentre le altre quattro con quello del pixel 2: 

| a  | a  | a  | a  | b  | b  | b  | b |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| a  | a  | a  | a  | b  | b  | b  | b |
| a  | a  | a  | a  | b  | b  | b  | b |
| a  | a  | a  | a  | b  | b  | b  | b |
| a  | a  | a  | a  | b  | b  | b  | b |
| a  | a  | a  | a  | b  | b  | b  | b |
| a  | a  | a  | a  | b  | b  | b  | b |
| a  | a  | a  | a  | b  | b  | b  | b |

∙ se è disponibile il livello 3, i due blocchi 4×4 in basso saranno riempiti rispettivamente con il  valore del proprio pixel in alto a sinistra: 

| a  | a  | a  | a  | b  | b  | b  | b |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| a  | a  | a  | a  | b  | b  | b  | b |
| a  | a  | a  | a  | b  | b  | b  | b |
| a  | a  | a  | a  | b  | b  | b  | b |
| **c**  | c  | c  | c  | **d**  | d  | d  | d |
| c  | c  | c  | c  | d  | d  | d  | d |
| c  | c  | c  | c  | d  | d  | d  | d |
| c  | c  | c  | c  | d  | d  | d  | d |

∙ e così via per i livelli 4, 5, 6 e 7 (non specificato, perché a quel punto i pixel sono tutti diversi): 

| a  | a  | e  | e  | b  | b  | f  | f  |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| a  | a  | e  | e  | b  | b  | f  | f  |
| a  | a  | e  | e  | b  | b  | f  | f  |
| a  | a  | e  | e  | b  | b  | f  | f  |
| **c**  | c  | **g**  | g  | **d**  | d  | **h**  | h  |
| c  | c  | g  | g  | d  | d  | h  | h  |
| c  | c  | g  | g  | d  | d  | h  | h  |
| c  | c  | g  | g  | d  | d  | h  | h  |

| a  | a  | e  | e  | b  | b  | f  | f |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| a  | a  | e  | e  | b  | b  | f  | f |
| **i**  | i  | **j**  | j  | **k**  | k  | **l**  | l |
| i  | i  | j  | j  | k  | k  | l  | l |
| **c**  | c  | **g**  | g  | **d**  | d  | **h**  | h |
| c  | c  | g  | g  | d  | d  | h  | h |
| **m**  | m  | **n**  | n  | **o**  | o  | **p**  | p |
| m  | m  | n  | n  | o  | o  | p  | p |

| a  | q  | e  | r  | b  | s  | f  | t  |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| a  | q  | e  | r  | b  | s  | f  | t  |
| **i**  | **u**  | **j**  | **v**  | **k**  | **w**  | **l**  | **x**  |
| i  | u  | j  | v  | k  | w  | l  | x  |
| **c**  | **y**  | **g**  | **z**  | **d**  | **1**  | **h**  | **2**  |
| c  | y  | g  | z  | d  | l  | h  | 2  |
| **m**  | **3**  | **n**  | **4**  | **o**  | **5**  | **p**  | **6**  |
| m  | 3  | n  | 4  | o  | 5  | p  | 6  |

| …  | …  | …  | …  | …  | …  | …  | … |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| …  | …  | …  | …  | …  | …  | …  | … |
| …  | …  | …  | …  | …  | …  | …  | … |
| …  | …  | …  | …  | …  | …  | …  | … |
| …  | …  | …  | …  | …  | …  | …  | … |
| …  | …  | …  | …  | …  | …  | …  | … |
| …  | …  | …  | …  | …  | …  | …  | … |
| …  | …  | …  | …  | …  | …  | …  | … |

Suggerimento: nel preparare la soluzione gestite prima l’opzione “c”, poi l’opzione “d” decodificando  solo la versione completa “rec\_7.pgm” ed infine fate gli altri livelli che richiedono di replicare i pixel  decodificati nei vicini. 
