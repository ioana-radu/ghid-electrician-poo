# 006. Circuit electric

[Mergi direct la problemele rezolvate](#probleme-rezolvate)

Categoria din capturi — Bazele electricității

## Definiție

Ansamblu de elemente legate prin care curentul poate circula într-o buclă închisă.

## Principiu și teorie

Nodul unește borne, ramura leagă noduri, iar ochiul reprezintă un traseu închis. O ramură deschisă are curent zero în modelul ideal.

## La ce ajută

Citești scheme și localizezi întreruperi sau conexiuni greșite.

## Aplicații în viața reală

Lanternă cu întrerupător și placă electronică cu mai multe ramuri.

## Exemplu de utilizare

Leagă +5 V → întrerupător → 330 Ω → LED roșu → 0 V. Identifică nodurile înainte de alimentare.

~~~text
+5 V ── S1 ── 330 Ω ── LED (anod spre S1) ── 0 V
~~~

1. Cu sursa oprită, verifică pe breadboard că bornele rezistorului sunt pe rânduri distincte. Conectează catodul LED-ului la retur. Identifică-l după marcaj sau în fișa piesei.
2. Setează 5 V și limita sursei la aproximativ 20 mA. Alimentează și compară starea LED-ului cu S1 deschis și închis.
3. Măsoară tensiunea pe rezistor, cu multimetrul configurat ca voltmetru. Dacă ai aproximativ 3 V, curentul este aproximativ 9 mA. Dacă LED-ul nu luminează, oprește sursa și verifică polaritatea, rândurile și continuitatea întrerupătorului.

## Probleme rezolvate

1. Contact deschis, model ideal. I = 0, deci căderea pe rezistor este 0 V.
2. Cu contact închis și LED de 2 V, I ≈ (5 − 2)/330 = 9,1 mA.

[Înapoi la cuprins](../CUPRINS.md)
