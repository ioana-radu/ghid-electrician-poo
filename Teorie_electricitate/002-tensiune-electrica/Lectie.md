# 002. Tensiune electrică

[Mergi direct la problemele rezolvate](#probleme-rezolvate)

Categoria din capturi — Bazele electricității

## Definiție

Diferență de potențial între două puncte, măsurată în volți.

## Principiu și teorie

1 V înseamnă 1 J/C. Semnul măsurat depinde de ordinea punctelor. Voltmetrul se conectează în paralel.

## La ce ajută

Verifici alimentarea și căderile de tensiune sub sarcină.

## Aplicații în viața reală

O baterie descărcată poate părea bună în gol. Un divizor produce o referință.

## Exemplu de utilizare

Pune două rezistoare de 1 kΩ în serie la 5 V și măsoară de la mijloc la minus.

~~~text
+5 V ── 1 kΩ ──●── 1 kΩ ── 0 V
                Vout
~~~

1. Configurează voltmetrul pe tensiune DC. Pune sonda neagră la 0 V și pe cea roșie în nodul Vout.
2. Adaugă, cu sursa oprită, un rezistor de 1 kΩ între Vout și 0 V. Brațul inferior devine 500 Ω.
3. Repornește și măsoară. Noua tensiune este 5 × 500/1500 ≈ 1,67 V. Diferența față de 2,5 V arată efectul sarcinii asupra divizorului.

## Probleme rezolvate

1. Care este tensiunea la mijloc fără sarcină? 5 × 1 / (1 + 1) = 2,5 V.
2. Sursei de 5 V îi urmează o cădere de 0,4 V pe fir. Sarcina primește 5 − 0,4 = 4,6 V.

[Înapoi la cuprins](../CUPRINS.md)
