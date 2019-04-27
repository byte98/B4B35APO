// Jednoduchy tridici algoritmus - selection sort

.globl    pole
.data
.align    2

// Pro spusteni je nutne nastavit jadro s delay-slotem

pole:
.word    5, 3, 4, 1, 15, 8, 9, 2, 10, 6, 11, 1, 6, 9, 12

.text
.globl _start
.ent _start

_start:

addi $s0, $0, 0  //Pozice pro kterou se aktualne hleda minimum (offset do pole, zvysuje se po 4 bajtech)
addi $s1, $0, 60 //Maximalni hodnota indexu/offsetu. Slouzi k ukonceni cyklu = pocet prvku v poli * 4 (aktualne 15 * 4)
add  $s2, $0, $s0 //Pracovni pozice (offset), prvek 
// $s3 - offset nejmensiho nalezeneho prvku v aktualnim behu
// $s4 - hodnota nejmensiho nalezeneho prvku
// $s5 - tmp

hlavni_cyklus:
	beq $s0, $s1, hlavni_cyklus_end
	
	lw $s4, pole($s0)
	add $s3, $s0, $0
	add $s2, $s0, $0
	
vnitrni_cyklus:
	beq $s2, $s1, vnitrni_cyklus_end
		
		lw $s5, pole($s2)
		
		bgt $s5, $s4, neni_minimum
			addi $s3, $s2, 0
			addi $s4, $s5, 0
neni_minimum:
		addi $s2, $s2, 4
		j vnitrni_cyklus
vnitrni_cyklus_end:
	lw $s5, pole($s0)
	sw $s4, pole($s0)
	sw $s5, pole($s3)
	
	addi $s0, $s0, 4
	j hlavni_cyklus
hlavni_cyklus_end:

//Koncova nekonecna smycka
end_loop:
	cache 9, 0  // vyprazdneni pameti cache
	break       // zastaveni simulatoru
	j end_loop
	nop

.end _start
