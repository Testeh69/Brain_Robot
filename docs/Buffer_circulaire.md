# Buffer Circulaire

## 1 Introduction

**Buffer circulaire** ou ring buffer est une étape pour gérer les flux de données en temps réel pour les communications UART.
L'idée est d'utiliser un tableau de taille fixe bouclé sur lui meme, d'ou le nom circulaire.

### Le concept visuel

Imagine un cercle divisé en cases. Tu as deux curseurs (index) :

Write (Écriture) : Là où tu ajoutes de nouvelles données.

Read (Lecture) : Là où tu récupères les données.

Le buffer est vide quand Read == Write.
Le buffer est plein quand Write rattrape Read (par l'arrière).

## Méthodes

**Définir une taille**:

Si la taille de ton buffer est une puissance de 2 (ex: 16, 64, 256), tu peux remplacer le modulo % (lent pour le CPU) par un ET binaire (&), beaucoup plus rapide :

``` C++

// Au lieu de : index = (index + 1) % 8;
index = (index + 1) & (BUFFER_SIZE - 1);
```

**Ajouter une méthode push (écriture de la donnée)**:

``` C++

   void push(int value){
            if (!full){
                buffer[write] = value;
                write = (write + 1) % size;
                updateFull();
            }
           
       }
```

**Ajouter une méthode pop (récupération de la donnée)**:

``` C++
int pop(){
            if (write == read && !full) {
                // Buffer vide
                return -1; // ou une autre valeur pour indiquer que le buffer est vide
            }
            int value = buffer[read];
            read = (read + 1) % size;
            full = false;
            return value;
                
       }       
```
