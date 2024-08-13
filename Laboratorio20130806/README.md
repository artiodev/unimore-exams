Esame di Laboratorio di Sistemi di Elaborazione Multimediali 06/08/2013 

***Esercizio*** 

Scrivere un programma in grado di eseguire il dump dell'header di immagini codificate secondo lo  standard TIFF e decodificare immagini a livelli di grigio (8 bit per pixel), salvando l’output in  un’immagine PGM. Lo standard TIFF prevede innumerevoli varianti e opzioni, ma per la decodifica ci  limiteremo a considerare solo il sottoinsieme di immagini per cui valgono le seguenti considerazioni: 

Codifica little endian (l’header comincia con II) 

ImageWidth \= larghezza dell'immagine   
ImageLength \= altezza dell'immagine   
BitsPerSample \= 8 (8 bit per pixel)   
Compression \= 1 (nessuna compressione)   
PhotometricInterpretation \= 1 (il valore 0 corrisponde al nero)   
RowsPerStrip \= ImageLength (è presente una sola "strip" che contiene tutta l'immagine) StripByteCounts \= ImageWidth\*ImageLength (la strip è grande come il numero di pixel dell'immagine) 

Il programma deve leggere due parametri da linea di comando: il primo è il nome del file di input, il  secondo il nome del file di output. E' necessario effettuare il dump di quanti più campi possibile, in base  alle proprie capacità, ma almeno bisogna identificare ed estrarre i campi ImageWidth, ImageLength e  StripOffsets per poter accedere ai dati dell'immagine da salvare. 

Vengono fornite le funzioni di base per manipolare immagini e per salvare in PGM. Leggere attentamente  lo standard, soprattutto la struttura dell'header e dell'IDF. Le informazioni necessarie sono nella pare 1  (Baseline TIFF).