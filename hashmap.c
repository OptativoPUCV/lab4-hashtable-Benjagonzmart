#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
  
  if(map==NULL || key==NULL) return;
  if(map->size==map->capacity)
  {
    enlarge(map);
  }
  long posicion=hash(key,map->capacity);
  Pair* par = searchMap(map,key);
  if (par == NULL) {
    // Si la clave no está presente, encuentra la próxima posición disponible
    while (map->buckets[posicion] != NULL) {
        posicion = (posicion + 1) % map->capacity;
    }
    // Crea un nuevo par y lo inserta en la posición encontrada
    map->buckets[posicion] = createPair(key, value);
    map->size++;
  } else {
    // Si la clave está presente, actualiza el valor del par existente
    par->value = value;
  }
  map->current = posicion;

}

void enlarge(HashMap * map) 
{
  enlarge_called = 1; //no borrar (testing purposes)
  Pair** aux = map->buckets;
  long capacidad = map->capacity;
  map->capacity = capacidad * 2;
  map->buckets = (Pair**)malloc(map->capacity * sizeof(Pair*));
  map->size = 0;
  for (long i = 0; i < capacidad; i++)
    {
      if (aux[i] != NULL)
      {
        insertMap(map, aux[i]->key, aux[i]->value);
        
      }
      
    }
  free(aux);

}


HashMap * createMap(long capacity) 
{
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  if (map == NULL)
  {
    return NULL;
  }
  map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
  if (map->buckets == NULL)
  {
    free(map);
    return NULL;
  }
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  return map;
}
  

void eraseMap(HashMap * map,  char * key) 
{
  if(map==NULL || key==NULL)
  {
    return;
  }
  Pair* par=searchMap(map,key);
  if(par!=NULL)
  {
    free(par->key);
    free(par->value);
    par->key = NULL;
    map->size--;
  }

  
}

Pair * searchMap(HashMap * map,  char * key)
{ 
  if(map==NULL || key==NULL)
  {
    return NULL;
  }
  long posicion;
  for (long i = 0; i < map->capacity; i++) {
    posicion = (hash(key, map->capacity) + i) % map->capacity; //busqueda lineal
      if (map->buckets[posicion] != NULL && is_equal(map->buckets[posicion]->key, key)) {
          map->current = posicion;
          return map->buckets[posicion];
      }
  }

  //Si llegamos aquí, no se encontró el par con la clave especificada
  return NULL;
  
}

Pair * firstMap(HashMap * map)
{
  if(map==NULL)
  {
    return NULL;
  }
  for(long i=0;i<map->capacity;i++)
    {
      if(map->buckets[i]!=NULL && map->buckets[i]->key != NULL)
      {
        map->current=i;
        return map->buckets[i];
        
      }
    }
  return NULL;


}

Pair * nextMap(HashMap * map) 
{
  if(map==NULL)
  {
    return NULL;
  }
  for(long i=map->current+1;i<map->capacity;i++)
    {
      if (map->buckets[i]!=NULL && map->buckets[i]->key != NULL)
      {
        map->current=i;
        return map->buckets[i];
      }
      
    }
  
  return NULL;
}
