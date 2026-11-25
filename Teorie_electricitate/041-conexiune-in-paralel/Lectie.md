# 041. Conexiune în paralel

[Mergi direct la problemele rezolvate](#probleme-rezolvate)

Categoria din capturi — Circuite în serie și în paralel

## Definiție

Elemente conectate la aceeași pereche de noduri.

## Principiu și teorie

Tensiunea e comună, iar curenții se adună. Pentru rezistoare, 1/R echiv = suma 1/R.

## La ce ajută

Alimentezi independent sarcini la aceeași tensiune.

## Aplicații în viața reală

Ramuri separate de iluminat și module pe magistrala de alimentare.

## Exemplu de utilizare

La 5 V, construiește două ramuri cu câte un LED și câte un rezistor de 330 Ω per LED.

~~~text
             ┌── 330 Ω ── LED1 ──┐
+5 V ────────┤                   ├── 0 V
             └── 330 Ω ── LED2 ──┘
~~~

1. Orientează anozii către +5 V și dă fiecărui LED propriul rezistor, deoarece căderile directe pot fi diferite.
2. Măsoară pe fiecare rezistor și calculează curenții din căderile de tensiune.
3. Oprește sursa și scoate o ramură. La repornire, cealaltă continuă să lumineze. Pentru LED-uri roșii de circa 2 V, curentul total scade de la aproximativ 18 mA la 9 mA.

## Probleme rezolvate

1. Două rezistoare de 1 kΩ în paralel au R echiv = 500 Ω. La 5 V consumă total 10 mA.
2. Elimini o ramură. Rezistența revine la 1 kΩ și curentul sursei scade la 5 mA.

[Înapoi la cuprins](../CUPRINS.md)
