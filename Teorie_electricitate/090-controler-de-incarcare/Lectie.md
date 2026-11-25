# 090. Controler de încărcare

[Mergi direct la problemele rezolvate](#probleme-rezolvate)

Categoria din capturi — Energie solară

## Definiție

Dispozitiv care gestionează încărcarea bateriei de la panou.

## Principiu și teorie

PWM conectează panoul periodic la baterie. MPPT caută punctul optim al panoului și folosește conversie DC–DC. Profilul depinde de chimia bateriei.

## La ce ajută

Eviți supraîncărcarea și alegi tensiunile și curentul admisibile.

## Aplicații în viața reală

Sistem solar autonom pentru senzori și iluminat.

## Exemplu de utilizare

Folosește simulatorul sau un kit educațional complet cu baterie compatibilă și limite de intrare specificate.

## Probleme rezolvate

1. Panou de 90 W, MPPT cu η = 95%, baterie la 12,5 V. Curent idealizat ≈ 90 × 0,95/12,5 = 6,84 A.
2. Dacă regulatorul poate furniza 5 A la 12,5 V, puterea maximă livrată la acel punct este 62,5 W.

[Înapoi la cuprins](../CUPRINS.md)
