Esame di Laboratorio di Sistemi di Elaborazione Multimediali 09/06/2015
Esercizio
Si scriva un programma a linea di comando che accetti le seguenti opzioni:
bayer\_decode  
Il programma deve gestire la linea di comando e caricare un file in formato PGM (secondo le specifiche
allegate nel file PGM Format Specification.html) in cui è memorizzato un file RAW a 16 bit
per pixel acquisito con un pattern di Bayer del tipo
R G
G B
Il programma deve caricare il file (attenzione alla endianness, leggere bene le specifiche), convertirlo a 8
bit per pixel, salvarlo come .pgm.
Una volta fatto questo, bisogna interpolare i colori mancanti utilizzando l’algoritmo in due passate visto a
lezione:
1\) nella prima passata si ricostruisce il verde per i pixel R e B. Chiamiamo i pixel nel seguente modo:
X1
G2
X3 G4 X5 G6 X7
G8
X9
dove X è un pixel R o B, mentre G è un pixel verde. L’algoritmo calcola i due valori seguenti:
Δ𝐻 \= \|𝐺4−𝐺6\|\+\|𝑋5−𝑋3\+𝑋5−𝑋7\|
Δ𝑉 \= \|𝐺2−𝐺8\|\+\|𝑋5−𝑋1\+𝑋5−𝑋9\|
e interpola il verde in posizione G5 così:
𝐺4\+𝐺6 𝑋5−𝑋3\+𝑋5−𝑋7
\+ Δ𝐻 \< Δ𝑉
2 4
𝐺2\+𝐺8 𝑋5−𝑋1\+𝑋5−𝑋9
𝐺5 \= \+ Δ𝐻 \> Δ𝑉
2 4
𝐺2\+𝐺4\+𝐺6\+𝐺8 𝑋5−𝑋1\+𝑋5−𝑋3\+𝑋5−𝑋7\+𝑋5−𝑋9
\+ Δ𝐻 \= Δ𝑉
{ 4 8
2\) nella seconda passata si interpolano i valori restanti nel seguente modo: per i pixel verdi si interpolano
R e B come media dei due R e B vicini (destra/sinistra, alto/basso), mentre in modo analogo a prima, per i
pixel rossi e blu si definisce una griglia:
1 2 3
4 5 6
7 8 9
se il pixel 5 è R, bisogna interpolare il B e viceversa. L’algoritmo calcola i due valori seguenti:
Δ𝑁 \= \|𝑋1−𝑋9\|\+\|𝐺5−𝐺1\+𝐺5−𝐺9\|
Δ𝑃 \= \|𝑋3−𝑋7\|\+\|𝐺5−𝐺3\+𝐺5−𝐺7\|
e interpola il valore in posizione 5 così:
𝑋1\+𝑋9 𝐺5−𝐺1\+𝐺5−𝐺9
\+ Δ𝑁 \< Δ𝑃
2 4
𝑋3\+𝑋7 𝐺5−𝐺3\+𝐺5−𝐺7
𝑋5 \= \+ Δ𝑁 \> Δ𝑃
2 4
𝑋1\+𝑋3\+𝑋7\+𝑋9 𝐺5−𝐺1\+𝐺5−𝐺3\+𝐺5−𝐺7\+𝐺5−𝐺9
\+ Δ𝑁 \= Δ𝑃
{ 4 8
Attenzione a saturare tra 0 e 255 i valori prodotti con questo algoritmo prima di metterli in un byte.
Il programma deve caricare interpolare il pattern e salvarlo come .ppm (secondo le
specifiche allegate nel file PPM Format Specification.html), come file a colori con 8 bit per
canale.
Pagina 1 di 1