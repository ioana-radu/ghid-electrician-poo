# 023. Relee

[Mergi direct la problemele rezolvate](#probleme-rezolvate)

Categoria din capturi — Dispozitive de protecție

## Definiție

Dispozitive ce folosesc un semnal de comandă pentru a comuta un circuit.

## Principiu și teorie

La releul electromagnetic, bobina deplasează contactele COM, NO și NC. Bobina DC poate genera un impuls la decuplare.

## La ce ajută

Interfațezi o comandă slabă cu o sarcină și poți separa electric două circuite în limitele releului.

## Aplicații în viața reală

Termostat, automatizări și module de comandă.

## Exemplu de utilizare

Comandă un releu de 5 V cu tranzistor sau modul compatibil. Într-un circuit cu tranzistor low-side, montează dioda de roată liberă cu catodul la +5 V.

Pentru exemplul cu bobina de 70 Ω, folosește un modul cu driver și diodă integrate sau un MOSFET de canal N compatibil cu tensiunea de comandă. Pentru varianta cu piese discrete, schema de 5 V poate fi verificată în simulator.

~~~text
+5 V ── bobină ── drenă MOSFET
                   sursă MOSFET ── 0 V
Diodă în paralel cu bobina: catod la +5 V, anod la drenă
+5 V ── COM/NO ── 330 Ω ── LED roșu ── 0 V
~~~

Pune o rezistență de 10 kΩ între poartă și 0 V ca să ții tranzistorul oprit fără comandă. Verifică în fișa tranzistorului rezistența în conducție la tensiunea porții pe care o ai. Pentru prima probă folosește numai sarcina LED de 5 V.

## Probleme rezolvate

1. O bobină de 70 Ω cere 5/70 ≈ 71 mA. Un GPIO limitat la 10 mA are nevoie de driver.
2. Cu LED între NO și retur și COM la +5 V, prin rezistor serie, LED-ul se aprinde când bobina închide contactul NO.

[Înapoi la cuprins](../CUPRINS.md)
