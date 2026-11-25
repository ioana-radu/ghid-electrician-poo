# 096. Componente electronice

[Mergi direct la problemele rezolvate](#probleme-rezolvate)

Categoria din capturi — Componente electronice

## Definiție

Elemente precum rezistor, condensator, inductor, diodă, tranzistor și circuit integrat.

## Principiu și teorie

Rezistorul disipă energie, condensatorul stochează energie electrică, inductorul energie magnetică, dioda conduce preferențial într-un sens, iar tranzistorul controlează un curent.

## La ce ajută

Alegi piese după funcție și limite și construiești circuite de semnal sau comandă.

## Aplicații în viața reală

Filtru RC, indicator LED și comandă tranzistorizată a unui releu.

## Exemplu de utilizare

Realizează un filtru trece-jos cu 10 kΩ în serie și 100 nF către masă. Ieșirea se ia pe condensator.

~~~text
Vin ── 10 kΩ ──●── Vout
               │
             100 nF
               │
              0 V
~~~

1. Folosește un condensator ceramic de 100 nF. În simulator, aplică un semnal sinusoidal de amplitudine mică și măsoară raportul Vout/Vin.
2. Compară valorile la 16 Hz, 159 Hz și 1,59 kHz. Modelul prezice aproximativ 0,995, 0,707 și 0,100.
3. Aplică o treaptă de 0–5 V și observă Vout. După 1 ms ar trebui să fie aproximativ 3,16 V. Dacă măsori fizic o astfel de tranziție, folosește un osciloscop sau un instrument suficient de rapid.

## Probleme rezolvate

1. Constanta de timp τ = 10 kΩ × 100 nF = 1 ms.
2. Frecvența de tăiere este f_c = 1/(2πτ) ≈ 159 Hz pentru sarcină de măsurare cu impedanță mare.

[Înapoi la cuprins](../CUPRINS.md)
