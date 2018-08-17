# Traductor
Proyecto para la materia traductores de lenguajes 2 UDG CUCEI

Se honesto contigo, no vas a aprender nada si solo copias el proyecto y lo entregas. porfa no lo hagas.

La gramatica que acepta de entrada son las siguientes sentencias

Asignacion:
  ID = "cadena";
  
  ID = REAL + REAL - REAL * REAL / (REAL % REAL);
  
  ID = ENTERO + ENTERO - ENTERO * ENTERO / (ENTERO % ENTERO);
  
  Tambien acepta ID's en la expresión mientras sean del mismo tipo.
  Los numeros pueden ser negativos
  
Print:

print(ID);

print(expresión); //Como las expresiónes que use en los ejemplos de asignacion

print("cadena");

If:

if(condicion)

{

  sentencia
  
}

else

{

}

puede ser cualquier cantidad de sentencias (tambien 0)
puede no llevar llaves pero solo acepta una sentencia
la condicion es una expresión seguida de un simbolo comparativo y otra expresion despues.
Los simbolos comparativos que acepta son:
<, >, <=, >=, ==, !=

else es opcional.
else tambien puede usarse sin llaves, aceptando solo una sentencia.

While:

while(condicion)

{

}

Recibe una condicion de la misma manera que la sentencia if.
Al igual que la sentencia if puede llevar 0 o muchas sentencias, o una sentencia si no se ponen llaves.
